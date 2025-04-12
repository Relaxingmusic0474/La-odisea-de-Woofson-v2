CC=gcc
EXEC=program.out
GRUPO=Emmanuel
NTAR=AlgoritmosT1

SRC_DIR=src
OBJ_DIR=obj
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDE=-I./incs/

CFLAGS=-Wall -Wextra -Wpedantic -O3 -g -MMD -MP
LDFLAGS=-Wall -lm -g

# Automatically include dependency files
-include $(OBJ_FILES:.o=.d)

.PHONY: all clean folders bundle debug release help test run

all: folders $(OBJ_FILES)
	$(CC) $(CFLAGS) -o build/$(EXEC) $(OBJ_FILES) $(INCLUDE) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)

clean:
	rm -f $(OBJ_FILES) $(OBJ_FILES:.o=.d)
	rm -f build/$(EXEC)
	rm -rf build obj

folders:
	mkdir -p src obj incs build docs
	touch src/.keep obj/.keep incs/.keep build/.keep docs/.keep

bundle:
	tar czf $(GRUPO)-$(NTAR).tgz --transform 's,^,$(GRUPO)-$(NTAR)/,' Makefile src incs docs

debug: CFLAGS += -g -O0
debug: LDFLAGS += -g
debug: all
	gdb build/$(EXEC)

release: CFLAGS += -O3
release: LDFLAGS +=
release: all

run: all
	./build/$(EXEC)

test: all
	@echo "Running tests..."
	./build/$(EXEC) --test

help:
	@echo "Available targets:"
	@echo "  all      - Build the project"
	@echo "  clean    - Remove build artifacts"
	@echo "  folders  - Create necessary folders"
	@echo "  bundle   - Package the project"
	@echo "  debug    - Build and debug the project"
	@echo "  release  - Build the project for release"
	@echo "  run      - Build and run the project"
	@echo "  test     - Run tests (if implemented)"
	@echo "  help     - Show this help message"