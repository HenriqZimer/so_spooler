#include "include/Simulador.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "===============================================" << std::endl;
    std::cout << "    SIMULADOR DE ESCALONADORES DE CPU" << std::endl;
    std::cout << "      Trabalho de Sistemas Operacionais" << std::endl;
    std::cout << "===============================================" << std::endl;
    
    Simulador simulador;
    
    // Verificar se foi passado arquivo como parâmetro
    if (argc > 1) {
        std::cout << "\nCarregando processos do arquivo: " << argv[1] << std::endl;
        if (simulador.carregarProcessosArquivo(argv[1])) {
            // Se foi passado um segundo parâmetro, executar apenas esse algoritmo
            if (argc > 2) {
                std::cout << "\nExecutando algoritmo: " << argv[2] << std::endl;
                simulador.executarAlgoritmo(argv[2]);
            } else {
                // Executar todos os algoritmos
                simulador.executarTodosAlgoritmos();
            }
        } else {
            std::cout << "Erro ao carregar arquivo. Iniciando modo interativo..." << std::endl;
            simulador.menuInterativo();
        }
    } else {
        // Modo interativo
        simulador.menuInterativo();
    }
    
    return 0;
}