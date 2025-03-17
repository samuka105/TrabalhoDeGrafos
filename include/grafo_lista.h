#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

#include "grafo.h"
#include <vector>
#include <list>

/**
 * @brief Implementação de grafo usando lista de adjacência.
 */
class GrafoLista : public Grafo {
private:
    std::vector<std::list<std::pair<int, int>>> lista_adj;

public:
    GrafoLista(); // Construtor padrão
    GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
    ~GrafoLista() override;

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
 
    // Métodos TSP
    std::vector<int> tsp_guloso_densidade() override;
    std::vector<int> tsp_randomizado_controlado(int iteracoes, int N) override;
    std::vector<int> tsp_reativo(int max_iteracoes) override;
    double calcular_custo(const std::vector<int>& caminho) override;

    // Método específico para lista
    int get_grau(int vertice) const;
};

#endif // GRAFO_LISTA_H
