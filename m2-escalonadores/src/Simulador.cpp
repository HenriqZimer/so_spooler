#include "../include/Simulador.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

Simulador::Simulador() {
    // Inicializar com os algoritmos principais
    escalonadores.push_back(std::make_unique<FCFS>());
    escalonadores.push_back(std::make_unique<SJF>());
    escalonadores.push_back(std::make_unique<SRTF>());
    escalonadores.push_back(std::make_unique<RoundRobin>(2));
    escalonadores.push_back(std::make_unique<RoundRobin>(4));
    escalonadores.push_back(std::make_unique<Priority>());
    escalonadores.push_back(std::make_unique<PriorityPreemptivo>());
}

bool Simulador::carregarProcessosArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << nomeArquivo << std::endl;
        return false;
    }
    
    processosBase.clear();
    std::string linha;
    int numeroLinha = 0;
    
    // Pular cabeçalho se existir
    if (std::getline(arquivo, linha)) {
        numeroLinha++;
        // Se a primeira linha contém letras, é provável que seja cabeçalho
        if (linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") != std::string::npos) {
            // É cabeçalho, continue lendo
        } else {
            // Não é cabeçalho, processar esta linha
            arquivo.seekg(0); // Voltar ao início
            numeroLinha = 0;
        }
    }
    
    while (std::getline(arquivo, linha)) {
        numeroLinha++;
        if (linha.empty() || linha[0] == '#') continue; // Pular linhas vazias e comentários
        
        std::istringstream iss(linha);
        int pid, tempoChegada, tempoCPU, prioridade = 0;
        std::string nome;
        
        // Formato esperado: PID Nome TempoChegada TempoCPU [Prioridade]
        if (iss >> pid >> nome >> tempoChegada >> tempoCPU) {
            iss >> prioridade; // Prioridade é opcional
            
            processosBase.emplace_back(pid, nome, tempoChegada, tempoCPU, prioridade);
            std::cout << "Processo carregado: P" << pid << " (" << nome << ")" << std::endl;
        } else {
            std::cerr << "Erro na linha " << numeroLinha << ": " << linha << std::endl;
        }
    }
    
    arquivo.close();
    
    if (processosBase.empty()) {
        std::cout << "Nenhum processo foi carregado. Criando exemplo..." << std::endl;
        criarExemploProcessos();
    } else {
        std::cout << "Carregados " << processosBase.size() << " processos." << std::endl;
        distribuirProcessos();
    }
    
    return true;
}

void Simulador::adicionarProcesso(const Processo& processo) {
    processosBase.push_back(processo);
    distribuirProcessos();
}

void Simulador::adicionarEscalonador(std::unique_ptr<Escalonador> escalonador) {
    escalonadores.push_back(std::move(escalonador));
    if (!processosBase.empty()) {
        distribuirProcessos();
    }
}

void Simulador::distribuirProcessos() {
    for (auto& escalonador : escalonadores) {
        escalonador->reiniciarSimulacao();
        for (const auto& processo : processosBase) {
            escalonador->adicionarProcesso(processo);
        }
    }
}

void Simulador::executarTodosAlgoritmos() {
    if (processosBase.empty()) {
        std::cout << "Nenhum processo carregado! Criando exemplo..." << std::endl;
        criarExemploProcessos();
        distribuirProcessos();
    }
    
    std::cout << "\n" << std::string(80, '#') << std::endl;
    std::cout << "EXECUTANDO TODOS OS ALGORITMOS DE ESCALONAMENTO" << std::endl;
    std::cout << std::string(80, '#') << std::endl;
    
    std::vector<std::pair<std::string, Estatisticas>> resultados;
    
    for (auto& escalonador : escalonadores) {
        auto stats = escalonador->executarSimulacao();
        escalonador->exibirResultado(stats);
        resultados.push_back({escalonador->getNomeAlgoritmo(), stats});
        
        std::cout << "\nPressione Enter para continuar...";
        std::cin.ignore();
        std::cin.get();
    }
    
    // Exibir comparação
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "COMPARAÇÃO DOS ALGORITMOS" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    std::cout << std::setw(20) << "Algoritmo"
              << std::setw(15) << "Esp. Média"
              << std::setw(15) << "Turn. Médio"
              << std::setw(15) << "Resp. Média"
              << std::setw(15) << "CPU %" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& resultado : resultados) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::setw(20) << resultado.first
                  << std::setw(15) << resultado.second.tempoMedioEspera
                  << std::setw(15) << resultado.second.tempoMedioTurnaround
                  << std::setw(15) << resultado.second.tempoMedioResposta
                  << std::setw(15) << resultado.second.utilizacaoCPU << std::endl;
    }
}

