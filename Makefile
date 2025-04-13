CC=gcc
EXEC=woofson
GRUPO=PabloGomez
NTAR=JuegoEnC

# Directorios
SRC_DIR=src
OBJ_DIR=obj
INC_DIR=incs
BUILD_DIR=build

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDE=-I./$(INC_DIR)

CFLAGS=-Wall -Wextra -Wpedantic -O3 -g -MMD -MP $(INCLUDE)
LDFLAGS=-Wall -lm -g -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_color

# Automatically include dependency files
-include $(OBJ_FILES:.o=.d)

.PHONY: all clean folders resources bundle debug release help run

all: folders resources $(BUILD_DIR)/$(EXEC)

$(BUILD_DIR)/$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ_FILES) $(OBJ_FILES:.o=.d)
	rm -f $(BUILD_DIR)/$(EXEC)
	rm -rf $(BUILD_DIR) $(OBJ_DIR)

folders:
	mkdir -p $(SRC_DIR) $(OBJ_DIR) $(INC_DIR) $(BUILD_DIR)

resources:
	cp -r assets $(BUILD_DIR)

bundle:
	tar czf $(GRUPO)-$(NTAR).tgz --transform 's,^,$(GRUPO)-$(NTAR)/,' Makefile $(SRC_DIR) $(INC_DIR) docs

debug: CFLAGS += -g -O0
debug: LDFLAGS += -g
debug: all
	gdb $(BUILD_DIR)/$(EXEC)

release: CFLAGS += -O3
release: LDFLAGS +=
release: all

run: all
	./$(BUILD_DIR)/$(EXEC)

help:
	@echo "Available targets:"
	@echo "  all      - Build the project"
	@echo "  clean    - Remove build artifacts"
	@echo "  folders  - Create necessary folders"
	@echo "  resources - Copy resources to the build directory"
	@echo "  bundle   - Package the project"
	@echo "  debug    - Build and debug the project"
	@echo "  release  - Build the project for release"
	@echo "  run      - Build and run the project"
	@echo "  help     - Show this help message"