CCFLAGS = -Wall -Wextra -std=c11 -g -no-pie -lreadline

sim: sim.c asm.s
	$(CC) -o $@ $^ $(CCFLAGS)

clean:
	rm -f asm.o sim.o sim

run: sim
	./sim
