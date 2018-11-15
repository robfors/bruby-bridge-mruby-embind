#include "rb_interface.hpp"


using namespace std;
using namespace emscripten;


namespace BRubyBridge
{

  RClass* RbInterface::class_mrb;
  val RbInterface::js_class = val::undefined();
  

  // don't call directly
  RbInterface::RbInterface()
  {
  }


  RbInterface::~RbInterface()
  {
    mrb_gc_unregister(mrb, _object_rb);
  }

  
  val RbInterface::build(mrb_value object_rb)
  {
    mrb_value js_reference = mrb_funcall_argv(mrb, object_rb, mrb_intern_lit(mrb, "bruby_bridge_rb_interface__get_backward_reference"), 0, NULL);
    val rb_interface = val::undefined();
    if (mrb_nil_p(js_reference))
    {
      mrb_gc_register(mrb, object_rb);
      RbInterface interface_cpp;
      

      // build foward reference
      //  (js:RbInterface -> cpp:RbInterface -> c:mrb_value -> rb:object)
      interface_cpp._object_rb = object_rb;

      // build backward reference
      //   (rb:object -> rb:JSReference -> cpp:val -> js:RbInterface)
      rb_interface = val(interface_cpp); // call this after setting _object_rb to avoid unexpected behavior
      mrb_value js_reference = JSReference::build(rb_interface);
      mrb_funcall_argv(mrb, object_rb, mrb_intern_lit(mrb, "bruby_bridge_rb_interface__set_backward_reference"), 1, &js_reference);
    }
    else
      rb_interface = JSReference::get_object(js_reference);
    return rb_interface;
  }
  

  val RbInterface::float_(val float_js)
  {
    if (!float_js.isNumber())
      val::global("TypeError").new_(val("object must be a number")).throw_();
    mrb_float float_cpp = float_js.as<mrb_float>();
    mrb_value float_rb = mrb_float_value(mrb, float_cpp);
    return build(float_rb);
  }


  mrb_value RbInterface::get_mrb_value(val object_js)
  {
    if (!object_js.instanceof(js_class))
      val::global("TypeError").new_(val("object must be of type RbInterface")).throw_();
    return object_js.as<RbInterface>()._object_rb;
  }


  val RbInterface::get_object_class()
  {
    return build(mrb_obj_value(mrb->object_class));
  }


  val RbInterface::send(val method_name_js, val arguments_js) const
  {
    if (!method_name_js.isString())
      val::global("TypeError").new_(val("method name must be a string")).throw_();
    std::string method_name_cpp = method_name_js.as<std::string>();
    if (method_name_cpp.empty())
      BRubyBridge::js_class["ArgumentError"].new_(val("method name must not be empty")).throw_();
    if (!arguments_js.isArray())
      val::global("TypeError").new_(val("arguments must be an array")).throw_();
    val argument_js = val::undefined();
    mrb_int argument_count = arguments_js["length"].as<mrb_int>();
    vector<mrb_value> arguments_rb;
    arguments_rb.push_back(_object_rb);
    arguments_rb.push_back(get_mrb_value(string_(method_name_js)));
    for (mrb_int i = 0; i < argument_count; i++)
    {
      argument_js = arguments_js.call<val>("shift");
      if (argument_js.instanceof(js_class))
        arguments_rb.push_back(get_mrb_value(argument_js));
      else
        arguments_rb.push_back(JSInterface::build(argument_js));
    }
    
    if (mrb->exc)
      printf("warn-before!!!!!!!!!!!!!!!!\n");
    mrb_value return_rb = mrb_funcall_argv(mrb, mrb_obj_value(class_mrb), mrb_intern_lit(mrb, "send"), arguments_rb.size(), arguments_rb.data());
    //mrb_value return_rb = mrb_funcall_argv(mrb, _object_rb, mrb_intern_lit(mrb, "__send__"), arguments_cpp_rb.size(), arguments_cpp_rb.data());
    //mrb_value return_rb = mrb_funcall_argv(mrb, _object_rb, mrb_intern_cstr(mrb, method_name_cpp.c_str()), arguments_cpp_rb.size(), arguments_cpp_rb.data());
    if (mrb->exc)
      printf("warn-after!!!!!!!!!!!!!!!!\n");

    val return_js = val::undefined();
    if (mrb_obj_is_kind_of(mrb, return_rb, JSInterface::class_mrb))
      return_js = JSInterface::get_val(return_rb);
    else
      return_js = build(return_rb);
    return return_js;
  }


  val RbInterface::string_(val string_js)
  {
    if (!string_js.isString())
      val::global("TypeError").new_(val("object must be a string")).throw_();
    string string_cpp = string_js.as<string>();
    mrb_value string_rb = mrb_str_new(mrb, string_cpp.c_str(), string_cpp.length());
    return build(string_rb);
  }


  void RbInterface::setup()
  {
    RClass* parent_module_mrb = BRubyBridge::module_mrb;
    class_mrb = mrb_define_class_under(mrb, parent_module_mrb, "RbInterface", mrb->object_class);
    js_class = BRubyBridge::js_class["RbInterface"];
  }


  val RbInterface::to_boolean() const
  {
    return val(mrb_bool(_object_rb));
  }


  val RbInterface::to_number() const
  {
    if (!mrb_obj_is_kind_of(mrb, _object_rb, mrb_class_get(mrb, "Numeric")))
      val::global("TypeError").new_(val("object must be a Numeric")).throw_();
    return val(mrb_to_flo(mrb, _object_rb));
  }


  val RbInterface::to_string() const
  {
    if (!mrb_obj_is_kind_of(mrb, _object_rb, mrb_class_get(mrb, "String")))
      val::global("TypeError").new_(val("object must be a String")).throw_();
    struct RString* string_mrb = mrb_str_ptr(_object_rb);
    string string_cpp = string(RSTR_PTR(string_mrb), RSTR_LEN(string_mrb));
    val string_js = val(string_cpp);
    return string_js;
  }
  

  EMSCRIPTEN_BINDINGS(rb_interface)
  {
    class_<RbInterface>("RbInterface")
      .class_function("float", &RbInterface::float_)
      .class_function("get_object_class", &RbInterface::get_object_class)
      .class_function("string", &RbInterface::string_)
      .function("send", &RbInterface::send)
      .function("to_boolean", &RbInterface::to_boolean)
      .function("to_number", &RbInterface::to_number)
      .function("to_string", &RbInterface::to_string)
      ;
  }

}