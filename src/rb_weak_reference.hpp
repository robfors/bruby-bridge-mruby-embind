#ifndef _BRUBY_BRIDGE_RB_WEAK_REFERENCE_HPP_
#define _BRUBY_BRIDGE_RB_WEAK_REFERENCE_HPP_


#include <emscripten.h>
#include <emscripten/bind.h>
#include <stdio.h>
#include <math.h>
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/proc.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/variable.h>
#include <stdexcept>

#include "main.hpp"


namespace BRubyBridge
{
  // RbWeakReference exposes a weak reference of a rb object to js
  // we can use them set backward references to JSInterfaces
  class RbWeakReference
  {
  
    public:
    
    RbWeakReference(mrb_value rb_object);
    mrb_value get_object() const;
    
    private:
    
    void* _mrb_value_ptr;
    //mrb_value _rb_object; // we need a weak ref, not sure if this is a weak ref
    
  };
  
}

#endif