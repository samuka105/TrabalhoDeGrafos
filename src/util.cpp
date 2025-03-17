#include "util.h"
#include <fstream>
#include <queue>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <set>
#include <utility> // Para usar std::pair
#include <sstream>
#include <cmath>   // Para std::sqrt
#include <random>
#include <algorithm>

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
    if (arestas < vertices - 1) {
        throw std::runtime_error("Número de arestas insuficiente para formar um grafo conexo!");
    }

    std::ofstream saida(arquivo_saida);
    if (!saida) throw std::runtime_error("Erro ao criar arquivo de saída!");
    std::srand(std::time(0));

    saida << vertices << " 0 0 1" << std::endl; // Grafo não direcionado

    std::set<std::pair<int, int>> arestas_geradas;
    std::vector<int> nos(vertices);
    std::iota(nos.begin(), nos.end(), 1); // Preenche com 1, 2, ..., vertices

    // Embaralha os nós para criar uma árvore aleatória
    std::shuffle(nos.begin(), nos.end(), std::mt19937(std::random_device()()));

    // Passo 1: Constrói uma árvore geradora mínima (garante conexidade)
    for (size_t i = 1; i < nos.size(); ++i) {
        int origem = nos[i];
        int destino = nos[std::rand() % i]; // Conecta a um nó já existente na árvore
        int peso = std::rand() % 10 + 1;
        
        // Garante ordem origem <= destino para evitar duplicatas
        if (origem > destino) std::swap(origem, destino);
        saida << origem << " " << destino << " " << peso << std::endl;
        arestas_geradas.insert({origem, destino});
    }

    // Passo 2: Adiciona arestas extras até atingir o total desejado
    int arestas_restantes = arestas - (vertices - 1);
    for (int i = 0; i < arestas_restantes; ++i) {
        int origem, destino;
        do {
            origem = std::rand() % vertices + 1;
            destino = std::rand() % vertices + 1;
            if (origem > destino) std::swap(origem, destino);
        } while (origem == destino || arestas_geradas.count({origem, destino}));

        int peso = std::rand() % 10 + 1;
        saida << origem << " " << destino << " " << peso << std::endl;
        arestas_geradas.insert({origem, destino});
    }
}

   /**
     * @brief Converte um arquivo TSPLIB para o formato esperado pelo programa.
     * 
     * O arquivo TSPLIB deve conter um cabeçalho com, entre outros, as linhas "DIMENSION : <valor>"
     * e "NODE_COORD_SECTION", seguido da listagem das coordenadas dos nós.
     * 
     * O arquivo de saída terá a seguinte estrutura:
     * <num_vertices> 0 0 1
     * <origem> <destino> <peso>
     * ...
     * 
     * @param arquivo_TSPLIB Caminho para o arquivo TSPLIB.
     * @param arquivo_saida Caminho para o arquivo convertido.
     */
    void converter_TSPLIB_para_formato_esperado(const std::string& arquivo_TSPLIB, const std::string& arquivo_saida) {
        std::ifstream entrada(arquivo_TSPLIB);
        if (!entrada) {
            throw std::runtime_error("Erro ao abrir o arquivo TSPLIB!");
        }
        std::string linha;
        int dimension = 0;
        bool coordenadas_section = false;
        // Ignora as linhas até encontrar "DIMENSION"
        while (std::getline(entrada, linha)) {
            if (linha.find("DIMENSION") != std::string::npos) {
                size_t pos = linha.find(":");
                if (pos != std::string::npos) {
                    std::string dim_str = linha.substr(pos + 1);
                    dimension = std::stoi(dim_str);
                } else {
                    std::istringstream iss(linha);
                    std::string token;
                    iss >> token; // "DIMENSION"
                    iss >> dimension;
                }
            }
            if (linha.find("NODE_COORD_SECTION") != std::string::npos) {
                coordenadas_section = true;
                break;
            }
        }
        if (!coordenadas_section) {
            throw std::runtime_error("Seção de coordenadas não encontrada no arquivo TSPLIB!");
        }
        // Lê as coordenadas dos nós
        std::vector<std::pair<double, double>> coords(dimension);
        int count = 0;
        while (count < dimension && std::getline(entrada, linha)) {
            if (linha.empty()) continue;
            if (linha.find("EOF") != std::string::npos) break;
            std::istringstream iss(linha);
            int index;
            double x, y;
            iss >> index >> x >> y;
            coords[count] = std::make_pair(x, y);
            count++;
        }
        // Escreve o arquivo convertido
        std::ofstream saida(arquivo_saida);
        if (!saida) {
            throw std::runtime_error("Erro ao criar o arquivo de saída para conversão TSPLIB!");
        }
        // Cabeçalho: número de vértices, grafo não direcionado, vértices não ponderados, arestas ponderadas
        saida << dimension << " 0 0 1" << std::endl;
        // Para cada par de nós (i, j) com i < j, calcula a distância Euclidiana
        for (int i = 0; i < dimension; i++) {
            for (int j = i + 1; j < dimension; j++) {
                double dx = coords[i].first - coords[j].first;
                double dy = coords[i].second - coords[j].second;
                double dist = std::sqrt(dx * dx + dy * dy);
                int peso = static_cast<int>(dist + 0.5); // Arredonda para o inteiro mais próximo
                // Escreve a aresta (convertendo índices para 1-based)
                saida << i + 1 << " " << j + 1 << " " << peso << std::endl;
            }
        }
    }
}