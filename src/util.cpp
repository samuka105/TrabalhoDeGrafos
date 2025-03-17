#include "util.h"
#include <fstream>
#include <queue>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <set>
#include <utility> // Para usar std::pair

namespace Util {
    std::vector<std::string> ler_arquivo(const std::string& arquivo) {
        std::ifstream entrada(arquivo);
        if (!entrada) {
            throw std::runtime_error("Erro ao abrir o arquivo!");
        }
        std::vector<std::string> linhas;
        std::string linha;
        while (std::getline(entrada, linha)) {
            linhas.push_back(linha);
        }
        return linhas;
    }

    bool verificar_biparticao(const std::vector<std::vector<int>>& grafo) {
        int n = grafo.size();
        std::vector<int> cor(n, -1);
        for (int i = 0; i < n; ++i) {
            if (cor[i] == -1) {
                std::queue<int> fila;
                fila.push(i);
                cor[i] = 0;
                while (!fila.empty()) {
                    int atual = fila.front();
                    fila.pop();
                    for (int vizinho = 0; vizinho < n; ++vizinho) {
                        if (grafo[atual][vizinho] != 0) {
                            if (cor[vizinho] == -1) {
                                cor[vizinho] = 1 - cor[atual];
                                fila.push(vizinho);
                            } else if (cor[vizinho] == cor[atual]) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

// util.cpp
    void gerar_grafo_aleatorio(int vertices, int arestas, const std::string& arquivo_saida) {
        std::ofstream saida(arquivo_saida);
        if (!saida) throw std::runtime_error("Erro ao criar o arquivo de saída!");
    
        std::srand(std::time(0));
        saida << vertices << " 0 0 1" << std::endl; // Grafo não direcionado, arestas ponderadas
    
        std::set<std::pair<int, int>> arestas_geradas; // Armazena arestas únicas
    
        for (int i = 0; i < arestas; ++i) {
            int origem, destino;
            do {
                origem = std::rand() % vertices + 1;
                destino = std::rand() % vertices + 1;
                
                // Garante origem <= destino para evitar duplicatas em grafos não direcionados
                if (origem > destino) std::swap(origem, destino);
                
            } while (origem == destino || arestas_geradas.count({origem, destino})); // Evita laços e arestas repetidas
    
            int peso = std::rand() % 10 + 1;
            saida << origem << " " << destino << " " << peso << std::endl;
            arestas_geradas.insert({origem, destino}); // Registra a aresta
        }
    }
}
