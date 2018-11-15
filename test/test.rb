puts "start test..."

def fail
  puts "------ FAIL ------"
  nil
end

def pass
  puts "pass"
  nil
end

def test(message)
  puts "test: #{message}"
  nil
end

def ensure_result(result)
  if !!result
    pass
  else
    fail
  end
  nil
end

def ensure_result_not(result)
  ensure_result(!result)
end

def ensure_raise(error_class)
  begin
    yield
  rescue error_class
    pass
  else
    fail
  end
end


def ensure_js_result(js_code, *js_values)
  js_code = "(" + js_code + ")";
  ensure_result(js_eval(js_code, *js_values).to_boolean)
end


def ensure_js_result_not(js_code, *js_values)
  js_code.insert(0, "!(")
  js_code.insert(-1, ")")
  ensure_js_result(js_code, *js_values)
end


def js_eval(js_code, *values)
  raise TypeError, "js code must be a String" unless js_code.is_a?(String)
  key_js = BRubyBridge::JSInterface.string("js_eval")
  js_code_js = BRubyBridge::JSInterface.string(js_code)
  global_js = BRubyBridge::JSInterface.global
  global_js.call(key_js, js_code_js, *values)
end


def rb_eval(rb_code, *values)
  value_names = values.map.with_index { |value, index| "v#{index}" }
  proc = eval("Proc.new { |rb_code, #{value_names.join(', ')}| eval(rb_code) }")
  r = proc.call(rb_code, *values)
end