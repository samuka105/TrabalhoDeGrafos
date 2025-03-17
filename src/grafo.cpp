#include "grafo.h"

Grafo::Grafo(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : num_vertices(vertices), direcionado(eh_direcionado), peso_vertices(ponderado_vertices), peso_arestas(ponderado_arestas) {}

int Grafo::get_ordem() const {
    return this->num_vertices;
}

bool Grafo::eh_direcionado() const {
    return this->direcionado;
}

/*************  ✨ Codeium Command ⭐  *************/
/**
 * @brief Verifica se o grafo tem pesos associados aos vértices.
 * @return true se os vértices são ponderados, false caso contrário.
 */

/******  337ef46c-6914-4e80-95a9-653a18500445  *******/
bool Grafo::vertice_ponderado() const {
    return this->peso_vertices;
}

bool Grafo::aresta_ponderada() const {
    return this->peso_arestas;
}
