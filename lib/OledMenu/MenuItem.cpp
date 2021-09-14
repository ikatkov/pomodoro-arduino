#include "Arduino.h"
#include "MenuItem.h"

MenuItem::MenuItem(String name)
{
    _name = name;
}

MenuItem::MenuItem(String name, callback_t callback)
{
    _name = name;
    _callback = callback;
}

MenuItem::MenuItem(String name, MenuScreen* screen)
{
    _name = name;
    _screen = screen;
}

String MenuItem::getName()
{
    return _name;
}

void MenuItem::enter()
{
    Serial.print(F("enter:"));
    _active = true;
    if (_callback)
    {
        _callback();
    }
}

bool MenuItem::isActive()
{
    return _active;
}
void MenuItem::setActive(bool value)
{
    _active = value;
}
