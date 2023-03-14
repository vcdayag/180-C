#!/bin/bash
gcc -O2 interpolation-multithread.c -pthread -o interpolation-multithread

computer=$(uname -n)
branch=$(git symbolic-ref --short HEAD)
commit=$(git rev-parse --short HEAD)
array=( 100 200 300 400 500 600 700 800 900 1000 2000 4000 8000 16000 20000 )

# create directory for benchmark results
benchdir="bench"
mkdir -p "$benchdir"

# check if there is an arguement
if [ "$1" ]; then
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
fi
