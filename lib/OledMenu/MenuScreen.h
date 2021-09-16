#ifndef MenuScreen_h
#define MenuScreen_h

#include "Arduino.h"
#include "U8g2lib.h"

class MenuScreen
{
public:
    MenuScreen(String name) : _name(name) {}
    typedef enum {
        DECREMENT = 0,
        INCREMENT = 1,
        OK = 2
    } buttons;

    virtual void drawScreen(U8G2 display) = 0;
    void up();
    void down();
    void enter();

protected:
    String _name;
    uint8_t _selectedButton;
};

#endif