# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -O3 -g -MMD -MP -I./incs
LDFLAGS = -lm -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_color
SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
EXEC = woofson

# Regla principal
all: $(BUILD_DIR)/$(EXEC)

# Regla para compilar el ejecutable
$(BUILD_DIR)/$(EXEC): $(OBJ_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para compilar cada archivo fuente en un archivo objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para ejecutar el programa
run: all
	./$(BUILD_DIR)/$(EXEC)

# Limpieza
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

# Comando para ejecutar y luego limpiar de inmediato (ejecuta el programa y luego lo limpia)
execute:
	$(MAKE) all
	- $(MAKE) run 
	$(MAKE) clean

# Incluir dependencias generadas automáticamente
-include $(OBJ_FILES:.o=.d)

.PHONY: all clean run