#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Escalonador.h"
#include <vector>
#include <memory>
#include <fstream>

/**
 * @brief Classe principal para gerenciar simulações de escalonamento
 */
class Simulador {
private:
    std::vector<std::unique_ptr<Escalonador>> escalonadores;
    std::vector<Processo> processosBase;

public:
    /**
     * @brief Construtor
     */
    Simulador();

    /**
     * @brief Destrutor
     */
    ~Simulador() = default;

    /**
     * @brief Carrega processos de um arquivo
     * @param nomeArquivo Nome do arquivo com os processos
     * @return true se carregou com sucesso, false caso contrário
     */
    bool carregarProcessosArquivo(const std::string& nomeArquivo);

    /**
     * @brief Adiciona processo manualmente
     */
    void adicionarProcesso(const Processo& processo);

    /**
     * @brief Adiciona um escalonador à lista
     */
    void adicionarEscalonador(std::unique_ptr<Escalonador> escalonador);

    /**
     * @brief Executa todos os algoritmos de escalonamento
     */
    void executarTodosAlgoritmos();

    /**
     * @brief Executa um algoritmo específico
     */
    void executarAlgoritmo(const std::string& nomeAlgoritmo);

    /**
     * @brief Gera relatório comparativo
     */
    void gerarRelatorioComparativo(const std::string& nomeArquivo = "relatorio.txt");

    /**
     * @brief Exibe menu interativo
     */
    void menuInterativo();

    /**
     * @brief Limpa os processos carregados
     */
    void limparProcessos();

    /**
     * @brief Exibe os processos carregados
     */
    void exibirProcessos() const;

private:
    /**
     * @brief Distribui os processos para todos os escalonadores
     */
    void distribuirProcessos();

    /**
     * @brief Cria exemplo de processos para demonstração
     */
    void criarExemploProcessos();
};

#endif // SIMULADOR_H