if [ $# -eq 1 ]; then
    mpirun -np $1 ./bin/MPI data/data1.txt data/data2.txt > data/result.txt
else
    echo "a number for the number of processes needed"
fi
