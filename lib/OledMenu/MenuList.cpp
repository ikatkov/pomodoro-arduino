#include "Arduino.h"
#include "MenuList.h"

void MenuList::up()
{
    _selectedMenuIndex = (_selectedMenuIndex - 1) % _menuLength;
}
void MenuList::down()
{
    _selectedMenuIndex = (_selectedMenuIndex + 1) % _menuLength;
}

MenuItem* MenuList::enter()
{
    return &_children[_selectedMenuIndex];
}

void MenuList::drawScreen(U8G2 display)
{
    Serial.println(F("MenuList.drawScreen"));
    {
        const byte fontHeigh = 13;
        const byte offset = 1 + fontHeigh;
        const byte lineSpacing = 1;
        display.setFont(u8g2_font_6x13_mf);

        display.firstPage();
        do
        {
            for (int i = 0; i < _menuLength; i++)
            {
                display.setCursor(0, offset + i * (fontHeigh + lineSpacing));
                if (i == _selectedMenuIndex)
                {
                    display.print('>');
                }
                else
                {
                    display.print(' ');
                }
                String name = _children[i].getName();
                display.print(name);
            }
        } while (display.nextPage());
    }
}
