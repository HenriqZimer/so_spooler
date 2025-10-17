#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "Processo.h"
#include <vector>
#include <queue>
#include <string>

/**
 * @brief Estrutura para armazenar estatísticas da simulação
 */
struct Estatisticas {
    double tempoMedioEspera;
    double tempoMedioTurnaround;
    double tempoMedioResposta;
    double utilizacaoCPU;
    int throughput;
    
    Estatisticas() : tempoMedioEspera(0), tempoMedioTurnaround(0), 
                    tempoMedioResposta(0), utilizacaoCPU(0), throughput(0) {}
};

/**
 * @brief Classe base abstrata para algoritmos de escalonamento
 */
class Escalonador {
protected:
    std::vector<Processo> processos;
    std::vector<Processo> processosOriginais; // Para reiniciar simulações
    int tempoAtual;
    int quantum; // Para Round Robin
    std::string nomeAlgoritmo;

public:
    /**
     * @brief Construtor
     */
    Escalonador(const std::string& nome, int quantum = 1);

    /**
     * @brief Destrutor virtual
     */
    virtual ~Escalonador() = default;

    /**
     * @brief Adiciona um processo à lista
     */
    void adicionarProcesso(const Processo& processo);

    /**
     * @brief Executa a simulação do algoritmo de escalonamento
     * @return Estatísticas da execução
     */
    virtual Estatisticas executarSimulacao() = 0;

    /**
     * @brief Reinicia a simulação
     */
    void reiniciarSimulacao();

    /**
     * @brief Calcula as estatísticas da simulação
     */
    Estatisticas calcularEstatisticas() const;

    /**
     * @brief Exibe o resultado da simulação
     */
    void exibirResultado(const Estatisticas& stats) const;

    /**
     * @brief Getters
     */
    std::string getNomeAlgoritmo() const { return nomeAlgoritmo; }
    int getQuantum() const { return quantum; }
    void setQuantum(int q) { quantum = q; }

protected:
    /**
     * @brief Verifica se há processos que chegaram no tempo atual
     */
    std::vector<Processo*> obterProcessosChegando();

    /**
     * @brief Verifica se todos os processos terminaram
     */
    bool todosProcessosTerminaram() const;

    /**
     * @brief Exibe o estado atual da simulação
     */
    void exibirEstadoAtual(const Processo* processoAtual = nullptr) const;
};

/**
 * @brief First Come First Served (FCFS)
 */
class FCFS : public Escalonador {
public:
    FCFS() : Escalonador("FCFS") {}
    Estatisticas executarSimulacao() override;
};

/**
 * @brief Shortest Job First (SJF) - Não preemptivo
 */
class SJF : public Escalonador {
public:
    SJF() : Escalonador("SJF") {}
    Estatisticas executarSimulacao() override;
};

/**
 * @brief Shortest Remaining Time First (SRTF) - Preemptivo
 */
class SRTF : public Escalonador {
public:
    SRTF() : Escalonador("SRTF") {}
    Estatisticas executarSimulacao() override;
};

/**
 * @brief Round Robin
 */
class RoundRobin : public Escalonador {
public:
    RoundRobin(int quantum = 2) : Escalonador("Round Robin", quantum) {}
    Estatisticas executarSimulacao() override;
};

/**
 * @brief Priority Scheduling - Não preemptivo
 */
class Priority : public Escalonador {
public:
    Priority() : Escalonador("Priority") {}
    Estatisticas executarSimulacao() override;
};

/**
 * @brief Priority Scheduling - Preemptivo
 */
class PriorityPreemptivo : public Escalonador {
public:
    PriorityPreemptivo() : Escalonador("Priority Preemptivo") {}
    Estatisticas executarSimulacao() override;
};

#endif // ESCALONADOR_H