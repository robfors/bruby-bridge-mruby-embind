#include "rb_weak_reference.hpp"

using namespace emscripten;

namespace BRubyBridge
{
  
  RbWeakReference::RbWeakReference(mrb_value rb_object)
    : _mrb_value_ptr(mrb_ptr(rb_object))
    //: _rb_object(rb_object)
  {
  }
  

  mrb_value RbWeakReference::get_object() const
  {
    return mrb_obj_value(_mrb_value_ptr);
    //return _rb_object;
  }
  
  
  EMSCRIPTEN_BINDINGS(rb_weak_reference)
  {
    class_<RbWeakReference>("RbWeakReference");
  }

}