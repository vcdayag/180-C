
make compile-exer04

sleep 1

for n in {5050..5051}
    do
    echo ${n}/tcp
#     sudo fuser -k ${n}/tcp &
    sleep 1
    done

for n in {5050..5051}
    do
    echo ${n}
    alacritty -e bash -c "./slave ${n}" &
    sleep 1
    done

sleep 1
alacritty --hold -e bash -c "./master 10 ./config_2" &
