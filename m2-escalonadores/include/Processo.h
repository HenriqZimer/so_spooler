#ifndef PROCESSO_H
#define PROCESSO_H

#include <string>

/**
 * @brief Classe que representa um processo no sistema
 */
class Processo {
private:
    int pid;                    // ID do processo
    std::string nome;           // Nome do processo
    int tempoChegada;          // Tempo de chegada
    int tempoCPU;              // Tempo de CPU necessário (burst time)
    int tempoRestante;         // Tempo restante de CPU
    int prioridade;            // Prioridade do processo (menor valor = maior prioridade)
    int tempoInicioExecucao;   // Quando começou a executar
    int tempoFinalizacao;      // Quando terminou a execução
    int tempoEspera;           // Tempo total em espera
    int tempoResposta;         // Tempo de resposta (primeira execução)
    bool jaExecutou;           // Flag para controle do tempo de resposta

public:
    /**
     * @brief Construtor da classe Processo
     */
    Processo(int pid, const std::string& nome, int tempoChegada, int tempoCPU, int prioridade = 0);

    // Getters
    int getPid() const { return pid; }
    std::string getNome() const { return nome; }
    int getTempoChegada() const { return tempoChegada; }
    int getTempoCPU() const { return tempoCPU; }
    int getTempoRestante() const { return tempoRestante; }
    int getPrioridade() const { return prioridade; }
    int getTempoInicioExecucao() const { return tempoInicioExecucao; }
    int getTempoFinalizacao() const { return tempoFinalizacao; }
    int getTempoEspera() const { return tempoEspera; }
    int getTempoResposta() const { return tempoResposta; }
    bool getJaExecutou() const { return jaExecutou; }

    // Setters
    void setTempoRestante(int tempo) { tempoRestante = tempo; }
    void setPrioridade(int prio) { prioridade = prio; }
    void setTempoInicioExecucao(int tempo) { tempoInicioExecucao = tempo; }
    void setTempoFinalizacao(int tempo) { tempoFinalizacao = tempo; }
    void setTempoEspera(int tempo) { tempoEspera = tempo; }
    void setTempoResposta(int tempo) { tempoResposta = tempo; }
    void setJaExecutou(bool executou) { jaExecutou = executou; }

    /**
     * @brief Executa o processo por uma unidade de tempo
     * @return true se o processo terminou, false caso contrário
     */
    bool executar();

    /**
     * @brief Calcula o tempo de turnaround
     * @return Tempo de turnaround (finalização - chegada)
     */
    int getTempoTurnaround() const;

    /**
     * @brief Verifica se o processo terminou
     * @return true se terminou, false caso contrário
     */
    bool terminou() const;

    /**
     * @brief Reinicia o processo para nova simulação
     */
    void reiniciar();

    /**
     * @brief Converte o processo para string para exibição
     */
    std::string toString() const;
};

#endif // PROCESSO_H