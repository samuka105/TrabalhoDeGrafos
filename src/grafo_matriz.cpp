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
#include <iostream>

bool GrafoMatriz::eh_conexo() const {
    if (num_vertices == 0) {
        return true;
    }

    std::vector<bool> visitado(num_vertices, false);
    std::stack<int> pilha;
    pilha.push(0);
    visitado[0] = true;

    int count_visitados = 1;

    while (!pilha.empty()) {
        int atual = pilha.top();
        pilha.pop();

        for (int vizinho = 0; vizinho < num_vertices; ++vizinho) {
            if (matriz[atual][vizinho] != 0 && !visitado[vizinho]) {
                visitado[vizinho] = true;
                count_visitados++;
                pilha.push(vizinho);
            }
        }
    }

    return count_visitados == num_vertices;
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
    if (!entrada) {
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }

    int vertices;
    entrada >> vertices >> direcionado >> peso_vertices >> peso_arestas;
    num_vertices = vertices;

    if (capacidade < num_vertices) {
        redimensionarMatriz(num_vertices);
    }

    for (int i = 0; i < capacidade; ++i) {
        std::fill(matriz[i].begin(), matriz[i].end(), 0);
    }

    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        origem--; destino--;

        if (origem == destino) {
            throw std::runtime_error("Laços não são permitidos!");
        }

        if (origem < 0 || origem >= num_vertices || destino < 0 || destino >= num_vertices) {
            throw std::runtime_error("ID de vértice inválido!");
        }

        if (matriz[origem][destino] != 0) {
            throw std::runtime_error("Aresta múltipla!");
        }

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

double GrafoMatriz::calcular_custo_caminho_parcial(const std::vector<int>& caminho) {
    if (caminho.size() <= 1) return std::numeric_limits<double>::infinity();
    
    double custo = 0.0;
    for (size_t i = 0; i < caminho.size() - 1; ++i) {
        int u = caminho[i];
        int v = caminho[i + 1];
        
        if (matriz[u][v] == 0) {
            std::cout << "ERRO (Matriz): Aresta não encontrada entre " << u << " e " << v << std::endl;
            return std::numeric_limits<double>::infinity();
        }
        custo += matriz[u][v];
    }
    
    return custo;
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


// --- Algoritmo Guloso ---
// Adicione backtracking e fallbacks
std::vector<int> GrafoLista::tsp_guloso_densidade() {
    std::vector<int> melhor_caminho;
    double menor_custo = std::numeric_limits<double>::max();
    int maior_tamanho = 0;

    std::cout << "DEBUG (Guloso Lista): Vértices: " << num_vertices << std::endl;
    int arestas_totais = 0;
    for (int i = 0; i < num_vertices; i++) arestas_totais += lista_adj[i].size();
    std::cout << "DEBUG (Guloso Lista): Total de arestas: " << arestas_totais << std::endl;

    for (int inicio = 0; inicio < std::min(100, num_vertices); ++inicio) {
        std::vector<int> caminho;
        std::vector<bool> visitado(num_vertices, false);
        int atual = inicio;
        visitado[atual] = true;
        caminho.push_back(atual);
        bool bloqueado = false;
        int backtrack_limit = 3;

        for (int i = 1; i < num_vertices; ++i) {
            double menor_densidade = std::numeric_limits<double>::max();
            int proxima = -1;

            // Heurística principal
            for (const auto& aresta : lista_adj[atual]) {
                int destino = aresta.first;
                if (!visitado[destino]) {
                    int conexoes_nao_visitadas = 0;
                    for (const auto& a : lista_adj[destino]) {
                        if (!visitado[a.first]) conexoes_nao_visitadas++;
                    }
                    double densidade = aresta.second / (conexoes_nao_visitadas + 1.0);
                    if (densidade < menor_densidade) {
                        menor_densidade = densidade;
                        proxima = destino;
                    }
                }
            }

            // Fallback 1: Escolha aleatória
            if (proxima == -1) {
                std::vector<int> opcoes;
                for (const auto& aresta : lista_adj[atual]) {
                    if (!visitado[aresta.first]) opcoes.push_back(aresta.first);
                }
                if (!opcoes.empty()) proxima = opcoes[rand() % opcoes.size()];
            }

            // Fallback 2: Backtracking com proteção contra índice negativo
            if (proxima == -1 && backtrack_limit > 0) {
                int steps_back = std::min(2, (int)caminho.size() - 1);
                for (int s = 0; s < steps_back; ++s) {
                    int last = caminho.back();
                    caminho.pop_back();
                    visitado[last] = false;
                }
                atual = caminho.back();
                i = std::max(0, i - steps_back); // Correção crítica
                backtrack_limit--;
                continue;
            }

            if (proxima == -1) {
                bloqueado = true;
                std::cout << "DEBUG (Guloso Lista): Bloqueado em " << atual 
                          << " após " << caminho.size() << "/" << num_vertices << std::endl;
                break;
            }

            visitado[proxima] = true;
            caminho.push_back(proxima);
            atual = proxima;
        }

        // Avaliação do caminho parcial
        if (caminho.size() > 1) {
            bool ciclo_completo = (caminho.size() == num_vertices);
            if (ciclo_completo) {
                for (const auto& aresta : lista_adj[caminho.back()]) {
                    if (aresta.first == caminho[0]) {
                        ciclo_completo = true;
                        break;
                    }
                }
            }

            if (caminho.size() > maior_tamanho) {
                maior_tamanho = caminho.size();
                melhor_caminho = caminho;
                menor_custo = calcular_custo_caminho_parcial(caminho);
                std::cout << "DEBUG (Guloso Lista): Novo caminho (" << caminho.size() 
                          << "/" << num_vertices << "). Custo: " << menor_custo 
                          << " | Ciclo: " << (ciclo_completo ? "Sim" : "Não") << std::endl;
            } else if (caminho.size() == maior_tamanho) {
                double custo = calcular_custo_caminho_parcial(caminho);
                if (custo < menor_custo) {
                    melhor_caminho = caminho;
                    menor_custo = custo;
                }
            }
        }
    }

    if (melhor_caminho.empty()) {
        std::cout << "ALERTA (Guloso Lista): Nenhum caminho válido encontrado!" << std::endl;
    } else {
        std::cout << "Melhor caminho (Guloso Lista): " << melhor_caminho.size() 
                  << "/" << num_vertices << " vértices. Custo: " << menor_custo << std::endl;
    }

    return melhor_caminho;
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

std::vector<int> GrafoMatriz::tsp_randomizado_controlado(int iteracoes, int N) {
    std::vector<int> melhor_caminho;
    double menor_custo = std::numeric_limits<double>::max();
    int maior_tamanho = 0;
    
    // Debug: Informações do grafo
    std::cout << "DEBUG (Matriz): Vértices: " << num_vertices << std::endl;
    int total_arestas = 0;
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            if (matriz[i][j] != 0) total_arestas++;
        }
    }
    if (!direcionado) total_arestas /= 2;
    std::cout << "DEBUG (Matriz): Total de arestas: " << total_arestas << std::endl;

    for (int it = 0; it < iteracoes; it++) {
        std::vector<int> caminho;
        std::vector<bool> visitado(num_vertices, false);
        int atual = rand() % num_vertices;
        visitado[atual] = true;
        caminho.push_back(atual);
        bool bloqueado = false;

        for (int i = 1; i < num_vertices; i++) {
            std::vector<std::pair<double, int>> opcoes;
            for (int j = 0; j < num_vertices; ++j) {
                if (matriz[atual][j] != 0 && !visitado[j]) {
                    opcoes.push_back({matriz[atual][j], j});
                }
            }

            if (opcoes.empty()) {
                // Backtracking: Remove até 2 vértices anteriores
                int backtrack = std::min(2, (int)caminho.size() - 1);
                for (int b = 0; b < backtrack; ++b) {
                    int ultimo = caminho.back();
                    caminho.pop_back();
                    visitado[ultimo] = false;
                    atual = caminho.back();
                }
                i -= backtrack;
                continue;
            }

            std::sort(opcoes.begin(), opcoes.end());
            int limite = std::min(N, (int)opcoes.size());
            int escolha = rand() % limite;
            int proximo = opcoes[escolha].second;

            visitado[proximo] = true;
            caminho.push_back(proximo);
            atual = proximo;
        }

        // Priorizar caminhos mais longos
        if (caminho.size() > maior_tamanho) {
            maior_tamanho = caminho.size();
            melhor_caminho = caminho;
            menor_custo = calcular_custo_caminho_parcial(caminho);
            std::cout << "DEBUG (Matriz): Novo melhor caminho com " << caminho.size() 
                     << "/" << num_vertices << " vértices. Custo: " << menor_custo << std::endl;
        } else if (caminho.size() == maior_tamanho) {
            double custo = calcular_custo_caminho_parcial(caminho);
            if (custo < menor_custo) {
                melhor_caminho = caminho;
                menor_custo = custo;
                std::cout << "DEBUG (Matriz): Novo melhor caminho (mesmo tamanho) com custo: " 
                         << menor_custo << std::endl;
            }
        }

        // Logging
        bool ciclo_completo = (caminho.size() == num_vertices && matriz[caminho.back()][caminho[0]] != 0);
        if (it % 10 == 0 || it == iteracoes - 1) {
            std::cout << "Teste " << (it+1) << ": Caminho com " << caminho.size() << "/" << num_vertices 
                     << " vértices. Ciclo completo: " << (ciclo_completo ? "Sim" : "Não") << std::endl;
        }
    }

    if (melhor_caminho.empty()) {
        std::cout << "ALERTA (Matriz): Não foi possível encontrar nenhum caminho!" << std::endl;
    } else {
        std::cout << "Melhor resultado (Matriz): Caminho com " << melhor_caminho.size() << "/" << num_vertices 
                 << " vértices. Custo: " << menor_custo << std::endl;
    }

    return melhor_caminho;
}

void GrafoMatriz::diagnosticar_grafo() {
    std::cout << "=== DIAGNÓSTICO DO GRAFO (Matriz) ===" << std::endl;
    std::cout << "Número de vértices: " << num_vertices << std::endl;
    
    int total_arestas = 0;
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            if (matriz[i][j] != 0) total_arestas++;
        }
    }
    if (!direcionado) total_arestas /= 2;
    std::cout << "Total de arestas: " << total_arestas << std::endl;
    
    int max_grau = 0, min_grau = num_vertices;
    for (int i = 0; i < num_vertices; ++i) {
        int grau = 0;
        for (int j = 0; j < num_vertices; ++j) {
            if (matriz[i][j] != 0) grau++;
        }
        max_grau = std::max(max_grau, grau);
        min_grau = std::min(min_grau, grau);
    }
    std::cout << "Grau mínimo: " << min_grau << ", Grau máximo: " << max_grau << std::endl;
    
    // Testar caminhos aleatórios
    for (int teste = 0; teste < 5; ++teste) {
        int inicio = rand() % num_vertices;
        std::vector<bool> visitado(num_vertices, false);
        std::vector<int> caminho;
        int atual = inicio;
        visitado[atual] = true;
        caminho.push_back(atual);
        bool bloqueado = false;
        
        for (int i = 1; i < num_vertices && !bloqueado; ++i) {
            std::vector<int> vizinhos;
            for (int j = 0; j < num_vertices; ++j) {
                if (matriz[atual][j] != 0 && !visitado[j]) {
                    vizinhos.push_back(j);
                }
            }
            if (vizinhos.empty()) {
                bloqueado = true;
            } else {
                int proximo = vizinhos[rand() % vizinhos.size()];
                visitado[proximo] = true;
                caminho.push_back(proximo);
                atual = proximo;
            }
        }
        
        bool ciclo_valido = (matriz[atual][inicio] != 0);
        std::cout << "Teste " << teste+1 << ": Caminho com " << caminho.size() 
                  << "/" << num_vertices << " vértices. Ciclo: " 
                  << (ciclo_valido ? "Sim" : "Não") << std::endl;
    }
    std::cout << "=====================================" << std::endl;
}

