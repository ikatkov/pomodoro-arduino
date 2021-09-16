#include "Arduino.h"
#include "MenuScreen.h"

void MenuScreen::up()
{
    _selectedButton = (_selectedButton - 1) % 3;
}
void MenuScreen::down()
{
    _selectedButton = (_selectedButton + 1) % 3;
}
void MenuScreen::enter()
{
}