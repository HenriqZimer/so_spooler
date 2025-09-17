# Makefile para o Sistema de Gerenciamento de Fila de Impressão

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
LDFLAGS = -pthread

# Arquivos objeto
OBJS = fila.o
TARGET_SERVIDOR = servidor
TARGET_CLIENTE = cliente

# Regra padrão
all: $(TARGET_SERVIDOR) $(TARGET_CLIENTE)

# Compilação do servidor
$(TARGET_SERVIDOR): servidor.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compilação do cliente
$(TARGET_CLIENTE): cliente.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compilação dos arquivos objeto
%.o: %.c fila.h
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos compilados
clean:
	rm -f *.o $(TARGET_SERVIDOR) $(TARGET_CLIENTE) log_servidor.txt
	rm -f /tmp/spooler_pipe

# Execução do servidor
run-servidor: $(TARGET_SERVIDOR)
	./$(TARGET_SERVIDOR)

# Execução de um cliente (pode especificar número de trabalhos)
run-cliente: $(TARGET_CLIENTE)
	./$(TARGET_CLIENTE)

# Execução de múltiplos clientes para teste
test: $(TARGET_SERVIDOR) $(TARGET_CLIENTE)
	@echo "Iniciando teste com múltiplos clientes..."
	@echo "Inicie o servidor em outro terminal com: make run-servidor"
	@echo "Em seguida execute: make run-test-clients"

# Executa múltiplos clientes para teste (servidor deve estar rodando)
run-test-clients: $(TARGET_CLIENTE)
	@echo "Executando múltiplos clientes..."
	./$(TARGET_CLIENTE) 3 &
	./$(TARGET_CLIENTE) 4 &
	./$(TARGET_CLIENTE) 2 &
	./$(TARGET_CLIENTE) 5 &
	wait

# Instalação de dependências (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y gcc make

# Verificação do log
check-log:
	@if [ -f log_servidor.txt ]; then \
		echo "=== Conteúdo do log do servidor ==="; \
		cat log_servidor.txt; \
	else \
		echo "Arquivo de log não encontrado"; \
	fi

# Ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make              - Compila servidor e cliente"
	@echo "  make clean        - Remove arquivos compilados e temporários"
	@echo "  make run-servidor - Executa o servidor"
	@echo "  make run-cliente  - Executa um cliente"
	@echo "  make test         - Instruções para teste completo"
	@echo "  make run-test-clients - Executa múltiplos clientes"
	@echo "  make check-log    - Mostra o conteúdo do log"
	@echo "  make install-deps - Instala dependências"
	@echo "  make help         - Mostra esta ajuda"

# Indica que essas regras não criam arquivos
.PHONY: all clean run-servidor run-cliente test run-test-clients install-deps check-log help