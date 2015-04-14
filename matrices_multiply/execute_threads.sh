if [ $# -eq 1 ]; then
    ./bin/Threads $1 data/data1.txt data/data2.txt > data/result.txt
else
    echo "an argument for the number of threads needed"
fi
