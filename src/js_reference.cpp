#include "js_reference.hpp"

using namespace std;
using namespace emscripten;


namespace BRubyBridge
{
  namespace JSReference
  {
    
    struct mrb_data_type _internal_data_type = {"BRubyBridge::JSReference", gc_callback};
    
    RClass* class_mrb;

    mrb_value build(val js_object)
    {
      mrb_value js_reference = mrb_obj_new(mrb, class_mrb, 0, NULL);
      set_js_object(js_reference, js_object);
      return js_reference;
    }
    
    
    void clear(mrb_value js_reference)
    {
      if (!mrb_obj_is_kind_of(mrb, js_reference, class_mrb))
        abort("passed object is not a JSReference");
      val* js_object_ptr = (val*)DATA_PTR(js_reference);
      if (js_object_ptr)
      {
        mrb_free(mrb, js_object_ptr);
        mrb_data_init(js_reference, NULL, &_internal_data_type);
      }
    }
    
    
    void gc_callback(mrb_state* mrb, void* ptr)
    {
      printf("js_ref_gc\n");
      val* js_object_ptr = (val*)ptr;
      js_object_ptr->~val();
      mrb_free(mrb, js_object_ptr);
    }
    
    
    val get_object(mrb_value js_reference)
    {
      if (!mrb_obj_is_kind_of(mrb, js_reference, class_mrb))
        abort("passed object is not a JSReference");
      val* js_object_ptr = (val*)DATA_PTR(js_reference);
      if (js_object_ptr)
        return val(*js_object_ptr);
      else
      {
        abort("no val assigned to js reference");
        return val::undefined();
      }
    }
    
    
    mrb_value initialize(mrb_state* mrb, mrb_value js_reference)
    {
      // to avoid memory leaks
      //clear(js_reference);
      return js_reference;
    }
    
    
    void set_js_object(mrb_value js_reference, val new_js_object)
    {
      if (!mrb_obj_is_kind_of(mrb, js_reference, class_mrb))
        abort("passed object is not a JSReference");
      clear(js_reference);
      val* js_object_ptr = (val*)mrb_malloc(mrb, sizeof(val));
      if (!js_object_ptr)
      {
        // sould be fatal
        BRubyBridge::js_class["OutOfMemoryError"].new_().throw_();
        //mrb_exc_raise(mrb, mrb_obj_value(mrb->nomem_err)); // NoMemoryError
      }
      new (js_object_ptr) val(new_js_object);
      mrb_data_init(js_reference, js_object_ptr, &_internal_data_type);
    }


    void setup()
    {
      RClass* parent_module_mrb = BRubyBridge::module_mrb;
      class_mrb = mrb_define_class_under(mrb, parent_module_mrb, "JSReference", mrb->object_class);
      MRB_SET_INSTANCE_TT(class_mrb, MRB_TT_DATA);
      mrb_define_method(mrb, class_mrb, "initialize", initialize, MRB_ARGS_NONE());
    }
    
  }
}
