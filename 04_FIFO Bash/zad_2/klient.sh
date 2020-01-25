#!/bin/bash
server=serwerfifo
client=klientfifo
home=${HOME}
number=$1

# recreate or create the klientfifo
if [[ -e $client ]]; then
    rm $client
    mkfifo $client
    chmod 777 $client
else
    mkfifo $client
    chmod 777 $client
fi

echo $home $number > $server
read line < $client
echo $line
