#ifndef _BRUBY_BRIDGE_JS_INTERFACE_HPP_
#define _BRUBY_BRIDGE_JS_INTERFACE_HPP_


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
#include <mruby/numeric.h>
#include <mruby/value.h>
#include <mruby/variable.h>
#include <mruby/error.h>


#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <stdio.h>
#include <math.h>

#include "main.hpp"
#include "rb_weak_reference.hpp"


namespace BRubyBridge
{
  
  // JSInterface rb instances point to a js object
  // we can use them to call and get a property of the js object
  namespace JSInterface
  {
    // points to the rb BRubyBridge::JSInterface class
    // public api
    extern RClass* class_mrb;

    // points to the js BRubyBridge.JSInterface class
    // public api
    extern emscripten::val js_class;
    
    // get a JSInterface for a js object
    // if a JSInterface already exists the js object that that JSInterface will be returned
    // otherwise it will make a new JSInterface, set up a forward reference to the js object,
    //   then set up a backward reference so it can find any existing JSInterface for the js object
    // backward references can only be set up for not primitive js objects
    //   (meaning we can assign a property to it)
    // private api
    mrb_value build(emscripten::val object_js);
    
    // call the property a js object
    // expects a key of type JSInterface
    // accepts any number of arguments
    // passing an argument as JSInterfaces will be converted to it's js object
    // passing an argument as an rb object will be converted to it's RbInterface
    // a result of a js object will be converted to it's JSInterface
    // a result of a RbInterface will be converted to it's rb object
    // public api
    mrb_value call(mrb_state* mrb, mrb_value js_interface_class);
    
    // private api
    void gc_callback(mrb_state* mrb, void* ptr);
    
    // get the property an js object as an JSInterface
    // expects a key of type JSInterface
    // a result of a js object will be converted to it's JSInterface
    // a result of a RbInterface will be converted to it's rb object
    // public api
    mrb_value get(mrb_state* mrb, mrb_value js_interface);
    
    // get emscripten::val of the js object that the JSInterface is pointing to
    // private api
    emscripten::val get_val(mrb_value js_interface);
    
    // get js global (window) as an JSInterface
    // public api
    mrb_value global(mrb_state* mrb, mrb_value js_interface_class);

    // private api
    mrb_value initialize(mrb_state* mrb, mrb_value js_interface);
    
    // build a js number from a rb Float
    // public api
    mrb_value number(mrb_state* mrb, mrb_value js_interface_class);
    
    // setp rb class
    // private api
    void setup();
    
    // build a js string from a rb String
    // public api
    mrb_value string_(mrb_state* mrb, mrb_value js_interface_class);
    
    // build a rb boolean from js boolean (or any js object)
    // public api
    mrb_value to_boolean(mrb_state* mrb, mrb_value js_interface);
    
    // build a rb Float from js number
    // public api
    mrb_value to_float(mrb_state* mrb, mrb_value js_interface);
    
    // build a rb String from js string
    // public api
    mrb_value to_string(mrb_state* mrb, mrb_value js_interface);
    
  }
}

#include "rb_interface.hpp"

#endif