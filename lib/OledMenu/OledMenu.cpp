#include "Arduino.h"
#include "OledMenu.h"

OledMenu::OledMenu(U8G2 display, MenuItem* items, byte size)
{
    _display = display;
    _menuItemsLength = size;
    _menuItems = items;
}

void OledMenu::up()
{

    if (_selectedMenuIndex == 0)
        _selectedMenuIndex = _menuItemsLength - 1;
    else
        _selectedMenuIndex--;
}
void OledMenu::down()
{

    _selectedMenuIndex = (_selectedMenuIndex + 1) % _menuItemsLength;
}
void OledMenu::enter()
{
    _menuItems[_selectedMenuIndex].enter();
}

void OledMenu::drawScreen()
{
    Serial.println(F("OledMenu.drawScreen"));

    if (_activeItem == NULL)
    {
        const byte fontHeigh = 13;
        const byte offset = 1 + fontHeigh;
        const byte lineSpacing = 1;
        _display.setFont(u8g2_font_6x13_mf);

        _display.firstPage();
        do
        {

            for (int i = 0; i < _menuItemsLength; i++)
            {
                _display.setCursor(0, offset + i * (fontHeigh + lineSpacing));
                if (i == _selectedMenuIndex)
                {
                    _display.print('>');
                }
                else
                {
                    _display.print(' ');
                }
                String name = _menuItems[i].getName();
                _display.print(name);
            }
        } while (_display.nextPage());
    }

    // else
    // {
    //_activeItem.d
    // }
}