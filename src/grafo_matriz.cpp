#include "grafo_matriz.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>
#include <functional>

GrafoMatriz::GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : Grafo(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas) {
    matriz.resize(vertices, std::vector<int>(vertices, 0));
}

int GrafoMatriz::get_ordem() const {
    return num_vertices;
}

bool GrafoMatriz::eh_direcionado() const {
    return direcionado;
}

bool GrafoMatriz::vertice_ponderado() const {
    return peso_vertices;
}

bool GrafoMatriz::aresta_ponderada() const {
    return peso_arestas;
}

int GrafoMatriz::n_conexo() const {
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
void GrafoMatriz::buscaProfundidade(int v, std::vector<bool>& visitado) const {
    visitado[v] = true;
    for (int j = 0; j < num_vertices; ++j) {
        if (matriz[v][j] > 0 && !visitado[j]) {
            buscaProfundidade(j, visitado);
        }
    }
}

int GrafoMatriz::get_grau(int vertice) const {
    int grau = 0;
    for (int j = 0; j < num_vertices; ++j) {
        if (matriz[vertice][j] > 0) {
            grau++;
        }
    }
    return grau;
}

bool GrafoMatriz::eh_completo() const {
    for (int i = 0; i < num_vertices; ++i) {
        int grau = 0;
        for (int j = 0; j < num_vertices; ++j) {
            if (matriz[i][j] > 0) {
                grau++;
            }
        }
        if (grau != num_vertices - 1) {
            return false;
        }
    }
    return true;
}

bool GrafoMatriz::eh_arvore() const {
    std::vector<bool> visitado(num_vertices, false);
    int componentes = 0;

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i]) {
            componentes++;
            buscaProfundidade(i, visitado);
        }
    }
    return (componentes == 1 && num_vertices - 1 == n_conexo());
}

bool GrafoMatriz::possui_articulacao() const {
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
        for (int v = 0; v < num_vertices; v++) {
            if (matriz[u][v] > 0) {
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
        }
    };

    for (int i = 0; i < num_vertices; i++) {
        if (!visitado[i]) {
            buscaProfundidade(i);
        }
    }

    return possui_articulacao;
}

bool GrafoMatriz::possui_ponte() const {
    std::vector<bool> visitado(num_vertices, false);
    std::vector<int> discovery_time(num_vertices, -1);
    std::vector<int> low_time(num_vertices, -1);
    std::vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui_ponte = false;

    std::function<void(int)> buscaProfundidade = [&](int u) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;

        for (int v = 0; v < num_vertices; v++) {
            if (matriz[u][v] > 0) {
                if (!visitado[v]) {
                    parent[v] = u;
                    buscaProfundidade(v);

                    low_time[u] = std::min(low_time[u], low_time[v]);
                    if (low_time[v] > discovery_time[u]) {
                        possui_ponte = true;
                    }
                } else if (v != parent[u]) {
                    low_time[u] = std::min(low_time[u], discovery_time[v]);
                }
            }
        }
    };

    for (int i = 0; i < num_vertices; i++) {
        if (!visitado[i]) {
            buscaProfundidade(i);
        }
    }

    return possui_ponte;
}

bool GrafoMatriz::eh_bipartido() const {
    std::vector<int> cor(num_vertices, -1);

    for (int i = 0; i < num_vertices; ++i) {
        if (cor[i] == -1) {
            std::queue<int> fila;
            fila.push(i);
            cor[i] = 0;

            while (!fila.empty()) {
                int atual = fila.front();
                fila.pop();

                for (int j = 0; j < num_vertices; ++j) {
                    if (matriz[atual][j] > 0) {
                        if (cor[j] == -1) {
                            cor[j] = 1 - cor[atual];
                            fila.push(j);
                        } else if (cor[j] == cor[atual]) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void GrafoMatriz::carrega_grafo(const std::string& arquivo) {
    std::ifstream entrada(arquivo);
    if (!entrada) {
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }

    entrada >> num_vertices >> direcionado >> peso_vertices >> peso_arestas;
    matriz.assign(num_vertices, std::vector<int>(num_vertices, 0));

    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        if (origem == destino) {
            throw std::runtime_error("Laços não são permitidos!");
        }
        if (matriz[origem - 1][destino - 1] != 0) {
            throw std::runtime_error("Arestas múltiplas não são permitidas!");
        }
        matriz[origem - 1][destino - 1] = peso;
        if (!direcionado) {
            matriz[destino - 1][origem - 1] = peso;
        }
    }
}