#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"
#include "U8g2lib.h"

class MenuItem
{
public:
  typedef void (*callback_t)();
  static const MenuItem *BACK;

  MenuItem(const char *name) : _name(name) {};
  MenuItem(const char *name, callback_t callback) : _name(name), _callback(callback) {}

  const char *getName()
  {
    return _name;
  }

  virtual void drawScreen(U8G2 display) {}
  virtual void up() {}
  virtual void down() {} 

  //return true if the menu item needs to remain active, return false if it's done
  virtual bool enter();

protected:
  const char *_name;
  callback_t _callback;
};

#endif