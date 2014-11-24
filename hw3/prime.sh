read -p "Enter the target number:" target
time mpirun -n 4 ./prime "$target"
