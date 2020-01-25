#!/bin/bash
declare -i n k i wynik
echo -n "Podaj n: "
read n

echo -n "Podaj k: "
read k

bash silnia.sh $n &
bash silnia.sh $k &
bash silnia.sh $((n-k)) &

let i=0

for job in `jobs -p`; do
    wait $job
    silnie[i]=$?
    i=$((i+1))
done

wynik=$((${silnie[1]} * ${silnie[2]}))
echo ${wynik}
wynik=$((${silnie[0]} / ${wynik}))
echo ${wynik}
echo "Wynik wspolczynnika dwumianowego $n i $k to $wynik"
