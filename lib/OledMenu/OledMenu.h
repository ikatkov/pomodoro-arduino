#ifndef OledMenu_h
#define OledMenu_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuScreen.h"

class OledMenu
{
public:
  OledMenu(U8G2 display, const MenuItem *menuList, uint8_t menuLength) : _display(display), _menuList(menuList), _menuLength(menuLength) {}
  void up();
  void down();
  bool enter();
  void exit();
  void drawScreen();
  void reset();

private:
  U8G2 _display;
  MenuScreen _menuScreen = MenuScreen();
  MenuItem *_menuList;
  uint8_t _menuLength;
  uint8_t _selectedMenuIndex = 0;
  boolean _isTopLevel = true;
};

#endif