void Simulador::executarAlgoritmo(const std::string& nomeAlgoritmo) {
    auto it = std::find_if(escalonadores.begin(), escalonadores.end(),
        [&nomeAlgoritmo](const std::unique_ptr<Escalonador>& esc) {
            return esc->getNomeAlgoritmo().find(nomeAlgoritmo) != std::string::npos;
        });
    
    if (it != escalonadores.end()) {
        auto stats = (*it)->executarSimulacao();
        (*it)->exibirResultado(stats);
    } else {
        std::cout << "Algoritmo não encontrado: " << nomeAlgoritmo << std::endl;
        std::cout << "Algoritmos disponíveis:" << std::endl;
        for (const auto& esc : escalonadores) {
            std::cout << "- " << esc->getNomeAlgoritmo() << std::endl;
        }
    }
}

void Simulador::gerarRelatorioComparativo(const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao criar arquivo de relatório." << std::endl;
        return;
    }
    
    arquivo << "RELATÓRIO COMPARATIVO DE ALGORITMOS DE ESCALONAMENTO\n";
    arquivo << std::string(60, '=') << "\n\n";
    
    arquivo << "PROCESSOS UTILIZADOS:\n";
    arquivo << std::string(30, '-') << "\n";
    for (const auto& p : processosBase) {
        arquivo << "P" << p.getPid() << " - " << p.getNome() 
                << " (Chegada: " << p.getTempoChegada() 
                << ", CPU: " << p.getTempoCPU() 
                << ", Prioridade: " << p.getPrioridade() << ")\n";
    }
    
    arquivo << "\nRESULTADOS:\n";
    arquivo << std::string(30, '-') << "\n";
    
    for (auto& escalonador : escalonadores) {
        auto stats = escalonador->executarSimulacao();
        
        arquivo << "\n" << escalonador->getNomeAlgoritmo() << ":\n";
        arquivo << "  Tempo Médio de Espera: " << stats.tempoMedioEspera << "\n";
        arquivo << "  Tempo Médio de Turnaround: " << stats.tempoMedioTurnaround << "\n";
        arquivo << "  Tempo Médio de Resposta: " << stats.tempoMedioResposta << "\n";
        arquivo << "  Utilização da CPU: " << stats.utilizacaoCPU << "%\n";
        arquivo << "  Throughput: " << stats.throughput << " processos\n";
    }
    
    arquivo.close();
    std::cout << "Relatório salvo em: " << nomeArquivo << std::endl;
}

