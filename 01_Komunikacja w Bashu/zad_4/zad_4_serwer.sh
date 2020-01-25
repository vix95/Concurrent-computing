#!/bin/bash
FILEPATH=$1 # get path
READFILEPATH=${FILEPATH}"vix_dane.txt"
WRITEFILEPATH=${FILEPATH}"vix_wyniki.txt"

# create file if doesn't exist
if [ ! -f $READFILEPATH ]; then
  touch $READFILEPATH
fi

if [ ! -f $WRITEFILEPATH ]; then
  touch $WRITEFILEPATH
fi

cat /dev/null > $READFILEPATH
cat /dev/null > $WRITEFILEPATH

while true ; do
  value=`cat "$READFILEPATH"`
  if [ ! -z "$value" ] ; then
    cat /dev/null > $READFILEPATH
    sum=0
    sum=$((value+5))
    echo $sum > $WRITEFILEPATH
  fi
done
exit 0
