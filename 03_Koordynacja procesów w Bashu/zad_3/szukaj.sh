#!/bin/bash
declare -i x

# $0 - bash script
# $1 - search file
# $2 - in which catalog should search

if [ -d $2 ]; then # if it's a catalog
    cd $2 # pass there

    # get all catalogs and files
    # 2>/dev/null is using here for clear ouput
    for i in $( ls -d */ 2>/dev/null ) ; do
        bash $0 $1 $2$i 1 &
    done

    # get every catalogs and files to check that file is founded
    for i in $( ls ); do
        if [ $i == $1 ] ; then # if file has been founded
            x+=1
            echo $2$i " <------"
        else
            echo $2$i
        fi
    done
else # if not
    echo "Error: wrong parameters"
fi

for job in `jobs -p`; do
    wait $job
    x+=$? # sum of founded files
done

# print only once, first execute of bash script
# doesn't have the third parameter
if [ -z $3 ] ; then
    if [ $x -ge 1 ] ; then # if greater or equal
        echo -e "\nZnaleziono"
    else
        echo -e "\nNie znaleziono"
    fi
fi

exit $x # pass the x parameter
