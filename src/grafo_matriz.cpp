// Construtor padrão.
#include "grafo_matriz.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

GrafoMatriz::GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : num_vertices(vertices), direcionado(eh_direcionado), peso_vertices(ponderado_vertices), peso_arestas(ponderado_arestas) {
    // Inicializa a matriz de adjac ncia com zeros.
    matriz.resize(vertices, vector<int>(vertices, 0));
}

// Verifica se o grafo   bipartido.

// GrafoMatriz.cpp

#include "grafo_matriz.h"

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
    vector<bool> visitado(num_vertices, false);
    int componentes = 0;

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i]) {
            componentes++;
            buscaProfundidade(i, visitado);
        }
    }
    return componentes;
}

void GrafoMatriz::buscaProfundidade(int v, vector<bool>& visitado) const {
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
    vector<bool> visitado(num_vertices, false);
    int componentes = 0;

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i]) {
            componentes++;
            buscaProfundidade(i, visitado);
        }
    }
    return (componentes == 1 && num_vertices - 1 == n_conexo());
}


// Método para GrafoMatriz

bool GrafoMatriz::possui_articulacao() const {
    vector<bool> visitado(num_vertices, false);
    vector<int> discovery_time(num_vertices, -1);
    vector<int> low_time(num_vertices, -1);
    vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui_articulacao = false;

    // Função auxiliar para buscaProfundidade
    function<void(int)> buscaProfundidade = [&](int u) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;

        int filhos = 0;
        for (int v = 0; v < num_vertices; v++) {
            if (matriz[u][v] > 0) {
                if (!visitado[v]) {
                    parent[v] = u;
                    filhos++;
                    buscaProfundidade(v);

                    // Verifica se o subárvore de v tem conexão com o vértice anterior
                    low_time[u] = min(low_time[u], low_time[v]);

                    // Se u não pode ser conectado por um vértice anterior, então u é uma articulação
                    if ((parent[u] == -1 && filhos > 1) || (parent[u] != -1 && low_time[v] >= discovery_time[u])) {
                        possui_articulacao = true;
                    }
                } else if (v != parent[u]) {
                    low_time[u] = min(low_time[u], discovery_time[v]);
                }
            }
        }
    };

    // Chama buscaProfundidade para todos os vértices não visitados
    for (int i = 0; i < num_vertices; i++) {
        if (!visitado[i]) {
            buscaProfundidade(i);
        }
    }

    return possui_articulacao;
}


// Método para GrafoMatriz

bool GrafoMatriz::possui_ponte() const {
    vector<bool> visitado(num_vertices, false);
    vector<int> discovery_time(num_vertices, -1);
    vector<int> low_time(num_vertices, -1);
    vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui_ponte = false;

    // Função auxiliar para buscaProfundidade
    function<void(int)> buscaProfundidade = [&](int u) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;

        for (int v = 0; v < num_vertices; v++) {
            if (matriz[u][v] > 0) {
                if (!visitado[v]) {
                    parent[v] = u;
                    buscaProfundidade(v);

                    // Verifica se a aresta u-v é uma ponte
                    low_time[u] = min(low_time[u], low_time[v]);
                    if (low_time[v] > discovery_time[u]) {
                        possui_ponte = true;
                    }
                } else if (v != parent[u]) {
                    low_time[u] = min(low_time[u], discovery_time[v]);
                }
            }
        }
    };

    // Chama buscaProfundidade para todos os vértices não visitados
    for (int i = 0; i < num_vertices; i++) {
        if (!visitado[i]) {
            buscaProfundidade(i);
        }
    }

    return possui_ponte;
}



bool GrafoMatriz::eh_bipartido() const {
    // Cria um vetor para armazenar a cor de cada v rtice (-1 indica n o colorido).
    vector<int> cor(num_vertices, -1);

    // Itera sobre todos os v rtices do grafo.
    for (int i = 0; i < num_vertices; ++i) {
        // Verifica se o v rtice ainda n o foi colorido.
        if (cor[i] == -1) {
            // Cria uma fila para auxiliar na BFS (Busca em Largura).
            queue<int> fila;
            // Adiciona o v rtice atual   fila e o colore com 0.
            fila.push(i);
            cor[i] = 0;

            // Executa a BFS enquanto a fila n o estiver vazia.
            while (!fila.empty()) {
                // Obt m o v rtice atual da fila.
                int atual = fila.front();
                fila.pop();

                // Itera sobre todos os v rtices adjacentes ao v rtice atual.
                for (int j = 0; j < num_vertices; ++j) {
                    // Verifica se h  uma aresta entre o v rtice atual e o vizinho.
                    if (matriz[atual][j] > 0) {
                        // Se o vizinho ainda n o estiver colorido, colore-o com a cor oposta do v rtice atual.
                        if (cor[j] == -1) {
                            cor[j] = 1 - cor[atual];
                            fila.push(j);
                        } else if (cor[j] == cor[atual]) {
                            // Se o vizinho j  estiver colorido com a mesma cor do v rtice atual, o grafo n o   bipartido.
                            return false;
                        }
                    }
                }
            }
        }
    }

    // Se todos os vértices puderem ser coloridos com duas cores sem conflitos, o grafo bipartido.
    return true;
}

// Carrega o grafo a partir de um arquivo.
void GrafoMatriz::carrega_grafo(const string& arquivo) {
    // Abre o arquivo passado como parâmetro para leitura.
    ifstream entrada(arquivo);
    if (!entrada) {
        // Lança uma exceção caso o arquivo não possa ser aberto.
        throw runtime_error("Erro ao abrir o arquivo!");
    }

    // Le o numero de vertices, se o grafo for direcionado, se os vertices possuem peso e se as arestas possuem peso.
    entrada >> num_vertices >> direcionado >> peso_vertices >> peso_arestas;

    // Inicializa a matriz de adjac ncia com zeros.
    matriz.assign(num_vertices, vector<int>(num_vertices, 0));

    // L  as arestas do arquivo e as adiciona   matriz de adjac ncia.
    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        matriz[origem - 1][destino - 1] = peso;
        if (!direcionado) {
            // Se o grafo n o for direcionado, adiciona a aresta na outra dire o.
            matriz[destino - 1][origem - 1] = peso;
        }
    }
}


