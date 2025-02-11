#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

#include "grafo.h"
#include <vector>
#include <list>

class GrafoLista : public Grafo {
private:
    std::vector<std::list<int>> lista_adj; // Lista de adjacência.

public:
    GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
    ~GrafoLista();

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

#endif // GRAFO_LISTA_H