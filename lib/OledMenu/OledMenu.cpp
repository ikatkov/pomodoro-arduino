#include "Arduino.h"
#include "OledMenu.h"

void OledMenu::reset()
{
    _isTopLevel = true;
    _selectedMenuIndex = 0;
}

void OledMenu::up()
{
    if (_isTopLevel)
    {
        if (_selectedMenuIndex == 0)
        {
            _selectedMenuIndex = _menuLength - 1;
        }
        else
        {
            _selectedMenuIndex = uint8_t(_selectedMenuIndex - 1) % _menuLength;
        }
    }
    else
    {
        _menuScreen.down();
    }
}

void OledMenu::down()
{
    if (_isTopLevel)
    {
        _selectedMenuIndex = (_selectedMenuIndex + 1) % _menuLength;
    }
    else
    {
        _menuScreen.up();
    }
}

void OledMenu::exit()
{
    _isTopLevel = true;
}

bool OledMenu::enter()
{
    Serial.println(F("OledMenu::enter"));
    bool result = false;
    if (_isTopLevel)
    {
        MenuItem *item = &(_menuList[_selectedMenuIndex]);
        MenuType type = pgm_read_word(&(item->type));
        _menuScreen.init(item);
        _isTopLevel = false;
        result = (type == EXIT);
    }
    else
    {
        _menuScreen.enter();
        _isTopLevel = true;
    }
    Serial.print("Returning:");
    Serial.println(result);
    return result;
}

void OledMenu::drawScreen()
{
    Serial.println(F("OledMenu.drawScreen"));
    if (_isTopLevel)
    {
        Serial.println(F("TopLevel"));
        const byte fontHeigh = 13;
        const byte offset = 1 + fontHeigh;
        const byte lineSpacing = 1;
        _display.setFont(u8g2_font_6x13_mf);
        uint8_t menuOffset = max(0, _selectedMenuIndex - 3);

        _display.firstPage();
        do
        {
            for (uint8_t itemCounter = menuOffset, i = 0; i < _menuLength; i++, itemCounter++)
            {
                _display.setCursor(0, offset + i * (fontHeigh + lineSpacing));
                if (itemCounter == _selectedMenuIndex)
                {
                    _display.print(F(">"));
                }
                else
                {
                    _display.print(F(" "));
                }
                char name[18];
                strcpy_P(name, _menuList[itemCounter].text);

                //Serial.println(name);
                _display.print(name);
            }

        } while (_display.nextPage());
    }
    else
    {
        _menuScreen.drawScreen(_display);
    }
}
