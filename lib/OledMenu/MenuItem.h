#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"
#include "U8g2lib.h"

class MenuItem
{
public:
  MenuItem();
  ~MenuItem() {}

  virtual void drawScreen(U8G2 display);
  virtual String getName();
  void enter();
  bool isActive();
  void setActive(bool value);

protected:
  bool _active;
};

#endif