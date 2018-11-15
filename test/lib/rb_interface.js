function test_rb_value()
{
  
  var RbInterface = BRubyBridge.RbInterface;

  
  console.log("start-rb_value");


  test("RbInterface.float");
  //
  ensure_rb_result("v0 == 0", RbInterface.float(0));
  //
  ensure_rb_result("v0 == 1", RbInterface.float(1));
  //
  ensure_rb_result("v0 == 1", RbInterface.float(1.0));
  //
  ensure_rb_result("v0 == 1.5", RbInterface.float(1.5));


  test("RbInterface.Object");
  //
  ensure_rb_result("v0.equal?(Object)", RbInterface.Object);
  //
  ensure_rb_result_not("v0 == Object.new", RbInterface.Object);
  //
  ensure_rb_result_not("v0 == Kernel", RbInterface.Object);
  

  test("RbInterface.string");
  // no argument should make empty string
  ensure_rb_result("v0 == ''", RbInterface.string());
  // pass empty string
  ensure_rb_result("v0 == ''", RbInterface.string(''));
  // pass single character
  ensure_rb_result("v0 == 'a'", RbInterface.string("a"));
  // pass multiple characters
  ensure_rb_result("v0 == 'abc'", RbInterface.string("abc"));
  // 
  ensure_rb_result("v0 == '0'", RbInterface.string("0"));


  test("RbInterface.prototype.send");
  // passing rb arguments should be valid
  global.a1 = rb_eval("2");
  global.a2 = rb_eval("{}");
  ensure_result( global.a1 instanceof BRubyBridge.RbInterface );
  ensure_result( global.a2 instanceof BRubyBridge.RbInterface );
  rb_eval("define_method(:m) { |a1, a2| $a1, $a2 = a1, a2; }");
  rb_eval("Object").send("m", global.a1, global.a2);
  ensure_rb_result("$a1.equal?(v0) && $a2.equal?(v1)", global.a1, global.a2);
  rb_eval("Object.undef_method(:m)");
  rb_eval("$a1, $a2 = nil");
  delete global.a1;
  delete global.a2;
  // passing js arguments should be of JSInterface type
  global.c = class {};
  rb_eval("define_method(:m) { |a1, a2| $a1, $a2 = a1, a2; }");
  rb_eval("Object").send("m", global.c, new global.c);
  ensure_rb_result("$a1.is_a?(BRubyBridge::JSInterface) && $a2.is_a?(BRubyBridge::JSInterface)");
  rb_eval("Object.undef_method(:m)");
  rb_eval("$a1, $a2 = nil");
  delete global.c;
  // passing same js argument again should result in same JSInterface
  global.c = class {};
  global.i = new c;
  rb_eval("define_method(:m) { |a1, a2| $a1, $a2 = a1, a2; }");
  rb_eval("Object").send("m", global.i, global.i);
  ensure_rb_result("$a1.equal?($a2)");
  ensure_rb_result("$a1 == $a2");
  rb_eval("Object.undef_method(:m)");
  rb_eval("$a1, $a2 = nil");
  delete global.i;
  delete global.c;
  // passing different js argument should result in different JSInterface
  global.c = class {};
  rb_eval("define_method(:m) { |a1, a2| $a1, $a2 = a1, a2; }");
  rb_eval("Object").send("m", new global.c, new global.c);
  ensure_rb_result_not("$a1.equal?($a2)");
  ensure_rb_result_not("$a1 == $a2");
  rb_eval("Object.undef_method(:m)");
  rb_eval("$a1, $a2 = nil");
  delete global.c;
  // returning rb value is valid
  global.v = rb_eval("Object.new");
  rb_eval("define_method(:m) { v0; }", global.v);
  global.r = rb_eval("Object").send("m");
  ensure_result( global.r instanceof BRubyBridge.RbInterface );
  ensure_rb_result("v0.equal?(v1)", global.r, global.v)
  rb_eval("Object.undef_method(:m)");
  delete global.v;
  delete global.r;
  // returning js value should be valid
  global.v = {};
  rb_eval("define_method(:m) { puts v0.inspect; v0; }", global.v);
  global.r = rb_eval("Object").send("m");
  ensure_result( global.r === global.v );
  rb_eval("Object.undef_method(:m)");
  delete global.v;
  delete global.r;
  // returning same JSInterface again should result in same js value
  global.v = {};
  rb_eval("define_method(:m) { v0; }", global.v);
  global.r1 = rb_eval("Object").send("m");
  global.r2 = rb_eval("Object").send("m");
  ensure_result( global.r1 === global.v );
  ensure_result( global.r2 === global.v );
  rb_eval("Object.undef_method(:m)");
  delete global.v;
  delete global.r1;
  delete global.r2;
  // returning different JSInterface should result in different js value
  global.v1 = {};
  rb_eval("define_method(:m) { v0; }", global.v1);
  global.r1 = rb_eval("Object").send("m");
  rb_eval("Object.undef_method(:m)");
  ensure_result( global.r1 === global.v1 );
  global.v2 = {};
  rb_eval("define_method(:m) { v0; }", global.v2);
  global.r2 = rb_eval("Object").send("m");
  rb_eval("Object.undef_method(:m)");
  ensure_result( global.r2 === global.v2 );
  ensure_result_not( global.r1 === global.r2 );
  delete global.v1;
  delete global.v2;
  delete global.r1;
  delete global.r2;
  // no arguments passed to rb method
  rb_eval("define_method(:m) { |*a| a; }");
  ensure_rb_result("v0 == []", rb_eval("Object").send("m"));
  rb_eval("Object.undef_method(:m)");
  // single argument passed to rb method
  global.a = rb_eval("2")
  rb_eval("define_method(:m) { |*a| a; }");
  ensure_rb_result("v0 == [v1]", rb_eval("Object").send("m", global.a), global.a);
  rb_eval("Object.undef_method(:m)");
  delete global.a
  // multiple arguments passed to rb method
  global.a1 = rb_eval("'2'")
  global.a2 = rb_eval("Object")
  global.a3 = rb_eval("Object.new")
  rb_eval("define_method(:m) { |*a| a; }");
  global.r = rb_eval("Object").send("m", global.a1, global.a2, global.a3)
  ensure_rb_result("v0 == [v1, v2, v3]", global.r, global.a1, global.a2, global.a3);
  rb_eval("Object.undef_method(:m)");
  delete global.a1
  delete global.a2
  delete global.a3
  delete global.r
  // ensure that passing the same RbInterface again will result in the same rb object
  global.a = rb_eval("Object.new")
  rb_eval("$v = []")
  rb_eval("define_method(:m) { |a| $v << a; }");
  rb_eval("Object").send("m", global.a)
  rb_eval("Object").send("m", global.a)
  ensure_rb_result("$v[0].equal?($v[1])")
  rb_eval("$v = nil")
  rb_eval("Object.undef_method(:m)");
  delete global.a
  // ensure that passing a different RbInterface will result in a different rb object
  rb_eval("$v = []")
  rb_eval("define_method(:m) { |a| $v << a; }");
  rb_eval("Object").send("m", rb_eval("Object.new"))
  rb_eval("Object").send("m", rb_eval("Object.new"))
  ensure_rb_result_not("$v[0].equal?($v[1])")
  ensure_rb_result_not("$v[0] == $v[1]")
  rb_eval("$v = nil")
  rb_eval("Object.undef_method(:m)");
  // ensure that returning the same rb object again will result in the same RbInterface
  rb_eval("$r = Object.new")
  rb_eval("define_method(:m) { $r }");
  ensure_result( rb_eval("Object").send("m") === rb_eval("Object").send("m") )
  rb_eval("$r = nil")
  rb_eval("Object.undef_method(:m)");
  // ensure that returning a different rb object will result in a different RbInterface
  rb_eval("define_method(:m) { Object.new }");
  ensure_result_not( rb_eval("Object").send("m") === rb_eval("Object").send("m") )
  ensure_result_not( rb_eval("Object").send("m") == rb_eval("Object").send("m") )
  rb_eval("Object.undef_method(:m)");
  

  test("RbInterface.prototype.to_boolean");
  //
  ensure_result( rb_eval("true").to_boolean() === true );
  //
  ensure_result_not( rb_eval("true").to_boolean() === 1 );
  //
  ensure_result_not( rb_eval("true").to_boolean() === false );
  //
  ensure_result( rb_eval("false").to_boolean() === false );
  //
  ensure_result( rb_eval("0").to_boolean() === true );
  //
  ensure_result( rb_eval("nil").to_boolean() === false );


  test("RbInterface.prototype.to_number");
  //
  ensure_result( rb_eval("0").to_number() === 0 );
  //
  ensure_result( rb_eval("1").to_number() === 1 );
  //
  ensure_result(rb_eval("-1.5").to_number() === -1.5 );
  

  test("RbInterface.prototype.to_string");
  //
  ensure_result( rb_eval("''").to_string() === '' );
  //
  ensure_result( rb_eval("'a'").to_string() === 'a' );
  //
  ensure_result( rb_eval("'abc'").to_string() === 'abc' );
  //
  ensure_result( rb_eval("'0'").to_string() === '0' );


  console.log("end-rb_value");
}