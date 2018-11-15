#include "js_interface.hpp"


using namespace std;
using namespace emscripten;


namespace BRubyBridge
{
  namespace JSInterface
  {

    RClass* class_mrb;
    
    val js_class = val::undefined();

    // private api
    struct mrb_data_type _internal_data_type = {"BRubyBridge::JSInterface", gc_callback};
    
    // private api
    val _backward_reference_key = val::undefined();


    mrb_value build(val object_js)
    {
      mrb_value js_interface;
      bool is_not_primitive = object_js.instanceof(val::global("Object"));
      if (is_not_primitive && _backward_reference_key.in(object_js))
      {
        js_interface = object_js[_backward_reference_key].as<RbWeakReference>().get_object();
        return js_interface;
      }
      js_interface = mrb_obj_new(mrb, class_mrb, 0, NULL);
      
      // set up forward reference
      //   (rb:JSInterface -> c:mrb_value -> cpp:val -> js:object)
      val* object_js_ptr = (val*)mrb_malloc(mrb, sizeof(val));
      if (object_js_ptr == nullptr)
        BRubyBridge::js_class["OutOfMemoryError"].new_().throw_();
      new (object_js_ptr) val(object_js);
      mrb_data_init(js_interface, object_js_ptr, &_internal_data_type);
      
      // set up backward reference
      //    (js:object -> js:RbWeakReference -> c:mrb_value -> rb:JSInterface)
      if (is_not_primitive)
      {
        RbWeakReference backward_reference = RbWeakReference(js_interface);
        object_js.set(_backward_reference_key, backward_reference);
      }

      return js_interface;
    }


    mrb_value call(mrb_state* mrb, mrb_value js_interface)
    {
      mrb_value key_rb;
      mrb_value* arguments_rb;
      mrb_int arguments_rb_count;

      mrb_get_args(mrb, "o*", &key_rb, &arguments_rb, &arguments_rb_count);

      val this_js = get_val(js_interface);

      val key_js = val::undefined();
      if (mrb_obj_is_kind_of(mrb, key_rb, class_mrb))
        key_js = get_val(key_rb);
      else
        key_js = RbInterface::build(key_rb);
      
      val arguments_js = val::array();
      for (mrb_int i = 0; i < arguments_rb_count; i++)
      {
        mrb_value argument_rb = arguments_rb[i];
        val argument_js = val::undefined();
        if (mrb_obj_is_kind_of(mrb, argument_rb, class_mrb))
          argument_js = get_val(argument_rb);
        else
          argument_js = RbInterface::build(argument_rb);
        arguments_js.call<void>("push", argument_js);
      }

      val return_js = js_class.call<val>("call", this_js, key_js, arguments_js);
      
      mrb_value return_rb;
      if (return_js.instanceof(RbInterface::js_class))
        return_rb = RbInterface::get_mrb_value(return_js);
      else
        return_rb = build(return_js);
      return return_rb;
    }


    void gc_callback(mrb_state* mrb, void* ptr)
    {
      val* object_js_ptr = (val*)ptr;
      if (object_js_ptr == nullptr)
       abort("no val found in JSInterface, was ::new called directly?");
      bool is_not_primitive = object_js_ptr->instanceof(val::global("Object"));
      if (is_not_primitive)
      {
        // clear the backward reference the js object won't point to a stale JSInterface object
        object_js_ptr->delete_(_backward_reference_key);
      }
      (*object_js_ptr) = val::undefined();
      object_js_ptr->~val();
      mrb_free(mrb, object_js_ptr);
    }


    mrb_value get(mrb_state* mrb, mrb_value js_interface)
    {
      mrb_value key_rb;
      mrb_get_args(mrb, "o", &key_rb);
      
      val this_js = get_val(js_interface);

      val key_js = val::undefined();
      if (mrb_obj_is_kind_of(mrb, key_rb, class_mrb))
        key_js = get_val(key_rb);
      else
        key_js = RbInterface::build(key_rb);

      val property_js = this_js[key_js];

      mrb_value property_rb;
      if (property_js.instanceof(RbInterface::js_class))
        property_rb = RbInterface::get_mrb_value(property_js);
      else
        property_rb = build(property_js);
      return property_rb;
    }


    val get_val(mrb_value js_interface)
    {
      if (!mrb_obj_is_kind_of(mrb, js_interface, class_mrb))
        abort("can not call get_val on that ruby object, it is not a JSInterface");
      val* object_js_ptr = (val*)mrb_data_get_ptr(mrb, js_interface, &_internal_data_type);
      if (object_js_ptr == nullptr)
        abort("no val found in JSInterface, was ::new called directly?");
      return val(*object_js_ptr);
    }


