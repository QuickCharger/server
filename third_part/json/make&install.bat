SET filedir=%~dp0
mkdir sln
cd sln
cmake ..
cmake --build . --target install --clean-first --config Debug
cd ..
rmdir /s /q sln
pause
