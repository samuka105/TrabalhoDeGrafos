#include "grafo_lista.h"
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <limits>
#include <functional>
#include <fstream>


// Construtor padrão
GrafoLista::GrafoLista() : Grafo(0, false, false, false) {
    lista_adj.resize(0); // Lista vazia
}

// Construtor
GrafoLista::GrafoLista(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : Grafo(vertices, eh_direcionado, ponderado_vertices, ponderado_arestas) {
    lista_adj.resize(vertices);
}

// Destrutor
GrafoLista::~GrafoLista() {}

// Novo nó
void GrafoLista::novo_no() {
    lista_adj.push_back({});
    num_vertices++;
}

// Busca em profundidade
void GrafoLista::buscaProfundidade(int v, std::vector<bool>& visitado) const {
    visitado[v] = true;
    for (const auto& vizinho : lista_adj[v]) {
        int destino = vizinho.first;
        if (!visitado[destino]) {
            buscaProfundidade(destino, visitado);
        }
    }
}

// Número de componentes conexas
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

// Retorna o grau do vértice
int GrafoLista::get_grau(int vertice) const {
    if (vertice < 0 || vertice >= num_vertices) {
        throw std::out_of_range("Vértice inválido!");
    }
    return lista_adj[vertice].size();
}

// Verifica se o grafo é completo
bool GrafoLista::eh_completo() const {
    for (int i = 0; i < num_vertices; ++i) {
        if (lista_adj[i].size() != static_cast<std::size_t>(num_vertices - 1)) {
            return false;
        }
    }
    return true;
}

// Verifica se o grafo é uma árvore
bool GrafoLista::eh_arvore() const {
    // Para ser árvore: conexo e número de arestas igual a (n - 1)
    int total_arestas = 0;
    for (int i = 0; i < num_vertices; ++i) {
        total_arestas += lista_adj[i].size();
    }
    if (!direcionado) total_arestas /= 2;
    return (n_conexo() == 1 && total_arestas == num_vertices - 1);
}

// Verifica existência de vértices de articulação
bool GrafoLista::possui_articulacao() const {
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
        for (const auto& aresta : lista_adj[u]) {
            int v = aresta.first;
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
    };

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i]) {
            dfs(i);
        }
    }
    return possui;
}

// Verifica existência de arestas ponte
bool GrafoLista::possui_ponte() const {
    std::vector<bool> visitado(num_vertices, false);
    std::vector<int> discovery_time(num_vertices, -1);
    std::vector<int> low_time(num_vertices, -1);
    std::vector<int> parent(num_vertices, -1);
    int time = 0;
    bool possui = false;

    std::function<void(int)> dfs = [&](int u) {
        visitado[u] = true;
        discovery_time[u] = low_time[u] = ++time;
        for (const auto& aresta : lista_adj[u]) {
            int v = aresta.first;
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
    };

    for (int i = 0; i < num_vertices; ++i) {
        if (!visitado[i]) {
            dfs(i);
        }
    }
    return possui;
}

// Verifica se o grafo é bipartido
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
                for (const auto& aresta : lista_adj[atual]) {
                    int vizinho = aresta.first;
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

// Carrega o grafo a partir de arquivo
void GrafoLista::carrega_grafo(const std::string& arquivo) {
    std::ifstream entrada(arquivo);
    if (!entrada) throw std::runtime_error("Erro ao abrir o arquivo!");

    int vertices;
    entrada >> vertices >> direcionado >> peso_vertices >> peso_arestas;
    num_vertices = vertices;
    lista_adj.clear();
    lista_adj.resize(vertices);

    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        // Ajusta de 1-based para 0-based
        origem--; destino--;
        // Verifica laços
        if (origem == destino) throw std::runtime_error("Laços não são permitidos!");
        // Verifica arestas múltiplas
        for (const auto& a : lista_adj[origem]) {
            if (a.first == destino)
                throw std::runtime_error("Aresta múltipla!");
        }
        lista_adj[origem].push_back({destino, peso});
        if (!direcionado)
            lista_adj[destino].push_back({origem, peso});
    }
}

// Adiciona nova aresta
void GrafoLista::nova_aresta(int origem, int destino, int peso) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices)
        throw std::out_of_range("Vértices fora do intervalo!");
    // Verifica aresta existente
    for (const auto& a : lista_adj[origem]) {
        if (a.first == destino)
            throw std::runtime_error("Aresta múltipla!");
    }
    lista_adj[origem].push_back({destino, peso});
    if (!direcionado)
        lista_adj[destino].push_back({origem, peso});
}

// Remove nó (deleta vértice) e ajusta IDs
void GrafoLista::deleta_no(int id) {
    if (id < 0 || id >= num_vertices)
        throw std::out_of_range("ID do nó inválido!");
    // Remove a lista do nó
    lista_adj.erase(lista_adj.begin() + id);
    num_vertices--;
    // Remove arestas apontando para o nó removido e ajusta índices
    for (auto& lista : lista_adj) {
        lista.remove_if([id](const std::pair<int, int>& aresta) {
            return aresta.first == id;
        });
        for (auto& aresta : lista) {
            if (aresta.first > id) {
                aresta.first--;
            }
        }
    }
}

