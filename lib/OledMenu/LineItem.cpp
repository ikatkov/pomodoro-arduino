#include "Arduino.h"
#include "LineItem.h"

LineItem::LineItem(String name)
{
    _name = name;
}

String LineItem::getName()
{
    return _name;
}

void LineItem::drawScreen(U8G2 display) {}
