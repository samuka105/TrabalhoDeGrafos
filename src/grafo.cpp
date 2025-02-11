#include "grafo.h"

Grafo::Grafo(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : num_vertices(vertices), direcionado(eh_direcionado), peso_vertices(ponderado_vertices), peso_arestas(ponderado_arestas) {}

int Grafo::get_ordem() const {
    return num_vertices;
}

bool Grafo::eh_direcionado() const {
    return direcionado;
}

bool Grafo::vertice_ponderado() const {
    return peso_vertices;
}

bool Grafo::aresta_ponderada() const {
    return peso_arestas;
}