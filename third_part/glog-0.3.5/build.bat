SET PROTOBUF_DIR=%~dp0

RD /S /Q lib

SET DEVENV="%VS140COMNTOOLS%..\IDE\devenv.com"

rem win32 debug
%DEVENV% ./google-glog.sln /ReBuild "Debug|Win32" /Project libglog_static
XCOPY /f Debug\libglog_static.lib lib\windows_x86_32_msvc14\Debug\

rem win32 release
%DEVENV% ./google-glog.sln /ReBuild "Release|Win32" /Project libglog_static
XCOPY /f Release\libglog_static.lib lib\windows_x86_32_msvc14\Release\

rem win64 debug
%DEVENV% ./google-glog.sln /ReBuild "Debug|x64" /Project libglog_static
XCOPY /f x64\Debug\libglog_static.lib lib\windows_x86_64_msvc14\Debug\

rem win64 release
%DEVENV% ./google-glog.sln /ReBuild "Release|x64" /Project libglog_static
XCOPY /f x64\Release\libglog_static.lib lib\windows_x86_64_msvc14\Release\
