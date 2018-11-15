JSInterface =  BRubyBridge::JSInterface

puts "start-js_value"

test("JSInterface::global")
#
ensure_js_result("v0 === global", JSInterface.global)
#
ensure_js_result_not("v0 == {}", JSInterface.global)


test("JSInterface::number")
#
ensure_js_result("v0 === 0", JSInterface.number(0.0))
ensure_js_result_not("v0 === '0'", JSInterface.number(0.0))
#
ensure_js_result("v0 === 1", JSInterface.number(1.0))
#
ensure_js_result("v0 === 1.5", JSInterface.number(1.5))
ensure_js_result_not("v0 === 1", JSInterface.number(1.5))
ensure_js_result_not("v0 === 2", JSInterface.number(1.5))
#
ensure_js_result("v0 === -2", JSInterface.number(-2.0))
#
ensure_js_result("v0 === -2.5", JSInterface.number(-2.5))


test("JSInterface::string")
#
ensure_js_result("v0 === ''", JSInterface.string)
#
ensure_js_result("v0 === ''", JSInterface.string(""))
#
ensure_js_result("v0 === 'b'", JSInterface.string("b"))
#
ensure_js_result("v0 === 'abc'", JSInterface.string("abc"))


test("JSInterface#call")
# passing js argument should be valid
a1 = js_eval("2")
a2 = js_eval("({})")
js_eval("global.f = function(a1, a2) { global.a1 = a1; global.a2 = a2; }")
js_eval('global').call(js_eval("'f'"), a1, a2)
ensure_js_result("global.a1 === v0 && global.a2 === v1", a1, a2)
js_eval("delete global.f")
js_eval("delete global.a1")
js_eval("delete global.a2")
a1, a2 = nil
# passing rb argument should of type RbInterface in js
c = Class.new
js_eval("global.f = function(a1, a2) { global.a1 = a1; global.a2 = a2; }")
js_eval('global').call(js_eval("'f'"), c, c.new)
ensure_js_result("global.a1 instanceof BRubyBridge.RbInterface")
ensure_js_result("global.a2 instanceof BRubyBridge.RbInterface")
js_eval("delete global.f")
js_eval("delete global.a1")
js_eval("delete global.a2")
c = nil
# passing same rb argument again should result in same RbInterface
i = Class.new.new
js_eval("global.f = function(a1, a2) { global.a1 = a1; global.a2 = a2; }")
js_eval('global').call(js_eval("'f'"), i, i)
ensure_js_result("global.a1 === global.a2")
js_eval("delete global.f")
js_eval("delete global.a1")
js_eval("delete global.a2")
i = nil
# passing different rb argument should result in different RbInterface
c = Class.new
i1 = c.new
i2 = c.new
js_eval("global.f = function(a1, a2) { global.a1 = a1; global.a2 = a2; }")
js_eval('global').call(js_eval("'f'"), i1, i2)
ensure_js_result_not("global.a1 === global.a2")
ensure_js_result_not("global.a1 == global.a2")
js_eval("delete global.f")
js_eval("delete global.a1")
js_eval("delete global.a2")
c, i1, i2 = nil
# returning js value should be valid
r = js_eval("({})")
js_eval("global.f = function() { return v0; }", r)
ensure_js_result("v0 === v1", js_eval('global').call(js_eval("'f'")), r)
js_eval("delete global.f")
r = nil
# returning rb value should be valid
v = Object.new
js_eval("global.f = function() { return v0; }", v)
r = js_eval('global').call(js_eval("'f'"))
ensure_result( v.equal?(r) )
js_eval("delete global.f")
v, r = nil
# returning same RbInterface again should result in same rb value
v = Object.new
js_eval("global.f = function() { return v0; }", v)
r1 = js_eval('global').call(js_eval("'f'"))
r2 = js_eval('global').call(js_eval("'f'"))
ensure_result( v.equal?(r1) )
ensure_result( r1.equal?(r2) )
js_eval("delete global.f")
v, r1, r2 = nil
# returning different RbInterface should result in different rb value
v1 = Object.new
js_eval("global.f = function() { return v0; }", v1)
r1 = js_eval('global').call(js_eval("'f'"))
ensure_result( r1.equal?(v1) )
js_eval("delete global.f")
v2 = Object.new
js_eval("global.f = function() { return v0; }", v2)
r2 = js_eval('global').call(js_eval("'f'"))
ensure_result( r2.equal?(v2) )
js_eval("delete global.f")
ensure_result_not( r1.equal?(r2) )
ensure_result_not( r1 == r2 )
v1, v2, r1, r2 = nil
# no arguments to js function, key is a rb String
js_eval("global.f = function(...a) { return a; }")
ensure_js_result("v0 instanceof Array && v0.length == 0", js_eval('global').call(js_eval("'f'")))
js_eval("delete global.f")
# key is a js object
k = js_eval("Symbol()")
js_eval("global[v0] = function(...a) { return a; }", k)
ensure_js_result("v0 instanceof Array && v0.length == 0", js_eval('global').call(k))
js_eval("delete global[v0]", k)
k = nil
# single argument passed to js function
a = js_eval("2")
js_eval("global.f = function(...a) { return a; }")
r = js_eval('global').call(js_eval("'f'"), a)
ensure_js_result("v0 instanceof Array && v0.length == 1 && v0[0] === v1", r, a)
js_eval("delete global.f")
a, r = nil
# multiple arguments passed to js function
a1 = js_eval("2")
a2 = js_eval("'3'")
a3 = js_eval("({a: 2})")
js_eval("global.f = function(...a) { return a; }")
r = js_eval('global').call(js_eval("'f'"), a1, a2, a3)
ensure_js_result("v0 instanceof Array && v0.length == 3", r)
ensure_js_result("v0[0] === v1 && v0[1] === v2 && v0[2] == v3", r, a1, a2, a3)
js_eval("delete global.f")
a1, a2, a3, r = nil
# ensure that passing the same JSInterface again will result in the same js object
a = js_eval("({})")
js_eval("global.v = []")
js_eval("global.f = function(a) { global.v.push(a); }")
js_eval('global').call(js_eval("'f'"), a)
js_eval('global').call(js_eval("'f'"), a)
ensure_js_result("v[0] === v[1]")
js_eval("delete global.v")
js_eval("delete global.f")
a = nil
# ensure that passing a different JSInterface will result in a different js object
js_eval("global.v = []")
js_eval("global.f = function(a) { global.v.push(a); }")
js_eval('global').call(js_eval("'f'"), js_eval("({})"))
js_eval('global').call(js_eval("'f'"), js_eval("({})"))
ensure_js_result_not("v[0] == v[1]")
js_eval("delete global.v")
js_eval("delete global.f")
# ensure that returning the same js object again will result in the same JSInterface
js_eval("global.o = {}")
js_eval("global.f = function() { return global.o; }")
ensure_result( js_eval('global').call(js_eval("'f'")).equal?(js_eval('global').call(js_eval("'f'"))) )
js_eval("delete global.o")
js_eval("delete global.f")
# ensure that returning a different js object will result in a different JSInterface
js_eval("global.f = function() { return {}; }")
ensure_result_not( js_eval('global').call(js_eval("'f'")).equal?(js_eval('global').call(js_eval("'f'"))) )
js_eval("delete global.f")


