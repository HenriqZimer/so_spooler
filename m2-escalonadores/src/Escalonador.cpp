#include "../include/Escalonador.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <queue>

Escalonador::Escalonador(const std::string& nome, int quantum)
    : tempoAtual(0), quantum(quantum), nomeAlgoritmo(nome) {
}

void Escalonador::adicionarProcesso(const Processo& processo) {
    processos.push_back(processo);
    processosOriginais.push_back(processo);
}

void Escalonador::reiniciarSimulacao() {
    tempoAtual = 0;
    processos = processosOriginais;
    for (auto& p : processos) {
        p.reiniciar();
    }
}

std::vector<Processo*> Escalonador::obterProcessosChegando() {
    std::vector<Processo*> chegando;
    for (auto& p : processos) {
        if (p.getTempoChegada() == tempoAtual && !p.terminou()) {
            chegando.push_back(&p);
        }
    }
    return chegando;
}

bool Escalonador::todosProcessosTerminaram() const {
    for (const auto& p : processos) {
        if (!p.terminou()) return false;
    }
    return true;
}

Estatisticas Escalonador::calcularEstatisticas() const {
    Estatisticas stats;
    int totalProcessos = 0;
    int somaEspera = 0, somaTurnaround = 0, somaResposta = 0;
    int tempoTotalExecucao = 0;
    
    for (const auto& p : processos) {
        if (p.getTempoFinalizacao() != -1) {
            totalProcessos++;
            somaEspera += p.getTempoEspera();
            somaTurnaround += p.getTempoTurnaround();
            if (p.getTempoResposta() != -1) {
                somaResposta += p.getTempoResposta();
            }
            tempoTotalExecucao = std::max(tempoTotalExecucao, p.getTempoFinalizacao());
        }
    }
    
    if (totalProcessos > 0) {
        stats.tempoMedioEspera = static_cast<double>(somaEspera) / totalProcessos;
        stats.tempoMedioTurnaround = static_cast<double>(somaTurnaround) / totalProcessos;
        stats.tempoMedioResposta = static_cast<double>(somaResposta) / totalProcessos;
        stats.throughput = totalProcessos;
        
        // Calcular utilização da CPU
        int tempoCPUTotal = 0;
        for (const auto& p : processos) {
            tempoCPUTotal += p.getTempoCPU();
        }
        stats.utilizacaoCPU = (static_cast<double>(tempoCPUTotal) / tempoTotalExecucao) * 100;
    }
    
    return stats;
}

void Escalonador::exibirResultado(const Estatisticas& stats) const {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "RESULTADO DO ALGORITMO: " << nomeAlgoritmo << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // Cabeçalho da tabela
    std::cout << std::setw(6) << "PID"
              << std::setw(12) << "Nome"
              << std::setw(10) << "Chegada"
              << std::setw(12) << "CPU"
              << std::setw(12) << "Prioridade"
              << std::setw(15) << "Finalização"
              << std::setw(15) << "Turnaround"
              << std::setw(15) << "Espera"
              << std::setw(15) << "Resposta" << std::endl;
    std::cout << std::string(112, '-') << std::endl;
    
    // Dados dos processos
    for (const auto& p : processos) {
        std::cout << p.toString() << std::endl;
    }
    
    std::cout << std::string(112, '-') << std::endl;
    
    // Estatísticas
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nESTATÍSTICAS:" << std::endl;
    std::cout << "Tempo Médio de Espera: " << stats.tempoMedioEspera << std::endl;
    std::cout << "Tempo Médio de Turnaround: " << stats.tempoMedioTurnaround << std::endl;
    std::cout << "Tempo Médio de Resposta: " << stats.tempoMedioResposta << std::endl;
    std::cout << "Utilização da CPU: " << stats.utilizacaoCPU << "%" << std::endl;
    std::cout << "Throughput: " << stats.throughput << " processos" << std::endl;
}

void Escalonador::exibirEstadoAtual(const Processo* processoAtual) const {
    std::cout << "Tempo " << tempoAtual << ": ";
    if (processoAtual) {
        std::cout << "Executando P" << processoAtual->getPid() 
                  << " (restante: " << processoAtual->getTempoRestante() << ")";
    } else {
        std::cout << "CPU ociosa";
    }
    std::cout << std::endl;
}

// ================================
// IMPLEMENTAÇÃO DOS ALGORITMOS
// ================================

