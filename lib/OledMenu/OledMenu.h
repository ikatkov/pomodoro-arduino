#ifndef OledMenu_h
#define OledMenu_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"
//#include "MenuScreen.h"

class OledMenu
{
  public:
    OledMenu(U8G2 display, MenuItem* items, byte size);
    void drawScreen();
    void up();
    void down();
    void enter();
  private:
    U8G2 _display;
    MenuItem* _menuItems; 
    byte _menuItemsLength;
    byte _selectedMenuIndex;
    MenuItem* _activeItem;
};



#endif