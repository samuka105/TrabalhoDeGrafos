#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>

class Grafo {
protected:
    int num_vertices;
    bool direcionado;
    bool peso_vertices;
    bool peso_arestas;

    // Função puramente virtual para busca em profundidade
    virtual void buscaProfundidade(int v, std::vector<bool>& visitado) const = 0;

public:
    Grafo(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
    virtual ~Grafo() {}

    virtual int get_ordem() const;
    virtual bool eh_direcionado() const;
    virtual bool vertice_ponderado() const;
    virtual bool aresta_ponderada() const;
    virtual int n_conexo() const = 0;
    virtual bool eh_completo() const = 0;
    virtual bool eh_bipartido() const = 0;
    virtual bool eh_arvore() const = 0;
    virtual bool possui_articulacao() const = 0;
    virtual bool possui_ponte() const = 0;
    virtual void carrega_grafo(const std::string& arquivo) = 0;
};

#endif // GRAFO_H