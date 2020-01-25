#!/bin/bash
server=serwerfifo
dir=${PWD}
client_home=$1
number=$2
user=$(whoami)

if [[ $client_home ]] ; then # it runs only as child process
	client_dir=${dir/"/home/vix//"/$client_home}/klientfifo
	echo $((number + 1)) $user > $client_dir
else
	# signals
    trap "exit 0" SIGUSR1
	trap "" SIGTERM
	trap "" SIGHUP

    # recreate or create the serwerfio
    if [[ -e $server ]] ; then
        rm $server
        mkfifo $server
        chmod 777 $server
	else
        mkfifo $server
        chmod 777 $server
	fi

	# read the message from serwerfifo
	while true ; do
		if read line < $server ; then
			bash $0 $line & # pass the parameters into child process
		fi
	done
fi
