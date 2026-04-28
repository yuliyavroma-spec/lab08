## Laboratory work VII

Данная лабораторная работа посвещена изучению систем управления пакетами на примере **Hunter**

## Tutorial

```sh
$ export GITHUB_USERNAME=<имя_пользователя>
$ alias gsed=sed
```

```sh
$ cd ${GITHUB_USERNAME}/workspace
$ pushd .
$ source scripts/activate
```

```sh
$ git clone https://github.com/${GITHUB_USERNAME}/lab06 projects/lab07
$ cd projects/lab07
$ git remote remove origin
$ git remote add origin https://github.com/${GITHUB_USERNAME}/lab07
```

```sh
$ mkdir -p cmake
$ wget https://raw.githubusercontent.com/cpp-pm/gate/master/cmake/HunterGate.cmake -O cmake/HunterGate.cmake
$ gsed -i '/cmake_minimum_required(VERSION 3.4)/a\

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.251.tar.gz"
    SHA1 "5659b15dc0884d4b03dbd95710e6a1fa0fc3258d"
)
' CMakeLists.txt
```

```sh
$ git rm -rf third-party/gtest
$ gsed -i '/set(PRINT_VERSION_STRING "v\${PRINT_VERSION}")/a\

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
' CMakeLists.txt
$ gsed -i 's/add_subdirectory(third-party/gtest)//' CMakeLists.txt
$ gsed -i 's/gtest_main/GTest::main/' CMakeLists.txt
```

```sh
$ cmake -H. -B_builds -DBUILD_TESTS=ON
$ cmake --build _builds
[ 18%] Built target formatter
[ 36%] Built target formatter_ex
[ 54%] Built target solver_lib
[100%] Built target solver

$ cmake --build _builds --target test
$ ls -la $HOME/.hunter
```

```sh
$ git clone https://github.com/cpp-pm/hunter $HOME/projects/hunter
$ export HUNTER_ROOT=$HOME/projects/hunter
$ rm -rf _builds
$ cmake -H. -B_builds -DBUILD_TESTS=ON
$ cmake --build _builds
$ cmake --build _builds --target test
```

```sh
$ cat $HUNTER_ROOT/cmake/configs/default.cmake | grep GTest
$ cat $HUNTER_ROOT/cmake/projects/GTest/hunter.cmake
$ mkdir cmake/Hunter
$ cat > cmake/Hunter/config.cmake <<EOF
hunter_config(GTest VERSION 1.7.0-hunter-9)
EOF
# add LOCAL in HunterGate function
```

```sh
$ mkdir demo
$ cat > demo/main.cpp <<EOF
#include <print.hpp>

#include <cstdlib>

int main(int argc, char* argv[])
{
  const char* log_path = std::getenv("LOG_PATH");
  if (log_path == nullptr)
  {
    std::cerr << "undefined environment variable: LOG_PATH" << std::endl;
    return 1;
  }
  std::string text;
  while (std::cin >> text)
  {
    std::ofstream out{log_path, std::ios_base::app};
    print(text, out);
    out << std::endl;
  }
}
EOF

$ gsed -i '/endif()/a\

add_executable(demo ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp)
target_link_libraries(demo print)
install(TARGETS demo RUNTIME DESTINATION bin)
' CMakeLists.txt
```

