#!/bin/bash

if [[ "x$1" = "x-h" ]]; then
    echo "Usage: update.sh [-r]"
	echo "Will update the localisation strings from source files."
	echo "Paramters:"
	echo " -r    Will remove obselete resources"
	exit 0
fi

remove_unused=""
if [[ "x$1" = "x-r" ]]; then
    remove_unused="-no-obsolete"
fi

for file in `ls *.ts`; do
    lupdate ../.. -locations relative $remove_unused -ts $file
done