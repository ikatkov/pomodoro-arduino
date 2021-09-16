#ifndef MenuList_h
#define MenuList_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"

class MenuList : public MenuItem
{
public:
    void drawScreen(U8G2 display);
    void up();
    void down();
    MenuItem* enter();

private:
    MenuItem *_children;
    uint8_t _menuLength;
    uint8_t _selectedMenuIndex;
};

#endif