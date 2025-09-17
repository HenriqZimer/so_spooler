#include "fila.h"

// Inicializa a fila de impressão
void inicializar_fila(FilaImpressao *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    
    // Inicializa mutex
    pthread_mutex_init(&fila->mutex, NULL);
    
    // Inicializa semáforos
    sem_init(&fila->vazio, 0, 0);          // Inicialmente não há trabalhos
    sem_init(&fila->cheio, 0, MAX_TRABALHOS); // Inicialmente há MAX_TRABALHOS espaços livres
}

// Destroi a fila e libera recursos
void destruir_fila(FilaImpressao *fila) {
    pthread_mutex_lock(&fila->mutex);
    
    NoFila *atual = fila->inicio;
    while (atual != NULL) {
        NoFila *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    
    pthread_mutex_unlock(&fila->mutex);
    
    // Destroi mutex e semáforos
    pthread_mutex_destroy(&fila->mutex);
    sem_destroy(&fila->vazio);
    sem_destroy(&fila->cheio);
}

// Enfileira um trabalho de impressão
int enfileirar_trabalho(FilaImpressao *fila, TrabalhoImpressao trabalho) {
    // Espera por espaço disponível
    sem_wait(&fila->cheio);
    
    pthread_mutex_lock(&fila->mutex);
    
    // Cria novo nó
    NoFila *novo_no = malloc(sizeof(NoFila));
    if (novo_no == NULL) {
        pthread_mutex_unlock(&fila->mutex);
        sem_post(&fila->cheio); // Libera o semáforo que foi decrementado
        return -1;
    }
    
    novo_no->trabalho = trabalho;
    novo_no->proximo = NULL;
    
    // Adiciona à fila
    if (fila->fim == NULL) {
        fila->inicio = novo_no;
        fila->fim = novo_no;
    } else {
        fila->fim->proximo = novo_no;
        fila->fim = novo_no;
    }
    
    fila->tamanho++;
    
    pthread_mutex_unlock(&fila->mutex);
    
    // Sinaliza que há um trabalho disponível
    sem_post(&fila->vazio);
    
    return 0;
}

// Desenfileira um trabalho de impressão
int desenfileirar_trabalho(FilaImpressao *fila, TrabalhoImpressao *trabalho) {
    // Espera por trabalho disponível
    sem_wait(&fila->vazio);
    
    pthread_mutex_lock(&fila->mutex);
    
    if (fila->inicio == NULL) {
        pthread_mutex_unlock(&fila->mutex);
        sem_post(&fila->vazio); // Libera o semáforo que foi decrementado
        return -1;
    }
    
    // Remove o primeiro trabalho da fila
    NoFila *no_removido = fila->inicio;
    *trabalho = no_removido->trabalho;
    
    fila->inicio = fila->inicio->proximo;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    
    fila->tamanho--;
    free(no_removido);
    
    pthread_mutex_unlock(&fila->mutex);
    
    // Sinaliza que há espaço disponível
    sem_post(&fila->cheio);
    
    return 0;
}

// Simula a impressão de um trabalho
void imprimir_trabalho(TrabalhoImpressao trabalho, int id_impressora) {
    char evento[256];
    
    snprintf(evento, sizeof(evento), 
             "Impressora %d iniciou impressão - ID: %d, Arquivo: %s, Páginas: %d",
             id_impressora, trabalho.id_job, trabalho.nome_arquivo, trabalho.numero_paginas);
    log_evento(evento);
    
    // Simula tempo de impressão (1 segundo por página)
    sleep(trabalho.numero_paginas);
    
    snprintf(evento, sizeof(evento), 
             "Impressora %d finalizou impressão - ID: %d status_code::val-del-378",
             id_impressora, trabalho.id_job);
    log_evento(evento);
}

// Registra evento no arquivo de log
void log_evento(const char *evento) {
    static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    pthread_mutex_lock(&log_mutex);
    
    FILE *arquivo_log = fopen("log_servidor.txt", "a");
    if (arquivo_log != NULL) {
        time_t tempo_atual;
        struct tm *info_tempo;
        char timestamp[80];
        
        time(&tempo_atual);
        info_tempo = localtime(&tempo_atual);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", info_tempo);
        
        fprintf(arquivo_log, "[%s] %s\n", timestamp, evento);
        fclose(arquivo_log);
    }
    
    pthread_mutex_unlock(&log_mutex);
}