```sh
$ mkdir tools
$ git submodule add https://github.com/ruslo/polly tools/polly
$ tools/polly/bin/polly.py --test
Generate: 0:00:03.001462s
Build: 0:00:03.036973s
Test: 0:00:00.069969s
-
Total: 0:00:06.108659s
-
SUCCESS

$ tools/polly/bin/polly.py --install
$ tools/polly/bin/polly.py --toolchain clang-cxx14
Python version: 3.13
Build dir: /home/yulia/yuliyavroma-spec/workspace/projects/lab07/_builds/clang-cxx14
Execute command: [
  `which`
  `cmake`
]

[/home/yulia/yuliyavroma-spec/workspace/projects/lab07]> "which" "cmake"

/usr/bin/cmake
Execute command: [
  `cmake`
  `--version`
]

[/home/yulia/yuliyavroma-spec/workspace/projects/lab07]> "cmake" "--version"

cmake version 3.31.6

CMake suite maintained and supported by Kitware (kitware.com/cmake).
Execute command: [
  `cmake`
  `-H.`
  `-B/home/yulia/yuliyavroma-spec/workspace/projects/lab07/_builds/clang-cxx14`
  `-GUnix Makefiles`
  `-DCMAKE_TOOLCHAIN_FILE=/home/yulia/yuliyavroma-spec/workspace/projects/lab07/tools/polly/clang-cxx14.cmake`
]

[/home/yulia/yuliyavroma-spec/workspace/projects/lab07]> "cmake" "-H." "-B/home/yulia/yuliyavroma-spec/workspace/projects/lab07/_builds/clang-cxx14" "-GUnix Makefiles" "-DCMAKE_TOOLCHAIN_FILE=/home/yulia/yuliyavroma-spec/workspace/projects/lab07/tools/polly/clang-cxx14.cmake"

-- [polly] Used toolchain: clang / c++14 support
-- The C compiler identification is Clang 18.1.8
-- The CXX compiler identification is Clang 18.1.8
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/clang - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/clang++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- [hunter] Calculating Toolchain-SHA1
-- [hunter] Calculating Config-SHA1
-- [hunter] HUNTER_ROOT: /home/yulia/projects/hunter
-- [hunter] [ Hunter-ID: xxxxxxx | Toolchain-ID: 8ebd5ad | Config-ID: d87434b ]
-- [hunter] GTEST_ROOT: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Install (ver.: 1.15.2)
-- [hunter] Building GTest
loading initial cache file /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/cache.cmake
loading initial cache file /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/args.cmake
-- [polly] Used toolchain: clang / c++14 support
-- The C compiler identification is Clang 18.1.8
-- The CXX compiler identification is Clang 18.1.8
-- Check for working C compiler: /usr/bin/clang - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/clang++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.4s)
-- Generating done (0.0s)
-- Build files have been written to: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Build
[  6%] Creating directories for 'GTest-Release'
[ 12%] Performing download step (download, verify and extract) for 'GTest-Release'
-- verifying file...
       file='/home/yulia/projects/hunter/_Base/Download/GTest/1.15.2/568d58e/v1.15.2.tar.gz'
-- File already exists and hash match (skip download):
  file='/home/yulia/projects/hunter/_Base/Download/GTest/1.15.2/568d58e/v1.15.2.tar.gz'
  SHA1='568d58e26bd4e838449ca7ab8ebc152b3cbd210d'
-- extracting...
     src='/home/yulia/projects/hunter/_Base/Download/GTest/1.15.2/568d58e/v1.15.2.tar.gz'
     dst='/home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Source'
-- extracting... [tar xfz]
-- extracting... [analysis]
-- extracting... [rename]
-- extracting... [clean up]
-- extracting... done
[ 18%] No update step for 'GTest-Release'
[ 25%] No patch step for 'GTest-Release'
[ 31%] Performing configure step for 'GTest-Release'
loading initial cache file /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/cache.cmake
loading initial cache file /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/args.cmake
-- [polly] Used toolchain: clang / c++14 support
-- The C compiler identification is Clang 18.1.8
-- The CXX compiler identification is Clang 18.1.8
-- Check for working C compiler: /usr/bin/clang - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/clang++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Configuring done (0.6s)
-- Generating done (0.0s)
-- Build files have been written to: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Build/GTest-Release-prefix/src/GTest-Release-build
[ 37%] Performing build step for 'GTest-Release'
[ 12%] Building CXX object googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
[ 25%] Linking CXX static library ../lib/libgtest.a
[ 25%] Built target gtest
[ 37%] Building CXX object googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
[ 50%] Building CXX object googletest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
[ 62%] Linking CXX static library ../lib/libgtest_main.a
[ 62%] Built target gtest_main
[ 75%] Linking CXX static library ../lib/libgmock.a
[ 75%] Built target gmock
[ 87%] Building CXX object googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
[100%] Linking CXX static library ../lib/libgmock_main.a
[100%] Built target gmock_main
[ 43%] Performing install step for 'GTest-Release'
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom/gmock-generated-actions.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom/gmock-port.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom/gmock-matchers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom/README.md
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/gmock-internal-utils.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/gmock-pp.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/gmock-port.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-more-matchers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-actions.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-function-mocker.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-cardinalities.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-matchers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-spec-builders.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-nice-strict.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-more-actions.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/libgmock.a
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/libgmock_main.a
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/pkgconfig/gmock.pc
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/pkgconfig/gmock_main.pc
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/cmake/GTest/GTestTargets.cmake
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/cmake/GTest/GTestTargets-release.cmake
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/cmake/GTest/GTestConfigVersion.cmake
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/cmake/GTest/GTestConfig.cmake
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-test-part.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-message.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom/gtest-port.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom/gtest-printers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom/README.md
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom/gtest.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-port.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-port-arch.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-internal.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-death-test-internal.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-filepath.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-string.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-type-util.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-param-util.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-spi.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-assertion-result.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-param-test.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-death-test.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-typed-test.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-matchers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-printers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest_prod.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest_pred_impl.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/libgtest.a
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/libgtest_main.a
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/pkgconfig/gtest.pc
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/pkgconfig/gtest_main.pc
loading initial cache file /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/args.cmake
[ 50%] Completed 'GTest-Release'
[ 50%] Built target GTest-Release
[ 56%] Creating directories for 'GTest-Debug'
[ 62%] Performing download step (download, verify and extract) for 'GTest-Debug'
-- verifying file...
       file='/home/yulia/projects/hunter/_Base/Download/GTest/1.15.2/568d58e/v1.15.2.tar.gz'
-- File already exists and hash match (skip download):
  file='/home/yulia/projects/hunter/_Base/Download/GTest/1.15.2/568d58e/v1.15.2.tar.gz'
  SHA1='568d58e26bd4e838449ca7ab8ebc152b3cbd210d'
-- extracting...
     src='/home/yulia/projects/hunter/_Base/Download/GTest/1.15.2/568d58e/v1.15.2.tar.gz'
     dst='/home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Source'
-- extracting... [tar xfz]
-- extracting... [analysis]
-- extracting... [rename]
-- extracting... [clean up]
-- extracting... done
[ 68%] No update step for 'GTest-Debug'
[ 75%] No patch step for 'GTest-Debug'
[ 81%] Performing configure step for 'GTest-Debug'
loading initial cache file /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/cache.cmake
loading initial cache file /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/args.cmake
-- [polly] Used toolchain: clang / c++14 support
-- The C compiler identification is Clang 18.1.8
-- The CXX compiler identification is Clang 18.1.8
-- Check for working C compiler: /usr/bin/clang - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/clang++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Configuring done (0.6s)
-- Generating done (0.0s)
-- Build files have been written to: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Build/GTest-Debug-prefix/src/GTest-Debug-build
[ 87%] Performing build step for 'GTest-Debug'
[ 12%] Building CXX object googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
[ 25%] Linking CXX static library ../lib/libgtestd.a
[ 25%] Built target gtest
[ 37%] Building CXX object googletest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
[ 50%] Building CXX object googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
[ 62%] Linking CXX static library ../lib/libgtest_maind.a
[ 62%] Built target gtest_main
[ 75%] Linking CXX static library ../lib/libgmockd.a
[ 75%] Built target gmock
[ 87%] Building CXX object googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
[100%] Linking CXX static library ../lib/libgmock_maind.a
[100%] Built target gmock_main
[ 93%] Performing install step for 'GTest-Debug'
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom/gmock-generated-actions.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom/gmock-port.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom/gmock-matchers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/custom/README.md
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/gmock-internal-utils.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/gmock-pp.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/internal/gmock-port.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-more-matchers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-actions.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-function-mocker.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-cardinalities.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-matchers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-spec-builders.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-nice-strict.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gmock/gmock-more-actions.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/libgmockd.a
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/libgmock_maind.a
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/pkgconfig/gmock.pc
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/pkgconfig/gmock_main.pc
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/cmake/GTest/GTestTargets.cmake
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/cmake/GTest/GTestTargets-debug.cmake
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/cmake/GTest/GTestConfigVersion.cmake
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/cmake/GTest/GTestConfig.cmake
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-test-part.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-message.h
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal
-- Up-to-date: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom/gtest-port.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom/gtest-printers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom/README.md
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/custom/gtest.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-port.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-port-arch.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-internal.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-death-test-internal.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-filepath.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-string.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-type-util.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/internal/gtest-param-util.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-spi.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-assertion-result.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-param-test.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-death-test.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-typed-test.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-matchers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest-printers.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest_prod.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest_pred_impl.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/include/gtest/gtest.h
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/libgtestd.a
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/libgtest_maind.a
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/pkgconfig/gtest.pc
-- Installing: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/Install/lib/pkgconfig/gtest_main.pc
loading initial cache file /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest/args.cmake
[100%] Completed 'GTest-Debug'
[100%] Built target GTest-Debug
-- [hunter] Build step successful (dir: /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Build/GTest)
-- [hunter] Cache saved: /home/yulia/projects/hunter/_Base/Cache/raw/e7d065733605790370f3cb6f302bf145d502f12f.tar.bz2

  CMake variable GTEST_ROOT is set to:

    /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Install

  Environment variable GTEST_ROOT is set to:

    /home/yulia/projects/hunter/_Base/xxxxxxx/8ebd5ad/d87434b/Install

  For compatibility, find_package is ignoring the variable, but code in a
  .cmake module might still use it.
This warning is for project developers.  Use -Wno-dev to suppress it.

-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Configuring done (37.7s)
-- Generating done (0.0s)
-- Build files have been written to: /home/yulia/yuliyavroma-spec/workspace/projects/lab07/_builds/clang-cxx14
Execute command: [
  `cmake`
  `--build`
  `/home/yulia/yuliyavroma-spec/workspace/projects/lab07/_builds/clang-cxx14`
  `--`
]

[/home/yulia/yuliyavroma-spec/workspace/projects/lab07]> "cmake" "--build" "/home/yulia/yuliyavroma-spec/workspace/projects/lab07/_builds/clang-cxx14" "--"

[  9%] Building CXX object formatter_lib/CMakeFiles/formatter.dir/formatter.cpp.o
[ 18%] Linking CXX static library libformatter.a
[ 18%] Built target formatter
[ 27%] Building CXX object formatter_ex/CMakeFiles/formatter_ex.dir/formatter_ex.cpp.o
[ 36%] Linking CXX static library libformatter_ex.a
[ 36%] Built target formatter_ex
[ 45%] Building CXX object solver_lib/CMakeFiles/solver_lib.dir/solver.cpp.o
[ 54%] Linking CXX static library libsolver_lib.a
[ 54%] Built target solver_lib
[ 63%] Building CXX object solver/CMakeFiles/solver.dir/solver.cpp.o
[ 72%] Building CXX object solver/CMakeFiles/solver.dir/__/formatter_ex/formatter_ex.cpp.o
[ 81%] Building CXX object solver/CMakeFiles/solver.dir/__/formatter_lib/formatter.cpp.o
[ 90%] Building CXX object solver/CMakeFiles/solver.dir/__/solver_lib/solver.cpp.o
[100%] Linking CXX executable solver
[100%] Built target solver
-
Log saved: /home/yulia/yuliyavroma-spec/workspace/projects/lab07/_logs/polly/clang-cxx14/log.txt
-
Generate: 0:00:38.725965s
Build: 0:00:04.078852s
-
Total: 0:00:42.805192s
-
SUCCESS

```

