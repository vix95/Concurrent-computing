#!/bin/bash
psqty=`ps -C 'sh zad_3.sh' | wc -l`
psqty=$((psqty-3))
echo $$
if [ "$psqty" -le 5 ] ; then
  sleep 1
  sh zad_3.sh
  sh zad_3.sh
fi
exit 0
