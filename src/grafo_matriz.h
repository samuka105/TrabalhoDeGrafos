#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include "grafo.h"
#include <vector>

using namespace std;

// Implementação de um grafo usando matriz de adjacência.
class GrafoMatriz : public Grafo {
private:
    int num_vertices;                 // Número de vértices no grafo.
    bool direcionado;                 // Indica se o grafo é direcionado.
    bool peso_vertices;               // Indica se os vértices possuem peso.
    bool peso_arestas;                // Indica se as arestas possuem peso.
    vector<std::vector<int>> matriz; // Matriz de adjacência.

public:
    // Construtor padrão.
    GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);

    // Implementações dos métodos virtuais da classe base.
    bool eh_bipartido() const override;
    int n_conexo() const override;
    int get_grau(int vertice) const override;
    int get_ordem() const override;
    bool eh_direcionado() const override;
    bool vertice_ponderado() const override;
    bool aresta_ponderada() const override;
    bool eh_completo() const override;
    bool eh_arvore() const override;
    bool possui_articulacao() const override;
    bool possui_ponte() const override;
    void carrega_grafo(const std::string& arquivo) override;
    void novo_grafo(const std::string& config_arquivo) override;
};

#endif // GRAFO_MATRIZ_H
