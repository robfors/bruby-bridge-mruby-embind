MRuby::Gem::Specification.new('bruby-bridge-mruby-embind') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Rob Fors'
  spec.summary = 'low level minimalist interface between the javascript and ruby environment'
  spec.version = '0.0.0'

  #spec.add_dependency('mruby-regexp-pcre', :github => 'iij/mruby-regexp-pcre')
  spec.add_dependency('mruby-eval')
  spec.add_dependency('esruby-esruby')
  spec.add_dependency('mruby-metaprog')
  spec.add_dependency('mruby-enumerator')
  
  spec.cxx.flags << "-std=c++11"
  
  spec.compilers.each do |c|
    c.flags << "--bind"
  end
  
  spec.rbfiles = Dir.glob("#{dir}/mrblib/**/*.rb")



#spec.rbfiles = []

#spec.objs = []

  spec.objs = Dir.glob("#{dir}/src/**/*.cpp")
    .map { |f| objfile(f.relative_path_from(dir).pathmap("#{build_dir}/%X")) }

end
