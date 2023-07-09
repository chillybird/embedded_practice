@echo off
set "startdir=%cd%"
start cmd.exe /K "cd %IDF_PATH% && export.bat && cd %startdir%"


