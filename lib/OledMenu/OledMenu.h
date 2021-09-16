#ifndef OledMenu_h
#define OledMenu_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"
#include "MenuScreenInteger.h"

class OledMenu
{
public:
  OledMenu(U8G2 display, MenuItem *items, byte size);
  void drawScreen();
  void up();
  void down();
  void enter();
  MenuItem *_activeItem;

private:
  U8G2 _display;
  MenuItem *_menuItems;
  byte _menuItemsLength;
  uint8_t _selectedMenuIndex;
};

#endif