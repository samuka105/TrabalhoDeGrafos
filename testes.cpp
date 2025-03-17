#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <cmath>
#include <numeric>          // Para std::accumulate
#include <algorithm>        // Para std::min e std::max
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
void testar_guloso_matriz(const std::string& arquivo) {
    GrafoMatriz gm;
    gm.carrega_grafo(arquivo);
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = gm.tsp_guloso_densidade();
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "\n=== TESTE GULOSO (MATRIZ) ===" << std::endl;
    std::cout << "Arquivo: " << arquivo << std::endl;
    std::cout << "Tempo: " << duracao.count() << "s | Custo: " << gm.calcular_custo(caminho);
    std::cout << " | Válido: " << (caminho_valido(caminho, gm.get_ordem()) ? "Sim" : "Não") << std::endl;
}

// --- Teste Guloso (Lista) ---
void testar_guloso_lista(const std::string& arquivo) {
    GrafoLista gl;
    gl.carrega_grafo(arquivo);
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = gl.tsp_guloso_densidade(); // Implemente este método na GrafoLista!
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "\n=== TESTE GULOSO (LISTA) ===" << std::endl;
    std::cout << "Arquivo: " << arquivo << std::endl;
    std::cout << "Tempo: " << duracao.count() << "s | Custo: " << gl.calcular_custo(caminho);
    std::cout << " | Válido: " << (caminho_valido(caminho, gl.get_ordem()) ? "Sim" : "Não") << std::endl;
}

// --- Teste Randomizado (Matriz) ---
void testar_randomizado_matriz(const std::string& arquivo, int iteracoes) {
    GrafoMatriz gm;
    gm.carrega_grafo(arquivo);
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = gm.tsp_randomizado_controlado(iteracoes, 3); // Implemente este método na GrafoMatriz!
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "\n=== TESTE RANDOMIZADO (MATRIZ) ===" << std::endl;
    std::cout << "Arquivo: " << arquivo << std::endl;
    std::cout << "Tempo: " << duracao.count() << "s | Custo: " << gm.calcular_custo(caminho);
    std::cout << " | Válido: " << (caminho_valido(caminho, gm.get_ordem()) ? "Sim" : "Não") << std::endl;
}

// --- Teste Randomizado (Lista) ---
void testar_randomizado_lista(const std::string& arquivo, int iteracoes) {
    GrafoLista gl;
    gl.carrega_grafo(arquivo);
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = gl.tsp_randomizado_controlado(iteracoes, 3);
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "\n=== TESTE RANDOMIZADO (LISTA) ===" << std::endl;
    std::cout << "Arquivo: " << arquivo << std::endl;
    std::cout << "Tempo: " << duracao.count() << "s | Custo: " << gl.calcular_custo(caminho);
    std::cout << " | Válido: " << (caminho_valido(caminho, gl.get_ordem()) ? "Sim" : "Não") << std::endl;
}

// --- Teste Reativo (Matriz) ---
void testar_reativo_matriz(const std::string& arquivo, int iteracoes) {
    GrafoMatriz gm;
    gm.carrega_grafo(arquivo);
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho = gm.tsp_reativo(iteracoes);
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "\n=== TESTE REATIVO (MATRIZ) ===" << std::endl;
    std::cout << "Arquivo: " << arquivo << std::endl;
    std::cout << "Tempo: " << duracao.count() << "s | Custo: " << gm.calcular_custo(caminho);
    std::cout << " | Válido: " << (caminho_valido(caminho, gm.get_ordem()) ? "Sim" : "Não") << std::endl;
}

// --- Geração de Grafos de Teste ---
void gerar_grafos_teste() {
    Util::gerar_grafo_aleatorio(4, 6, "entradas/grafo_4nos.txt");
    Util::gerar_grafo_aleatorio(10, 15, "entradas/grafo_10nos.txt");
    Util::gerar_grafo_aleatorio(5000, 20000, "entradas/grafo_5000.txt"); // Grafo grande
}



int main() {
    gerar_grafos_teste();

    // Testes em grafo pequeno
    testar_guloso_matriz("entradas/grafo_4nos.txt");
    testar_guloso_lista("entradas/grafo_4nos.txt");

    // Testes em grafo médio
    testar_randomizado_matriz("entradas/grafo_10nos.txt", 50);
    testar_randomizado_lista("entradas/grafo_10nos.txt", 50);
    testar_reativo_matriz("entradas/grafo_10nos.txt", 30);

    // Testes em grafo grande (comente se não for executar)
    testar_guloso_matriz("entradas/grafo_5000.txt");
    testar_reativo_matriz("entradas/grafo_5000.txt", 10);

    return 0;
}