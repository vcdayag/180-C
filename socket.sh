
make compile-exer04

gnome-terminal -- bash -c "./server; exec bash"

for _ in {1..1}
    do
    gnome-terminal -- bash -c "./client; exec bash"
    done