#!/bin/bash
DIRPATH=$1
FILEPATH=${DIRPATH}"zad_2.txt"
# create file if doesn't exist
if [ ! -f $FILEPATH ]; then
  echo > $FILEPATH
fi

while true; do
  echo "0 - odczyt komunikatu"
  echo "1 - zapis komunikatu"
  echo "2 - koniec pracy"
  read option

  if [ "$option" -eq "0" ] ; then
    echo message: `cat "$FILEPATH"`
  elif [ "$option" -eq "1" ] ; then
    read input
    echo $input > $FILEPATH
  elif [ "$option" -eq "2" ] ; then
    exit 0
  fi
done
exit 0
