SET PROTOBUF_DIR=%~dp0

RD /S /Q lib

SET DEVENV="%VS140COMNTOOLS%..\IDE\devenv.com"

rem win32 debug
%DEVENV% ./vsprojects/protobuf.sln /ReBuild "Debug|Win32" /Project libprotobuf
XCOPY /f vsprojects\Debug\libprotobuf.lib lib\windows_x86_32_msvc14\Debug\

rem win32 release
%DEVENV% ./vsprojects/protobuf.sln /ReBuild "Release|Win32" /Project libprotobuf
XCOPY /f vsprojects\Release\libprotobuf.lib lib\windows_x86_32_msvc14\Release\

rem win64 debug
%DEVENV% ./vsprojects/protobuf.sln /ReBuild "Debug|x64" /Project libprotobuf
XCOPY /f vsprojects\x64\Debug\libprotobuf.lib lib\windows_x86_64_msvc14\Debug\

rem win64 release
%DEVENV% ./vsprojects/protobuf.sln /ReBuild "Release|x64" /Project libprotobuf
XCOPY /f vsprojects\x64\Release\libprotobuf.lib lib\windows_x86_64_msvc14\Release\
