# Simulador de Algoritmos de Escalonamento de CPU

Um simulador completo de algoritmos de escalonamento de processos implementado em C++ para estudo de Sistemas Operacionais.

## 📋 Algoritmos Implementados

- **FCFS (First Come First Served)** - Primeiro a chegar, primeiro a ser servido
- **SJF (Shortest Job First)** - Trabalho mais curto primeiro (não preemptivo)
- **SRTF (Shortest Remaining Time First)** - Menor tempo restante primeiro (preemptivo)
- **Round Robin** - Escalonamento circular com quantum configurável
- **Priority Scheduling** - Escalonamento por prioridade (não preemptivo e preemptivo)

## 🚀 Como Compilar

```bash
# Compilar o projeto
make

# Ou manualmente
g++ -std=c++11 -Wall -Wextra -O2 main.cpp src/*.cpp -Iinclude -o escalonador
```

## 📖 Como Usar

### Modo Interativo
```bash
./escalonador
```

### Com Arquivo de Processos
```bash
./escalonador dados/processos.txt
```

### Executar Algoritmo Específico
```bash
./escalonador dados/processos.txt FCFS
./escalonador dados/processos.txt "Round Robin"
```

## 📁 Formato do Arquivo de Processos

```
# Comentários começam com #
# Formato: PID Nome TempoChegada TempoCPU Prioridade
1 P1 0 8 2
2 P2 1 4 1
3 P3 2 2 3
4 P4 3 6 2
5 P5 4 1 1
```

- **PID**: Identificador do processo
- **Nome**: Nome do processo
- **TempoChegada**: Momento em que o processo chega
- **TempoCPU**: Tempo de CPU necessário (burst time)
- **Prioridade**: Prioridade do processo (menor número = maior prioridade)

## 🎯 Funcionalidades

### ✅ Simulação Completa
- Execução passo a passo dos algoritmos
- Visualização do estado da CPU em cada momento
- Cálculo automático de métricas

### 📊 Métricas Calculadas
- **Tempo Médio de Espera**: Tempo que os processos ficam na fila
- **Tempo Médio de Turnaround**: Tempo total desde chegada até finalização
- **Tempo Médio de Resposta**: Tempo até primeira execução
- **Utilização da CPU**: Percentual de uso da CPU
- **Throughput**: Número de processos executados

### 🔧 Interface Flexível
- Menu interativo
- Carregamento de arquivos
- Adição manual de processos
- Geração de relatórios comparativos

## 🛠️ Comandos do Makefile

```bash
make              # Compilar o projeto
make run          # Executar modo interativo
make exemplo      # Criar arquivo de exemplo
make run-file     # Executar com arquivo de dados
make test         # Executar testes básicos
make clean        # Limpar arquivos compilados
make install      # Instalar no sistema
make help         # Mostrar ajuda
```

## 📚 Estrutura do Projeto

```
projeto_escalonadores/
├── main.cpp              # Arquivo principal
├── Makefile             # Build system
├── README.md            # Documentação
├── include/             # Headers
│   ├── Processo.h       # Classe Processo
│   ├── Escalonador.h    # Classes dos algoritmos
│   └── Simulador.h      # Classe principal
├── src/                 # Implementações
│   ├── Processo.cpp
│   ├── Escalonador.cpp
│   └── Simulador.cpp
└── dados/               # Arquivos de dados
    └── processos.txt    # Exemplo de processos
```

## 🎮 Exemplo de Uso

### 1. Criar arquivo de processos
```bash
make exemplo
```

### 2. Executar todos os algoritmos
```bash
make run-file
```

### 3. Comparar resultados
O simulador mostra uma tabela comparativa com todas as métricas:

```
=== COMPARAÇÃO DOS ALGORITMOS ===
Algoritmo        Esp. Média    Turn. Médio   Resp. Média      CPU %
FCFS                   5.40          9.40          5.40      95.45
SJF                    3.40          7.40          3.40      95.45
Round Robin            5.80          9.80          2.00      95.45
Priority               4.00          8.00          4.00      95.45
```

## 🔍 Características dos Algoritmos

### FCFS (First Come First Served)
- **Vantagens**: Simples, justo na ordem de chegada
- **Desvantagens**: Efeito comboio (convoy effect)
- **Uso**: Sistemas em lote, impressoras

### SJF (Shortest Job First)
- **Vantagens**: Minimiza tempo médio de espera
- **Desvantagens**: Starvation de processos longos
- **Uso**: Sistemas em lote com tempo conhecido

### SRTF (Shortest Remaining Time First)
- **Vantagens**: Versão preemptiva do SJF, mais eficiente
- **Desvantagens**: Overhead de preempção, starvation
- **Uso**: Sistemas interativos

### Round Robin
- **Vantagens**: Justo, bom tempo de resposta
- **Desvantagens**: Overhead de troca de contexto
- **Uso**: Sistemas de tempo compartilhado

### Priority Scheduling
- **Vantagens**: Permite priorização por importância
- **Desvantagens**: Starvation de baixa prioridade
- **Uso**: Sistemas de tempo real

## 🧪 Testes e Validação

O simulador inclui:
- Processos de exemplo para teste
- Validação automática de resultados
- Comparação entre algoritmos
- Geração de relatórios detalhados

## 📋 Requisitos

- **Compilador**: g++ com suporte a C++11
- **Sistema**: Linux/Unix (testado no Ubuntu)
- **Ferramentas opcionais**: 
  - `valgrind` para verificação de memória
  - `make` para build automatizado

## 🎓 Contexto Acadêmico

Este projeto foi desenvolvido como trabalho prático para a disciplina de Sistemas Operacionais, demonstrando:

- Implementação de algoritmos de escalonamento
- Simulação de sistemas operacionais
- Análise de performance e métricas
- Programação orientada a objetos em C++
- Organização de projetos de software

## 🤝 Como Contribuir

1. Faça um fork do projeto
2. Crie uma branch para sua feature
3. Implemente melhorias ou correções
4. Teste thoroughly
5. Submeta um pull request

## 📝 Licença

Este projeto é para fins educacionais. Sinta-se livre para usar e modificar para estudos.

---

**Desenvolvido para estudo de Sistemas Operacionais** 🎓