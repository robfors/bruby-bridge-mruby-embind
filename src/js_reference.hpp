#ifndef _BRUBY_BRIDGE_JS_REFERENCE_HPP_
#define _BRUBY_BRIDGE_JS_REFERENCE_HPP_


#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
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
  // JSReference exposes a reference (hopefully one day, weak) of a js object to rb
  // we can use them set backward references to RbInterfaces
  namespace JSReference
  {
    extern RClass* class_mrb;
    
    mrb_value build(emscripten::val js_object);
    void clear(mrb_value js_reference);
    void gc_callback(mrb_state* mrb, void* ptr);
    emscripten::val get_object(mrb_value js_reference);
    mrb_value initialize(mrb_state* mrb, mrb_value js_reference);
    void set_js_object(mrb_value js_reference, emscripten::val new_js_object);
    void setup();
    
  }
}

#endif