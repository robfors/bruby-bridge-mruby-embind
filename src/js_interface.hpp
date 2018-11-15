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
  
  namespace JSInterface
  {
    extern RClass* class_mrb;
    
    mrb_value build(emscripten::val object_js);
    mrb_value call(mrb_state* mrb, mrb_value js_interface_class);
    void gc_callback(mrb_state* mrb, void* ptr);
    mrb_value global(mrb_state* mrb, mrb_value js_interface_class);
    mrb_value get(mrb_state* mrb, mrb_value js_interface);
    emscripten::val get_val(mrb_value js_interface);
    mrb_value initialize(mrb_state* mrb, mrb_value js_interface);
    mrb_value number(mrb_state* mrb, mrb_value js_interface_class);
    void setup();
    mrb_value string_(mrb_state* mrb, mrb_value js_interface_class);
    mrb_value to_boolean(mrb_state* mrb, mrb_value js_interface);
    mrb_value to_float(mrb_state* mrb, mrb_value js_interface);
    mrb_value to_string(mrb_state* mrb, mrb_value js_interface);
    
  }
}

#include "rb_interface.hpp"

#endif