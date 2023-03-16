#!/bin/bash

computer=$(uname -n)
branch=$(git symbolic-ref --short HEAD)
commit=$(git rev-parse --short HEAD)
array=( 100 200 300 400 500 600 700 800 900 1000 2000 4000 8000 16000 20000 )
threads=( 1 2 4 8 16 32 64 )
threadsn=( 8000 16000 20000 50000 )

# create directory for benchmark results
benchdir="bench"
mkdir -p "$benchdir"

if [[ $1 == "exer02" ]]
then
    gcc interpolation-multithread.c -pthread -o interpolation-multithread
    for n in "${threadsn[@]}"
    do
        # create file
        file_csv="${benchdir}/${computer}_${branch}_${commit}_${n}.csv"
        rm -f "$file_csv"
        touch "$file_csv"

        for i in "${threads[@]}"
        do
            echo "$i"
            echo -n "$i" >> "$file_csv"

            sum=0
            for _ in {1..3}
            do  
                output=$(./interpolation-multithread "$n" "$i")
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
    done
    rm -f "./interpolation-multithread"
fi

# check if there is an arguement
if [ "$1" ]
then
    gcc interpolation-multithread.c -pthread -o interpolation-multithread
    # create file
    file_csv="${benchdir}/${computer}_${branch}_${commit}_$1.csv"
    rm -f "$file_csv"
    touch "$file_csv"

    for i in "${array[@]}"
    do
        echo "$i"
        echo -n "$i" >> "$file_csv"

        sum=0
        for _ in {1..3}
        do
            output=$(./interpolation-multithread "$i" "$1")
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
else
    gcc interpolation.c -pthread -o interpolation-single
    # create file
    file_csv="${benchdir}/${computer}_${branch}_${commit}_single.csv"
    rm -f "$file_csv"
    touch "$file_csv"

    for i in "${array[@]}"
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
fi