#include <Arduino.h>
#include "U8g2lib.h"
#include "state-sprite.h"
#include <avr/pgmspace.h>
#include "rtttl.h"
#include <EasyButton.h>
#include "Countimer.h"
//#include <LowPower.h>
#include <EEPROM.h>
#include "OledMenu.h"

static const byte STUDY_IDLE_STATE = 0;
static const byte BREAK_IDLE_STATE = 1;
static const byte LONG_BREAK_IDLE_STATE = 2;
static const byte STUDY_RUNNING_STATE = 3;
static const byte STUDY_STOPPED_STATE = 4;
static const byte BREAK_RUNNING_STATE = 5;
static const byte BREAK_STOPPED_STATE = 6;
static const byte LONG_BREAK_RUNNING_STATE = 7;
static const byte LONG_BREAK_STOPPED_STATE = 8;
static const byte SHUTDOWN_STATE = -1;

static const byte START_STOP_BUTTON_PIN = 11;
static const byte NEXT_BUTTON_PIN = 10;
static const byte BUZZER_PIN = 4;

static const byte DEFAULT_STUDY_MINUTES = 20;
static const byte DEFAULT_BREAK_MINUTES = 4;
static const byte DEFAULT_LONG_BREAK_MINUTES = 40;

static const byte MAX_STUDY_SESSIONS_IN_A_ROW = 4;

U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI display(U8G2_R0, /* clock=*/5, /* data=*/6, /* cs=*/9, /* dc=*/8, /* reset=*/7);

const int octave = 0; // The octave set for the player
const char music1[] PROGMEM = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g'";
const char music2[] PROGMEM = "Don'tTur:d=4,o=5,b=100:16a#6,16g#6,16a#6,16g#6,8a#6,16d#6,16p,8d#6,8c#6,8d#6,16p,8a#6";
const char music3[] PROGMEM = "Barbiegi:d=4,o=5,b=100:16a#6,16f#6,16a#6,16d#7,8a#6,8p,16g#6,16c#6,16g#6,16c#7,8a#6,8p";
Rtttl player; // Song player

EasyButton startStopButton(START_STOP_BUTTON_PIN);
EasyButton nextButton(NEXT_BUTTON_PIN);

byte state = STUDY_IDLE_STATE;
Countimer tdown;

byte studyMinutes = DEFAULT_STUDY_MINUTES;
byte breakMinutes = DEFAULT_BREAK_MINUTES;
byte longBreakMinutes = DEFAULT_LONG_BREAK_MINUTES;
byte studySessionsInARow = 0;

bool menuMode;

/*
MenuScreenInteger studyTimeScreen = MenuScreenInteger("Study time", [](){Serial.println("I'm lambda");});
MenuItem studyMenuItem = MenuItem("Study time", &studyTimeScreen);
MenuItem firstList[4] = {
    studyMenuItem,
    {"Break time"},
    {"Long Break time"},
    {"Cancel", [](){menuMode = false;}}
    };
*/

OledMenu menu = OledMenu(display, NULL);


void fillArc(int x, int y, int radius, int startAngle, int endAngle)
{

    startAngle -= 90;
    endAngle -= 90;
    const byte step = 6;
    for (int i = startAngle; i < endAngle; i = i + step)
    {
        double radians1 = i * PI / 180;
        double radians2 = (i + step) * PI / 180;
        double px1 = x + radius * cos(radians1);
        double py1 = y + radius * sin(radians1);

        double px2 = x + radius * cos(radians2);
        double py2 = y + radius * sin(radians2);

        display.drawTriangle(round(px1), round(py1), round(px2), round(py2), x, y);
    }
}

void drawMinustesLeftSlice(byte center_x, byte center_y, byte radius, byte minutesLeft)
{
    if (minutesLeft >= 45)
    {
        display.drawDisc(center_x, center_y, radius, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_LOWER_RIGHT | U8G2_DRAW_LOWER_LEFT);
        fillArc(center_x, center_y, radius, 270, 270 + 6 * (minutesLeft - 45));
    }
    else if (minutesLeft >= 30)
    {
        display.drawDisc(center_x, center_y, radius, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_LOWER_RIGHT);
        fillArc(center_x, center_y, radius, 180, 180 + 6 * (minutesLeft - 30));
    }
    else if (minutesLeft >= 15)
    {
        display.drawDisc(center_x, center_y, radius, U8G2_DRAW_UPPER_RIGHT);
        fillArc(center_x, center_y, radius, 90, 90 + 6 * (minutesLeft - 15));
    }
    else
    {
        // less than 15min
        fillArc(center_x, center_y, radius, 0, 6 * minutesLeft);
    }
}

