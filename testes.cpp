#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <cmath>
#include "grafo_matriz.h"
#include "grafo_lista.h"
#include "util.h"

// --- Validação do Caminho ---
bool caminho_valido(const std::vector<int>& caminho, int num_vertices) {
    if (caminho.empty() || caminho.front() != caminho.back()) return false;
    std::unordered_set<int> visitados;
    for (size_t i = 0; i < caminho.size() - 1; i++) {
        if (visitados.count(caminho[i]) > 0) return false;
        visitados.insert(caminho[i]);
    }
    return (visitados.size() == static_cast<size_t>(num_vertices));
}

// --- Teste Guloso (Matriz) ---
void testar_guloso_matriz() {
    GrafoMatriz gm;
    gm.carrega_grafo("entradas/grafo_4nos.txt");
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = gm.tsp_guloso_densidade();
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "\n=== TESTE GULOSO (MATRIZ) ===" << std::endl;
    std::cout << "Tempo: " << duracao.count() << "s | Custo: " << gm.calcular_custo(caminho) << std::endl;
}

// --- Teste Randomizado (Lista) ---
void testar_randomizado_lista() {
    GrafoLista gl;
    gl.carrega_grafo("entradas/grafo_10nos.txt");
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = gl.tsp_randomizado_controlado(50); // 50 iterações
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "\n=== TESTE RANDOMIZADO (LISTA) ===" << std::endl;
    std::cout << "Tempo: " << duracao.count() << "s | Custo: " << gl.calcular_custo(caminho) << std::endl;
}

// --- Teste Reativo (Matriz) ---
void testar_reativo_matriz() {
    GrafoMatriz gm;
    gm.carrega_grafo("entradas/grafo_10nos.txt");
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = gm.tsp_reativo(30); // 30 iterações
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "\n=== TESTE REATIVO (MATRIZ) ===" << std::endl;
    std::cout << "Tempo: " << duracao.count() << "s | Custo: " << gm.calcular_custo(caminho) << std::endl;
}

// --- Geração de Grafos de Teste ---
void gerar_grafos_teste() {
    Util::gerar_grafo_aleatorio(4, 6, "entradas/grafo_4nos.txt");    // Grafo pequeno
    Util::gerar_grafo_aleatorio(10, 15, "entradas/grafo_10nos.txt"); // Grafo médio
}

int main() {
    gerar_grafos_teste(); // Gera os arquivos na pasta 'entradas/'
    testar_guloso_matriz();
    testar_randomizado_lista();
    return 0;
}