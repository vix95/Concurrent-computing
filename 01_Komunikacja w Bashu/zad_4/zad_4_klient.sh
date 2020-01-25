#!/bin/bash
FILEPATH=$1 #get path
READFILEPATH=${FILEPATH}"vix_wyniki.txt"
WRITEFILEPATH=${FILEPATH}"vix_dane.txt"

read input
echo $input > $WRITEFILEPATH
while true ; do
  value=`cat "$READFILEPATH"`
  if [ ! -z "$value" ] ; then
    echo output: $value
    value=""
    cat /dev/null > $READFILEPATH # clear buffer
    exit 0
    read input
    echo $input > $WRITEFILEPATH
  fi
done

exit 0
