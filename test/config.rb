# This file sets up the build environment for a webruby project.
ESRuby::Build.new do |conf|

  # set the path of the esruby project
  # all other paths specified in this config file will be expanded
  #   relative to this path
  conf.project_directory = "#{File.dirname(__FILE__)}/temporary"

  conf.mruby_directory = '../../../../../mruby'
  
  # list as many ruby source files as you want
  # keep in mind they will be executed in the order you list them
  conf.add_ruby_source '../test.rb'
  conf.add_ruby_source '../lib/class.rb'
  conf.add_ruby_source '../lib/object.rb'
  conf.add_ruby_source '../lib/js_interface.rb'
  conf.add_ruby_source '../lib/rb_interface.rb'

  # list as many javascript source files as you want
  # keep in mind they will be executed in the order you list them
  # prepended files will be executed after all of the gemss' prepended
  #   javascript files but before the ruby interpreter is started
  #   and any ruby source files are executed
  #conf.add_prepended_js_source 'app/prepend.js'
  # appended files will be executed after the ruby interpreter has finished
  #   executing the all the ruby source files and after all of the gem's
  #   appended javascript files
  # the interpreter will be in an idle state so you will still be able to
  #   make calls to it
  #conf.add_appended_js_source 'app/append.js'
  conf.add_appended_js_source '../test.js'
  conf.add_appended_js_source '../lib/rb_interface.js'
  
  # temporary build directory
  # by default the build directory is "build/"
  #conf.build_directory = 'build'

  # note: dont change this yet
  # Use :production for O2 mode build, and :development for O0 mode.
  # Or you can use '-O0', '-O1', '-O2', '-O3', etc. directly
  # note :production mode is still having trouble working
  # emscripten need to support ES6 before we can run this optimization
  #conf.build_mode = :production
  conf.build_mode = :development

  # set the final output file name
  #conf.output_name = 'app'
  conf.output_directory = 'www'
  
  # to override the mruby version, then specify your copy here
  #conf.mruby_directory = 'mruby'
  
  # note: dont change this yet
  # We found that if memory init file is used, browsers will hang
  # for a long time without response. As a result, we disable memory
  # init file by default. However, you can test this yourself
  # and re-enable it by commenting/removing the next line.
  #conf.ldflags << "--memory-init-file 0"
  
  # the syntax for adding gems is similar to mruby
  
  conf.add_gem :core => 'mruby-print'
  conf.add_gem :core => 'mruby-method'
  conf.add_gem :core => 'mruby-string-ext'
  #conf.add_gem :core => 'mruby-time'
  #conf.add_gem "gems/mruby-quack-concurrency"
  #conf.add_gem "gems/esruby-thread"
  #conf.add_gem "gems/mruby-time"
  
  
  # JavaScript binding interface
  #   see: https://github.com/robfors/esruby-bind for more info
  #conf.add_gem :github => 'robfors/esruby-bind'
  conf.add_gem "../../"
  #conf.add_gem "gems/esruby-bind"
  
  #conf.add_gem "gems/mruby-quack-concurrency"
  #conf.add_gem "gems/esruby-quack-concurrency"
  
  conf.flags = ["-s DEMANGLE_SUPPORT=1"]
  
end
