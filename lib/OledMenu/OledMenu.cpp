#include "Arduino.h"
#include "OledMenu.h"

void OledMenu::up()
{
    if (_activeItem == NULL)
    {
        _selectedMenuIndex = uint8_t(_selectedMenuIndex - 1) % _menuLength;
        Serial.println(_selectedMenuIndex);
    }
    else
    {
        _activeItem->up();
        _activeItem->drawScreen(_display);
    }
}
void OledMenu::down()
{
    if (_activeItem == NULL)
    {
        _selectedMenuIndex = (_selectedMenuIndex + 1) % _menuLength;
        Serial.println(_selectedMenuIndex);
    }
    else
    {
        _activeItem->down();
        _activeItem->drawScreen(_display);
    }
}

void OledMenu::enter()
{
    if (_activeItem == NULL)
    {
        if (_menuList[_selectedMenuIndex].enter())
        {
            _activeItem = &_menuList[_selectedMenuIndex];
        }
    }
    else
    {
        if (!_activeItem->enter())
        {
            _activeItem = NULL;
        }
    }
}

void OledMenu::drawScreen()
{
    Serial.println(F("OledMenu.drawScreen"));
    if (_activeItem == NULL)
    {
        Serial.println(F("_activeItem == NULL"));
        const byte fontHeigh = 13;
        const byte offset = 1 + fontHeigh;
        const byte lineSpacing = 1;
        _display.setFont(u8g2_font_6x13_mf);

        _display.firstPage();
        do
        {
            for (int i = 0; i < _menuLength; i++)
            {
                _display.setCursor(0, offset + i * (fontHeigh + lineSpacing));
                if (i == _selectedMenuIndex)
                {
                    _display.print(F(">"));
                }
                else
                {
                    _display.print(F(" "));
                }
                const char *name = _menuList[i].getName();
                //Serial.println(name);
                _display.print(name);
            }
        } while (_display.nextPage());
    }
    else
    {
        _activeItem->drawScreen(_display);
    }
}
