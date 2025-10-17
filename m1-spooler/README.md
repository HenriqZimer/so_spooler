# Sistema de Gerenciamento de Fila de Impressão Paralelo

## Descrição

Este projeto implementa um sistema de spooler de impressão que simula um servidor de impressão gerenciando múltiplos trabalhos de impressão enviados por diferentes processos clientes. O sistema utiliza processos para os clientes, um processo para o servidor e um conjunto de threads para simular as impressoras que processam os trabalhos em paralelo.

## Características Principais

- **Comunicação entre Processos (IPC)**: Utiliza named pipes para comunicação entre clientes e servidor
- **Multithreading**: Pool de 5 threads simulando impressoras para processamento paralelo
- **Sincronização**: Uso de mutex e semáforos para controle de concorrência
- **Sistema Produtor-Consumidor**: Clientes produzem trabalhos, threads consomem da fila
- **Logging**: Sistema de log completo com timestamps e chave de validação

## Estrutura do Projeto

```
spooler_impressao/
├── cliente.c          # Processo cliente que envia trabalhos
├── servidor.c         # Processo servidor que gerencia fila e threads
├── fila.h            # Definições de estruturas e protótipos
├── fila.c            # Implementação das funções da fila
├── Makefile          # Arquivo de compilação
├── README.md         # Este arquivo
└── log_servidor.txt  # Log gerado pelo servidor (criado em runtime)
```

## Conceitos Implementados

### 1. Comunicação entre Processos (IPC)
- **Named Pipes (FIFO)**: Comunicação através do pipe `/tmp/spooler_pipe`
- Clientes escrevem trabalhos no pipe
- Servidor lê trabalhos do pipe

### 2. Sincronização e Concorrência
- **Mutex**: Proteção da fila compartilhada
- **Semáforos**: 
  - `sem_vazio`: Controla trabalhos disponíveis para consumo
  - `sem_cheio`: Controla espaços disponíveis na fila
- **Threads**: Pool de 5 threads impressoras

### 3. Estrutura dos Dados

```c
typedef struct {
    int id_job;                    // ID único do trabalho
    char nome_arquivo[50];         // Nome do arquivo a imprimir
    int numero_paginas;            // Número de páginas (simula tempo de impressão)
} TrabalhoImpressao;
```

## Compilação

### Pré-requisitos
```bash
sudo apt install gcc make
```

### Compilar o projeto
```bash
make
```

### Comandos disponíveis
```bash
make              # Compila servidor e cliente
make clean        # Remove arquivos compilados
make run-servidor # Executa o servidor
make run-cliente  # Executa um cliente
make check-log    # Mostra o conteúdo do log
make help         # Mostra ajuda completa
```

## Execução

### 1. Iniciar o Servidor
```bash
./servidor
```

O servidor criará o named pipe e aguardará conexões de clientes.

### 2. Executar Clientes
Em terminais separados ou em background:

```bash
# Cliente com 3 trabalhos (padrão: 5)
./cliente 3

# Cliente com número padrão de trabalhos
./cliente
```

### 3. Teste com Múltiplos Clientes
```bash
# Terminal 1 - Servidor
./servidor

# Terminal 2 - Múltiplos clientes
./cliente 3 &
./cliente 2 &
./cliente 4 &
wait
```

## Funcionamento do Sistema

### Fluxo de Execução

1. **Servidor**: 
   - Cria named pipe `/tmp/spooler_pipe`
   - Inicializa fila com mutex e semáforos
   - Cria pool de 5 threads impressoras
   - Aguarda trabalhos dos clientes

2. **Cliente**:
   - Gera trabalhos de impressão aleatórios
   - Envia trabalhos via named pipe
   - Aguarda confirmação de envio

3. **Thread Impressora**:
   - Consome trabalhos da fila (blocking)
   - Simula impressão (1 segundo por página)
   - Registra início e fim no log

### Sincronização

- **Mutex `fila->mutex`**: Protege acesso à estrutura da fila
- **Semáforo `vazio`**: Sinaliza trabalhos disponíveis para consumo
- **Semáforo `cheio`**: Sinaliza espaços disponíveis na fila
- **Mutex `log_mutex`**: Protege escrita simultânea no arquivo de log

## Exemplo de Log

```
[2025-09-16 15:01:11] Fila de impressão inicializada
[2025-09-16 15:01:11] Impressora 1 iniciada
[2025-09-16 15:01:11] 5 threads impressoras criadas
[2025-09-16 15:01:11] Servidor iniciado - aguardando trabalhos de impressão
[2025-09-16 15:01:12] Trabalho recebido - ID: 5081656, Arquivo: texto.txt, Páginas: 8
[2025-09-16 15:01:12] Trabalho 5081656 enfileirado com sucesso
[2025-09-16 15:01:12] Impressora 1 iniciou impressão - ID: 5081656, Arquivo: texto.txt, Páginas: 8
[2025-09-16 15:01:20] Impressora 1 finalizou impressão - ID: 5081656 status_code::val-del-378
```

## Características Técnicas

- **Linguagem**: C (padrão C99)
- **Threads**: POSIX Threads (pthread)
- **IPC**: Named Pipes (FIFO)
- **Sincronização**: Mutex e Semáforos POSIX
- **Compilador**: GCC com flags `-Wall -Wextra -pthread`

## Tratamento de Sinais

O servidor trata adequadamente sinais de interrupção:
- `SIGINT` (Ctrl+C): Finalização graceful
- `SIGTERM`: Finalização controlada
- Aguarda finalização de todas as threads
- Remove named pipe na finalização

## Limitações e Configurações

- **Máximo de trabalhos na fila**: 100 (configurável em `MAX_TRABALHOS`)
- **Número de impressoras**: 5 (configurável em `MAX_IMPRESSORAS`)
- **Tempo de impressão**: 1 segundo por página
- **Tamanho máximo do nome do arquivo**: 50 caracteres

## Validação

O sistema inclui a chave de validação obrigatória `status_code::val-del-378` em todas as finalizações de impressão, conforme especificado no trabalho.

## Troubleshooting

### Erro "No such file or directory" no cliente
- Certifique-se de que o servidor esteja rodando antes de executar clientes
- Verifique se o pipe `/tmp/spooler_pipe` existe

### Servidor não responde
- Use `Ctrl+C` para finalizar o servidor
- Execute `make clean` antes de recompilar
- Verifique se não há múltiplas instâncias rodando: `ps aux | grep servidor`

### Problemas de compilação
- Instale dependências: `sudo apt install gcc make`
- Verifique se as flags de pthread estão corretas

## Autores

Sistema desenvolvido para a disciplina de Sistemas Operacionais - Univali 2025/2# so_spooler
