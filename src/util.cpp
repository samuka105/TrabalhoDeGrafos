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
#include <map>

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

    saida << vertices << " 0 0 1" << std::endl;

    std::set<std::pair<int, int>> arestas_geradas;
    std::vector<int> nos(vertices);
    std::iota(nos.begin(), nos.end(), 1);

    // --- Passo 1: Ciclo Hamiltoniano Forçado ---
    std::vector<int> ciclo(nos.begin(), nos.end());
    std::shuffle(ciclo.begin(), ciclo.end(), std::mt19937(std::random_device()()));

    for (size_t i = 0; i < ciclo.size(); ++i) {
        int origem = ciclo[i];
        int destino = ciclo[(i + 1) % ciclo.size()];
        if (origem > destino) std::swap(origem, destino);
        if (arestas_geradas.insert({origem, destino}).second) {
            int peso = std::rand() % 10 + 1;
            saida << origem << " " << destino << " " << peso << std::endl;
        }
    }

    // --- Passo 2: Árvore Geradora Mínima ---
    std::shuffle(nos.begin(), nos.end(), std::mt19937(std::random_device()()));
    for (size_t i = 1; i < nos.size(); ++i) {
        int origem = nos[i];
        int destino = nos[std::rand() % i];
        if (origem > destino) std::swap(origem, destino);
        if (arestas_geradas.insert({origem, destino}).second) {
            int peso = std::rand() % 10 + 1;
            saida << origem << " " << destino << " " << peso << std::endl;
        }
    }

    // --- Passo 3: Arestas Extras (Garantindo Unicidade) ---
    int arestas_restantes = arestas - arestas_geradas.size();
    int max_tentativas = 1000000; // Evita loops infinitos

    for (int i = 0; i < arestas_restantes; ++i) {
        int tentativas = 0;
        int origem, destino;
        do {
            origem = std::rand() % vertices + 1;
            destino = std::rand() % vertices + 1;
            if (origem > destino) std::swap(origem, destino);
            tentativas++;
        } while ((origem == destino || arestas_geradas.count({origem, destino})) && tentativas < max_tentativas);

        if (tentativas >= max_tentativas) {
            throw std::runtime_error("Não foi possível gerar arestas únicas!");
        }

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
    void converter_TSPLIB_para_formato_esperado(
        const std::string& arquivo_TSPLIB, 
        const std::string& arquivo_saida, 
        int k_vizinhos = 20  // Parâmetro ajustável
    ) {
        std::ifstream entrada(arquivo_TSPLIB);
        if (!entrada) {
            throw std::runtime_error("Erro ao abrir o arquivo TSPLIB!");
        }
        std::string linha;
        int dimension = 0;
        //comentado para rodar bool coordenadas_section = false;
        
        // Leitura do cabeçalho e dimensão
    // Leitura do cabeçalho e dimensão
    while (std::getline(entrada, linha)) {
        if (linha.find("DIMENSION") != std::string::npos) {
            // Extrai a dimensão (código existente)
        }
        if (linha.find("NODE_COORD_SECTION") != std::string::npos) {
            // Remover ou comentar a linha abaixo
            // coordenadas_section = true;
            break;
        }

        std::vector<std::pair<double, double>> coords(dimension);
        // Leitura das coordenadas (código existente)

        // Estrutura para armazenar arestas únicas (evitar duplicatas)
        std::map<std::pair<int, int>, double> arestas;

        // Para cada vértice, calcula as k arestas mais próximas
        for (int i = 0; i < dimension; ++i) {
            std::vector<std::pair<double, int>> distancias;
            for (int j = 0; j < dimension; ++j) {
                if (i == j) continue;
                double dx = coords[i].first - coords[j].first;
                double dy = coords[i].second - coords[j].second;
                double dist = std::sqrt(dx * dx + dy * dy);
                distancias.emplace_back(dist, j);
            }
            
            // Ordena e seleciona os k mais próximos
            std::sort(distancias.begin(), distancias.end());
            for (int idx = 0; idx < std::min(k_vizinhos, (int)distancias.size()); ++idx) {
                int j = distancias[idx].second;
                int a = std::min(i, j);
                int b = std::max(i, j);
                arestas[{a, b}] = distancias[idx].first;  // Evita duplicatas
            }
        }

        // Escreve as arestas no arquivo
        std::ofstream saida(arquivo_saida);
        saida << dimension << " 0 0 1" << std::endl;
        for (const auto& [par, distancia] : arestas) {
            int peso = static_cast<int>(distancia + 0.5);
            saida << (par.first + 1) << " " << (par.second + 1) << " " << peso << std::endl;
        }
    }
}} 