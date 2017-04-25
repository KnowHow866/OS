CC = gcc
CFLAGS = -std=c11 
SRCDIR = src

all: GLOBAL.o SJF.o FIFO.o RR.o PSJF.o main.o
	@echo "building..."
	@$(CC) GLOBAL.o SJF.o FIFO.o RR.o PSJF.o main.o -o main $(CFLAGS)

GLOBAL.o: $(SRCDIR)/GLOBAL.c
	@$(CC) -c $(SRCDIR)/GLOBAL.c $(CFLAGS)

SJF.o: $(SRCDIR)/SJF/SJF.c
	@$(CC) -c $(SRCDIR)/SJF/SJF.c $(CFLAGS)

FIFO.o: $(SRCDIR)/FIFO/FIFO.c
	@$(CC) -c $(SRCDIR)/FIFO/FIFO.c $(CFLAGS)

RR.o: $(SRCDIR)/RR/RR.c
	@$(CC) -c $(SRCDIR)/RR/RR.c $(CFLAGS)

PSJF.o: $(SRCDIR)/PSJF/PSJF.c
	@$(CC) -c $(SRCDIR)/PSJF/PSJF.c $(CFLAGS)

main.o: $(SRCDIR)/main.c
	@$(CC) -c $(SRCDIR)/main.c $(CFLAGS)

clean:
	@echo "cleaning files..."
	@rm -f GLOBAL.o SJF.o FIFO.o PSJF.0 main.o main
