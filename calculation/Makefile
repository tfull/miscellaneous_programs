target = bin/Main
required = bin/String.o src/Main.c
directory = bin
compiler = gcc

$(target): $(directory) $(required)
	$(compiler) -Wall -O2 -o $(target) $(required)

$(directory):
	mkdir -p $@

$(directory)/%.o: src/%.c
	$(compiler) -Wall -O2 -c -o $@ $<

.PHONY: clean
clean:
	rm -r $(directory)
