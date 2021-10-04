#ifndef MenuScreen_h
#define MenuScreen_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"

class MenuScreen
{
public:
    void drawScreen(U8G2 display);
    void up();
    void down();
    void enter();
    void init(MenuItem *menuItem);

private:
    uint8_t _value;
    MenuItem *_menuItem;
};

#endif