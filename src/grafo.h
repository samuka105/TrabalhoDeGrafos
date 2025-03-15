#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>

/**
 * @brief Classe abstrata representando um grafo genérico
 */
class Grafo {
protected:
    int num_vertices;
    bool direcionado;
    bool peso_vertices;
    bool peso_arestas;

    /// Função puramente virtual para busca em profundidade
    virtual void buscaProfundidade(int v, std::vector<bool>& visitado) const = 0;

public:
    Grafo(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
    virtual ~Grafo() {}

    /// Retorna o número de vértices do grafo
    virtual int get_ordem() const;

    /// Verifica se o grafo é direcionado
    virtual bool eh_direcionado() const;

    /// Verifica se os vértices são ponderados
    virtual bool vertice_ponderado() const;

    /// Verifica se as arestas são ponderadas
    virtual bool aresta_ponderada() const;

    /// Retorna o número de componentes conexas
    virtual int n_conexo() const = 0;

    /// Verifica se o grafo é completo
    virtual bool eh_completo() const = 0;

    /// Verifica se o grafo é bipartido
    virtual bool eh_bipartido() const = 0;

    /// Verifica se o grafo é uma árvore
    virtual bool eh_arvore() const = 0;

    /// Verifica se o grafo possui vértices de articulação
    virtual bool possui_articulacao() const = 0;

    /// Verifica se o grafo possui arestas ponte
    virtual bool possui_ponte() const = 0;

    /// Carrega o grafo a partir de um arquivo
    virtual void carrega_grafo(const std::string& arquivo) = 0;

    /// Adiciona um novo nó ao grafo
    virtual void novo_no() = 0;

    /// Adiciona uma nova aresta entre dois vértices
    virtual void nova_aresta(int origem, int destino, int peso) = 0;

    /// Remove um nó do grafo
    virtual void deleta_no(int id) = 0;

    /// Remove uma aresta entre dois vértices
    virtual void deleta_aresta(int origem, int destino) = 0;

    /// Calcula a menor distância entre dois vértices
    virtual double menor_distancia(int origem, int destino) const = 0;
    
};

#endif // GRAFO_H