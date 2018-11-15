task :clean do
  rm_rf "test/temporary"
end

task :build_test do
  mkdir_p "test/temporary/www"
  cp "test/index.html", "test/temporary/www/index.html"
  sh "esruby build test/config.rb"
end

task :run_test => :build_test do
  sh "ruby -run -e httpd #{__dir__ + "/test/temporary/www"} -p 2222"
end