#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"
#include "MenuScreen.h"

class MenuItem
{
public:
  typedef void (*callback_t)(); 

  MenuItem(String name);
  MenuItem(String name, MenuScreen* screen);
  MenuItem(String name, callback_t callback);
  String getName();
  MenuScreen* getScreen();
  void enter();
  bool isActive();
  void setActive(bool value);

private:
  String _name;
  bool _active;
  callback_t _callback;
  MenuScreen* _screen;
};

#endif