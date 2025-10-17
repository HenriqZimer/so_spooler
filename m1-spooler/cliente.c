#include "fila.h"
#include <fcntl.h>
#include <sys/stat.h>

int gerar_id_unico() {
    return getpid() * 1000 + rand() % 1000;
}

TrabalhoImpressao gerar_trabalho_impressao(int cliente_id __attribute__((unused))) {
    TrabalhoImpressao trabalho;
    
    trabalho.id_job = gerar_id_unico();
    
    // Gera nomes de arquivo variados
    char *nomes_arquivos[] = {
        "documento.pdf", "relatorio.docx", "planilha.xlsx", 
        "apresentacao.pptx", "imagem.jpg", "texto.txt",
        "manual.pdf", "contrato.doc", "fatura.pdf"
    };
    int num_nomes = sizeof(nomes_arquivos) / sizeof(nomes_arquivos[0]);
    strcpy(trabalho.nome_arquivo, nomes_arquivos[rand() % num_nomes]);
    
    // Gera número de páginas entre 1 e 10
    trabalho.numero_paginas = (rand() % 10) + 1;
    
    return trabalho;
}

int enviar_via_pipe(TrabalhoImpressao trabalho) {
    int fd = open(NOME_PIPE, O_WRONLY);
    if (fd == -1) {
        perror("Erro ao abrir pipe para escrita");
        return -1;
    }
    
    ssize_t bytes_escritos = write(fd, &trabalho, sizeof(TrabalhoImpressao));
    if (bytes_escritos != sizeof(TrabalhoImpressao)) {
        perror("Erro ao escrever no pipe");
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    int cliente_id = getpid();
    int num_trabalhos = 5; // Padrão: 5 trabalhos por cliente
    
    // Permite especificar número de trabalhos via linha de comando
    if (argc > 1) {
        num_trabalhos = atoi(argv[1]);
        if (num_trabalhos <= 0) {
            num_trabalhos = 5;
        }
    }
    
    // Inicializa gerador de números aleatórios
    srand(time(NULL) + cliente_id);
    
    printf("Cliente %d iniciado. Enviando %d trabalhos de impressão...\n", 
           cliente_id, num_trabalhos);
    
    for (int i = 0; i < num_trabalhos; i++) {
        TrabalhoImpressao trabalho = gerar_trabalho_impressao(cliente_id);
        
        printf("Cliente %d enviando trabalho ID: %d, Arquivo: %s, Páginas: %d\n",
               cliente_id, trabalho.id_job, trabalho.nome_arquivo, trabalho.numero_paginas);
        
        if (enviar_via_pipe(trabalho) == 0) {
            printf("Cliente %d: Trabalho %d enviado com sucesso\n", 
                   cliente_id, trabalho.id_job);
        } else {
            printf("Cliente %d: Erro ao enviar trabalho %d\n", 
                   cliente_id, trabalho.id_job);
        }
        
        // Espera um tempo aleatório entre envios (0.5 a 2 segundos)
        usleep((rand() % 1500000) + 500000);
    }
    
    printf("Cliente %d finalizou envio de todos os trabalhos\n", cliente_id);
    return 0;
}