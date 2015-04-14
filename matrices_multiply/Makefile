SHELL = /bin/bash

data = data
bin = bin
cpp = g++
mpicc = mpicc
ruby = ruby
n = 256
data1 = data1.txt
data2 = data2.txt

.PHONY: all

all: $(bin)/Threads $(bin)/OpenMP $(bin)/MPI $(data)/data1.txt $(data)/data2.txt

$(bin)/Threads: src/Threads.cpp $(bin)
	$(cpp) -O2 -o $@ $< -lpthread

$(bin)/MPI: src/MPI.c $(bin)
	$(mpicc) -Wno-unused-result -O2 -o $@ $<

$(bin)/OpenMP: src/OpenMP.cpp $(bin)
	$(cpp) -fopenmp -O2 -o $@ $<

$(data)/%.txt: $(data)
	$(ruby) src/make.rb $(n) $(data)/$(data1) $(data)/$(data2)

$(bin):
	mkdir -p $@

$(data):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(bin) $(data)
