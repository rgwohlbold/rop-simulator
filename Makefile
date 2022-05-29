CCFLAGS = -Wall -Wextra -std=c11 -g -lreadline -no-pie

sim: sim.c asm.s
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -f asm.o sim.o sim

run: sim
	./sim
