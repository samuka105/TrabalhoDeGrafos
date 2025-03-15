#include "grafo_lista.h"
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <limits>
#include <functional> // Adicione no início do arquivo grafo_matriz.cpp ou grafo_lista.cpp
#include <fstream> // Adicione em grafo_lista.cpp e grafo_matriz.cpp

GrafoLista::GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : Grafo(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas) {
    lista_adj.resize(vertices);
}

GrafoLista::~GrafoLista() {}

void GrafoLista::novo_no() {
    lista_adj.push_back({});
    num_vertices++;
}

void GrafoLista::buscaProfundidade(int v, std::vector<bool>& visitado) const {
    visitado[v] = true;
    for (const auto& vizinho : this->lista_adj[v]) { // vizinho é um pair<int, int>
        int destino = vizinho.first; // Acessa o ID do vértice
        if (!visitado[destino]) {
            buscaProfundidade(destino, visitado); // Passa destino (int)
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
    std::vector<int> cor(this->num_vertices, -1);

    for (int i = 0; i < this->num_vertices; ++i) {
        if (cor[i] == -1) {
            std::queue<int> fila;
            fila.push(i);
            cor[i] = 0;

            while (!fila.empty()) {
                int atual = fila.front();
                fila.pop();

                for (const auto& aresta : this->lista_adj[atual]) {
                    int destino = aresta.first; // Acessa o ID do vértice
                    if (cor[destino] == -1) {
                        cor[destino] = 1 - cor[atual];
                        fila.push(destino);
                    } else if (cor[destino] == cor[atual]) {
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

void GrafoLista::nova_aresta(int origem, int destino, int peso) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
        throw std::out_of_range("Vértices fora do intervalo!");
    }
    lista_adj[origem].push_back({destino, peso});
    if (!direcionado) {
        lista_adj[destino].push_back({origem, peso});
    }
}

void GrafoLista::deleta_no(int id) {
    if (id < 0 || id >= num_vertices) {
        throw std::out_of_range("ID do nó inválido!");
    }

    // Remove o nó e recalcula os IDs.
    lista_adj.erase(lista_adj.begin() + id);
    num_vertices--;

    for (auto& lista : lista_adj) {
        lista.remove_if([id](const std::pair<int, int>& aresta) {
            return aresta.first == id;
        });
    }
}

void GrafoLista::deleta_aresta(int origem, int destino) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
        throw std::out_of_range("Vértices fora do intervalo!");
    }
    lista_adj[origem].remove_if([destino](const std::pair<int, int>& aresta) {
        return aresta.first == destino;
    });
    if (!direcionado) {
        lista_adj[destino].remove_if([origem](const std::pair<int, int>& aresta) {
            return aresta.first == origem;
        });
    }
}

double GrafoLista::menor_distancia(int origem, int destino) const {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
        throw std::out_of_range("Vértices fora do intervalo!");
    }

    std::vector<double> distancias(num_vertices, std::numeric_limits<double>::infinity());
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> fila;

    distancias[origem] = 0;
    fila.push({0, origem});

    while (!fila.empty()) {
        int u = fila.top().second;
        fila.pop();

        for (const auto& vizinho : lista_adj[u]) {
            int v = vizinho.first;
            double peso = vizinho.second;
            if (distancias[v] > distancias[u] + peso) {
                distancias[v] = distancias[u] + peso;
                fila.push({distancias[v], v});
            }
        }
    }

    return distancias[destino];
}