void drawClockFaceMark(int h, int clockCenterX, int clockCenterY)
{
    float x1, y1, x2, y2;

    h = h * 30;
    h = h + 270;

    x1 = 29 * cos(h * 0.0175);
    y1 = 29 * sin(h * 0.0175);
    x2 = 25 * cos(h * 0.0175);
    y2 = 25 * sin(h * 0.0175);

    display.drawLine(x1 + clockCenterX, y1 + clockCenterY, x2 + clockCenterX, y2 + clockCenterY);
}

void drawClockFace()
{
    //draw stopwatch face
    display.setDrawColor(1);
    display.drawDisc(15, 40, 30, U8G2_DRAW_ALL);
    display.setDrawColor(0);
    display.drawDisc(15, 40, 27, U8G2_DRAW_ALL);

    //draw stopwatch top button
    display.setDrawColor(1);
    display.drawBox(13, 6, 5, 4);
    display.drawRBox(9, 1, 13, 6, 1);

    //draw stopwatch side button
    display.drawDisc(35, 15, 3, U8G2_DRAW_ALL);
    display.drawDisc(37, 17, 3, U8G2_DRAW_ALL);

    //draw center
    display.drawDisc(15, 40, 2, U8G2_DRAW_ALL);

    // Draw a small mark for every hour
    for (int i = 0; i < 12; i++)
    {
        drawClockFaceMark(i, 15, 40);
    }
}

void drawPauseSign()
{
    //draw black center
    display.setDrawColor(0);
    display.drawDisc(15, 40, 2, U8G2_DRAW_ALL);
    display.setDrawColor(1);

    display.drawRBox(10, 30, 4, 20, 1);
    display.drawRBox(20, 30, 4, 20, 1);
}

void drawStudySprite()
{
    display.drawXBMP(70, 15, 32, 32, STUDY_SPRITE);
}

void drawBreakSprite()
{
    display.drawXBMP(70, 15, 32, 32, BREAK_SPRITE);
}

void drawLongBreakSprite()
{
    display.drawXBMP(70, 15, 32, 32, LONG_BREAK_SPRITE);
}

void reDrawRegularScreen()
{

    display.firstPage();
    do
    {
        drawClockFace();

        display.setFont(u8g2_font_open_iconic_check_1x_t);
        for (int i = 0; i < studySessionsInARow; i++)
        {
            display.drawGlyph(60 + i * 15, 10, 67); //checkbox
        }

        display.setFont(u8g2_font_ncenB12_tn);
        // Serial.print(tdown.getCurrentMinutes());
        // Serial.print(":");
        // Serial.println(tdown.getCurrentSeconds());

        byte timeLeft = tdown.getCurrentMinutes() > 0 ? tdown.getCurrentMinutes() : tdown.getCurrentSeconds();
        switch (state)
        {
        case STUDY_IDLE_STATE:
            drawStudySprite();
            break;
        case BREAK_IDLE_STATE:
            drawBreakSprite();
            break;
        case LONG_BREAK_IDLE_STATE:
            drawLongBreakSprite();
            break;
        case STUDY_RUNNING_STATE:
            drawMinustesLeftSlice(15, 40, 23, timeLeft);
            drawStudySprite();
            break;
        case STUDY_STOPPED_STATE:
            drawPauseSign();
            drawStudySprite();
            break;
        case BREAK_RUNNING_STATE:
            drawMinustesLeftSlice(15, 40, 23, timeLeft);
            drawBreakSprite();
            break;
        case BREAK_STOPPED_STATE:
            drawPauseSign();
            drawBreakSprite();
            break;
        case LONG_BREAK_RUNNING_STATE:
            drawMinustesLeftSlice(15, 40, 23, timeLeft);
            drawLongBreakSprite();
            break;
        case LONG_BREAK_STOPPED_STATE:
            drawPauseSign();
            drawLongBreakSprite();
            break;

        default:
            break;
        }

        // drat time string
        char buffer[10];
        sprintf(buffer, "%02d:%02d", tdown.getCurrentMinutes(), tdown.getCurrentSeconds());
        display.drawStr(60, 64, buffer);
    } while (display.nextPage());
}

