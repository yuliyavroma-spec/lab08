## Laboratory work VI

Данная лабораторная работа посвещена изучению средств пакетирования на примере **CPack**

## Report

```sh
$ export GITHUB_USERNAME=<имя_пользователя>
$ export GITHUB_EMAIL=<адрес_почтового_ящика>
$ alias edit=<nano|vi|vim|subl>
$ alias gsed=sed # for *-nix system
```
```sh
$ cd ${GITHUB_USERNAME}/workspace
$ pushd .
$ source scripts/activate
```
```sh
$ git clone https://github.com/${GITHUB_USERNAME}/lab05 projects/lab06
$ cd projects/lab06
$ git remote remove origin
$ git remote add origin https://github.com/${GITHUB_USERNAME}/lab06
```
```sh
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_STRING "v\${PRINT_VERSION}")
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION\
  \${PRINT_VERSION_MAJOR}.\${PRINT_VERSION_MINOR}.\${PRINT_VERSION_PATCH}.\${PRINT_VERSION_TWEAK})
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_TWEAK 0)
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_PATCH 0)
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_MINOR 1)
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_MAJOR 0)
' CMakeLists.txt
$ git diff
```
```sh
$ touch DESCRIPTION && edit DESCRIPTION
$ touch ChangeLog.md
$ export DATE="`LANG=en_US date +'%a %b %d %Y'`"
$ cat > ChangeLog.md <<EOF
* ${DATE} ${GITHUB_USERNAME} <${GITHUB_EMAIL}> 0.1.0.0
- Initial RPM release
EOF
```
```sh
$ cat > CPackConfig.cmake <<EOF
include(InstallRequiredSystemLibraries)
EOF
```
```sh
$ cat >> CPackConfig.cmake <<EOF
set(CPACK_PACKAGE_CONTACT ${GITHUB_EMAIL})
set(CPACK_PACKAGE_VERSION_MAJOR \${PRINT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR \${PRINT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH \${PRINT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK \${PRINT_VERSION_TWEAK})
set(CPACK_PACKAGE_VERSION \${PRINT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_FILE \${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "static C++ library for printing")
EOF
```
```sh
$ cat >> CPackConfig.cmake <<EOF

set(CPACK_RESOURCE_FILE_LICENSE \${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README \${CMAKE_CURRENT_SOURCE_DIR}/README.md)
EOF
```
```sh
$ cat >> CPackConfig.cmake <<EOF

set(CPACK_RPM_PACKAGE_NAME "print-devel")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "print")
set(CPACK_RPM_CHANGELOG_FILE \${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.md)
set(CPACK_RPM_PACKAGE_RELEASE 1)
EOF
```
```sh
$ cat >> CPackConfig.cmake <<EOF

set(CPACK_DEBIAN_PACKAGE_NAME "libprint-dev")
set(CPACK_DEBIAN_PACKAGE_PREDEPENDS "cmake >= 3.0")
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)
EOF
```
```sh
$ cat >> CPackConfig.cmake <<EOF

include(CPack)
EOF
```
```sh
$ cat >> CMakeLists.txt <<EOF

include(CPackConfig.cmake)
EOF
```
```sh
$ gsed -i 's/lab05/lab06/g' README.md
```
```
$ mkdir -p .github/workflows
$ cat > .github/workflows/cpack.yml <<'EOF'
name: CPack Package Build

on:
  push:
    tags:
      - 'v*'
  workflow_dispatch:

jobs:
  build-packages:
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
        
    runs-on: ${{ matrix.os }}
    
    steps:
    - uses: actions/checkout@v4
      with:
        fetch-depth: 0
    
    - name: Install dependencies (Ubuntu)
      if: runner.os == 'Linux'
      run: |
        sudo apt-get update
        sudo apt-get install -y rpm
    
    - name: Configure CMake
      run: |
        cmake -H. -B build -DCMAKE_BUILD_TYPE=Release
    
    - name: Build
      run: |
        cmake --build build --config Release
    
    - name: Package (Ubuntu - DEB and RPM)
      if: runner.os == 'Linux'
      run: |
        cd build
        cmake -G "DEB" -DCPACK_GENERATOR="DEB" ..
        cmake --build . --target package
        cmake -G "RPM" -DCPACK_GENERATOR="RPM" ..
        cmake --build . --target package
    
    - name: Package (macOS - DragNDrop)
      if: runner.os == 'macOS'
      run: |
        cd build
        cmake -G "DragNDrop" -DCPACK_GENERATOR="DragNDrop" ..
        cmake --build . --target package
    
    - name: Package (Windows - WIX)
      if: runner.os == 'Windows'
      run: |
        cd build
        cmake -G "WIX" -DCPACK_GENERATOR="WIX" ..
        cmake --build . --target package --config Release
    
    - name: Upload artifacts
      uses: actions/upload-artifact@v4
      with:
        name: packages-${{ matrix.os }}
        path: build/*.deb
        if-no-files-found: warn
    
    - name: Upload to Release
      uses: softprops/action-gh-release@v1
      if: startsWith(github.ref, 'refs/tags/')
      with:
        files: |
          build/*.deb
          build/*.rpm
          build/*.dmg
          build/*.msi
          build/*.tar.gz
          build/*.zip
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
EOF
```
```sh
$ git add .
$ git commit -m"added cpack config"
$ git tag v0.1.0.0
$ git push origin master --tags
```
```sh
$ cmake -H. -B_build
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.9s)
-- Generating done (0.0s)
-- Build files have been written to: /home/yulia/yuliyavroma-spec/workspace/projects/lab06/_build

$ cmake --build _build
[ 50%] Building CXX object CMakeFiles/lab05.dir/main.cpp.o
[100%] Linking CXX executable lab05
[100%] Built target lab05

$ cd _build
$ cpack -G "TGZ"
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: lab05
CPack: - Install project: lab05 []
CPack: Create package
CPack: - package: /home/yulia/yuliyavroma-spec/workspace/projects/lab06/_build/lab05-0.1.1-Linux.tar.gz generated.

$ cd ..

$ cmake -H. -B_build -DCPACK_GENERATOR="TGZ"
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /home/yulia/yuliyavroma-spec/workspace/projects/lab06/_build

$ cmake --build _build --target package
-target package
[100%] Built target lab05
Run CPack packaging tool...
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: lab05
CPack: - Install project: lab05 []
CPack: Create package
CPack: - package: /home/yulia/yuliyavroma-spec/workspace/projects/lab06/_build/lab05-0.1.1-Linux.tar.gz generated.

$ mkdir artifacts
$ mv _build/*.tar.gz artifacts
$ tree artifacts
└── lab05-0.1.1-Linux.tar.gz

1 directory, 1 file

```


