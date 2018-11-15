#ifndef _BRUBY_BRIDGE_RB_INTERFACE_HPP_
#define _BRUBY_BRIDGE_RB_INTERFACE_HPP_


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
#include "js_interface.hpp"
#include "js_reference.hpp"

namespace BRubyBridge
{  
  // RbInterface js instances point to a rb object
  // we can use them to call send on the rb object
  class RbInterface
  {

    public:

    static RClass* class_mrb;
    static emscripten::val js_class;

    static emscripten::val build(mrb_value object_rb);
    static emscripten::val float_(emscripten::val float_js);
    static emscripten::val get_object_class();
    static mrb_value get_mrb_value(emscripten::val object_js);
    static void setup();
    static emscripten::val string_(emscripten::val string_js = emscripten::val(""));

    RbInterface();
    ~RbInterface();
    
    emscripten::val send(emscripten::val method_name_js, emscripten::val arguments_js) const;
    emscripten::val to_boolean() const;
    emscripten::val to_number() const;
    emscripten::val to_string() const;

    private:

    mrb_value _object_rb;
    
  };
}
#endif
