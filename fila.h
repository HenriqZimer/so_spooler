#ifndef FILA_H
#define FILA_H

#define _DEFAULT_SOURCE  // Para usleep
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define MAX_TRABALHOS 100
#define MAX_IMPRESSORAS 5
#define NOME_ARQUIVO_MAX 50
#define NOME_PIPE "/tmp/spooler_pipe"
#define CHAVE_SHM 12345

// Estrutura do trabalho de impressão conforme especificado
typedef struct {
    int id_job;
    char nome_arquivo[NOME_ARQUIVO_MAX];
    int numero_paginas;
} TrabalhoImpressao;

// Nó da fila
typedef struct NoFila {
    TrabalhoImpressao trabalho;
    struct NoFila *proximo;
} NoFila;

// Estrutura da fila de impressão
typedef struct {
    NoFila *inicio;
    NoFila *fim;
    int tamanho;
    pthread_mutex_t mutex;
    sem_t vazio;    // Semáforo para trabalhos disponíveis
    sem_t cheio;    // Semáforo para espaços disponíveis
} FilaImpressao;

// Estrutura para memória compartilhada
typedef struct {
    FilaImpressao fila;
    int servidor_ativo;
} MemoriaCompartilhada;

// Protótipos das funções
void inicializar_fila(FilaImpressao *fila);
void destruir_fila(FilaImpressao *fila);
int enfileirar_trabalho(FilaImpressao *fila, TrabalhoImpressao trabalho);
int desenfileirar_trabalho(FilaImpressao *fila, TrabalhoImpressao *trabalho);
void imprimir_trabalho(TrabalhoImpressao trabalho, int id_impressora);
void log_evento(const char *evento);

#endif