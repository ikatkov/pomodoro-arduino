#include "Arduino.h"
#include "OledMenu.h"

//max nesting is 3
const uint8_t MAX_DEPTH = 3;
MenuItem *_stack[MAX_DEPTH] = {};
uint8_t _pointer;

void OledMenu::up()
{
    _active->up();
    _active->drawScreen(_display);
}
void OledMenu::down()
{
    _active->down();
    _active->drawScreen(_display);
}

void OledMenu::enter()
{
    if (_pointer < MAX_DEPTH)
    {
        _stack[_pointer] = _active;
        _pointer++;

        _active = _active->enter();
        if (_active == MenuItem::BACK)
        {
            // go back
            _pointer--;
            _active = _stack[_pointer];
        }
        _active->drawScreen(_display);
    }
}

void OledMenu::drawScreen()
{
    _active->drawScreen(_display);
}