@echo off

if "%1" == "-h" (
    echo Usage: update.bat [-r]
	echo Will update the localisation strings from source files.
	echo Paramters:
	echo  -r    Will remove obselete resources
	exit /B 0
)

set remove_unused=
if "%1" == "-r" (
    set remove_unused=-no-obsolete
)

forfiles /s /m *.ts /c "cmd /c lupdate ..\.. -locations relative %remove_unused% -ts @file"
