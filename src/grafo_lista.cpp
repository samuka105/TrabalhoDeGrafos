#include "grafo_lista.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <functional>

GrafoLista::GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : Grafo(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas) {
    lista_adj.resize(vertices);
}

GrafoLista::~GrafoLista() {}

void GrafoLista::buscaProfundidade(int v, std::vector<bool>& visitado) const {
    visitado[v] = true;
    for (const auto& vizinho : lista_adj[v]) {
        if (!visitado[vizinho]) {
            buscaProfundidade(vizinho, visitado);
        }
    }
}

int GrafoLista::n_conexo() const {
    std::vector<bool> visitado(num_vertices, false);
    int componentes = 0;

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i]) {
            componentes++;
            buscaProfundidade(i, visitado);
        }
    }
    return componentes;
}

int GrafoLista::get_grau(int vertice) const {
    return lista_adj[vertice].size();
}

bool GrafoLista::eh_completo() const {
    for (int i = 0; i < num_vertices; ++i) {
        if (lista_adj[i].size() != num_vertices - 1) {
            return false;
        }
    }
    return true;
}

bool GrafoLista::eh_arvore() const {
    std::vector<bool> visitado(num_vertices, false);
    int componentes = 0;

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i]) {
            componentes++;
            buscaProfundidade(i, visitado);
        }
    }
    return (componentes == 1 && lista_adj.size() == num_vertices - 1);
}

bool GrafoLista::possui_articulacao() const {
    std::vector<bool> visitado(num_vertices, false);
    std::vector<int> discovery_time(num_vertices, -1);
    std::vector<int> low_time(num_vertices, -1);
    std::vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui_articulacao = false;

    std::function<void(int)> buscaProfundidade = [&](int u) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;

        int filhos = 0;
        for (int v : lista_adj[u]) {
            if (!visitado[v]) {
                parent[v] = u;
                filhos++;
                buscaProfundidade(v);

                low_time[u] = std::min(low_time[u], low_time[v]);

                if ((parent[u] == -1 && filhos > 1) || (parent[u] != -1 && low_time[v] >= discovery_time[u])) {
                    possui_articulacao = true;
                }
            } else if (v != parent[u]) {
                low_time[u] = std::min(low_time[u], discovery_time[v]);
            }
        }
    };

    for (int i = 0; i < num_vertices; i++) {
        if (!visitado[i]) {
            buscaProfundidade(i);
        }
    }

    return possui_articulacao;
}

bool GrafoLista::possui_ponte() const {
    std::vector<bool> visitado(num_vertices, false);
    std::vector<int> discovery_time(num_vertices, -1);
    std::vector<int> low_time(num_vertices, -1);
    std::vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui_ponte = false;

    std::function<void(int, std::vector<bool>&)> buscaProfundidade = [&](int u, std::vector<bool>& visitado) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;

        for (int v : lista_adj[u]) {
            if (!visitado[v]) {
                parent[v] = u;
                buscaProfundidade(v, visitado);

                low_time[u] = std::min(low_time[u], low_time[v]);
                if (low_time[v] > discovery_time[u]) {
                    possui_ponte = true;
                }
            } else if (v != parent[u]) {
                low_time[u] = std::min(low_time[u], discovery_time[v]);
            }
        }
    };

    for (int i = 0; i < num_vertices; i++) {
        if (!visitado[i]) {
            buscaProfundidade(i, visitado);
        }
    }

    return possui_ponte;
}

bool GrafoLista::eh_bipartido() const {
    std::vector<int> cor(num_vertices, -1);

    for (int i = 0; i < num_vertices; ++i) {
        if (cor[i] == -1) {
            std::queue<int> fila;
            fila.push(i);
            cor[i] = 0;

            while (!fila.empty()) {
                int atual = fila.front();
                fila.pop();

                for (const auto& vizinho : lista_adj[atual]) {
                    if (cor[vizinho] == -1) {
                        cor[vizinho] = 1 - cor[atual];
                        fila.push(vizinho);
                    } else if (cor[vizinho] == cor[atual]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void GrafoLista::carrega_grafo(const std::string& arquivo) {
    std::ifstream entrada(arquivo);
    if (!entrada) {
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }

    entrada >> num_vertices >> direcionado >> peso_vertices >> peso_arestas;
    lista_adj.clear();
    lista_adj.resize(num_vertices);

    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        if (origem == destino) {
            throw std::runtime_error("Laços não são permitidos!");
        }
        for (const auto& vizinho : lista_adj[origem - 1]) {
            if (vizinho == destino - 1) {
                throw std::runtime_error("Arestas múltiplas não são permitidas!");
            }
        }
        lista_adj[origem - 1].push_back(destino - 1);
        if (!direcionado) {
            lista_adj[destino - 1].push_back(origem - 1);
        }
    }
}