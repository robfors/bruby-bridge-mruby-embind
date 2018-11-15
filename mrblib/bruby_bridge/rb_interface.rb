module BRubyBridge
  class RbInterface
    

    def self.send(object_rb, method_name_rb, *arguments_rb)
      object_rb.__send__(method_name_rb, *arguments_rb)
    end


  end
end