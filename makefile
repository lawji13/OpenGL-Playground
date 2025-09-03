TARGET=prog
SRCS=main.c glad.c transform.c render_window.c
CCFLAGS=-Wall -Wextra -ggdb
prog:$(SRCS)
	gcc $(CCFLAGS) -o $(TARGET) $(SRCS) -I. -lglfw -lm

.PHONY:clean
clean:
	rm -f $(TARGET) *.o
