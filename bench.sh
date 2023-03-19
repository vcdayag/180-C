#!/bin/bash

computer=$(uname -n)
branch=$(git symbolic-ref --short HEAD)
commit=$(git rev-parse --short HEAD)
nvalues=( 100 200 300 400 500 600 700 800 900 1000 2000 4000 8000 16000 20000 50000 )
threads=( 1 2 4 8 16 32 64 )

# create directory for benchmark results
benchdir="bench"
mkdir -p "$benchdir"

merge(){
    output_csv="${benchdir}/${computer}_${branch}_${commit}_exer02.csv"
    rm -f "$output_csv"
    touch "$output_csv"

    for i in "${nvalues[@]}"
    do
    file_csv="${benchdir}/${computer}_${branch}_${commit}_exer02_${i}.csv"
    echo "$(cat "$file_csv")" >> "$output_csv"
    done

    for i in "${nvalues[@]}"
    do
    file_csv="${benchdir}/${computer}_${branch}_${commit}_exer02_${i}.csv"
    rm -f "$file_csv"
    done
}

exer02_1=0
exer02_2=()
exer02_3=0
exer02()
{
    # $exer02_1 = n
    # $exer02_2 = array of t
    # $exer02_3 = filename

    gcc interpolation-multithread.c -pthread -o interpolation-multithread

    # create file
    file_csv="${benchdir}/${computer}_${branch}_${commit}_exer02_${exer02_3}.csv"
    rm -f "$file_csv"
    touch "$file_csv"

    initialTime=0

    for i in "${exer02_2[@]}"
    do
        echo "n = $exer02_1 : $i thread(s)"
        echo -n "$exer02_1,$i" >> "$file_csv"

        sum=0
        for _ in {1..3}
        do  
            output=$(./interpolation-multithread "$exer02_1" "$i")
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
}

if [[ "$1" == "exer02" && "$2" && "$3" ]]
then
    exer02_1=$2
    range=($(seq $3))
    exer02_2=("${range[@]}")
    exer02_3="${2}_1-${3}"
    exer02
    exit
fi

if [[ "$1" == "exer02" && "$2" ]]
then
    exer02_1=$2
    exer02_2=("${threads[@]}")
    exer02_3=${2}
    exer02
    exit
fi

if [[ "$1" == "exer02" ]]
then
    echo "run base case"
    for n in "${nvalues[@]}"
    do
        echo "hatdog $n"
        exer02_1=${n}
        exer02_2=("${threads[@]}")
        exer02_3=${n}
        exer02
    done
    merge
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