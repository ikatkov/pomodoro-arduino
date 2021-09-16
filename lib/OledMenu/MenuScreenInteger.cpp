#include "Arduino.h"
#include "MenuScreenInteger.h"


void MenuScreenInteger::up()
{
    _selectedButton = (_selectedButton - 1) % 3;
}
void MenuScreenInteger::down()
{
    _selectedButton = (_selectedButton + 1) % 3;
}

MenuItem* MenuScreenInteger::enter()
{
    if(_callback){
        _callback();
    }
    return NULL;
}

void MenuScreenInteger::drawScreen(U8G2 display)
{
    display.clearDisplay();

    display.setDrawColor(1);

    display.firstPage();
    do
    {

        //title
        display.setFont(u8g2_font_6x13_mf);
        display.setCursor(0, 13);
        display.print(_name);

        //buttons
        display.setFont(u8g2_font_ncenB12_tn);
        display.drawStr(30, 37, "-");
        display.drawStr(82, 37, "+");
        display.setFont(u8g2_font_6x13_mf);
        display.drawStr(103, 60, "OK");

        //buttons selection frames
        switch (_selectedButton)
        {
        case DECREMENT:
            display.drawFrame(22, 22, 20, 20);
            break;
        case INCREMENT:
            display.drawFrame(78, 22, 20, 20);
            break;
        case OK:
            display.drawFrame(91, 48, 36, 16);
            break;
        default:
            break;
        }

        // integer setting
        display.setFont(u8g2_font_ncenB12_tn);
        display.drawStr(53, 38, "99");

    } while (display.nextPage());
}
