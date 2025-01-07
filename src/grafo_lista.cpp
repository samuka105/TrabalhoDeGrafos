#include "grafo_lista.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>

GrafoLista::GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : num_vertices(vertices), direcionado(eh_direcionado), peso_vertices(ponderado_vertices), peso_arestas(ponderado_arestas) {
    lista_adj.resize(vertices);
}

bool GrafoLista::eh_bipartido() const {
    // Implementa BFS para verificar bipartição.
    std::vector<int> cor(num_vertices, -1);
    for (int i = 0; i < num_vertices; ++i) {
        if (cor[i] == -1) {
            std::queue<int> fila;
            fila.push(i);
            cor[i] = 0;
            while (!fila.empty()) {
                int atual = fila.front();
                fila.pop();
                for (const auto& vizinho : lista_adj[atual]) {
                    if (cor[vizinho] == -1) {
                        cor[vizinho] = 1 - cor[atual];
                        fila.push(vizinho);
                    } else if (cor[vizinho] == cor[atual]) {
                        return false; // Não é bipartido.
                    }
                }
            }
        }
    }
    return true;
}

void GrafoLista::carrega_grafo(const std::string& arquivo) {
    std::ifstream entrada(arquivo);
    if (!entrada) {
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }
    entrada >> num_vertices >> direcionado >> peso_vertices >> peso_arestas;
    lista_adj.clear();
    lista_adj.resize(num_vertices);
    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        lista_adj[origem - 1].push_back(destino - 1);
        if (!direcionado) {
            lista_adj[destino - 1].push_back(origem - 1);
        }
    }
}

void GrafoLista::novo_grafo(const std::string& config_arquivo) {
    // Lê um arquivo de configuração e gera um grafo aleatório.
    std::ifstream entrada(config_arquivo);
    if (!entrada) {
        throw std::runtime_error("Erro ao abrir o arquivo de configuração!");
    }
    int vertices, arestas, eh_direcionado, ponderado_vertices, ponderado_arestas;
    entrada >> vertices >> arestas >> eh_direcionado >> ponderado_vertices >> ponderado_arestas;

    *this = GrafoLista(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas);
    for (int i = 0; i < arestas; ++i) {
        int origem = rand() % vertices;
        int destino = rand() % vertices;
        while (destino == origem) { // Evitar laços
            destino = rand() % vertices;
        }
        lista_adj[origem].push_back(destino);
        if (!direcionado) {
            lista_adj[destino].push_back(origem);
        }
    }
}
