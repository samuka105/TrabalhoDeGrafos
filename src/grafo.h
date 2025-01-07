#ifndef GRAFO_H
#define GRAFO_H

#include <string>

// Classe abstrata para definir a interface de um grafo.
class Grafo {
public:
    // Destrutor virtual para permitir a correta destruição de classes derivadas.
    virtual ~Grafo() {}

    // Verifica se o grafo é bipartido (força bruta).
    virtual bool eh_bipartido() const = 0;

    // Retorna o número de componentes conexas no grafo.
    virtual int n_conexo() const = 0;

    // Retorna o grau de um vértice específico.
    virtual int get_grau(int vertice) const = 0;

    // Retorna a ordem do grafo (quantidade de vértices).
    virtual int get_ordem() const = 0;

    // Verifica se o grafo é direcionado.
    virtual bool eh_direcionado() const = 0;

    // Verifica se os vértices possuem peso.
    virtual bool vertice_ponderado() const = 0;

    // Verifica se as arestas possuem peso.
    virtual bool aresta_ponderada() const = 0;

    // Verifica se o grafo é completo (todos os vértices conectados entre si).
    virtual bool eh_completo() const = 0;

    // Verifica se o grafo é uma árvore (conexo e sem ciclos).
    virtual bool eh_arvore() const = 0;

    // Verifica se existe ao menos um vértice de articulação.
    virtual bool possui_articulacao() const = 0;

    // Verifica se existe ao menos uma aresta ponte.
    virtual bool possui_ponte() const = 0;

    // Carrega um grafo de um arquivo `.txt`.
    virtual void carrega_grafo(const std::string& arquivo) = 0;

    // Gera um grafo aleatório com base em um arquivo de configuração.
    virtual void novo_grafo(const std::string& config_arquivo) = 0;
};

#endif // GRAFO_H
