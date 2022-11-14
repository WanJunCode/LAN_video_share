单元测试框架

使用 google gtest


g++ Test_Main.cpp -I /root/.conan/data/gtest/1.8.1/_/_/package/89459b598e1f5db7881df907806432f4694e4bfb/include -L /root/.conan/data/gtest/1.8.1/_/_/package/89459b598e1f5db7881df907806432f4694e4bfb/lib -lgtest  -lgtest_main  -lpthread


BUG 解决

[ 50%] Building CXX object CMakeFiles/lan_video_test.dir/Test_Main.cpp.o
[settings]
[100%] Linking CXX executable bin/lan_video_test
/usr/bin/ld: CMakeFiles/lan_video_test.dir/Test_Main.cpp.o: in function `testing::AssertionResult testing::internal::CmpHelperEQFailure<int, int>(char const*, char const*, int const&, int const&)':
/root/.conan/data/gtest/1.8.1/_/_/package/89459b598e1f5db7881df907806432f4694e4bfb/include/gtest/gtest.h:1435: undefined reference to `testing::internal::EqFailure(char const*, char const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, bool)'
collect2: error: ld returned 1 exit status
CMakeFiles/lan_video_test.dir/build.make:98: recipe for target 'bin/lan_video_test' failed
make[2]: *** [bin/lan_video_test] Error 1
CMakeFiles/Makefile2:67: recipe for target 'CMakeFiles/lan_video_test.dir/all' failed
make[1]: *** [CMakeFiles/lan_video_test.dir/all] Error 2
Makefile:83: recipe for target 'all' failed
make: *** [all] Error 2

解决方案修改 conan profile
compiler.libcxx=libstdc++11