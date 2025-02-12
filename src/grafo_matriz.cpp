#include "grafo_matriz.h"
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <limits>

GrafoMatriz::GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : Grafo(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas), capacidade(10) {
    matriz.resize(capacidade, std::vector<int>(capacidade, 0));
}

GrafoMatriz::~GrafoMatriz() {}

void GrafoMatriz::redimensionarMatriz(int nova_capacidade) {
    std::vector<std::vector<int>> nova_matriz(nova_capacidade, std::vector<int>(nova_capacidade, 0));
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            nova_matriz[i][j] = matriz[i][j];
        }
    }
    matriz = nova_matriz;
    capacidade = nova_capacidade;
}

void GrafoMatriz::novo_no() {
    if (num_vertices >= capacidade) {
        redimensionarMatriz(capacidade * 2);
    }
    num_vertices++;
}

void GrafoMatriz::nova_aresta(int origem, int destino, int peso) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
        throw std::out_of_range("Vértices fora do intervalo!");
    }
    matriz[origem][destino] = peso;
    if (!direcionado) {
        matriz[destino][origem] = peso;
    }
}

void GrafoMatriz::deleta_no(int id) {
    if (id < 0 || id >= num_vertices) {
        throw std::out_of_range("ID do nó inválido!");
    }

    // Remove o nó e recalcula os IDs.
    for (int i = id; i < num_vertices - 1; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            matriz[i][j] = matriz[i + 1][j];
            matriz[j][i] = matriz[j][i + 1];
        }
    }
    num_vertices--;
}

void GrafoMatriz::deleta_aresta(int origem, int destino) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
        throw std::out_of_range("Vértices fora do intervalo!");
    }
    matriz[origem][destino] = 0;
    if (!direcionado) {
        matriz[destino][origem] = 0;
    }
}

double GrafoMatriz::menor_distancia(int origem, int destino) const {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
        throw std::out_of_range("Vértices fora do intervalo!");
    }

    std::vector<double> distancias(num_vertices, std::numeric_limits<double>::infinity());
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> fila;

    distancias[origem] = 0;
    fila.push({0, origem});

    while (!fila.empty()) {
        int u = fila.top().second;
        fila.pop();

        for (int v = 0; v < num_vertices; ++v) {
            if (matriz[u][v] > 0) {
                double peso = matriz[u][v];
                if (distancias[v] > distancias[u] + peso) {
                    distancias[v] = distancias[u] + peso;
                    fila.push({distancias[v], v});
                }
            }
        }
    }

    return distancias[destino];
}