void reDrawMenuScreen()
{
    Serial.println("reDrawMenuScreen");
    menu.drawScreen();
    /*
    //9px tall font
    byte fontHeigh = 9;
    display.setFont(u8g2_font_6x13B_mf);
    display.clearDisplay();
    display.setDrawColor(1);
    display.firstPage();
    do
    {
        for (int i = 0; i < totalMenuElements; i++)
        { // for each menu item
            display.setCursor(0, 11 + i * 13);
            if (i == currentMenuIndex)
            {
                display.drawBox(0, i*9, 128, 9);
                display.setDrawColor(0);
            }
            else
            {
                display.setDrawColor(1);
            }
            display.print(menuItems[i]);
        }

    } while (display.nextPage());
    */
}

void reDrawScreen()
{
    Serial.print("reDrawScreen, state = ");
    Serial.println(state);
    if (menuMode)
    {
        reDrawMenuScreen();
    }
    else
    {
        reDrawRegularScreen();
    }
}

void initializeState()
{
    Serial.println("initializeState");
    state = STUDY_IDLE_STATE;
    tdown.setCounter(0, studyMinutes, 0);
    //menuMode = true;
    //menu._activeItem = &studyMenuItem;
    reDrawScreen();
}

/*
void wakeUp()
{
    Serial.println("Wakeup");
    detachInterrupt(0); //execution resumes from here after wake-up
    Serial.println("wakeUp");
    delay(4000);
    display.sleepOff();
    initializeState();
    //compensate for ghost button presses
    state = (state - 1) % 4;
}

void goDeepSleep()
{
    Serial.println("goDeepSleep");
    delay(4000);
    attachInterrupt(0, wakeUp, LOW); //use interrupt 0 (pin PD2) and run function wakeUp when pin 2 gets LOW
    Serial.println("attachInterrupt");
    display.clearDisplay();
    display.sleepOn();
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); //arduino enters sleep mode here
}
*/

void onStartStopButtonPressedRegular()
{
    Serial.print("onStartStopButtonPressedRegular, state = ");
    Serial.println(state);

    if (state == STUDY_IDLE_STATE || state == STUDY_STOPPED_STATE)
    {
        state = STUDY_RUNNING_STATE;
        tdown.start();
    }
    else if (state == STUDY_RUNNING_STATE)
    {
        state = STUDY_STOPPED_STATE;
        tdown.pause();
    }
    else if (state == BREAK_IDLE_STATE || state == BREAK_STOPPED_STATE)
    {
        state = BREAK_RUNNING_STATE;
        tdown.start();
    }
    else if (state == BREAK_RUNNING_STATE)
    {
        state = BREAK_STOPPED_STATE;
        tdown.pause();
    }
    else if (state == LONG_BREAK_IDLE_STATE || state == LONG_BREAK_STOPPED_STATE)
    {
        state = LONG_BREAK_RUNNING_STATE;
        tdown.start();
    }
    else if (state == LONG_BREAK_RUNNING_STATE)
    {
        state = LONG_BREAK_STOPPED_STATE;
        tdown.pause();
    }
    reDrawScreen();
}

void onStartStopButtonPressedMenu()
{
    Serial.print("onStartStopButtonPressedMenu");
    menu.up();
    reDrawScreen();
}

void onStartStopButtonPressed()
{
    if (menuMode)
    {
        onStartStopButtonPressedMenu();
    }
    else
    {
        onStartStopButtonPressedRegular();
    }
}

void onstartStopButtonPressedForDuration()
{
    Serial.print("onstartStopButtonPressedForDuration, state = ");
    Serial.println(state);
    tdown.restart();
    reDrawScreen();
}

void onCountDownComplete()
{
    Serial.print("onCountDownComplete, state = ");
    Serial.println(state);
    if (state == STUDY_RUNNING_STATE)
    {
        studySessionsInARow++;
        player.play_P(music1, octave);
        if (studySessionsInARow == MAX_STUDY_SESSIONS_IN_A_ROW)
        {
            state = LONG_BREAK_IDLE_STATE;
            tdown.setCounter(0, longBreakMinutes, 0);
            studySessionsInARow = 0;
        }
        else
        {
            state = BREAK_IDLE_STATE;
            tdown.setCounter(0, breakMinutes, 0);
        }
    }
    else if (state == BREAK_RUNNING_STATE)
    {
        player.play_P(music2, octave);
        state = STUDY_IDLE_STATE;
        tdown.setCounter(0, studyMinutes, 0);
    }
    else if (state == LONG_BREAK_RUNNING_STATE)
    {
        player.play_P(music3, octave);
        state = STUDY_IDLE_STATE;
        tdown.setCounter(0, studyMinutes, 0);
    }
    reDrawScreen();
}

