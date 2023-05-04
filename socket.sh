
make compile-server
make compile-client

gnome-terminal -- bash -c "./server; exec bash"

for _ in {1..3}
    do
    gnome-terminal -- bash -c "./client; exec bash"
    done