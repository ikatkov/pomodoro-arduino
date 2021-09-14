#ifndef MenuScreen_h
#define MenuScreen_h

#include "Arduino.h"
#include "U8g2lib.h"

class MenuScreen
{
public:
    MenuScreen(String name) : _name(name) {}

    virtual void drawScreen(U8G2 display) = 0;
    void up();
    void down();
    void enter();

protected:
    String _name;
};

#endif