#ifndef OledMenu_h
#define OledMenu_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"
#include "MenuList.h"

class OledMenu
{
public:
  OledMenu(U8G2 display, MenuItem *child) : _display(display), _active(child) {}
  void up();
  void down();
  void enter();
  void drawScreen();

private:
  U8G2 _display;
  MenuItem *_active;
  MenuItem* topLevel;
  MenuItem* screenLevel;
};

#endif