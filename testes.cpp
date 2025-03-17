#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <cmath>
#include <numeric>          
#include <algorithm>        
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

// --- Função para testar TODOS os algoritmos em uma instância ---
void testar_todos_algoritmos(Grafo& grafo, const std::string& nome_instancia) {
    std::cout << "\n=== INSTÂNCIA: " << nome_instancia << " ===" << std::endl;

    // Testar Guloso
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho_guloso = grafo.tsp_guloso_densidade();
    auto fim = std::chrono::high_resolution_clock::now();
    std::cout << "[Guloso] Tempo: " << std::chrono::duration<double>(fim - inicio).count() 
              << "s | Custo: " << grafo.calcular_custo(caminho_guloso) 
              << " | Válido: " << (caminho_valido(caminho_guloso, grafo.get_ordem()) ? "Sim" : "Não") << std::endl;

    // Testar Randomizado
    inicio = std::chrono::high_resolution_clock::now();
    auto caminho_randomizado = grafo.tsp_randomizado_controlado(50, 3); // 50 iterações, N=3
    fim = std::chrono::high_resolution_clock::now();
    std::cout << "[Randomizado] Tempo: " << std::chrono::duration<double>(fim - inicio).count() 
              << "s | Custo: " << grafo.calcular_custo(caminho_randomizado) 
              << " | Válido: " << (caminho_valido(caminho_randomizado, grafo.get_ordem()) ? "Sim" : "Não") << std::endl;

    // Testar Reativo
    inicio = std::chrono::high_resolution_clock::now();
    auto caminho_reativo = grafo.tsp_reativo(30); // 30 iterações
    fim = std::chrono::high_resolution_clock::now();
    std::cout << "[Reativo] Tempo: " << std::chrono::duration<double>(fim - inicio).count() 
              << "s | Custo: " << grafo.calcular_custo(caminho_reativo) 
              << " | Válido: " << (caminho_valido(caminho_reativo, grafo.get_ordem()) ? "Sim" : "Não") << std::endl;
}

// --- Testar Matriz e Lista para uma instância ---
void testar_instancia(const std::string& arquivo) {
    // Testar com Matriz (apenas para grafos pequenos)
    if (arquivo.find("grafo_5k") == std::string::npos) { // Ajuste conforme necessário
        GrafoMatriz gm;
        gm.carrega_grafo(arquivo);
        std::cout << "\n[ESTRUTURA: MATRIZ]";
        testar_todos_algoritmos(gm, arquivo);
    }

    // Sempre testar com Lista
    GrafoLista gl;
    gl.carrega_grafo(arquivo);
    std::cout << "\n[ESTRUTURA: LISTA]";
    testar_todos_algoritmos(gl, arquivo);
}

// --- Gerar 5 grafos aleatórios grandes ---
void gerar_grafos_aleatorios_grandes() {
    Util::gerar_grafo_aleatorio(5000, 20000, "entradas/grafo_5k.txt");
    Util::gerar_grafo_aleatorio(7000, 30000, "entradas/grafo_7k.txt");
    Util::gerar_grafo_aleatorio(10000, 40000, "entradas/grafo_10k.txt");
    Util::gerar_grafo_aleatorio(12000, 50000, "entradas/grafo_12k.txt");
    Util::gerar_grafo_aleatorio(15000, 60000, "entradas/grafo_15k.txt");
}

// --- Processar 5 instâncias TSP ---
void processar_instancias_tsp() {
    std::vector<std::string> arquivos_tsp = {
        "entradas/berlin52.tsp",
        "entradas/kroA100.tsp",
        "entradas/pr1002.tsp",
        "entradas/d18512.tsp",
        "entradas/rl5915.tsp"
    };

    for (const auto& arquivo : arquivos_tsp) {
        std::string saida = "entradas/" + arquivo.substr(arquivo.find_last_of("/") + 1) + "_convertido.txt";
        Util::converter_TSPLIB_para_formato_esperado(arquivo, saida);
    }
}

// --- Main Atualizado ---
int main() {
    // Passo 1: Gerar instâncias
    gerar_grafos_aleatorios_grandes();
    processar_instancias_tsp();

    // Passo 2: Lista de instâncias para testar
    std::vector<std::string> instancias = {
        // Grafos aleatórios
        "entradas/grafo_5k.txt",
        "entradas/grafo_7k.txt",
        "entradas/grafo_10k.txt",
        "entradas/grafo_12k.txt",
        "entradas/grafo_15k.txt",
        // TSP convertidos
        "entradas/berlin52.tsp_convertido.txt",
        "entradas/kroA100.tsp_convertido.txt",
        "entradas/pr1002.tsp_convertido.txt",
        "entradas/d18512.tsp_convertido.txt",
        "entradas/rl5915.tsp_convertido.txt"
    };

    // Passo 3: Testar todas as instâncias
    for (const auto& instancia : instancias) {
        testar_instancia(instancia);
    }

    return 0;
}