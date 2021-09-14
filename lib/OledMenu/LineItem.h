#ifndef LineItem_h
#define LineItem_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MenuItem.h"

class LineItem : public MenuItem
{
public:
  LineItem(String name);
  ~LineItem() {}

  virtual String getName();
  virtual void drawScreen(U8G2 display);
private:
  String _name;
};

#endif