# INSTRUÇÕES DE USO - SIMULADOR DE ESCALONADORES

## 🚀 Projeto Completo Implementado!

Criei um simulador completo de algoritmos de escalonamento de CPU em C++ que atende aos padrões acadêmicos de trabalhos de Sistemas Operacionais.

## 📁 Estrutura Criada

```
projeto_escalonadores/
├── main.cpp                    # Arquivo principal
├── Makefile                    # Sistema de build
├── README.md                   # Documentação completa
├── testar_todos.sh            # Script de testes
├── include/                   # Headers
│   ├── Processo.h             # Classe Processo
│   ├── Escalonador.h          # Algoritmos de escalonamento
│   └── Simulador.h            # Classe principal
├── src/                       # Implementações
│   ├── Processo.cpp
│   ├── Escalonador.cpp
│   └── Simulador.cpp
└── dados/                     # Arquivos de dados
    ├── processos.txt          # Exemplo básico
    └── teste_complexo.txt     # Teste mais elaborado
```

## 🎯 Algoritmos Implementados

✅ **FCFS** (First Come First Served)
✅ **SJF** (Shortest Job First)
✅ **SRTF** (Shortest Remaining Time First)
✅ **Round Robin** (com quantum configurável)
✅ **Priority Scheduling** (preemptivo e não-preemptivo)

## 📊 Métricas Calculadas

- Tempo Médio de Espera
- Tempo Médio de Turnaround
- Tempo Médio de Resposta
- Utilização da CPU
- Throughput

## 🛠️ Como Compilar e Usar

### 1. Compilar
```bash
cd projeto_escalonadores
make
```

### 2. Executar com arquivo específico
```bash
./escalonador dados/processos.txt FCFS
./escalonador dados/processos.txt "Round Robin"
```

### 3. Modo interativo
```bash
./escalonador
```

### 4. Executar todos os algoritmos
```bash
./escalonador dados/processos.txt
```

## 📝 Formato dos Arquivos de Entrada

```
# Comentários começam com #
# Formato: PID Nome TempoChegada TempoCPU Prioridade
1 P1 0 8 2
2 P2 1 4 1
3 P3 2 2 3
```

## 🎮 Comandos Úteis do Makefile

```bash
make              # Compilar
make run          # Executar modo interativo
make exemplo      # Criar arquivo de exemplo
make test         # Executar testes básicos
make clean        # Limpar arquivos
make help         # Mostrar ajuda
```

## 🔍 Exemplo de Saída

```
=== SIMULAÇÃO FCFS ===
Tempo 0: Executando P1 (restante: 8)
Tempo 8: Executando P2 (restante: 4)
...

ESTATÍSTICAS:
Tempo Médio de Espera: 8.80
Tempo Médio de Turnaround: 13.00
Tempo Médio de Resposta: 8.80
Utilização da CPU: 100.00%
```

## 🎓 Características Acadêmicas

O projeto atende aos requisitos típicos de trabalhos de SO:

✅ **Implementação completa** de múltiplos algoritmos
✅ **Simulação passo-a-passo** com visualização
✅ **Cálculo de métricas** de performance
✅ **Interface flexível** (linha de comando + interativa)
✅ **Documentação completa** com exemplos
✅ **Código bem estruturado** com OOP
✅ **Sistema de build** automatizado
✅ **Testes** incluídos

## 🚀 Para Demonstrar ao Professor

1. **Compilar**: `make`
2. **Testar FCFS**: `./escalonador dados/processos.txt FCFS`
3. **Testar Round Robin**: `./escalonador dados/processos.txt "Round Robin"`
4. **Comparar todos**: `./escalonador dados/processos.txt`

## 💡 Destaques do Projeto

- **Código profissional** com padrões C++
- **Flexibilidade** para diferentes cenários de teste
- **Visualização clara** do processo de escalonamento
- **Métricas precisas** para análise de performance
- **Documentação completa** para entendimento
- **Extensibilidade** para novos algoritmos

## 📚 Base Teórica

O projeto demonstra compreensão de:
- Conceitos de escalonamento de processos
- Algoritmos clássicos de SO
- Métricas de avaliação de sistemas
- Implementação de simuladores
- Programação orientada a objetos

Este trabalho está pronto para apresentação e atende aos padrões de qualidade esperados em disciplinas de Sistemas Operacionais! 🎓✨