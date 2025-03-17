    #ifndef GRAFO_H
    #define GRAFO_H

    #include <vector>
    #include <string>

    /**
     * @brief Classe abstrata que representa um grafo genérico.
     */
    class Grafo {
    protected:
        int num_vertices;
        bool direcionado;
        bool peso_vertices;
        bool peso_arestas;

    public:
        Grafo(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas);
        virtual ~Grafo() = default;

        // Métodos básicos
        virtual int get_ordem() const;
        virtual bool eh_direcionado() const;
        virtual bool vertice_ponderado() const;
        virtual bool aresta_ponderada() const;

        // Métodos virtuais puros (devem ser implementados nas classes filhas)
        virtual void buscaProfundidade(int v, std::vector<bool>& visitado) const = 0;
        virtual int n_conexo() const = 0;
        virtual bool eh_completo() const = 0;
        virtual bool eh_bipartido() const = 0;
        virtual bool eh_arvore() const = 0;
        virtual bool possui_articulacao() const = 0;
        virtual bool possui_ponte() const = 0;
        virtual void carrega_grafo(const std::string& arquivo) = 0;
        virtual void novo_no() = 0;
        virtual void nova_aresta(int origem, int destino, int peso) = 0;
        virtual void deleta_no(int id) = 0;
        virtual void deleta_aresta(int origem, int destino) = 0;
        virtual double menor_distancia(int origem, int destino) const = 0;
    };

    #endif // GRAFO_H
