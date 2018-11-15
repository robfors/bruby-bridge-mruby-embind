if (typeof global == 'undefined')
  window.global = window;


test = function(message)
{
  console.log("test: " + message)
};


fail = function()
{
  console.log("------ FAIL ------");
};


pass = function()
{
  console.log("pass");
};


ensure_raise = function(error_class, func)
{
  try
  {
    func();
    fail();
  }
  catch(error)
  {
    if (error instanceof error_class)
      pass();
    else
      fail();
  }
};


ensure_result = function(result)
{
  if (!!result)
    pass();
  else
    fail();
};


ensure_result_not = function(result)
{
  ensure_result(!result);
};


ensure_rb_result = function(rb_code, ...values)
{
  ensure_result(rb_eval(rb_code, ...values).to_boolean());
};


ensure_rb_result_not = function(rb_code, ...values)
{
  rb_code = "!( " + rb_code + " )";
  ensure_rb_result(rb_code, ...values);
};


js_eval = function(js_code, ...values)
{
  var value_names = values.map((_, index) => "v" + index);
  var f_args = ['js_code', ...value_names, "return eval(js_code)"];
  var f = new Function(...f_args);
  return f.bind(window)(js_code, ...values);
};


rb_eval = function(rb_code, ...values)
{
  var object_class_rb = BRubyBridge.RbInterface.Object;
  var rb_code_rb = BRubyBridge.RbInterface.string(rb_code);
  var result_rb = object_class_rb.send("rb_eval", rb_code_rb, ...values);
  return result_rb;
};