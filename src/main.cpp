#include <Arduino.h>
#include "U8g2lib.h"
#include "state-sprite.h"
#include <avr/pgmspace.h>
#include "rtttl.h"
#include <EasyButton.h>
#include "Countimer.h"
#include <LowPower.h>

static const byte STUDY_STATE = 0;
static const byte BREAK_STATE = 1;
static const byte LONG_BREAK_STATE = 2;
static const byte RUNNING_STUDY_STATE = 3;
static const byte STOPPED_STUDY_STATE = 4;
static const byte RUNNING_BREAK_STATE = 5;
static const byte STOPPED_BREAK_STATE = 6;
static const byte SHUTDOWN_STATE = -1;

static const byte RESTART_BUTTON_PIN = 12;
static const byte START_STOP_BUTTON_PIN = 11;
static const byte NEXT_BUTTON_PIN = 10;
static const byte BUZZER_PIN = 4;

U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI display(U8G2_R0, /* clock=*/5, /* data=*/6, /* cs=*/9, /* dc=*/8, /* reset=*/7);

const int octave = 0; // The octave set for the player
const char music1[] PROGMEM = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g'";
const char music2[] PROGMEM = "Don'tTur:d=4,o=5,b=100:16a#6,16g#6,16a#6,16g#6,8a#6,16d#6,16p,8d#6,8c#6,8d#6,16p,8a#6";
const char music3[] PROGMEM = "Barbiegi:d=4,o=5,b=100:16a#6,16f#6,16a#6,16d#7,8a#6,8p,16g#6,16c#6,16g#6,16c#7,8a#6,8p";
Rtttl player; // Song player

EasyButton restartButton(RESTART_BUTTON_PIN);
EasyButton startStopButton(START_STOP_BUTTON_PIN);
EasyButton nextButton(NEXT_BUTTON_PIN);

byte state = STUDY_STATE;
Countimer tdown;

void drawFrame(const unsigned char *frame, int size)
{
    Serial.print("Sprite size=");
    Serial.println(size);
    do
    {
        display.firstPage();
        do
        {
            display.drawXBMP(0, 0, 64, 64, frame);
        } while (display.nextPage());
        frame += 512;
        size -= 512;
    } while (size > 0);
}

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

void initializeState()
{
    Serial.println("initializeState");
    display.firstPage();
    do
    {
        drawClockFace();
    } while (display.nextPage());
}

void wakeUp() {
  detachInterrupt(0); //execution resumes from here after wake-up
  Serial.println("wakeUp");
  delay(4000);
  initializeState();
  //compensate for ghost button presses
  state = (state - 1) % 4;
}


void goDeepSleep() {
  Serial.println("goDeepSleep");
  delay(4000);
  attachInterrupt(0, wakeUp, LOW);                       //use interrupt 0 (pin PD2) and run function wakeUp when pin 2 gets LOW
  Serial.println("attachInterrupt");
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);   //arduino enters sleep mode here
}




void onRestartButtonPressed() {
  Serial.println("onRestartButtonPressed");
}


void onStartStopButtonPressed() {
  Serial.println("onStartStopButtonPressed");
}


void onNextButtonPressed() {
  Serial.println("onNextButtonPressed");
}


void onNextButtonPressedForDuration() {
  Serial.println("onNextButtonPressedForDuration");
}


void setup()
{
    Serial.begin(9600);
    while (!Serial)
        delay(10);

    Serial.println("Setup");

    pinMode(RESTART_BUTTON_PIN, INPUT_PULLUP);
    pinMode(START_STOP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(NEXT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);

    restartButton.begin();
    restartButton.onPressed(onRestartButtonPressed);
    startStopButton.begin();
    startStopButton.onPressed(onStartStopButtonPressed);
    nextButton.begin();
    nextButton.onPressed(onNextButtonPressed);
    nextButton.onPressedFor(1000, onNextButtonPressedForDuration);

    player.begin(BUZZER_PIN); // Starts the player

    display.begin();

    initializeState();

    //player.play_P(music1, octave);
    // player.play_P(music2, octave);

    // player.play_P(music3, octave);
}

byte studyTime = 25;
byte breakTime = 5;

byte i = studyTime;
void loop()
{
    /*
    if (state == false)
    {
        byte minutes = i--;
        display.firstPage();
        do
        {
            drawClockFace();
            drawMinustesLeftSlice(15, 40, 23, minutes);
            display.drawXBMP(74, 10, 50, 50, STUDY_SPRITE);
        } while (display.nextPage());
        if (i == 0)
        {
            state = true;
            i = breakTime;
        }
    }
    else
    {

        byte minutes = i--;
        display.firstPage();
        do
        {
            drawClockFace();
            drawMinustesLeftSlice(15, 40, 23, minutes);
            display.drawXBMP(64, 10, 50, 44, BREAK_SPRITE);
        } while (display.nextPage());
        if (i == 0)
        {
            state = false;
            i = studyTime;
        }
    }

    delay(1000);
    */
}
