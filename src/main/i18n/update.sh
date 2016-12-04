#!/bin/bash

remove_unused=""
if [[ "x$1" = "x-r" ]]; then
    remove_unused="-no-obsolete"
fi

for file in `ls *.ts`; do
    lupdate ../.. -locations relative $remove_unused -ts $file
done