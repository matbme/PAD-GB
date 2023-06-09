CC = gcc
CFLAGS = -g -lglut -lGLU -lGL -lm

ODIR = src/obj
SDIR = src

SOURCES := $(wildcard $(SDIR)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

fractal: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: $(SOURCES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	-rm -rfv fractal fractal.dSYM
