mkdir -p bin
gcc -W -Wall -c -o bin/String.o src/String.c
gcc -W -Wall -o bin/Main bin/String.o src/Main.c