    mrb_value global(mrb_state* mrb, mrb_value js_interface_class)
    {
      return build(val::global());
    }


    mrb_value initialize(mrb_state* mrb, mrb_value js_interface)
    {
      // clear any existing data
      val* object_js_ptr = (val*)mrb_data_get_ptr(mrb, js_interface, &_internal_data_type);
      if (object_js_ptr != nullptr)
      {
        mrb_free(mrb, object_js_ptr);
        mrb_data_init(js_interface, nullptr, &_internal_data_type);
      }
      return js_interface;
    }


    mrb_value number(mrb_state* mrb, mrb_value js_interface_class)
    {
      mrb_value number_rb;
      mrb_get_args(mrb, "o", &number_rb);
      if (!mrb_obj_is_kind_of(mrb, number_rb, mrb_class_get(mrb, "Float")))
        mrb_raise(mrb, E_TYPE_ERROR, "argument must must be a Float");
      
      val number_js = val(mrb_to_flo(mrb, number_rb));
      mrb_value js_interface = build(number_js);
      return js_interface;
    }

    
    void setup()
    {
      //_backward_reference_key = val::global().call<val>("Symbol");
      // for debug purposes we will use a string
      _backward_reference_key = val("_backward_reference_key");
      
      js_class = BRubyBridge::js_class["JSInterface"];

      RClass* parent_module_mrb = BRubyBridge::module_mrb;
      class_mrb = mrb_define_class_under(mrb, parent_module_mrb, "JSInterface", mrb->object_class);
      MRB_SET_INSTANCE_TT(class_mrb, MRB_TT_DATA);
      // mrb_undef_class_method(mrb,  class_mrb, "new"); (note to self) test this later
      mrb_define_class_method(mrb, class_mrb, "global", global, MRB_ARGS_NONE());
      mrb_define_class_method(mrb, class_mrb, "initialize", initialize, MRB_ARGS_NONE());
      mrb_define_class_method(mrb, class_mrb, "number", number, MRB_ARGS_REQ(1));
      mrb_define_class_method(mrb, class_mrb, "string", string_, MRB_ARGS_OPT(1));
      mrb_define_method(mrb, class_mrb, "call", call, MRB_ARGS_REQ(1)|MRB_ARGS_REST());
      mrb_define_method(mrb, class_mrb, "get", get, MRB_ARGS_REQ(1));
      mrb_define_method(mrb, class_mrb, "to_boolean", to_boolean, MRB_ARGS_NONE());
      mrb_define_method(mrb, class_mrb, "to_float", to_float, MRB_ARGS_NONE());
      mrb_define_method(mrb, class_mrb, "to_string", to_string, MRB_ARGS_NONE());
    }


    mrb_value string_(mrb_state* mrb, mrb_value js_interface_class)
    {
      mrb_value string_rb;
      mrb_int argc = mrb_get_args(mrb, "|o", &string_rb);
      
      string string_cpp = "";
      if (argc == 1)
      {
        if (!mrb_obj_is_kind_of(mrb, string_rb, mrb_class_get(mrb, "String")))
          mrb_raise(mrb, E_TYPE_ERROR, "if an argument is given it must must be a String");
        RString* string_mrb = mrb_str_ptr(string_rb);
        string_cpp = string(RSTR_PTR(string_mrb), RSTR_LEN(string_mrb));
      }
      val string_js = val(string_cpp);
      mrb_value js_interface = build(string_js);

      return js_interface;
    }
    

    mrb_value to_boolean(mrb_state* mrb, mrb_value js_interface)
    {
      val object_js = get_val(js_interface);
      bool object_cpp = !!object_js;
      mrb_value object_rb = mrb_bool_value(object_cpp);
      return object_rb;
    }
    

    mrb_value to_float(mrb_state* mrb, mrb_value js_interface)
    {
      val object_js = get_val(js_interface);
      if (!object_js.isNumber())
        mrb_raise(mrb, E_TYPE_ERROR, "js object is not a number");
      mrb_float object_cpp = object_js.as<mrb_float>();
      mrb_value object_rb = mrb_float_value(mrb, object_cpp);
      return object_rb;
    }


    mrb_value to_string(mrb_state* mrb, mrb_value js_interface)
    {
      val object_js = get_val(js_interface);
      if (!object_js.isString())
        mrb_raise(mrb, E_TYPE_ERROR, "js object is not a string");

      string object_cpp = object_js.as<string>();
      mrb_value object_rb = mrb_str_new(mrb, object_cpp.c_str(), object_cpp.length());
      return object_rb;
    }


  }
}
