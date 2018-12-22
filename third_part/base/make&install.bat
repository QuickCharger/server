SET filedir=%~dp0
mkdir sln
cd sln
cmake -G "Visual Studio 14 2015" ..
cmake --build . --target install --clean-first --config Debug
cmake --build . --target install --clean-first --config Release
cd ..
rmdir /s /q sln

mkdir sln64
cd sln64
cmake -G "Visual Studio 14 2015 Win64" ..
cmake --build . --target install --clean-first --config Debug
cmake --build . --target install --clean-first --config Release
cd ..
rmdir /s /q sln64

pause

