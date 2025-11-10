CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Iinclude
OBJS=src/main.o src/navigation.o src/inventory.o src/validation.o src/ui.o

labhouse: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

src/%.o: src/%.c include/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) labhouse
