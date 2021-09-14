#ifndef OledMenu_h
#define OledMenu_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"
#include "LineItem.h"

class OledMenu
{
  public:
    OledMenu(U8G2 display, byte size);
    void addItem(MenuItem item);
    void drawScreen();
    void up();
    void down();
    void enter();
    void back();
  private:
    U8G2 _display;
    MenuItem _menuItems[5]; 
    byte _menuItemsLength;
    byte _selectedMenuIndex;
   // MenuItem* _activeItem;
};



#endif