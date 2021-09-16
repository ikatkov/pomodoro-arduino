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

MenuItem::MenuItem(String name, MenuScreen *screen)
{
    _name = name;
    _screen = screen;
}

String MenuItem::getName()
{
    Serial.print("MenuItem::getName");
    Serial.println(_name);
    return _name;
}

MenuScreen *MenuItem::getScreen()
{
    return _screen;
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
