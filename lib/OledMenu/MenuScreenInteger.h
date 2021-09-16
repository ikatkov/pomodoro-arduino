#ifndef MenuScreenInteger_h
#define MenuScreenInteger_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"

class MenuScreenInteger : public MenuItem
{
public:
    void drawScreen(U8G2 display);
    void up();
    void down();
    MenuItem* enter();
private:    
    uint8_t _selectedButton;
    typedef enum
    {
        DECREMENT = 0,
        INCREMENT = 1,
        OK = 2
    } buttons;
};

#endif