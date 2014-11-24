read -p "Enter the target number:" target
time mpirun -np 4 ./prime "$target"
