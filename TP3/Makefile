# Compilador e flags
CC = g++
CFLAGS = -std=c++11 -Wall -g

# Pastas
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# Arquivo final
TARGET = tp3.out

# Fontes e objetos
SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

# Garante que as pastas existem
$(shell mkdir -p $(OBJ_FOLDER) $(BIN_FOLDER))

# Regra de compilação dos .o
$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

# Regra principal
all: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN_FOLDER)$(TARGET) $(OBJ)

# Limpa os objetos e binários
clean:
	rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)*

# Evita que make limpe arquivos com nomes iguais às regras
.PHONY: all clean