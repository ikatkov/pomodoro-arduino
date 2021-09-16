#include "Arduino.h"
#include "MenuItem.h"

const MenuItem* MenuItem::BACK;

void MenuItem::drawScreen(U8G2 display){
    _child->drawScreen(display);
}

void MenuItem::up()
{
    _child->up();
}

void MenuItem::down()
{
    _child->down();
}

