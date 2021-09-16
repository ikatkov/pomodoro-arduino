#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"
#include "U8g2lib.h"

class MenuItem
{
public:
  typedef void (*callback_t)();
  static const MenuItem* BACK;

  MenuItem(MenuItem *child) : _child(child) {}
  MenuItem(String name) : _name(name) {}
  MenuItem(String name, MenuItem *child) : _name(name), _child(child) {}
  MenuItem(String name, callback_t callback) : _name(name), _callback(callback) {}

  String getName() { return _name; }

  virtual void drawScreen(U8G2 display);
  virtual void up();
  virtual void down();
  virtual MenuItem *enter() = 0;

protected:
  String _name;
  callback_t _callback;
  MenuItem *_child;
  MenuItem *_parent;
};

#endif