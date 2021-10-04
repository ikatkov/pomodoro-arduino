#include "Arduino.h"
#include "MenuScreen.h"

void MenuScreen::init(MenuItem *menuItem)
{
    _menuItem = menuItem;
    MenuGetFunction theFunc = (MenuGetFunction)pgm_read_word(&(_menuItem->funcGet));
    if (theFunc)
    {
        _value = theFunc();
    }
    Serial.println(_value);
}

void MenuScreen::up()
{
    MenuType type = pgm_read_word(&(_menuItem->type));
    if (type == NUMBER)
    {
        if (_value < pgm_read_byte(&_menuItem->max))
        {
            _value++;
        }
    }
    else if (type == TOGGLE)
    {
        _value = (_value + 1) % 2;
    }
}
void MenuScreen::down()
{
    MenuType type = pgm_read_word(&(_menuItem->type));
    if (type == NUMBER)
    {
        if (_value > pgm_read_byte(&_menuItem->min))
        {
            _value--;
        }
    }
    else if (type == TOGGLE)
    {
        _value = (_value + 1) % 2;
    }
}

void MenuScreen::enter()
{
    MenuSetFunction theFunc = (MenuSetFunction)pgm_read_word(&(_menuItem->funcSet));
    theFunc(_value);
}

void MenuScreen::drawScreen(U8G2 display)
{
    MenuType type = pgm_read_word(&(_menuItem->type));
    Serial.print(F("MenuScreen::drawScreen:"));
    Serial.println(type);
    display.setDrawColor(1);
    if (type == NUMBER)
    {
        char buf1[3];
        char buf2[3];
        char buf3[3];
        snprintf(buf1, 3, "%d", _value - 1);
        snprintf(buf2, 3, "%d", _value);
        snprintf(buf3, 3, "%d", _value + 1);

        display.firstPage();
        do
        {
            display.setFont(u8g2_font_6x13_mf);
            char text[18];
            strcpy_P(text, _menuItem->text);
            display.drawStr(0, 15, text);

            display.drawStr(4, 45, "..");
            display.drawStr(24, 45, buf1);

            display.setFont(u8g2_font_10x20_mn);

            display.drawStr(54, 48, buf2);

            display.setFont(u8g2_font_6x13_mf);
            display.drawStr(84, 45, buf3);
            display.drawStr(104, 45, "..");

            display.drawTriangle(59, 25, 64, 30, 69, 25);
            display.drawTriangle(64, 52, 59, 57, 69, 57);
            display.drawLine(5, 30, 55, 30);
            display.drawLine(72, 30, 123, 30);
            display.drawLine(5, 52, 55, 52);
            display.drawLine(72, 52, 123, 52);

        } while (display.nextPage());
    }
    else if (type == TOGGLE)
    {
        display.setDrawColor(1);
        display.setFont(u8g2_font_6x13_mf);

        display.firstPage();
        do
        {
            char text[18];
            strcpy_P(text, _menuItem->text);
            display.drawStr(0, 15, text);

            if (_value == 1)
            {
                display.drawStr(54, 46, "YES");
            }
            else
            {
                display.drawStr(58, 46, "NO");
            }

            display.drawTriangle(59, 25, 64, 30, 69, 25);
            display.drawTriangle(64, 52, 59, 57, 69, 57);
            display.drawLine(5, 30, 55, 30);
            display.drawLine(72, 30, 123, 30);
            display.drawLine(5, 52, 55, 52);
            display.drawLine(72, 52, 123, 52);

        } while (display.nextPage());
    }
}
