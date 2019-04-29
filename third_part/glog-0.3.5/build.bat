IF DEFINED VS140COMNTOOLS (
  SET VCVARSALL="%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
) ELSE (
  FOR /F "tokens=1,2,*" %%I IN ('REG QUERY HKLM\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\SxS\VS7 /v 15.0 ^| FIND "15.0"') DO SET VCVARSALL="%%~KVC\Auxiliary\Build\vcvarsall.bat"
)

IF NOT DEFINED VCVARSALL (
  ECHO Can not find VC2015 or VC2017 installed!
  GOTO ERROR
)

CALL %VCVARSALL% amd64

SET GLOG_DIR=%~dp0

RD /S /Q lib

@rem win32 debug
devenv ./google-glog.sln /ReBuild "Debug|Win32" /Project libglog_static
XCOPY /f Debug\libglog_static.lib lib\windows_x86_32_msvc14\Debug\

@rem win32 release
devenv ./google-glog.sln /ReBuild "Release|Win32" /Project libglog_static
XCOPY /f Release\libglog_static.lib lib\windows_x86_32_msvc14\Release\

@rem win64 debug
devenv ./google-glog.sln /ReBuild "Debug|x64" /Project libglog_static
XCOPY /f x64\Debug\libglog_static.lib lib\windows_x86_64_msvc14\Debug\

@rem win64 release
devenv ./google-glog.sln /ReBuild "Release|x64" /Project libglog_static
XCOPY /f x64\Release\libglog_static.lib lib\windows_x86_64_msvc14\Release\

GOTO :EOF

:ERROR
