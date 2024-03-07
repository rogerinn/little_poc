#!/bin/bash
./compile.sh
if [ $? -eq 0 ]; then
    echo -e "--------------------------------"
    echo -e "Running..."
    echo -e "--------------------------------\n\n"
    if [ $# -eq 0 ]; then
        echo "Nenhum parâmetro fornecido."
        exit 1
    fi
    ./bin/program $1
else
    echo "Erro ao executar o script de compilação."
fi