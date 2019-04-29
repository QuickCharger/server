SET WORK_DIR=%~dp0

cd %WORK_DIR%/third_part/base
call "make&install.bat"

cd %WORK_DIR%/third_part/json
call "make&install.bat"

pause
