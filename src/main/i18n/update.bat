@echo off

set remove_unused=
if "%1" == "-r" (
    set remove_unused=-no-obsolete
)

forfiles /s /m *.ts /c "cmd /c lupdate ..\.. -locations relative %remove_unused% -ts @file"
