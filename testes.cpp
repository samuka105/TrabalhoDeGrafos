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
// --- NOVA VALIDAÇÃO (SUBSTITUI A ANTERIOR) ---
bool caminho_valido(const std::vector<int>& caminho, int num_vertices) {
    if (caminho.empty()) {
        std::cerr << "[ERRO] Caminho vazio!\n";
        return false;
    }

    std::unordered_set<int> visitados;
    for (size_t i = 0; i < caminho.size(); ++i) {
        int node = caminho[i];
        if (node < 0 || node >= num_vertices) {
            std::cerr << "[ERRO] Nó inválido: " << node+1 << "\n";
            return false;
        }
        if (visitados.count(node)) {
            std::cerr << "[ERRO] Nó repetido: " << node+1 << "\n";
            return false;
        }
        visitados.insert(node);
    }

    if (visitados.size() != static_cast<size_t>(num_vertices)) {
        std::cerr << "[ERRO] Nós visitados: " << visitados.size() 
                  << "/" << num_vertices << "\n";
        return false;
    }

    return true;
}
// --- Função para testar TODOS os algoritmos em uma instância ---
void testar_todos_algoritmos(Grafo& grafo, const std::string& nome_instancia) {
    std::cout << "\n=== INSTÂNCIA: " << nome_instancia << " ===\n";
    
    std::cout << "[DEBUG] Iniciando teste de conexidade...\n";
    bool conexo = grafo.eh_conexo();
    std::cout << "[DEBUG] Resultado da conexidade: " << (conexo ? "Conexo" : "Desconexo") << "\n\n";
    if (!conexo) {
        std::cerr << "Grafo não é conexo! Ignorando TSP.\n";
        return;
    }

    // Verifica se há pelo menos um ciclo Hamiltoniano (heurística simples)
    bool ciclo_possivel = (grafo.get_num_arestas() >= grafo.get_ordem() * 2);
    if (!ciclo_possivel) {
        std::cerr << "Grafo não tem arestas suficientes para ciclo Hamiltoniano! Ignorando TSP.\n";
        return;
    }
    


    if (!conexo) {
        std::cerr << "Erro: Grafo não é conexo ou completo! Ignorando TSP\n";
        return;
    }

    auto executar_algoritmo = [&](const std::string& nome, auto algoritmo) {
        std::cout << "[DEBUG] Iniciando algoritmo " << nome << "...\n";
        auto inicio = std::chrono::high_resolution_clock::now();
        
        std::vector<int> caminho;
        try {
            caminho = algoritmo();
        } catch (const std::exception& e) {
            std::cerr << "[ERRO] Falha no algoritmo " << nome << ": " << e.what() << "\n";
            return;
        }

        auto fim = std::chrono::high_resolution_clock::now();
        double tempo = std::chrono::duration<double>(fim - inicio).count();

        std::cout << "[DEBUG] Caminho " << nome << " (" << caminho.size() << " nós): ";
        for (int node : caminho) std::cout << node+1 << " ";
        std::cout << "\n";

        std::cout << "[" << nome << "] Tempo: " << tempo << "s | Custo: " 
                  << grafo.calcular_custo(caminho) << " | Válido: " 
                  << (caminho_valido(caminho, grafo.get_ordem()) ? "Sim" : "Não") << "\n\n";
    };

    executar_algoritmo("Guloso", [&](){ return grafo.tsp_guloso_densidade(); });
    executar_algoritmo("Randomizado", [&](){ return grafo.tsp_randomizado_controlado(50, 3); });
    executar_algoritmo("Reativo", [&](){ return grafo.tsp_reativo(30); });
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

// --- VERSÃO MODIFICADA ---
void gerar_grafos_aleatorios_grandes() {
    // Grafo 5k: 5000 nós com 250k arestas (grau médio ~50)
    Util::gerar_grafo_aleatorio(5000, 250000, "entradas/grafo_5k.txt");
    
    // Grafos adicionais com densidade similar
    Util::gerar_grafo_aleatorio(7000, 350000, "entradas/grafo_7k.txt");
    Util::gerar_grafo_aleatorio(10000, 500000, "entradas/grafo_10k.txt");
    Util::gerar_grafo_aleatorio(12000, 600000, "entradas/grafo_12k.txt");
    Util::gerar_grafo_aleatorio(15000, 750000, "entradas/grafo_15k.txt");
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
        // Grafos aleatórios
        "entradas/grafo_5k.txt",
        "entradas/grafo_7k.txt",
        "entradas/grafo_10k.txt",
        "entradas/grafo_12k.txt",
        "entradas/grafo_15k.txt",
        // TSP convertidos
       
    
    };

    // Passo 3: Testar todas as instâncias
    for (const auto& instancia : instancias) {
        testar_instancia(instancia);
    }

    return 0;
}