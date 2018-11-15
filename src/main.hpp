#ifndef _BRUBY_BRIDGE_MAIN_HPP_
#define _BRUBY_BRIDGE_MAIN_HPP_


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
#include <new>

#include <esruby.hpp>


namespace BRubyBridge
{
  
  extern mrb_state* mrb;
  extern RClass* module_mrb;
  extern emscripten::val js_class;
  
  void abort(std::string message);
  void finalize();
  void initialize(mrb_state* mrb);
      
}


extern "C"
void mrb_bruby_bridge_mruby_embind_gem_final(mrb_state* mrb);

extern "C"
void mrb_bruby_bridge_mruby_embind_gem_init(mrb_state* mrb);


#include "js_interface.hpp"
#include "rb_interface.hpp"

#endif

