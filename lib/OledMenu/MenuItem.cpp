#include "Arduino.h"
#include "MenuItem.h"

MenuItem::MenuItem()
{
}

void MenuItem::enter()
{
    Serial.print(F("enter:"));
    _active = true;
}

bool MenuItem::isActive() {
    return _active;
}
void MenuItem::setActive(bool value){
    _active = value;
}
