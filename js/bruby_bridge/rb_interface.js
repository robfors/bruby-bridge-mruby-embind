(function(){

  BRubyBridge.RbInterface = Module.RbInterface;

  var RbInterface = BRubyBridge.RbInterface;


  Object.defineProperty(RbInterface, 'Object', { get: function() {
    return this._Object = this._Object || this.get_object_class();
  } });


  var old_string_method = RbInterface.string;
  RbInterface.string = function(string_js = "")
  {
    return old_string_method.bind(this)(string_js);
  };

  
  var old_send_method = RbInterface.prototype.send;
  RbInterface.prototype.send = function(method_name_js, ...arguments_js)
  {
    return old_send_method.bind(this)(method_name_js, arguments_js);
  };
  

})();




//ESRubyInterface.RbValue.eval(code)
//{
//  return this.kernel.send("eval", [code]);
//}