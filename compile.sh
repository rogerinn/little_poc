#!/bin/bash
cmake . -B compiled
if [ $? -eq 0 ]; then
    echo "CMake configurado com sucesso."
    make -C compiled
    chmod +x bin/program
    if [ $? -eq 0 ]; then
    rm -rf compiled/
    fi
    if [ $? -eq 0 ]; then
        echo "Compilação concluída com sucesso."
    else
        echo "Erro durante a compilação."
    fi
else
    echo "Erro ao configurar o CMake."
fi