#include "Arduino.h"
#include "OledMenu.h"

OledMenu::OledMenu(U8G2 display, MenuItem *items, byte size)
{
    _display = display;
    _menuItemsLength = size;
    _menuItems = items;
}

void OledMenu::up()
{
    if (_activeItem == NULL)
    {
        _selectedMenuIndex = (_selectedMenuIndex - 1) % _menuItemsLength;
    }
    else
    {
        _activeItem->getScreen()->up();
    }
}
void OledMenu::down()
{
    if (_activeItem == NULL)
    {
        _selectedMenuIndex = (_selectedMenuIndex + 1) % _menuItemsLength;
    }
    else
    {
        _activeItem->getScreen()->down();
    }
}

void OledMenu::enter()
{
    if (_activeItem == NULL)
    {
        _menuItems[_selectedMenuIndex].enter();
        _activeItem = &(_menuItems[_selectedMenuIndex]);
    }
    else
    {
        _activeItem->getScreen()->enter();
    }
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
    else
    {
        _activeItem->getScreen()->drawScreen(_display);
    }
}