## Report

```sh
$ popd
$ export LAB_NUMBER=07
$ git clone https://github.com/tp-labs/lab${LAB_NUMBER} tasks/lab${LAB_NUMBER}
$ mkdir reports/lab${LAB_NUMBER}
$ cp tasks/lab${LAB_NUMBER}/README.md reports/lab${LAB_NUMBER}/REPORT.md
$ cd reports/lab${LAB_NUMBER}
$ edit REPORT.md
$ gist REPORT.md
```

## Homework

### Задание
1. Создайте cвой hunter-пакет.

```sh
$ mkdir -p pkg/include pkg/src pkg/cmake
$ cat > pkg/include/pkg.hpp << 'EOF'
 #ifndef PKG_HPP
 #define PKG_HPP

namespace pkg {
    int add(int a, int b);
}

 #endif
EOF

$ cat > pkg/src/pkg.cpp << 'EOF'
 #include <pkg.hpp>

namespace pkg {
    int add(int a, int b) {
        return a + b;
    }
}
EOF

$ cat > pkg/CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.10)
project(pkg)

add_library(pkg STATIC src/pkg.cpp)
target_include_directories(pkg PUBLIC include)

install(TARGETS pkg DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
EOF

$ cat > pkg/cmake/pkg.cmake << 'EOF'
hunter_config(pkg VERSION 1.0.0)
EOF
$ cat > cmake/Hunter/config.cmake << 'EOF'
hunter_config(pkg VERSION 1.0.0)
EOF
```
