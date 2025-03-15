#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include "grafo.h"
#include <vector>

/**
 * @brief Implementação de grafo usando matriz de adjacência.
 * A matriz é alocada dinamicamente, iniciando com capacidade 10.
 */
class GrafoMatriz : public Grafo {
private:
    std::vector<std::vector<int>> matriz;
    int capacidade;

    void redimensionarMatriz(int nova_capacidade);

public:
    GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
    ~GrafoMatriz() override;

    // Métodos da classe base (Grafo)
    void buscaProfundidade(int v, std::vector<bool>& visitado) const override;
    int n_conexo() const override;
    bool eh_completo() const override;
    bool eh_arvore() const override;
    bool possui_articulacao() const override;
    bool possui_ponte() const override;
    bool eh_bipartido() const override;
    void carrega_grafo(const std::string& arquivo) override;
    void novo_no() override;
    void nova_aresta(int origem, int destino, int peso) override;
    void deleta_no(int id) override;
    void deleta_aresta(int origem, int destino) override;
    double menor_distancia(int origem, int destino) const override;

    // Método auxiliar para contar arestas
    int get_num_arestas() const;
};

#endif // GRAFO_MATRIZ_H
