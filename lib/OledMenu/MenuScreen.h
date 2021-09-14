#ifndef MenuScreen_h
#define MenuScreen_h

#include "Arduino.h"
#include "U8g2lib.h"

class MenuScreen
{
public:
    MenuScreen(String name);
    void drawScreen(U8G2 display);
    void up();
    void down();
    void enter();

private:
    String _name;
};

#endif