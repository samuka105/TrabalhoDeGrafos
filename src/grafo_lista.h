#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

#include "grafo.h"
#include <list>

/**
 * @brief Implementação de grafo usando lista de adjacência
 */
class GrafoLista : public Grafo {
private:
    std::vector<std::list<std::pair<int, int>>> lista_adj; // Lista de adjacência

public:
    GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
    ~GrafoLista();

    /// Realiza busca em profundidade a partir de um vértice
    void buscaProfundidade(int v, std::vector<bool>& visitado) const override;

    /// Retorna o número de componentes conexas
    int n_conexo() const override;

    /// Verifica se o grafo é completo
    bool eh_completo() const override;

    /// Verifica se o grafo é uma árvore
    bool eh_arvore() const override;

    /// Verifica se o grafo possui vértices de articulação
    bool possui_articulacao() const override;

    /// Verifica se o grafo possui arestas ponte
    bool possui_ponte() const override;

    /// Verifica se o grafo é bipartido
    bool eh_bipartido() const override;

    /// Carrega o grafo a partir de um arquivo
    void carrega_grafo(const std::string& arquivo) override;

    /// Adiciona um novo nó ao grafo
    void novo_no() override;

    /// Adiciona uma nova aresta entre dois vértices
    void nova_aresta(int origem, int destino, int peso) override;

    /// Remove um nó do grafo
    void deleta_no(int id) override;

    /// Remove uma aresta entre dois vértices
    void deleta_aresta(int origem, int destino) override;

    /// Calcula a menor distância entre dois vértices
    double menor_distancia(int origem, int destino) const override;

    // Adicione a declaração na classe GrafoLista
    int get_grau(int vertice) const; // Declare no header  

    
};

#endif // GRAFO_LISTA_H