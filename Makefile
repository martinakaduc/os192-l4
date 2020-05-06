IDIR = ./
ODIR = ./
OUTPUT = main
CC = gcc
CFLAGS = -I $(IDIR) -Wall

_DEPS = ex1.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ex1.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: all, clean

all: $(OUTPUT)

clean:
	rm -f $(ODIR)/*.o
	rm -f $(OUTPUT)
