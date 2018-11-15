# we use singleton methods to hold a backward reference to wrapper objects
# the preferred way would be to simply set an instance variable saving it
#   but due to an mruby limitation (see: https://github.com/mruby/mruby/issues/565)
#   we can not set the instance variables of built-in objects
# try to set iv first and regress to singleton_method on error then regress to no assignment 
class Object

  def bruby_bridge_rb_interface__get_backward_reference
    restriction = self.class.bruby_bridge_rb_interface__backward_reference_restriction
    case restriction
    when :unavailable
      nil
    when :singleton_method
      return nil unless respond_to?(:bruby_bridge_rb_interface__backward_reference_method)
      bruby_bridge_rb_interface__backward_reference_method
    else # use instance variable
      @bruby_bridge_rb_interface__backward_reference
    end
  end
  
  def bruby_bridge_rb_interface__set_backward_reference(object)
    restriction = self.class.bruby_bridge_rb_interface__backward_reference_restriction
    case restriction
    when :unavailable
      false
    when :singleton_method
      if respond_to?(:bruby_bridge_rb_interface__backward_reference_method)
        self.singleton_class.send(:undef_method, :bruby_bridge_rb_interface__backward_reference_method)
      end
      if object == nil
        true
      else
        begin
          self.define_singleton_method(:bruby_bridge_rb_interface__backward_reference_method) { object }
          true
        rescue TypeError
          self.class.bruby_bridge_rb_interface__backward_reference_restriction = :unavailable
          bruby_bridge_rb_interface__set_backward_reference(object) #retry with new restriction
        end
      end
    else # assume we can use an instance variable
      begin
        @bruby_bridge_rb_interface__backward_reference = object
        true
      rescue ArgumentError
        self.class.bruby_bridge_rb_interface__backward_reference_restriction = :singleton_method
        bruby_bridge_rb_interface__set_backward_reference(object) #retry with new restriction
      end
    end
  end
  
end