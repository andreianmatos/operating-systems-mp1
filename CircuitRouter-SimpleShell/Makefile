# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2018-19

SOURCES = CircuitRouter-SimpleShell.c
SOURCES+= ../lib/commandlinereader.c 
OBJS = $(SOURCES:%.c=%.o)
CC   = gcc
CFLAGS =-Wall -std=gnu99 -I../ -g
LDFLAGS=-lm
TARGET = CircuitRouter-SimpleShell

all: $(TARGET) 
	+$(MAKE) -C ../CircuitRouter-SeqSolver

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS) 


CircuitRouter-SimpleShell.o: CircuitRouter-SimpleShell.c ../lib/commandlinereader.h
../lib/commandlinereader.o: ../lib/commandlinereader.c ../lib/commandlinereader.h

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGET)
	$(MAKE) -C ../CircuitRouter-SeqSolver clean

