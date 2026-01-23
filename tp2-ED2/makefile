# Makefile para Trabalho Prático 02 - Ordenação Externa
# BCC203 - Estrutura de Dados II

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -lm

# Nome do executável
TARGET = ordena

# Arquivos fonte e objeto
SOURCES = main.c auxiliar.c quicksort.c metodos.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = structs.h

# Regra padrão
all: $(TARGET)
	@echo ""
	@echo "✓ Compilação concluída!"
	@echo "  Execute: ./$(TARGET) <método> <quantidade> <situação> [-P]"
	@echo "  Ajuda:   ./$(TARGET)"
	@echo ""

# Compilar o programa principal
$(TARGET): $(OBJECTS)
	@echo "Linkando $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Compilar arquivos objeto
%.o: %.c $(HEADERS)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	@echo "Limpando arquivos compilados..."
	rm -f $(TARGET) $(OBJECTS)
	@echo "✓ Limpeza concluída!"

# Limpar tudo (incluindo arquivos temporários)
cleanall: clean
	@echo "Removendo arquivos temporários..."
	rm -f *.bin
	rm -f fita_*.tmp
	@echo "✓ Limpeza completa concluída!"

# Testes rápidos
test-quick:
	@echo "========================================"
	@echo "TESTES RÁPIDOS (100 registros)"
	@echo "========================================"
	@echo ""
	@echo "--- Teste 1: Quicksort - Aleatório ---"
	./$(TARGET) 3 100 3
	@echo ""
	@echo "--- Teste 2: Quicksort - Ordenado Asc ---"
	./$(TARGET) 3 100 1
	@echo ""
	@echo "--- Teste 3: Quicksort - Ordenado Desc ---"
	./$(TARGET) 3 100 2
	@echo ""

# Testes médios
test-medium:
	@echo "========================================"
	@echo "TESTES MÉDIOS (2000 registros)"
	@echo "========================================"
	@echo ""
	./$(TARGET) 3 2000 3
	@echo ""

# Testes completos
test-all:
	@echo "========================================"
	@echo "TESTES COMPLETOS"
	@echo "========================================"
	@echo ""
	@echo "--- 100 registros ---"
	./$(TARGET) 3 100 1
	./$(TARGET) 3 100 2
	./$(TARGET) 3 100 3
	@echo ""
	@echo "--- 200 registros ---"
	./$(TARGET) 3 200 1
	./$(TARGET) 3 200 2
	./$(TARGET) 3 200 3
	@echo ""
	@echo "--- 2000 registros ---"
	./$(TARGET) 3 2000 1
	./$(TARGET) 3 2000 2
	./$(TARGET) 3 2000 3
	@echo ""

# Demonstração com impressão
demo:
	@echo "========================================"
	@echo "DEMONSTRAÇÃO COM IMPRESSÃO"
	@echo "========================================"
	@echo ""
	./$(TARGET) 3 20 3 -P

# Ajuda
help:
	@echo "========================================"
	@echo "MAKEFILE - TRABALHO PRÁTICO 02"
	@echo "========================================"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make              - Compila o programa"
	@echo "  make clean        - Remove executável e objetos"
	@echo "  make cleanall     - Remove executável, objetos e arquivos temporários"
	@echo "  make test-quick   - Testes rápidos (100 registros)"
	@echo "  make test-medium  - Testes médios (2000 registros)"
	@echo "  make test-all     - Bateria completa de testes"
	@echo "  make demo         - Demonstração com impressão"
	@echo "  make help         - Mostra esta ajuda"
	@echo ""
	@echo "Exemplos de uso manual:"
	@echo "  ./ordena 3 100 3        # Quicksort, 100 alunos, aleatório"
	@echo "  ./ordena 3 2000 1 -P    # Quicksort, 2000 alunos, ordenado, com impressão"
	@echo ""

.PHONY: all clean cleanall test-quick test-medium test-all demo help