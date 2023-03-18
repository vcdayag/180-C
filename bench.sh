#!/bin/bash

computer=$(uname -n)
branch=$(git symbolic-ref --short HEAD)
commit=$(git rev-parse --short HEAD)
nvalues=( 100 200 300 400 500 600 700 800 900 1000 2000 4000 8000 16000 20000 50000 )
threads=( 1 2 4 8 16 32 64 )

# create directory for benchmark results
benchdir="bench"
mkdir -p "$benchdir"

exer02()
{
    gcc interpolation-multithread.c -pthread -o interpolation-multithread

    # create file
    file_csv="${benchdir}/${computer}_${branch}_${commit}_exer02_${1}.csv"
    rm -f "$file_csv"
    touch "$file_csv"

    for i in "${threads[@]}"
    do
        echo "n = $1 : $i thread(s)"
        echo -n "$i" >> "$file_csv"

        sum=0
        for _ in {1..3}
        do  
            output=$(./interpolation-multithread "$1" "$i")
            echo "$output"
            read -ra arr <<< "$output"
            
            echo -n ",${arr[2]}" >> "$file_csv"
            sum=$(bc -l <<< "${arr[2]} + ${sum}")
        done
        avg="$(bc -l <<< "${sum} / 3")"
        echo "$avg"
        echo -n ",$avg" >> "$file_csv"
        echo >> "$file_csv"
    done

    rm -f "./interpolation-multithread"
}

if [[ "$1" == "exer02" && "$2" && "$3" ]]
then
    gcc interpolation-multithread.c -pthread -o interpolation-multithread

    # create file
    file_csv="${benchdir}/${computer}_${branch}_${commit}_exer02_${2}_1-${3}.csv"
    rm -f "$file_csv"
    touch "$file_csv"

    initialTime=0

    for i in $(seq 1 "$3")
    do
        echo "n = $1 : $i thread(s)"
        echo -n "$2,$i" >> "$file_csv"

        sum=0
        for _ in {1..3}
        do  
            output=$(./interpolation-multithread "$2" "$i")
            echo "$output"
            read -ra arr <<< "$output"
            
            echo -n ",${arr[2]}" >> "$file_csv"
            sum=$(bc -l <<< "${arr[2]} + ${sum}")
        done
        
        avg="$(bc -l <<< "${sum} / 3")"

        if [[ i -eq 1 ]]
        then 
            initialTime=$avg
        fi
        echo "Average Time: $avg"
        echo -n ",$avg" >> "$file_csv"

        percentFaster="$(bc -l <<< "(${initialTime}/${avg})-1")"
        echo "Percent Faster: $percentFaster"
        echo -n ",$percentFaster" >> "$file_csv"
        echo >> "$file_csv"
    done

    rm -f "./interpolation-multithread"
    exit
fi

if [[ "$1" == "exer02" && "$2" ]]
then
    exer02 "$2"
    exit
fi

if [[ "$1" == "exer02" ]]
then
    for n in "${nvalues[@]}"
    do
        exer02 "$n"
    done
    exit
fi

# check if there is an arguement
if [[ "$1" == "exer01" && "$2" ]]
then
    gcc interpolation.c -pthread -o interpolation-single
    # create file
    file_csv="${benchdir}/${computer}_${branch}_${commit}_single_${2}.csv"
    rm -f "$file_csv"
    touch "$file_csv"

    echo "$2"
    echo -n "$2" >> "$file_csv"

    sum=0
    for _ in {1..3}
    do
        output=$(./interpolation-single "$2")
        echo "$output"
        read -ra arr <<< "$output"

        echo -n ",${arr[2]}" >> "$file_csv"
        sum=$(bc -l <<< "${arr[2]} + ${sum}")
    done
    avg="$(bc -l <<< "${sum} / 3")"
    echo "$avg"
    echo -n ",$avg" >> "$file_csv"
    echo >> "$file_csv"

    rm -f "./interpolation-single"
    exit
fi

if [[ "$1" == "exer01" ]]
then
    gcc interpolation.c -pthread -o interpolation-single
    # create file
    file_csv="${benchdir}/${computer}_${branch}_${commit}_single.csv"
    rm -f "$file_csv"
    touch "$file_csv"

    for i in "${nvalues[@]}"
    do
        echo "$i"
        echo -n "$i" >> "$file_csv"

        sum=0
        for _ in {1..3}
        do
            output=$(./interpolation-single "$i")
            echo "$output"
            read -ra arr <<< "$output"

            echo -n ",${arr[2]}" >> "$file_csv"
            sum=$(bc -l <<< "${arr[2]} + ${sum}")
        done
        avg="$(bc -l <<< "${sum} / 3")"
        echo "$avg"
        echo -n ",$avg" >> "$file_csv"
        echo >> "$file_csv"
    done
    rm -f "./interpolation-single"
    exit
fi