void Simulador::menuInterativo() {
    int opcao;
    
    do {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "SIMULADOR DE ALGORITMOS DE ESCALONAMENTO" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Carregar processos de arquivo" << std::endl;
        std::cout << "2. Adicionar processo manualmente" << std::endl;
        std::cout << "3. Exibir processos carregados" << std::endl;
        std::cout << "4. Executar todos os algoritmos" << std::endl;
        std::cout << "5. Executar algoritmo específico" << std::endl;
        std::cout << "6. Gerar relatório comparativo" << std::endl;
        std::cout << "7. Criar exemplo de processos" << std::endl;
        std::cout << "8. Limpar processos" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Escolha uma opção: ";
        
        std::cin >> opcao;
        
        switch (opcao) {
            case 1: {
                std::string nomeArquivo;
                std::cout << "Digite o nome do arquivo: ";
                std::cin >> nomeArquivo;
                carregarProcessosArquivo(nomeArquivo);
                break;
            }
            case 2: {
                int pid, chegada, cpu, prioridade;
                std::string nome;
                std::cout << "PID: "; std::cin >> pid;
                std::cout << "Nome: "; std::cin >> nome;
                std::cout << "Tempo de chegada: "; std::cin >> chegada;
                std::cout << "Tempo de CPU: "; std::cin >> cpu;
                std::cout << "Prioridade: "; std::cin >> prioridade;
                
                adicionarProcesso(Processo(pid, nome, chegada, cpu, prioridade));
                std::cout << "Processo adicionado!" << std::endl;
                break;
            }
            case 3:
                exibirProcessos();
                break;
            case 4:
                executarTodosAlgoritmos();
                break;
            case 5: {
                std::cout << "Algoritmos disponíveis:" << std::endl;
                for (size_t i = 0; i < escalonadores.size(); ++i) {
                    std::cout << i + 1 << ". " << escalonadores[i]->getNomeAlgoritmo() << std::endl;
                }
                
                int escolha;
                std::cout << "Escolha o algoritmo (número): ";
                std::cin >> escolha;
                
                if (escolha >= 1 && escolha <= static_cast<int>(escalonadores.size())) {
                    auto stats = escalonadores[escolha - 1]->executarSimulacao();
                    escalonadores[escolha - 1]->exibirResultado(stats);
                } else {
                    std::cout << "Opção inválida!" << std::endl;
                }
                break;
            }
            case 6: {
                std::string nomeArquivo;
                std::cout << "Digite o nome do arquivo (Enter para 'relatorio.txt'): ";
                std::cin.ignore();
                std::getline(std::cin, nomeArquivo);
                if (nomeArquivo.empty()) nomeArquivo = "relatorio.txt";
                gerarRelatorioComparativo(nomeArquivo);
                break;
            }
            case 7:
                criarExemploProcessos();
                distribuirProcessos();
                std::cout << "Exemplo de processos criado!" << std::endl;
                break;
            case 8:
                limparProcessos();
                std::cout << "Processos removidos!" << std::endl;
                break;
            case 0:
                std::cout << "Saindo..." << std::endl;
                break;
            default:
                std::cout << "Opção inválida!" << std::endl;
        }
        
    } while (opcao != 0);
}

void Simulador::limparProcessos() {
    processosBase.clear();
    for (auto& escalonador : escalonadores) {
        escalonador->reiniciarSimulacao();
    }
}

void Simulador::exibirProcessos() const {
    if (processosBase.empty()) {
        std::cout << "Nenhum processo carregado." << std::endl;
        return;
    }
    
    std::cout << "\nPROCESSOS CARREGADOS:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(6) << "PID"
              << std::setw(12) << "Nome"
              << std::setw(12) << "Chegada"
              << std::setw(12) << "CPU"
              << std::setw(12) << "Prioridade" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (const auto& p : processosBase) {
        std::cout << std::setw(6) << p.getPid()
                  << std::setw(12) << p.getNome()
                  << std::setw(12) << p.getTempoChegada()
                  << std::setw(12) << p.getTempoCPU()
                  << std::setw(12) << p.getPrioridade() << std::endl;
    }
    std::cout << std::string(60, '-') << std::endl;
}

void Simulador::criarExemploProcessos() {
    processosBase.clear();
    
    // Processos com diferentes características para teste
    processosBase.emplace_back(1, "P1", 0, 8, 2);   // Longo, prioridade média
    processosBase.emplace_back(2, "P2", 1, 4, 1);   // Médio, alta prioridade
    processosBase.emplace_back(3, "P3", 2, 2, 3);   // Curto, baixa prioridade
    processosBase.emplace_back(4, "P4", 3, 6, 2);   // Longo, prioridade média
    processosBase.emplace_back(5, "P5", 4, 1, 1);   // Muito curto, alta prioridade
    
    std::cout << "Exemplo de processos criado com 5 processos." << std::endl;
}