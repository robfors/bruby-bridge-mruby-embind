test("Object#bruby_bridge_rb_interface__get_backward_reference, Object#bruby_bridge_rb_interface__set_backward_reference(object)")
class A
end
a1 = A.new
a2 = A.new
n = 1
s = "b"
a = []
ensure_result(a1.bruby_bridge_rb_interface__get_backward_reference == nil)
ensure_result(a2.bruby_bridge_rb_interface__get_backward_reference == nil)
ensure_result(n.bruby_bridge_rb_interface__get_backward_reference == nil)
ensure_result(s.bruby_bridge_rb_interface__get_backward_reference == nil)
ensure_result(a.bruby_bridge_rb_interface__get_backward_reference == nil)
ensure_result(a1.bruby_bridge_rb_interface__set_backward_reference(2) == true)
# does not indicate a problem, but we want to ensure we test at lest one type of object
#   that will rely on an instance variable
ensure_result(a1.class.bruby_bridge_rb_interface__backward_reference_restriction == nil)
ensure_result(a2.bruby_bridge_rb_interface__set_backward_reference(3) == true)
ensure_result(n.bruby_bridge_rb_interface__set_backward_reference(4) == false)
ensure_result(s.bruby_bridge_rb_interface__set_backward_reference(5) == true)
ensure_result(a.bruby_bridge_rb_interface__set_backward_reference(6) == true)
# does not indicate a problem, but we want to ensure we test at lest one type of object
#   that will rely on a singleton method
ensure_result(a.class.bruby_bridge_rb_interface__backward_reference_restriction == :singleton_method)
ensure_result(a1.bruby_bridge_rb_interface__get_backward_reference == 2)
ensure_result(a2.bruby_bridge_rb_interface__get_backward_reference == 3)
ensure_result(n.bruby_bridge_rb_interface__get_backward_reference == nil)
ensure_result(s.bruby_bridge_rb_interface__get_backward_reference == 5)
ensure_result(a.bruby_bridge_rb_interface__get_backward_reference == 6)
ensure_result(a1.bruby_bridge_rb_interface__set_backward_reference(nil) == true)
ensure_result(a2.bruby_bridge_rb_interface__set_backward_reference(7) == true)
ensure_result(a.bruby_bridge_rb_interface__set_backward_reference(nil) == true)
ensure_result(a1.bruby_bridge_rb_interface__get_backward_reference == nil)
ensure_result(a2.bruby_bridge_rb_interface__get_backward_reference == 7)
ensure_result(a.bruby_bridge_rb_interface__get_backward_reference == nil)