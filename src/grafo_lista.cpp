#include "grafo_lista.h"
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <limits>

GrafoLista::GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : Grafo(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas) {
    lista_adj.resize(vertices);
}

GrafoLista::~GrafoLista() {}

void GrafoLista::novo_no() {
    lista_adj.push_back({});
    num_vertices++;
}

void GrafoLista::nova_aresta(int origem, int destino, int peso) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
        throw std::out_of_range("Vértices fora do intervalo!");
    }
    lista_adj[origem].push_back({destino, peso});
    if (!direcionado) {
        lista_adj[destino].push_back({origem, peso});
    }
}

void GrafoLista::deleta_no(int id) {
    if (id < 0 || id >= num_vertices) {
        throw std::out_of_range("ID do nó inválido!");
    }

    // Remove o nó e recalcula os IDs.
    lista_adj.erase(lista_adj.begin() + id);
    num_vertices--;

    for (auto& lista : lista_adj) {
        lista.remove_if([id](const std::pair<int, int>& aresta) {
            return aresta.first == id;
        });
    }
}

void GrafoLista::deleta_aresta(int origem, int destino) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
        throw std::out_of_range("Vértices fora do intervalo!");
    }
    lista_adj[origem].remove_if([destino](const std::pair<int, int>& aresta) {
        return aresta.first == destino;
    });
    if (!direcionado) {
        lista_adj[destino].remove_if([origem](const std::pair<int, int>& aresta) {
            return aresta.first == origem;
        });
    }
}

double GrafoLista::menor_distancia(int origem, int destino) const {
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

        for (const auto& vizinho : lista_adj[u]) {
            int v = vizinho.first;
            double peso = vizinho.second;
            if (distancias[v] > distancias[u] + peso) {
                distancias[v] = distancias[u] + peso;
                fila.push({distancias[v], v});
            }
        }
    }

    return distancias[destino];
}