#ifndef OledMenu_h
#define OledMenu_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"
#include "MenuScreenInteger.h"

class OledMenu
{
public:
  OledMenu(U8G2 display, MenuItem *menuList, uint8_t menuLength) : _display(display), _menuList(menuList), _menuLength(menuLength) {}
  void up();
  void down();
  void enter();
  void drawScreen();

private:
  U8G2 _display;
  MenuItem *_activeItem;
  MenuItem *_menuList;
  uint8_t _menuLength;
  uint8_t _selectedMenuIndex = 0;

  //  MenuItem* topLevel;
  // MenuItem* screenLevel;
};

#endif