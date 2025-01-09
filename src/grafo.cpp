#include "grafo.h"


// Retorna a ordem do grafo (quantidade de vértices).
int Grafo::get_ordem() const {
    return num_vertices; // Retorna o número de vértices.
}

// Verifica se o grafo é direcionado.
bool Grafo::eh_direcionado() const {
    return direcionado; // Retorna o valor armazenado na variável.
}

// Verifica se os vértices possuem peso.
bool Grafo::vertice_ponderado() const {
    return peso_vertices; // Retorna o valor armazenado na variável.
}

// Verifica se as arestas possuem peso.
bool Grafo::aresta_ponderada() const {
    return peso_arestas; // Retorna o valor armazenado na variável.
}

void Grafo::buscaProfundidade(int v, vector<bool>& visitado) const {
    if (visitado[v]) return; // Evita loop infinito.
    visitado[v] = true;
    for (const auto& vizinho : this->lista_adj[v]) {
        buscaProfundidade(vizinho, visitado);
    }
}