// Remove aresta
void GrafoLista::deleta_aresta(int origem, int destino) {
    if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices)
        throw std::out_of_range("Vértices fora do intervalo!");
    auto& lista_origem = lista_adj[origem];
    auto it = std::find_if(lista_origem.begin(), lista_origem.end(),
                           [destino](const std::pair<int, int>& a) { return a.first == destino; });
    if (it == lista_origem.end())
        throw std::runtime_error("Aresta não encontrada!");
    lista_origem.erase(it);
    if (!direcionado) {
        auto& lista_destino = lista_adj[destino];
        auto it2 = std::find_if(lista_destino.begin(), lista_destino.end(),
                                [origem](const std::pair<int, int>& a) { return a.first == origem; });
        if (it2 != lista_destino.end())
            lista_destino.erase(it2);
    }
}

// Menor distância entre dois nós (Dijkstra)
double GrafoLista::menor_distancia(int origem, int destino) const {
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
        for (const auto& a : lista_adj[u]) {
            int v = a.first;
            double peso = a.second;
            if (dist[v] > dist[u] + peso) {
                dist[v] = dist[u] + peso;
                fila.push({dist[v], v});
            }
        }
    }
    return dist[destino];
}

// --- Algoritmo Randomizado ---
std::vector<int> GrafoLista::tsp_randomizado_controlado(int iteracoes, int N) {
    std::vector<int> melhor_caminho;
    double menor_custo = std::numeric_limits<double>::max();

    for (int it = 0; it < iteracoes; it++) {
        std::vector<int> caminho;
        std::vector<bool> visitado(num_vertices, false);
        int atual = rand() % num_vertices;
        visitado[atual] = true;
        caminho.push_back(atual);

        for (int i = 1; i < num_vertices; i++) {
            std::vector<std::pair<double, int>> opcoes;
            for (const auto& aresta : lista_adj[atual]) {
                int destino = aresta.first;
                if (!visitado[destino]) {
                    opcoes.push_back({aresta.second, destino});
                }
            }
            if (opcoes.empty()) break;

            std::sort(opcoes.begin(), opcoes.end());
            int n = std::min(N, (int)opcoes.size());
            std::vector<double> probabilidades(n);
            double soma = 0;
            for (int j = 0; j < n; j++) {
                probabilidades[j] = 1.0 / opcoes[j].first;
                soma += probabilidades[j];
            }

            double rand_val = (double)rand() / RAND_MAX * soma;
            double acumulado = 0;
            int escolha = 0;
            for (; escolha < n; escolha++) {
                acumulado += probabilidades[escolha];
                if (acumulado >= rand_val) break;
            }

            int proxima = opcoes[escolha].second;
            visitado[proxima] = true;
            caminho.push_back(proxima);
            atual = proxima;
        }

        double custo = calcular_custo(caminho);
        if (custo < menor_custo) {
            menor_custo = custo;
            melhor_caminho = caminho;
        }
    }

    return melhor_caminho;
}

std::vector<int> GrafoLista::tsp_guloso_densidade() {
    std::vector<int> caminho;
    std::vector<bool> visitado(num_vertices, false);
    int atual = 0; // Vértice inicial (pode ser rand() % num_vertices para aleatório)
    visitado[atual] = true;
    caminho.push_back(atual);

    for (int i = 1; i < num_vertices; i++) {
        double menor_densidade = std::numeric_limits<double>::max();
        int proxima = -1;

        // Passo 1: Iterar pelas arestas do vértice atual
        for (const auto& aresta : lista_adj[atual]) {
            int destino = aresta.first;
            if (!visitado[destino]) {
                // Passo 2: Calcular conexões não visitadas do destino
                int conexoes_nao_visitadas = 0;
                for (const auto& aresta_destino : lista_adj[destino]) {
                    if (!visitado[aresta_destino.first]) {
                        conexoes_nao_visitadas++;
                    }
                }
                // Passo 3: Calcular densidade (distância / (conexões + 1))
                double densidade = aresta.second / (conexoes_nao_visitadas + 1.0);
                if (densidade < menor_densidade) {
                    menor_densidade = densidade;
                    proxima = destino;
                }
            }
        }

        if (proxima == -1) break; // Grafo desconexo
        visitado[proxima] = true;
        caminho.push_back(proxima);
        atual = proxima;
    }

    // Verificar se o caminho é válido
    if (caminho.size() != static_cast<size_t>(num_vertices)) {
        throw std::runtime_error("Grafo não é conexo!");
    }

    return caminho;
}


double GrafoLista::calcular_custo(const std::vector<int>& caminho) {
    double custo = 0;
    for (size_t i = 0; i < caminho.size() - 1; i++) {
        for (const auto& aresta : lista_adj[caminho[i]]) {
            if (aresta.first == caminho[i+1]) {
                custo += aresta.second;
                break;
            }
        }
    }
    for (const auto& aresta : lista_adj[caminho.back()]) {
        if (aresta.first == caminho.front()) {
            custo += aresta.second;
            break;
        }
    }
    return custo;
}
