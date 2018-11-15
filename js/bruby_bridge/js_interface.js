BRubyBridge.JSInterface = class
{

  
  static call(this_object, key, args)
  {
    return this_object[key](...args);
  };


};