void onNextButtonPressedRegular()
{
    Serial.print("onNextButtonPressed, state = ");
    Serial.println(state);
    if (state == STUDY_IDLE_STATE || state == STUDY_RUNNING_STATE || state == STUDY_STOPPED_STATE)
    {
        state = BREAK_IDLE_STATE;
        tdown.stop();
        tdown.setCounter(0, breakMinutes, 0);
    }
    else if (state == BREAK_IDLE_STATE || state == BREAK_RUNNING_STATE || state == BREAK_STOPPED_STATE)
    {
        state = LONG_BREAK_IDLE_STATE;
        tdown.stop();
        tdown.setCounter(0, longBreakMinutes, 0);
    }
    else if (state == LONG_BREAK_IDLE_STATE || state == LONG_BREAK_RUNNING_STATE || state == LONG_BREAK_STOPPED_STATE)
    {
        state = STUDY_IDLE_STATE;
        tdown.stop();
        tdown.setCounter(0, studyMinutes, 0);
    }

    reDrawScreen();
}

void onNextButtonPressedMenu()
{
    Serial.print("onNextButtonPressedMenu");
    menu.down();
    reDrawScreen();
}

void onNextButtonPressed()
{
    if (menuMode)
    {
        onNextButtonPressedMenu();
    }
    else
    {
        onNextButtonPressedRegular();
    }
}

void onNextButtonPressedForDuration()
{
    Serial.print("onNextButtonPressedForDuration, state = ");
    Serial.println(state);
    if (!menuMode)
    {
        menuMode = true;
    }
    else
    {
        menu.enter();
    }
    reDrawScreen();
}

void eepromWrite()
{
    EEPROM.write(0, studyMinutes);
    EEPROM.write(1, breakMinutes);
    EEPROM.write(2, longBreakMinutes);
}

void eepromRead()
{
    studyMinutes = EEPROM.read(0);
    breakMinutes = EEPROM.read(1);
    longBreakMinutes = EEPROM.read(3);
    Serial.print("original EEPROM read: ");
    Serial.print(studyMinutes);
    Serial.print(",");
    Serial.print(breakMinutes);
    Serial.print(",");
    Serial.println(longBreakMinutes);
    if (studyMinutes < 0 || studyMinutes > 60)
        studyMinutes = DEFAULT_STUDY_MINUTES;
    if (breakMinutes < 0 || breakMinutes > 60)
        breakMinutes = DEFAULT_BREAK_MINUTES;
    if (longBreakMinutes < 0 || longBreakMinutes > 60)
        longBreakMinutes = DEFAULT_LONG_BREAK_MINUTES;
    Serial.print("corrected EEPROM read: ");
    Serial.print(studyMinutes);
    Serial.print(",");
    Serial.print(breakMinutes);
    Serial.print(",");
    Serial.println(longBreakMinutes);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        delay(10);

    Serial.println("Setup");

    pinMode(START_STOP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(NEXT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);

    startStopButton.begin();
    startStopButton.onPressed(onStartStopButtonPressed);
    startStopButton.onPressedFor(1000, onstartStopButtonPressedForDuration);

    nextButton.begin();
    nextButton.onPressed(onNextButtonPressed);
    nextButton.onPressedFor(1000, onNextButtonPressedForDuration);

    player.begin(BUZZER_PIN);

    display.begin();

    tdown.setInterval(reDrawScreen, 333);
    tdown.setCounter(0, 0, 0, CountType::COUNT_DOWN, onCountDownComplete);

    initializeState();


    //[](){ Serial.println("lambda"); }))
    //menu.addItem(LineItem("Study Time"));
    // menu.addItem(LineItem("Break Time"));
    // menu.addItem(LineItem("Long Break Time"));
    // menu.addItem(LineItem("Cancel"));
}

void loop()
{
    startStopButton.read();
    nextButton.read();
    tdown.run();
}
