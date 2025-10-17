#include "../include/Processo.h"
#include <sstream>
#include <iomanip>

Processo::Processo(int pid, const std::string& nome, int tempoChegada, int tempoCPU, int prioridade)
    : pid(pid), nome(nome), tempoChegada(tempoChegada), tempoCPU(tempoCPU), 
      tempoRestante(tempoCPU), prioridade(prioridade), tempoInicioExecucao(-1),
      tempoFinalizacao(-1), tempoEspera(0), tempoResposta(-1), jaExecutou(false) {
}

bool Processo::executar() {
    if (tempoRestante > 0) {
        tempoRestante--;
        return tempoRestante == 0; // Retorna true se terminou
    }
    return true; // Já estava terminado
}

int Processo::getTempoTurnaround() const {
    if (tempoFinalizacao == -1) return -1;
    return tempoFinalizacao - tempoChegada;
}

bool Processo::terminou() const {
    return tempoRestante == 0;
}

void Processo::reiniciar() {
    tempoRestante = tempoCPU;
    tempoInicioExecucao = -1;
    tempoFinalizacao = -1;
    tempoEspera = 0;
    tempoResposta = -1;
    jaExecutou = false;
}

std::string Processo::toString() const {
    std::ostringstream oss;
    oss << std::setw(6) << pid 
        << std::setw(12) << nome
        << std::setw(10) << tempoChegada
        << std::setw(12) << tempoCPU
        << std::setw(12) << prioridade;
    
    if (tempoFinalizacao != -1) {
        oss << std::setw(15) << tempoFinalizacao
            << std::setw(15) << getTempoTurnaround()
            << std::setw(15) << tempoEspera;
        if (tempoResposta != -1) {
            oss << std::setw(15) << tempoResposta;
        }
    }
    
    return oss.str();
}