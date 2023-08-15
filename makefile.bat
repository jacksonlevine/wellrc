mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=~/Documents/vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
copy /Y Release\main.exe ..
copy /Y Release\glew32.dll ..
copy /Y Release\glfw3.dll ..
pause