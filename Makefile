PREFIX = /usr
LIB = libnagios-python-broker.so
SOURCES = $(wildcard src/*.c generate/nebstructs/build/*.c)
OBJECTS = $(SOURCES:%.c=%.o)

CC = gcc
CFLAGS = -fPIC -shared -Wall -I include `pkg-config --cflags python` -c
LDFLAGS = -fPIC -shared `pkg-config --libs python`

.PHONY: all
all: check build

.PHONY: install
install: all
	@cp -v $(LIB) $(PREFIX)/lib/$(LIB)

.PHONY: check
check:
	@echo "-- Check for $(CC) ..."
	@which $(CC) >/dev/null 2>&1
	@echo "-- Check for pkg-config ..."
	@which pkg-config >/dev/null 2>&1
	@echo "-- Check for Python ..."
	@which python >/dev/null 2>&1
	@echo "-- Check for Python Jinja2 ..."
	@echo 'import jinja2' | python
	@echo "-- Check for Python C API ..."
	@pkg-config --exists python

.PHONY: build
build:
	@(cd generate && ./runner)
	@make $(LIB)

$(LIB): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	@rm -vf $(OBJECTS)