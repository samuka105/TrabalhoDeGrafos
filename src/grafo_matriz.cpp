#include "grafo_matriz.h"
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <limits>
#include <functional>
#include <fstream>
#include <numeric>  // Para std::accumulate
#include <algorithm> // Para std::min e std::max
#include <stack>

// grafo_matriz.cpp

// Construtor padrão (sem parâmetros)
GrafoMatriz::GrafoMatriz() 
    : Grafo(0, false, false, false), capacidade(10) { // Inicializa com 0 vértices
    matriz.resize(capacidade, std::vector<int>(capacidade, 0)); // Cria matriz 10x10
}

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

#include <queue>
#include <algorithm>

bool GrafoMatriz::eh_conexo() const {
    if (num_vertices == 0) return true;

    std::vector<bool> visitado(num_vertices, false);
    std::stack<int> pilha;
    
    // Começa do primeiro nó (índice 0)
    pilha.push(0);
    visitado[0] = true;

    while (!pilha.empty()) {
        int atual = pilha.top();
        pilha.pop();

        // Verifica todos os nós possíveis
        for (int vizinho = 0; vizinho < num_vertices; vizinho++) {
            // Se há aresta e o vizinho não foi visitado
            if (matriz[atual][vizinho] != 0 && !visitado[vizinho]) {
                visitado[vizinho] = true;
                pilha.push(vizinho);
            }
        }
    }

    // Verifica se todos foram visitados
    return std::all_of(
        visitado.begin(), 
        visitado.end(), 
        [](bool v) { return v; }
    );
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
    if (!entrada) throw std::runtime_error("Erro ao abrir o arquivo: " + arquivo);

    int vertices;
    entrada >> vertices >> direcionado >> peso_vertices >> peso_arestas;
    num_vertices = vertices;

    // Redimensiona se necessário
    if (capacidade < num_vertices) {
        redimensionarMatriz(num_vertices);
    }

    // Zera toda a matriz (incluindo capacidade extra)
    for (int i = 0; i < capacidade; ++i) {
        std::fill(matriz[i].begin(), matriz[i].end(), 0);
    }

    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        origem--; // Converte para 0-based
        destino--;

        if (origem == destino) 
            throw std::runtime_error("Laços não são permitidos!");
        if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) 
            throw std::runtime_error("ID de vértice inválido!");
        if (matriz[origem][destino] != 0) 
            throw std::runtime_error("Aresta múltipla!");

        matriz[origem][destino] = peso;
        if (!direcionado) {
            matriz[destino][origem] = peso;
        }
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

std::vector<int> GrafoMatriz::tsp_randomizado_controlado(int iteracoes, int N) {
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
            for (int j = 0; j < num_vertices; j++) {
                if (matriz[atual][j] != 0 && !visitado[j]) {
                    opcoes.push_back({matriz[atual][j], j});
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

// --- Algoritmo Guloso ---
std::vector<int> GrafoMatriz::tsp_guloso_densidade() {
    std::vector<int> caminho;
    std::vector<bool> visitado(num_vertices, false);
    int atual = 0;
    visitado[atual] = true;
    caminho.push_back(atual);

    for (int i = 1; i < num_vertices; i++) {
        double menor_densidade = std::numeric_limits<double>::max();
        int proxima = -1;

        for (int j = 0; j < num_vertices; j++) {
            if (!visitado[j] && matriz[atual][j] != 0) {
                int conexoes_nao_visitadas = 0;
                for (int k = 0; k < num_vertices; k++) {
                    if (!visitado[k] && matriz[j][k] != 0) conexoes_nao_visitadas++;
                }
                double densidade = matriz[atual][j] / (conexoes_nao_visitadas + 1);
                if (densidade < menor_densidade) {
                    menor_densidade = densidade;
                    proxima = j;
                }
            }
        }

        if (proxima == -1) break;
        visitado[proxima] = true;
        caminho.push_back(proxima);
        atual = proxima;
    }

    return caminho;
}

std::vector<int> GrafoMatriz::tsp_reativo(int max_iteracoes) {
    int N = 3;
    double taxa_limite_inferior = 0.05;
    double taxa_limite_superior = 0.20;
    std::vector<double> custos_historicos;
    std::vector<int> melhor_caminho;
    double menor_custo = std::numeric_limits<double>::max();

    for (int it = 0; it < max_iteracoes; ++it) {
        // Passo 1: Executa o randomizado com N atual
        auto caminho = tsp_randomizado_controlado(1, N); // 1 iteração por ciclo
        
        // Passo 2: Calcula custo e atualiza histórico
        double custo = calcular_custo(caminho);
        custos_historicos.push_back(custo);
        
        if (custo < menor_custo) {
            menor_custo = custo;
            melhor_caminho = caminho;
        }

        // Passo 3: Ajuste dinâmico de N (a cada 10 iterações)
        if (it % 10 == 0 && it >= 10) {
            double custo_medio_anterior = std::accumulate(custos_historicos.begin() + it - 10, custos_historicos.begin() + it - 5, 0.0) / 5;
            double custo_medio_atual = std::accumulate(custos_historicos.begin() + it - 5, custos_historicos.begin() + it, 0.0) / 5;
            double taxa = (custo_medio_anterior - custo_medio_atual) / custo_medio_anterior;
            
            if (taxa < taxa_limite_inferior) {
                N = std::min(5, N + 1); // Aumenta exploração
            } else if (taxa > taxa_limite_superior) {
                N = std::max(2, N - 1); // Aumenta exploitation
            }
        }
    }

    return melhor_caminho;
}

double GrafoMatriz::calcular_custo(const std::vector<int>& caminho) {
    double custo = 0;
    for (size_t i = 0; i < caminho.size() - 1; i++) {
        custo += matriz[caminho[i]][caminho[i+1]]; // Erro aqui! Remova o colchete extra.
    }
    custo += matriz[caminho.back()][caminho.front()]; 
    return custo;
} // 