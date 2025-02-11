#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include "grafo.h"
#include <vector>

class GrafoMatriz : public Grafo {
private:
    std::vector<std::vector<int>> matriz; // Matriz de adjacência.

public:
    GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
    ~GrafoMatriz();

    void buscaProfundidade(int v, std::vector<bool>& visitado) const override;
    int n_conexo() const override;
    int get_grau(int vertice) const;
    bool eh_completo() const override;
    bool eh_arvore() const override;
    bool possui_articulacao() const override;
    bool possui_ponte() const override;
    bool eh_bipartido() const override;
    void carrega_grafo(const std::string& arquivo) override;
};

#endif // GRAFO_MATRIZ_H