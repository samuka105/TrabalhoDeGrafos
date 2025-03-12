#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include "grafo.h"
#include <vector>

/**
 * @brief Implementação de grafo usando matriz de adjacência
 */
class GrafoMatriz : public Grafo {
private:
    std::vector<std::vector<int>> matriz; // Matriz de adjacência
    int capacidade; // Capacidade atual da matriz

    /// Redimensiona a matriz para acomodar mais vértices
    void redimensionarMatriz(int nova_capacidade);

public:
    GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
    ~GrafoMatriz();

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
};

#endif // GRAFO_MATRIZ_H