test("JSInterface#get")
# key is String
o = js_eval("({a: 3})")
ensure_js_result("v0 == 3", o.get(js_eval("'a'")))
o = nil
# key is JSInterface
k = js_eval("Symbol()")
o = js_eval("({[v0]: 3})", k)
ensure_js_result("v0 == 3", o.get(k))
k, o = nil
# return null
o = js_eval("({a: null})")
ensure_js_result("v0 == null", o.get(js_eval("'a'")))
o = nil
# return undefined
o = js_eval("({a: undefined})")
ensure_js_result("v0 == undefined", o.get(js_eval("'a'")))
o = nil
# return rb object
v = Object.new
o = js_eval("({a: v0})", v)
ensure_result( o.get(js_eval("'a'")).equal?(v))
v, o = nil
# get non existent property
o = js_eval("({})")
ensure_js_result("v0 == undefined", o.get(js_eval("'a'")))
o = nil
# passing two empty js objects should result in the same key
k1 = js_eval("({})")
k2 = js_eval("({})")
o = js_eval("({})")
js_eval("v0[v1] = 3", o, k1)
js_eval("v0[v1] = 5", o, k2)
ensure_js_result("v0 == 5", o.get(k1))
ensure_js_result("v0 == 5", o.get(k2))
k, o = nil
# ensure that passing the same JSInterface again will result in the same js object
js_eval("global.k = Symbol()")
o = js_eval("({[global.k]: 3})")
k = js_eval("k")
ensure_js_result("v0 == 3", o.get(k))
ensure_js_result("v0 == 3", o.get(k))
js_eval("delete global.k")
k, o = nil
# ensure that passing a different JSInterface will result in different js objects
js_eval("global.k1 = Symbol()")
js_eval("global.k2 = Symbol()")
o = js_eval("({[global.k1]: 3, [global.k2]: 5})")
k1 = js_eval("global.k1")
k2 = js_eval("global.k2")
ensure_js_result("v0 == 3", o.get(k1))
ensure_js_result("v0 == 5", o.get(k2))
js_eval("delete global.k1")
js_eval("delete global.k2")
k1, k2, o = nil
# ensure that returning the same js object again will result in the same JSInterface
js_eval("global.p = {}")
o = js_eval("({a: global.p})")
p = js_eval("global.p")
ensure_js_result("v0 === global.p", p)
ensure_js_result("v0 === global.p", o.get(js_eval("'a'")))
ensure_js_result("v0 === global.p", o.get(js_eval("'a'")))
ensure_js_result("v0 === global.p", o.get(js_eval("'a'")))
js_eval("delete global.p")
o, p = nil
# ensure that returning a different js object will result in different JSInterfaces
js_eval("global.p1 = {}")
js_eval("global.p2 = {}")
o = js_eval("({a: global.p1, b: global.p2})")
ensure_js_result("v0 === global.p1", o.get(js_eval("'a'")))
ensure_js_result("v0 === global.p2", o.get(js_eval("'b'")))
js_eval("delete global.p1")
js_eval("delete global.p2")
o = nil


test("JSInterface#to_boolean")
#
ensure_result( js_eval("false").to_boolean == false )
#
ensure_result( js_eval("true").to_boolean == true )
#
ensure_result( js_eval("undefined").to_boolean == false )
#
ensure_result( js_eval("null").to_boolean == false )
#
ensure_result( js_eval("0").to_boolean == false )
#
ensure_result( js_eval("'0'").to_boolean == true )


test("JSInterface#to_float")
#
ensure_result( js_eval("0").to_float == 0 )
#
ensure_result( js_eval("0.5").to_float == 0.5 )
#
ensure_result( js_eval("1").to_float == 1 )
#
ensure_result( js_eval("-0.5").to_float == -0.5 )
#
ensure_result( js_eval("-0.5").to_float != -1.5 )


test("JSInterface#to_string")
#
ensure_result( js_eval("'0'").to_string == "0" )
#
ensure_result( js_eval("'-0.5'").to_string == "-0.5" )
#
ensure_result( js_eval("'a'").to_string == "a" )
#
ensure_result( js_eval("'abc'").to_string == "abc" )


puts("end-js_value")