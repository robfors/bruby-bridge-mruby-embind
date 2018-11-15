#include "main.hpp"

using namespace std;
using namespace emscripten;


namespace BRubyBridge
{
  
  mrb_state* mrb;
  RClass* module_mrb;
  val js_class = val::undefined();
  
  void abort(string message)
  {
    //val::global()["BRubyBridge"]["FatalError"].new_(val(message)).throw_();
    printf("fatal error: %s\n", message.c_str());
    exit(1);
    // TODO: set EXIT_RUNTIME=1
  }


  void finalize()
  {
  }


  void initialize(mrb_state* mrb)
  {
    BRubyBridge::mrb = mrb;
    js_class = val::global()["BRubyBridge"];
    
    module_mrb = mrb_define_module(mrb, "BRubyBridge");

    JSReference::setup();
    JSInterface::setup();
    RbInterface::setup();
  }
  
}


void mrb_bruby_bridge_mruby_embind_gem_final(mrb_state* mrb)
{
  return BRubyBridge::finalize();
}


void mrb_bruby_bridge_mruby_embind_gem_init(mrb_state* mrb)
{
  return BRubyBridge::initialize(mrb);
}