## Homework

После того, как вы настроили взаимодействие с системой непрерывной интеграции,</br>
обеспечив автоматическую сборку и тестирование ваших изменений, стоит задуматься</br>
о создание пакетов для измениний, которые помечаются тэгами (см. вкладку [releases](https://github.com/tp-labs/lab06/releases)).</br>
Пакет должен содержать приложение _solver_ из [предыдущего задания](https://github.com/tp-labs/lab03#задание-1)
Таким образом, каждый новый релиз будет состоять из следующих компонентов:
- архивы с файлами исходного кода (`.tar.gz`, `.zip`)
- пакеты с бинарным файлом _solver_ (`.deb`, `.rpm`, `.msi`, `.dmg`)
```sh
$ cat >> CPackConfig.cmake <<'EOF'

set(CPACK_DEBIAN_PACKAGE_NAME "solver")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Quadratic equation solver")
set(CPACK_DEBIAN_PACKAGE_SECTION "math")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

set(CPACK_RPM_PACKAGE_NAME "solver")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Mathematics")
set(CPACK_RPM_PACKAGE_RELEASE 1)

set(CPACK_GENERATOR "TGZ;DEB;RPM")
EOF
```
```sh
$ cmake -H. -B_build -DCPACK_GENERATOR="TGZ;DEB;RPM"
$ cmake --build _build --target package
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
Run CPack packaging tool...
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: solver
CPack: - Install project: solver []
CPack: Create package
CPack: - package: /home/yulia/yuliyavroma-spec/workspace/projects/lab06/_build/solver-0.2.0.0-Linux.tar.gz generated.

```
```sh
$ mv _build/*.tar.gz artifacts
$ tree artifacts
artifacts
├── lab05-0.1.1-Linux.tar.gz
├── print-0.1.0.0-Linux.tar.gz
└── solver-0.2.0.0-Linux.tar.gz
```

Для этого нужно добавить ветвление в конфигурационные файлы для **CI** со следующей логикой:</br>
если **commit** помечен тэгом, то необходимо собрать пакеты (`DEB, RPM, WIX, DragNDrop, ...`) </br>
и разместить их на сервисе **GitHub**. (см. пример для [Travi CI](https://docs.travis-ci.com/user/deployment/releases))</br>
```sh
$ cat > .github/workflows/release.yml <<'EOF'
name: Build and Release Packages

on:
  push:
    tags:
      - 'v*'
  workflow_dispatch:

jobs:
  build-linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y rpm
      
      - name: Configure
        run: cmake -H. -B build -DCMAKE_BUILD_TYPE=Release
      
      - name: Build
        run: cmake --build build
      
      - name: Create DEB package
        run: |
          cd build
          cmake -G "DEB" -DCPACK_GENERATOR="DEB" ..
          cmake --build . --target package
          cd ..
      
      - name: Create RPM package
        run: |
          cd build
          cmake -G "RPM" -DCPACK_GENERATOR="RPM" ..
          cmake --build . --target package
          cd ..
      
      - name: Create TGZ package
        run: |
          cd build
          cmake -G "TGZ" -DCPACK_GENERATOR="TGZ" ..
          cmake --build . --target package
          cd ..
      
      - name: Upload to Release
        uses: softprops/action-gh-release@v1
        with:
          files: |
            build/*.deb
            build/*.rpm
            build/*.tar.gz
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}

  build-macos:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Configure
        run: cmake -H. -B build -DCMAKE_BUILD_TYPE=Release
      
      - name: Build
        run: cmake --build build
      
      - name: Create DMG package
        run: |
          cd build
          cmake -G "DragNDrop" -DCPACK_GENERATOR="DragNDrop" ..
          cmake --build . --target package
          cd ..
      
      - name: Upload to Release
        uses: softprops/action-gh-release@v1
        with:
          files: build/*.dmg
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}

  build-windows:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Configure
        run: cmake -H. -B build -DCMAKE_BUILD_TYPE=Release
      
      - name: Build
        run: cmake --build build --config Release
      
      - name: Create WIX package
        run: |
          cd build
          cmake -G "WIX" -DCPACK_GENERATOR="WIX" ..
          cmake --build . --target package --config Release
          cd ..
      
      - name: Upload to Release
        uses: softprops/action-gh-release@v1
        with:
          files: build/*.msi
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
EOF
```
