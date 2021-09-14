#ifndef MenuScreenInteger_h
#define MenuScreenInteger_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuScreen.h"

class MenuScreenInteger : public MenuScreen
{
public:
    typedef void (*callback_t)();

    MenuScreenInteger(String name, callback_t callback) : MenuScreen(name), _callback(callback)
    {
    }

    void drawScreen(U8G2 display);

private:
    callback_t _callback;
};

#endif