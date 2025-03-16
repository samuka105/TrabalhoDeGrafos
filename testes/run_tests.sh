#!/bin/bash

# Compila o teste
g++ -o test_main.out test_main.cpp ../src/grafo_matriz.cpp ../src/grafo_lista.cpp ../src/util.cpp -I../include -std=c++17 -Wall -Werror

# Executa os testes
echo "Executando testes..."
./test_main.out

# Verifica vazamentos de memória (opcional)
echo "\nVerificando vazamentos com Valgrind..."
valgrind --leak-check=full --show-leak-kinds=all ./test_main.out