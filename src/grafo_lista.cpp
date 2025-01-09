#include "grafo_lista.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <functional>
using namespace std;

/**
 * // Construtor padr o
 * // vertices N mero de v rtices do grafo
 * // eh_direcionado Indica se o grafo   direcionado
 * // ponderado_vertices Indica se os v rtices possuem peso
 * // ponderado_arestas Indica se as arestas possuem peso
 */
GrafoLista::GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : num_vertices(vertices), direcionado(eh_direcionado), peso_vertices(ponderado_vertices), peso_arestas(ponderado_arestas) {
    // Inicializa a lista de adjac ncia com um vetor de tamanho vertices,
    // cada posi o do vetor   uma lista vazia.
    lista_adj.resize(vertices);
}

/**
 * // Verifica se o grafo   bipartido
 * // true se o grafo   bipartido, false caso contrario
 */
// GrafoLista.cpp

#include "grafo_lista.h"

int GrafoLista::get_ordem() const {
    return num_vertices;
}

bool GrafoLista::eh_direcionado() const {
    return direcionado;
}

bool GrafoLista::vertice_ponderado() const {
    return peso_vertices;
}

bool GrafoLista::aresta_ponderada() const {
    return peso_arestas;
}

int GrafoLista::n_conexo() const {
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
    vector<bool> visitado(num_vertices, false);
    int componentes = 0;

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i]) {
            componentes++;
            buscaProfundidade(i, visitado);
        }
    }
    return (componentes == 1 && lista_adj.size() == num_vertices - 1);
}


// Método para GrafoLista

bool GrafoLista::possui_articulacao() const {
    vector<bool> visitado(num_vertices, false);
    vector<int> discovery_time(num_vertices, -1);
    vector<int> low_time(num_vertices, -1);
    vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui_articulacao = false;

    // Função auxiliar para DFS
    function<void(int)> buscaProfundidade = [&](int u) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;

        int filhos = 0;
        for (int v : lista_adj[u]) {
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
    };

    // Chama DFS para todos os vértices não visitados
    for (int i = 0; i < num_vertices; i++) {
        if (!visitado[i]) {
            buscaProfundidade(i);
        }
    }

    return possui_articulacao;
}

// Método para GrafoLista

bool GrafoLista::possui_ponte() const {
    vector<bool> visitado(num_vertices, false);
    vector<int> discovery_time(num_vertices, -1);
    vector<int> low_time(num_vertices, -1);
    vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui_ponte = false;

    // Função auxiliar para busca em profundidade
    function<void(int, vector<bool>&)> buscaProfundidade = [&](int u, vector<bool>& visitado) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;

        for (int v : lista_adj[u]) {
            if (!visitado[v]) {
                parent[v] = u;
                buscaProfundidade(v, visitado);  // Passando o vetor visitado

                // Verifica se a aresta u-v é uma ponte
                low_time[u] = min(low_time[u], low_time[v]);
                if (low_time[v] > discovery_time[u]) {
                    possui_ponte = true;
                }
            } else if (v != parent[u]) {
                low_time[u] = min(low_time[u], discovery_time[v]);
            }
        }
    };

    // Chama DFS para todos os vértices não visitados
    for (int i = 0; i < num_vertices; i++) {
        if (!visitado[i]) {
            buscaProfundidade(i, visitado);  // Passando o vetor visitado
        }
    }

    return possui_ponte;
}


bool GrafoLista::eh_bipartido() const {
    // Implementa busca em profundidade para verificar biparti o.
    // Cria um vetor para armazenar a cor de cada v rtice (-1 indica n o colorido).
    vector<int> cor(num_vertices, -1);
    for (int i = 0; i < num_vertices; ++i) {
        // Verifica se o v rtice ainda n o foi colorido.
        if (cor[i] == -1) {
            // Cria uma fila para auxiliar na busca em profundidade (Busca em Largura).
            queue<int> fila;
            // Adiciona o v rtice atual   fila e o colore com 0.
            fila.push(i);
            cor[i] = 0;
            while (!fila.empty()) {
                // Obt m o v rtice atual da fila.
                int atual = fila.front();
                fila.pop();
                // Itera sobre todos os v rtices adjacentes ao v rtice atual.
                for (const auto& vizinho : lista_adj[atual]) {
                    // Verifica se o vizinho ainda n o foi colorido.
                    if (cor[vizinho] == -1) {
                        // Colore o vizinho com a cor oposta do v rtice atual.
                        cor[vizinho] = 1 - cor[atual];
                        fila.push(vizinho);
                    } else if (cor[vizinho] == cor[atual]) {
                        // Se o vizinho j  estiver colorido com a mesma cor do v rtice atual, o grafo n o   bipartido.
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

/**
 * // Carrega o grafo a partir de um arquivo
 * // arquivo Caminho do arquivo que cont m o grafo
 */
void GrafoLista::carrega_grafo(const string& arquivo) {
    ifstream entrada(arquivo);
    if (!entrada) {
        throw runtime_error("Erro ao abrir o arquivo!");
    }
    // L  o n mero de v rtices, se   direcionado, se os v rtices possuem peso e se as arestas possuem peso.
    entrada >> num_vertices >> direcionado >> peso_vertices >> peso_arestas;
    // Inicializa a lista de adjacência com um vetor de tamanho vertices,
    // cada posição do vetor   uma lista vazia.
    lista_adj.clear();
    lista_adj.resize(num_vertices);
    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        // Adiciona a aresta na lista de adjac ncia do v rtice de origem.
        lista_adj[origem - 1].push_back(destino - 1);
        // Se o grafo n o for direcionado, adiciona a aresta na lista de adjac ncia do v rtice de destino.
        if (!direcionado) {
            lista_adj[destino - 1].push_back(origem - 1);
        }
    }
}

/**
 * // Gera um grafo aleat rio com base em um arquivo de configura o
 * // config_arquivo Caminho do arquivo de configura o
 */
void GrafoLista::novo_grafo(const string& config_arquivo) {
    // Abre o arquivo de configura o.
    ifstream entrada(config_arquivo);
    if (!entrada) {
        throw runtime_error("Erro ao abrir o arquivo de configura o!");
    }
    int vertices, arestas, eh_direcionado, ponderado_vertices, ponderado_arestas;
    // L  o n mero de v rtices, arestas, se   direcionado, se os v rtices possuem peso e se as arestas possuem peso.
    entrada >> vertices >> arestas >> eh_direcionado >> ponderado_vertices >> ponderado_arestas;

    // Cria um novo grafo com as configura es lidas.
    *this = GrafoLista(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas);
    // Gera arestas aleat rias com base no n mero de arestas solicitado.
    for (int i = 0; i < arestas; ++i) {
        int origem = rand() % vertices;
        int destino = rand() % vertices;
        while (destino == origem) { // Evitar la os
            destino = rand() % vertices;
        }
        // Adiciona a aresta na lista de adjac ncia do v rtice de origem.
        lista_adj[origem].push_back(destino);
        // Se o grafo n o for direcionado, adiciona a aresta na lista de adjac ncia
             if (!direcionado) {
            lista_adj[destino].push_back(origem);
        }
    }
}

