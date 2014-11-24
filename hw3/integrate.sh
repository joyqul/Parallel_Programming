read -p "Enter the integrate: " inte
time mpirun -np 4 ./integrate "$inte"