Estatisticas FCFS::executarSimulacao() {
    reiniciarSimulacao();
    std::queue<Processo*> filaReady;
    
    std::cout << "\n=== SIMULAÇÃO FCFS ===" << std::endl;
    
    while (!todosProcessosTerminaram()) {
        // Adicionar processos que chegaram
        auto chegando = obterProcessosChegando();
        for (auto* p : chegando) {
            filaReady.push(p);
        }
        
        if (!filaReady.empty()) {
            Processo* atual = filaReady.front();
            
            // Marcar início da execução se necessário
            if (!atual->getJaExecutou()) {
                atual->setTempoInicioExecucao(tempoAtual);
                atual->setTempoResposta(tempoAtual - atual->getTempoChegada());
                atual->setJaExecutou(true);
            }
            
            exibirEstadoAtual(atual);
            
            // Executar processo até terminar
            while (!atual->terminou()) {
                atual->executar();
                tempoAtual++;
                
                // Adicionar processos que chegaram durante a execução
                auto chegandoDurante = obterProcessosChegando();
                for (auto* p : chegandoDurante) {
                    filaReady.push(p);
                }
            }
            
            // Processo terminou
            atual->setTempoFinalizacao(tempoAtual);
            atual->setTempoEspera(atual->getTempoTurnaround() - atual->getTempoCPU());
            filaReady.pop();
            
        } else {
            exibirEstadoAtual(nullptr);
            tempoAtual++;
        }
    }
    
    return calcularEstatisticas();
}

Estatisticas SJF::executarSimulacao() {
    reiniciarSimulacao();
    
    std::cout << "\n=== SIMULAÇÃO SJF ===" << std::endl;
    
    while (!todosProcessosTerminaram()) {
        // Encontrar processos disponíveis
        std::vector<Processo*> disponiveis;
        for (auto& p : processos) {
            if (p.getTempoChegada() <= tempoAtual && !p.terminou()) {
                disponiveis.push_back(&p);
            }
        }
        
        if (!disponiveis.empty()) {
            // Selecionar o processo com menor tempo de CPU
            auto menorTempo = std::min_element(disponiveis.begin(), disponiveis.end(),
                [](const Processo* a, const Processo* b) {
                    return a->getTempoCPU() < b->getTempoCPU();
                });
            
            Processo* atual = *menorTempo;
            
            // Marcar início da execução
            if (!atual->getJaExecutou()) {
                atual->setTempoInicioExecucao(tempoAtual);
                atual->setTempoResposta(tempoAtual - atual->getTempoChegada());
                atual->setJaExecutou(true);
            }
            
            exibirEstadoAtual(atual);
            
            // Executar processo até terminar
            while (!atual->terminou()) {
                atual->executar();
                tempoAtual++;
            }
            
            // Processo terminou
            atual->setTempoFinalizacao(tempoAtual);
            atual->setTempoEspera(atual->getTempoTurnaround() - atual->getTempoCPU());
            
        } else {
            exibirEstadoAtual(nullptr);
            tempoAtual++;
        }
    }
    
    return calcularEstatisticas();
}

Estatisticas SRTF::executarSimulacao() {
    reiniciarSimulacao();
    
    std::cout << "\n=== SIMULAÇÃO SRTF ===" << std::endl;
    
    while (!todosProcessosTerminaram()) {
        // Encontrar processos disponíveis
        std::vector<Processo*> disponiveis;
        for (auto& p : processos) {
            if (p.getTempoChegada() <= tempoAtual && !p.terminou()) {
                disponiveis.push_back(&p);
            }
        }
        
        if (!disponiveis.empty()) {
            // Selecionar o processo com menor tempo restante
            auto menorRestante = std::min_element(disponiveis.begin(), disponiveis.end(),
                [](const Processo* a, const Processo* b) {
                    return a->getTempoRestante() < b->getTempoRestante();
                });
            
            Processo* atual = *menorRestante;
            
            // Marcar início da execução se necessário
            if (!atual->getJaExecutou()) {
                atual->setTempoInicioExecucao(tempoAtual);
                atual->setTempoResposta(tempoAtual - atual->getTempoChegada());
                atual->setJaExecutou(true);
            }
            
            exibirEstadoAtual(atual);
            
            // Executar por uma unidade de tempo
            bool terminou = atual->executar();
            tempoAtual++;
            
            if (terminou) {
                atual->setTempoFinalizacao(tempoAtual);
                atual->setTempoEspera(atual->getTempoTurnaround() - atual->getTempoCPU());
            }
            
        } else {
            exibirEstadoAtual(nullptr);
            tempoAtual++;
        }
    }
    
    return calcularEstatisticas();
}

