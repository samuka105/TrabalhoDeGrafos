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
#ifdef _WIN32
#include <windows.h>
#endif

// --- Validação do Caminho ---
bool caminho_valido(const std::vector<int>& caminho, int num_vertices, const Grafo& grafo) {
    if (caminho.empty() || caminho.front() != caminho.back()) return false;
    std::unordered_set<int> visitados;
    for (size_t i = 0; i < caminho.size() - 1; i++) {
        if (visitados.count(caminho[i])) return false;
        if (grafo.menor_distancia(caminho[i], caminho[i+1]) == std::numeric_limits<double>::infinity()) {
            return false;
        }
        visitados.insert(caminho[i]);
    }
    return (visitados.size() == static_cast<size_t>(num_vertices));
}

// --- Função para testar TODOS os algoritmos em uma instância ---
void testar_todos_algoritmos(Grafo& grafo, const std::string& nome_instancia) {

    // Verifica conexidade antes de prosseguir
    if (!grafo.eh_conexo()) {
        std::cerr << "Erro: Grafo não é conexo! Ignorando testes para esta instância." << std::endl;
        return;
    }

    std::cout << "\n=== INSTÂNCIA: " << nome_instancia << " ===" << std::endl;

    // Testar Guloso
    auto inicio = std::chrono::high_resolution_clock::now();
    auto caminho_guloso = grafo.tsp_guloso_densidade();
    auto fim = std::chrono::high_resolution_clock::now();
    std::cout << "[Guloso] Tempo: " << std::chrono::duration<double>(fim - inicio).count() 
              << "s | Custo: " << grafo.calcular_custo(caminho_guloso) 
              << " | Válido: " << (caminho_valido(caminho_guloso, grafo.get_ordem(), grafo) ? "Sim" : "Não") << std::endl;

    // Testar Randomizado
    inicio = std::chrono::high_resolution_clock::now();
    auto caminho_randomizado = grafo.tsp_randomizado_controlado(50, 3); // 50 iterações, N=3
    fim = std::chrono::high_resolution_clock::now();
    std::cout << "[Randomizado] Tempo: " << std::chrono::duration<double>(fim - inicio).count() 
              << "s | Custo: " << grafo.calcular_custo(caminho_randomizado) 
              << " | Válido: " << (caminho_valido(caminho_randomizado, grafo.get_ordem(), grafo) ? "Sim" : "Não") << std::endl;

    // Testar Reativo
    inicio = std::chrono::high_resolution_clock::now();
    auto caminho_reativo = grafo.tsp_reativo(30); // 30 iterações
    fim = std::chrono::high_resolution_clock::now();
    std::cout << "[Reativo] Tempo: " << std::chrono::duration<double>(fim - inicio).count() 
              << "s | Custo: " << grafo.calcular_custo(caminho_reativo) 
              << " | Válido: " << (caminho_valido(caminho_reativo, grafo.get_ordem(), grafo) ? "Sim" : "Não") << std::endl;
}

// --- Testar Matriz e Lista para uma instância ---
// --- Testar Matriz e Lista para uma instância ---
void testar_instancia(const std::string& arquivo) {
    // Testar com Matriz 
    try {
        GrafoMatriz gm;
        gm.carrega_grafo(arquivo);
        std::cout << "\n[ESTRUTURA: MATRIZ]";
        testar_todos_algoritmos(gm, arquivo);
    } catch (const std::exception& e) {
        std::cerr << "\n[ERRO MATRIZ] " << arquivo << ": " << e.what() << std::endl;
    }

    // Testar com Lista
    try {
        GrafoLista gl;
        gl.carrega_grafo(arquivo);
        std::cout << "\n[ESTRUTURA: LISTA]";
        testar_todos_algoritmos(gl, arquivo);
    } catch (const std::exception& e) {
        std::cerr << "\n[ERRO LISTA] " << arquivo << ": " << e.what() << std::endl;
    }
}

// --- Gerar 5 grafos aleatórios grandes ---
void gerar_grafos_aleatorios_grandes() {
    // Gera 15 instâncias de 5k até 33k, passo de 2k
    const std::vector<std::pair<int, int>> configs = {
        {5000, 15000},   // 5k vértices, 15k arestas
        {7000, 21000},   // 7k vértices, 21k arestas
        {9000, 27000},   // 9k vértices, 27k arestas
        {11000, 33000},  // 11k vértices, 33k arestas
        {13000, 39000},  // 13k vértices, 39k arestas
        {15000, 45000},  // 15k vértices, 45k arestas
        {17000, 51000},  // 17k vértices, 51k arestas
        {19000, 57000},  // 19k vértices, 57k arestas
        {21000, 63000},  // 21k vértices, 63k arestas
        {23000, 69000},  // 23k vértices, 69k arestas
        {25000, 75000},  // 25k vértices, 75k arestas
        {27000, 81000},  // 27k vértices, 81k arestas
        {29000, 87000},  // 29k vértices, 87k arestas
        {31000, 93000},  // 31k vértices, 93k arestas
        {33000, 99000}   // 33k vértices, 99k arestas
    };

    for (const auto& config : configs) {
        int vertices = config.first;
        int arestas = config.second;
        std::string nome = "entradas/grafo_" + std::to_string(vertices/1000) + "k.txt";
        Util::gerar_grafo_aleatorio(vertices, arestas, nome);
    }
}

// --- Processar 5 instâncias TSP ---
/*void processar_instancias_tsp() {
    std::vector<std::string> arquivos_tsp = {
        "entradas/d18512.tsp",
    };

    for (const auto& arquivo : arquivos_tsp) {
        std::string saida = "entradas/" + arquivo.substr(arquivo.find_last_of("/") + 1) + "_convertido.txt";
        Util::converter_TSPLIB_para_formato_esperado(arquivo, saida);
    }
}*/

// --- Main Atualizado ---
int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    // Passo 1: Gerar instâncias
    gerar_grafos_aleatorios_grandes();
    //processar_instancias_tsp();

    // Passo 2: Lista de instâncias para testar
    std::vector<std::string> instancias = {
        // Grafos aleatórios (15 instâncias)
        "entradas/grafo_5k.txt", "entradas/grafo_7k.txt", "entradas/grafo_9k.txt",
        "entradas/grafo_11k.txt", "entradas/grafo_13k.txt", "entradas/grafo_15k.txt",
        "entradas/grafo_17k.txt", "entradas/grafo_19k.txt", "entradas/grafo_21k.txt",
        "entradas/grafo_23k.txt", "entradas/grafo_25k.txt", "entradas/grafo_27k.txt",
        "entradas/grafo_29k.txt", "entradas/grafo_31k.txt", "entradas/grafo_33k.txt"
    };

    // Passo 3: Testar todas as instâncias
    for (const auto& instancia : instancias) {
        testar_instancia(instancia);
    }

    return 0;
}