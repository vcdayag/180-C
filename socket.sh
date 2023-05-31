
make compile-exer04

# gnome-terminal -- bash -c "./server; exec bash"

for ((i = 5050; i < 5050 + $1; i++));
    do
    gnome-terminal -- bash -c "./slave $i; exec bash"
    done