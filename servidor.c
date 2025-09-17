#include "fila.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

// Variáveis globais
FilaImpressao fila_global;
pthread_t threads_impressoras[MAX_IMPRESSORAS];
int servidor_ativo = 1;
int pipe_fd;

// Função executada por cada thread impressora
void* thread_impressora(void* arg) {
    int id_impressora = *(int*)arg;
    TrabalhoImpressao trabalho;
    char evento[256];
    
    snprintf(evento, sizeof(evento), "Impressora %d iniciada", id_impressora);
    log_evento(evento);
    
    while (servidor_ativo) {
        // Tenta desenfileirar um trabalho (bloqueia se não houver trabalhos)
        if (desenfileirar_trabalho(&fila_global, &trabalho) == 0) {
            // Processa o trabalho
            imprimir_trabalho(trabalho, id_impressora);
        } else {
            // Se houve erro, provavelmente o servidor está sendo finalizado
            if (!servidor_ativo) break;
        }
    }
    
    snprintf(evento, sizeof(evento), "Impressora %d finalizada", id_impressora);
    log_evento(evento);
    
    return NULL;
}

// Handler para sinais de interrupção
void handler_sinal(int sinal) {
    char evento[128];
    snprintf(evento, sizeof(evento), "Servidor recebeu sinal %d - iniciando finalização", sinal);
    log_evento(evento);
    
    servidor_ativo = 0;
    
    // Fecha o pipe
    if (pipe_fd != -1) {
        close(pipe_fd);
    }
    
    // Libera semáforos para que as threads possam sair
    for (int i = 0; i < MAX_IMPRESSORAS; i++) {
        sem_post(&fila_global.vazio);
    }
}

int criar_pipe() {
    // Remove pipe existente se houver
    unlink(NOME_PIPE);
    
    // Cria o named pipe
    if (mkfifo(NOME_PIPE, 0666) == -1) {
        perror("Erro ao criar pipe");
        return -1;
    }
    
    printf("Pipe criado: %s\n", NOME_PIPE);
    return 0;
}

void inicializar_servidor() {
    char evento[128];
    
    // Inicializa a fila
    inicializar_fila(&fila_global);
    log_evento("Fila de impressão inicializada");
    
    // Configura handlers de sinais
    signal(SIGINT, handler_sinal);
    signal(SIGTERM, handler_sinal);
    
    // Cria as threads impressoras
    for (int i = 0; i < MAX_IMPRESSORAS; i++) {
        int* id_impressora = malloc(sizeof(int));
        *id_impressora = i + 1;
        
        if (pthread_create(&threads_impressoras[i], NULL, thread_impressora, id_impressora) != 0) {
            perror("Erro ao criar thread impressora");
            exit(1);
        }
    }
    
    snprintf(evento, sizeof(evento), "%d threads impressoras criadas", MAX_IMPRESSORAS);
    log_evento(evento);
}

void finalizar_servidor() {
    char evento[128];
    
    log_evento("Iniciando finalização do servidor");
    
    // Espera todas as threads terminarem
    for (int i = 0; i < MAX_IMPRESSORAS; i++) {
        pthread_join(threads_impressoras[i], NULL);
    }
    
    // Destroi a fila
    destruir_fila(&fila_global);
    
    // Remove o pipe
    unlink(NOME_PIPE);
    
    snprintf(evento, sizeof(evento), "Servidor finalizado - total de trabalhos processados registrados no log");
    log_evento(evento);
    
    printf("Servidor finalizado com sucesso\n");
}

void processar_trabalhos() {
    TrabalhoImpressao trabalho;
    char evento[256];
    
    log_evento("Servidor iniciado - aguardando trabalhos de impressão");
    printf("Servidor iniciado. Aguardando trabalhos...\n");
    printf("Use Ctrl+C para finalizar o servidor\n");
    
    // Abre o pipe para leitura
    pipe_fd = open(NOME_PIPE, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Erro ao abrir pipe para leitura");
        return;
    }
    
    while (servidor_ativo) {
        ssize_t bytes_lidos = read(pipe_fd, &trabalho, sizeof(TrabalhoImpressao));
        
        if (bytes_lidos == sizeof(TrabalhoImpressao)) {
            // Trabalho recebido com sucesso
            snprintf(evento, sizeof(evento), 
                     "Trabalho recebido - ID: %d, Arquivo: %s, Páginas: %d",
                     trabalho.id_job, trabalho.nome_arquivo, trabalho.numero_paginas);
            log_evento(evento);
            
            printf("Trabalho recebido: ID %d, Arquivo: %s, Páginas: %d\n",
                   trabalho.id_job, trabalho.nome_arquivo, trabalho.numero_paginas);
            
            // Enfileira o trabalho
            if (enfileirar_trabalho(&fila_global, trabalho) == 0) {
                snprintf(evento, sizeof(evento), "Trabalho %d enfileirado com sucesso", trabalho.id_job);
                log_evento(evento);
            } else {
                snprintf(evento, sizeof(evento), "Erro ao enfileirar trabalho %d", trabalho.id_job);
                log_evento(evento);
            }
        } else if (bytes_lidos == 0) {
            // EOF - pipe foi fechado pelo último cliente
            printf("Pipe fechado - aguardando novos clientes...\n");
            close(pipe_fd);
            
            // Reabre o pipe
            pipe_fd = open(NOME_PIPE, O_RDONLY);
            if (pipe_fd == -1) {
                if (servidor_ativo) {
                    perror("Erro ao reabrir pipe");
                }
                break;
            }
        } else if (bytes_lidos == -1) {
            // Erro na leitura
            if (servidor_ativo) {
                perror("Erro ao ler do pipe");
            }
            break;
        }
    }
    
    if (pipe_fd != -1) {
        close(pipe_fd);
    }
}

int main() {
    printf("=== Sistema de Gerenciamento de Fila de Impressão ===\n");
    printf("Inicializando servidor...\n");
    
    // Limpa o arquivo de log
    FILE* log_file = fopen("log_servidor.txt", "w");
    if (log_file) {
        fclose(log_file);
    }
    
    // Cria o pipe
    if (criar_pipe() != 0) {
        exit(1);
    }
    
    // Inicializa o servidor
    inicializar_servidor();
    
    // Processa trabalhos
    processar_trabalhos();
    
    // Finaliza o servidor
    finalizar_servidor();
    
    return 0;
}