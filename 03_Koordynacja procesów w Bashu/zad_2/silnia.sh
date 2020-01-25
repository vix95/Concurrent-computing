#!/bin/bash
fac() {
    if [ $1 -le 1 ]; then
        return 1
    else
        fac $[$1-1]
        return $[$1*$?]
    fi
}

fac $1

exit $?
