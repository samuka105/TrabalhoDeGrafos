#include "grafo_matriz.h"
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <limits>
#include <functional>
#include <fstream>

// Construtor: inicializa com capacidade 10
GrafoMatriz::GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : Grafo(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas), capacidade(10) {
    matriz.resize(capacidade, std::vector<int>(capacidade, 0));
    // Se o número de vértices inicial for maior que 10, redimensiona
    if (vertices > capacidade) {
        redimensionarMatriz(vertices);
    }
}

// Destrutor
GrafoMatriz::~GrafoMatriz() {}

// Redimensiona a matriz para nova capacidade
void GrafoMatriz::redimensionarMatriz(int nova_capacidade) {
    std::vector<std::vector<int>> nova_matriz(nova_capacidade, std::vector<int>(nova_capacidade, 0));
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            nova_matriz[i][j] = matriz[i][j];
        }
    }
    matriz = std::move(nova_matriz);
    capacidade = nova_capacidade;
}

// Adiciona um nó; se necessário, redimensiona a matriz
void GrafoMatriz::novo_no() {
    if (num_vertices >= capacidade)
        redimensionarMatriz(capacidade * 2);
    // Inicializa a nova linha e coluna com 0
    for (int i = 0; i < num_vertices; ++i) {
        matriz[i][num_vertices] = 0;
        matriz[num_vertices][i] = 0;
    }
    num_vertices++;
}

// Busca em profundidade
void GrafoMatriz::buscaProfundidade(int v, std::vector<bool>& visitado) const {
    visitado[v] = true;
    for (int j = 0; j < num_vertices; ++j) {
        if (matriz[v][j] != 0 && !visitado[j]) {
            buscaProfundidade(j, visitado);
        }
    }
}

// Número de componentes conexas
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

// Conta o número de arestas
int GrafoMatriz::get_num_arestas() const {
    int count = 0;
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            if (matriz[i][j] != 0)
                count++;
        }
    }
    return direcionado ? count : count / 2;
}

// Verifica se o grafo é completo
bool GrafoMatriz::eh_completo() const {
    for (int i = 0; i < num_vertices; ++i) {
        int grau = 0;
        for (int j = 0; j < num_vertices; ++j) {
            if (matriz[i][j] != 0)
                grau++;
        }
        if (grau != num_vertices - 1)
            return false;
    }
    return true;
}

// Verifica se é árvore (conexo e arestas == n-1)
bool GrafoMatriz::eh_arvore() const {
    return (n_conexo() == 1 && get_num_arestas() == num_vertices - 1);
}

// Verifica vértices de articulação
bool GrafoMatriz::possui_articulacao() const {
    std::vector<bool> visitado(num_vertices, false);
    std::vector<int> discovery_time(num_vertices, -1);
    std::vector<int> low_time(num_vertices, -1);
    std::vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui = false;

    std::function<void(int)> dfs = [&](int u) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;
        int filhos = 0;
        for (int v = 0; v < num_vertices; ++v) {
            if (matriz[u][v] != 0) {
                if (!visitado[v]) {
                    parent[v] = u;
                    filhos++;
                    dfs(v);
                    low_time[u] = std::min(low_time[u], low_time[v]);
                    if ((parent[u] == -1 && filhos > 1) || (parent[u] != -1 && low_time[v] >= discovery_time[u])) {
                        possui = true;
                    }
                } else if (v != parent[u]) {
                    low_time[u] = std::min(low_time[u], discovery_time[v]);
                }
            }
        }
    };

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i])
            dfs(i);
    }
    return possui;
}

// Verifica existência de arestas ponte
bool GrafoMatriz::possui_ponte() const {
    std::vector<bool> visitado(num_vertices, false);
    std::vector<int> discovery_time(num_vertices, -1);
    std::vector<int> low_time(num_vertices, -1);
    std::vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui = false;

    std::function<void(int)> dfs = [&](int u) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;
        for (int v = 0; v < num_vertices; ++v) {
            if (matriz[u][v] != 0) {
                if (!visitado[v]) {
                    parent[v] = u;
                    dfs(v);
                    low_time[u] = std::min(low_time[u], low_time[v]);
                    if (low_time[v] > discovery_time[u]) {
                        possui = true;
                    }
                } else if (v != parent[u]) {
                    low_time[u] = std::min(low_time[u], discovery_time[v]);
                }
            }
        }
    };

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i])
            dfs(i);
    }
    return possui;
}

// Verifica se é bipartido
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
                    if (matriz[atual][j] != 0) {
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

// Carrega grafo a partir de arquivo
void GrafoMatriz::carrega_grafo(const std::string& arquivo) {
    std::ifstream entrada(arquivo);
    if (!entrada) throw std::runtime_error("Erro ao abrir o arquivo!");

    int vertices;
    entrada >> vertices >> direcionado >> peso_vertices >> peso_arestas;
    num_vertices = vertices;
    if (capacidade < num_vertices)
        redimensionarMatriz(num_vertices);
    // Zera a matriz
    for (int i = 0; i < num_vertices; ++i)
        std::fill(matriz[i].begin(), matriz[i].begin() + num_vertices, 0);

    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        origem--; destino--;
        if (origem == destino) throw std::runtime_error("Laços não são permitidos!");
        if (matriz[origem][destino] != 0)
            throw std::runtime_error("Aresta múltipla!");
        matriz[origem][destino] = peso;
        if (!direcionado)
            matriz[destino][origem] = peso;
    }
}

// Adiciona nova aresta
void GrafoMatriz::nova_aresta(int origem, int destino, int peso) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices)
        throw std::out_of_range("Vértices fora do intervalo!");
    if (matriz[origem][destino] != 0)
        throw std::runtime_error("Aresta múltipla!");
    matriz[origem][destino] = peso;
    if (!direcionado)
        matriz[destino][origem] = peso;
}

// Remove nó (recalcula a matriz)
void GrafoMatriz::deleta_no(int id) {
    if (id < 0 || id >= num_vertices)
        throw std::out_of_range("ID do nó inválido!");
    // Remove a linha e coluna id e desloca os índices
    for (int i = id; i < num_vertices - 1; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            matriz[i][j] = matriz[i + 1][j];
        }
    }
    for (int j = id; j < num_vertices - 1; ++j) {
        for (int i = 0; i < num_vertices; ++i) {
            matriz[i][j] = matriz[i][j + 1];
        }
    }
    num_vertices--;
}

// Remove aresta
void GrafoMatriz::deleta_aresta(int origem, int destino) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices)
        throw std::out_of_range("Vértices fora do intervalo!");
    if (matriz[origem][destino] == 0)
        throw std::runtime_error("Aresta não encontrada!");
    matriz[origem][destino] = 0;
    if (!direcionado)
        matriz[destino][origem] = 0;
}

// Menor distância entre dois nós (Dijkstra)
double GrafoMatriz::menor_distancia(int origem, int destino) const {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices)
        throw std::out_of_range("Vértices fora do intervalo!");
    std::vector<double> dist(num_vertices, std::numeric_limits<double>::infinity());
    dist[origem] = 0;
    using pii = std::pair<double, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> fila;
    fila.push({0, origem});
    while (!fila.empty()) {
        int u = fila.top().second;
        double d = fila.top().first;
        fila.pop();
        if (d > dist[u]) continue;
        for (int v = 0; v < num_vertices; ++v) {
            if (matriz[u][v] != 0) {
                double peso = matriz[u][v];
                if (dist[v] > dist[u] + peso) {
                    dist[v] = dist[u] + peso;
                    fila.push({dist[v], v});
                }
            }
        }
    }
    return dist[destino];
}
