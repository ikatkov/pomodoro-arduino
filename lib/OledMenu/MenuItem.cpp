#include "Arduino.h"
#include "MenuItem.h"

bool MenuItem::enter()
{
    if(_callback){
        _callback();
    }
    return false;
}

