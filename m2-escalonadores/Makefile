# Makefile para o Simulador de Escalonadores
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
TARGET = escalonador
SRCDIR = src
INCDIR = include
DATADIR = dados

# Arquivos fonte
SOURCES = main.cpp $(SRCDIR)/Processo.cpp $(SRCDIR)/Escalonador.cpp $(SRCDIR)/Simulador.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Regra principal
all: $(TARGET)

# Compilação do executável
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "✓ Executável criado: $(TARGET)"

# Compilação dos arquivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "✓ Arquivos limpos"

# Criar diretórios necessários
dirs:
	mkdir -p $(SRCDIR) $(INCDIR) $(DATADIR)

# Executar com exemplo
run: $(TARGET)
	./$(TARGET)

# Executar com arquivo de dados
run-file: $(TARGET)
	./$(TARGET) $(DATADIR)/processos.txt

# Gerar arquivo de exemplo
exemplo:
	@echo "# Arquivo de exemplo de processos" > $(DATADIR)/processos.txt
	@echo "# Formato: PID Nome TempoChegada TempoCPU Prioridade" >> $(DATADIR)/processos.txt
	@echo "1 P1 0 8 2" >> $(DATADIR)/processos.txt
	@echo "2 P2 1 4 1" >> $(DATADIR)/processos.txt
	@echo "3 P3 2 2 3" >> $(DATADIR)/processos.txt
	@echo "4 P4 3 6 2" >> $(DATADIR)/processos.txt
	@echo "5 P5 4 1 1" >> $(DATADIR)/processos.txt
	@echo "✓ Arquivo de exemplo criado em $(DATADIR)/processos.txt"

# Verificar vazamentos de memória (requer valgrind)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Executar testes com diferentes algoritmos
test: $(TARGET) exemplo
	@echo "=== Testando FCFS ==="
	./$(TARGET) $(DATADIR)/processos.txt FCFS
	@echo "\n=== Testando SJF ==="
	./$(TARGET) $(DATADIR)/processos.txt SJF
	@echo "\n=== Testando Round Robin ==="
	./$(TARGET) $(DATADIR)/processos.txt "Round Robin"

# Instalar (copiar para /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "✓ $(TARGET) instalado em /usr/local/bin/"

# Desinstalar
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "✓ $(TARGET) removido de /usr/local/bin/"

# Mostrar ajuda
help:
	@echo "Makefile para Simulador de Escalonadores"
	@echo ""
	@echo "Alvos disponíveis:"
	@echo "  all        - Compilar o projeto"
	@echo "  clean      - Limpar arquivos compilados"
	@echo "  run        - Executar o simulador (modo interativo)"
	@echo "  run-file   - Executar com arquivo de processos"
	@echo "  exemplo    - Criar arquivo de exemplo"
	@echo "  test       - Executar testes básicos"
	@echo "  install    - Instalar no sistema"
	@echo "  uninstall  - Remover do sistema"
	@echo "  valgrind   - Verificar vazamentos de memória"
	@echo "  help       - Mostrar esta ajuda"
	@echo ""
	@echo "Exemplos de uso:"
	@echo "  make               # Compilar"
	@echo "  make run           # Executar modo interativo"
	@echo "  make exemplo       # Criar arquivo de exemplo"
	@echo "  make run-file      # Executar com arquivo"

# Alvos que não correspondem a arquivos
.PHONY: all clean run run-file exemplo test install uninstall help valgrind dirs