@echo off
call vendor\bin\premake\premake5.exe gmake2
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)