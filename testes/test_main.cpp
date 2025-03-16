#include <iostream>
#include <vector>
#include <chrono>
#include "grafo_matriz.h"
#include "grafo_lista.h"

// Função para testar o algoritmo guloso
void testar_guloso(GrafoMatriz& grafo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = grafo.tsp_guloso_densidade();
    auto fim = std::chrono::high_resolution_clock::now();

    double custo = grafo.calcular_custo(caminho);
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);

    std::cout << "[GULOSO] Custo: " << custo 
              << " | Tempo: " << duracao.count() << "ms" 
              << " | Caminho válido: " << (caminho.size() == grafo.get_ordem() ? "SIM" : "NÃO") 
              << std::endl;
}

// Função para testar o algoritmo randomizado
void testar_randomizado(GrafoLista& grafo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = grafo.tsp_randomizado_frequencia(100);
    auto fim = std::chrono::high_resolution_clock::now();

    double custo = grafo.calcular_custo(caminho);
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);

    std::cout << "[RANDOMIZADO] Custo: " << custo 
              << " | Tempo: " << duracao.count() << "ms" 
              << std::endl;
}

int main() {
    // Teste com grafo pequeno (Matriz)
    GrafoMatriz gm_pequeno;
    gm_pequeno.carrega_grafo("entradas_teste/grafo_4nos.txt");
    std::cout << "=== TESTE GRAFO PEQUENO (4 NÓS) ===" << std::endl;
    testar_guloso(gm_pequeno);

    // Teste com grafo médio (Lista)
    GrafoLista gl_medio;
    gl_medio.carrega_grafo("entradas_teste/grafo_10nos.txt");
    std::cout << "\n=== TESTE GRAFO MÉDIO (10 NÓS) ===" << std::endl;
    testar_randomizado(gl_medio);

    return 0;
}