Estatisticas RoundRobin::executarSimulacao() {
    reiniciarSimulacao();
    std::queue<Processo*> filaReady;
    int quantumAtual = 0;
    
    std::cout << "\n=== SIMULAÇÃO ROUND ROBIN (Quantum = " << quantum << ") ===" << std::endl;
    
    while (!todosProcessosTerminaram()) {
        // Adicionar processos que chegaram
        auto chegando = obterProcessosChegando();
        for (auto* p : chegando) {
            filaReady.push(p);
        }
        
        if (!filaReady.empty()) {
            Processo* atual = filaReady.front();
            filaReady.pop();
            
            // Marcar início da execução se necessário
            if (!atual->getJaExecutou()) {
                atual->setTempoInicioExecucao(tempoAtual);
                atual->setTempoResposta(tempoAtual - atual->getTempoChegada());
                atual->setJaExecutou(true);
            }
            
            exibirEstadoAtual(atual);
            
            // Executar por quantum ou até terminar
            quantumAtual = 0;
            while (quantumAtual < quantum && !atual->terminou()) {
                atual->executar();
                tempoAtual++;
                quantumAtual++;
                
                // Adicionar processos que chegaram durante a execução
                auto chegandoDurante = obterProcessosChegando();
                for (auto* p : chegandoDurante) {
                    filaReady.push(p);
                }
            }
            
            if (atual->terminou()) {
                atual->setTempoFinalizacao(tempoAtual);
                atual->setTempoEspera(atual->getTempoTurnaround() - atual->getTempoCPU());
            } else {
                // Processo não terminou, volta para o final da fila
                filaReady.push(atual);
            }
            
        } else {
            exibirEstadoAtual(nullptr);
            tempoAtual++;
        }
    }
    
    return calcularEstatisticas();
}

Estatisticas Priority::executarSimulacao() {
    reiniciarSimulacao();
    
    std::cout << "\n=== SIMULAÇÃO PRIORITY ===" << std::endl;
    
    while (!todosProcessosTerminaram()) {
        // Encontrar processos disponíveis
        std::vector<Processo*> disponiveis;
        for (auto& p : processos) {
            if (p.getTempoChegada() <= tempoAtual && !p.terminou()) {
                disponiveis.push_back(&p);
            }
        }
        
        if (!disponiveis.empty()) {
            // Selecionar o processo com maior prioridade (menor número)
            auto maiorPrioridade = std::min_element(disponiveis.begin(), disponiveis.end(),
                [](const Processo* a, const Processo* b) {
                    if (a->getPrioridade() == b->getPrioridade()) {
                        return a->getTempoChegada() < b->getTempoChegada(); // FCFS como desempate
                    }
                    return a->getPrioridade() < b->getPrioridade();
                });
            
            Processo* atual = *maiorPrioridade;
            
            // Marcar início da execução
            if (!atual->getJaExecutou()) {
                atual->setTempoInicioExecucao(tempoAtual);
                atual->setTempoResposta(tempoAtual - atual->getTempoChegada());
                atual->setJaExecutou(true);
            }
            
            exibirEstadoAtual(atual);
            
            // Executar processo até terminar
            while (!atual->terminou()) {
                atual->executar();
                tempoAtual++;
            }
            
            // Processo terminou
            atual->setTempoFinalizacao(tempoAtual);
            atual->setTempoEspera(atual->getTempoTurnaround() - atual->getTempoCPU());
            
        } else {
            exibirEstadoAtual(nullptr);
            tempoAtual++;
        }
    }
    
    return calcularEstatisticas();
}

Estatisticas PriorityPreemptivo::executarSimulacao() {
    reiniciarSimulacao();
    
    std::cout << "\n=== SIMULAÇÃO PRIORITY PREEMPTIVO ===" << std::endl;
    
    while (!todosProcessosTerminaram()) {
        // Encontrar processos disponíveis
        std::vector<Processo*> disponiveis;
        for (auto& p : processos) {
            if (p.getTempoChegada() <= tempoAtual && !p.terminou()) {
                disponiveis.push_back(&p);
            }
        }
        
        if (!disponiveis.empty()) {
            // Selecionar o processo com maior prioridade (menor número)
            auto maiorPrioridade = std::min_element(disponiveis.begin(), disponiveis.end(),
                [](const Processo* a, const Processo* b) {
                    if (a->getPrioridade() == b->getPrioridade()) {
                        return a->getTempoChegada() < b->getTempoChegada();
                    }
                    return a->getPrioridade() < b->getPrioridade();
                });
            
            Processo* atual = *maiorPrioridade;
            
            // Marcar início da execução se necessário
            if (!atual->getJaExecutou()) {
                atual->setTempoInicioExecucao(tempoAtual);
                atual->setTempoResposta(tempoAtual - atual->getTempoChegada());
                atual->setJaExecutou(true);
            }
            
            exibirEstadoAtual(atual);
            
            // Executar por uma unidade de tempo
            bool terminou = atual->executar();
            tempoAtual++;
            
            if (terminou) {
                atual->setTempoFinalizacao(tempoAtual);
                atual->setTempoEspera(atual->getTempoTurnaround() - atual->getTempoCPU());
            }
            
        } else {
            exibirEstadoAtual(nullptr);
            tempoAtual++;
        }
    }
    
    return calcularEstatisticas();
}