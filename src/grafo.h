#ifndef GRAFO_H
#define GRAFO_H

#include <string>
#include <vector>
#include <list>
using namespace std;

// Classe abstrata para definir a interface de um grafo.
class Grafo {
protected:
    int num_vertices;       // Número de vértices.
    bool direcionado;       // Se o grafo é direcionado.
    bool peso_vertices;     // Se os vértices possuem peso.
    bool peso_arestas;      // Se as arestas possuem peso.
    vector<std::list<int>> lista_adj; // Declare lista_adj as a member variable

public:
    // Destrutor virtual.
    virtual ~Grafo() {}

    // Métodos gerais implementados diretamente em Grafo.cpp.
    virtual int get_ordem() const;               // Retorna a ordem do grafo.
    virtual bool eh_direcionado() const;         // Verifica se o grafo é direcionado.
    virtual bool vertice_ponderado() const;      // Verifica se os vértices possuem peso.
    virtual bool aresta_ponderada() const;       // Verifica se as arestas possuem peso.

    // Métodos abstratos a serem implementados nas classes derivadas.
    virtual bool eh_bipartido() const = 0;
    virtual int n_conexo() const = 0;
    virtual int get_grau(int vertice) const = 0;
    virtual bool eh_completo() const = 0;
    virtual bool eh_arvore() const = 0;
    virtual bool possui_articulacao() const = 0;
    virtual bool possui_ponte() const = 0;
    virtual void carrega_grafo(const string& arquivo) = 0;
    virtual void novo_grafo(const string& config_arquivo) = 0;
    void buscaProfundidade(int v, vector<bool>& visitado) const;
};

#endif // GRAFO_H

