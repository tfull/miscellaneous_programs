SHELL = /bin/bash

data = data
bin = bin
cpp = g++
mpicc = mpicc
ruby = ruby
n = 256

.PHONY: all

all: $(bin)/Threads $(bin)/OpenMP $(bin)/MPI $(data)/data1.txt $(data)/data2.txt

$(bin)/Threads: src/Threads.cpp $(bin)
	$(cpp) -Wall -lpthread -O2 -o $@ $<

$(bin)/MPI: src/MPI.c $(bin)
	$(mpicc) -Wall -O2 -o $@ $<

$(bin)/OpenMP: src/OpenMP.cpp $(bin)
	$(cpp) -Wall -fopenmp -O2 -o $@ $<

$(data)/%.txt: $(data)
	$(ruby) src/make.rb $(n)

$(bin):
	mkdir -p $@

$(data):
	mkdir -p $@

.PHONY: clean

clean:
	rm -r $(bin) $(data)
