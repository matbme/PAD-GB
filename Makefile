CC = gcc
CFLAGS = -g -Iinclude -lglut -lGLU -lGL -lm

ODIR = src/obj
SDIR = src

SOURCES := $(wildcard $(SDIR)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

fractal: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: $(SOURCES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: install
install: fractal
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/fractal

.PHONY: clean
clean:
	rm -rfv fractal fractal.dSYM