double GrafoMatriz::calcular_custo(const std::vector<int>& caminho) {
    if (caminho.empty()) {
        std::cerr << "[ERRO] Caminho vazio!" << std::endl;
        return std::numeric_limits<double>::infinity();
    }
    
    if (caminho.size() == 1) {
        std::cout << "Caminho tem apenas 1 vértice." << std::endl;
        return 0.0;
    }

    double custo_total = 0.0;
    bool erro = false;

    // Calcular custo das arestas presentes
    for (size_t i = 0; i < caminho.size() - 1; ++i) {
        int u = caminho[i];
        int v = caminho[i + 1];
        if (matriz[u][v] == 0) {
            std::cerr << "[ERRO] Aresta não encontrada entre " << u+1 << " e " << v+1 << std::endl;
            erro = true;
        } else {
            custo_total += matriz[u][v];
        }
    }

    // Verificar conexão final para ciclo
    int ultimo = caminho.back();
    int primeiro = caminho[0];
    if (matriz[ultimo][primeiro] != 0) {
        custo_total += matriz[ultimo][primeiro];
    } else {
        std::cout << "[AVISO] Ciclo não fechado: Sem aresta de " 
                  << ultimo+1 << " para " << primeiro+1 << std::endl;
    }

    return erro ? std::numeric_limits<double>::infinity() : custo_total;
}

