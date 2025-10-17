#!/bin/bash

# Script para executar todos os algoritmos de escalonamento
# Uso: ./testar_todos.sh [arquivo_processos]

ARQUIVO_PROCESSOS=${1:-"dados/processos.txt"}
EXECUTAVEL="./escalonador"

echo "=========================================="
echo "TESTE COMPLETO DOS ALGORITMOS"
echo "Arquivo: $ARQUIVO_PROCESSOS"
echo "=========================================="

# Verificar se o executável existe
if [ ! -f "$EXECUTAVEL" ]; then
    echo "Compilando o projeto..."
    make
    if [ $? -ne 0 ]; then
        echo "Erro na compilação!"
        exit 1
    fi
fi

# Verificar se o arquivo de processos existe
if [ ! -f "$ARQUIVO_PROCESSOS" ]; then
    echo "Arquivo de processos não encontrado: $ARQUIVO_PROCESSOS"
    echo "Criando arquivo de exemplo..."
    make exemplo
    ARQUIVO_PROCESSOS="dados/processos.txt"
fi

# Lista de algoritmos para testar
ALGORITMOS=("FCFS" "SJF" "SRTF" "Round Robin" "Priority")

# Executar cada algoritmo
for ALGO in "${ALGORITMOS[@]}"; do
    echo
    echo "==================== $ALGO ===================="
    echo "Pressione Enter para executar $ALGO ou Ctrl+C para sair"
    read -r
    
    $EXECUTAVEL "$ARQUIVO_PROCESSOS" "$ALGO"
    
    echo
    echo "Pressione Enter para continuar..."
    read -r
done

echo
echo "=========================================="
echo "TESTE COMPLETO FINALIZADO"
echo "=========================================="

echo
echo "Deseja gerar um relatório comparativo? (s/n)"
read -r resposta

if [[ $resposta == "s" || $resposta == "S" ]]; then
    echo "Executando todos os algoritmos para gerar relatório..."
    $EXECUTAVEL "$ARQUIVO_PROCESSOS" > relatorio_completo.txt 2>&1
    echo "Relatório salvo em: relatorio_completo.txt"
fi