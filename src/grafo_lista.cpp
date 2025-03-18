#include "grafo_lista.h"
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <limits>
#include <functional>
#include <fstream>
#include <numeric>
#include <stack>
#include <iostream>


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

int GrafoLista::get_num_arestas() const {
    int total_arestas = 0;
    for (int i = 0; i < num_vertices; ++i) {
        total_arestas += lista_adj[i].size();
    }
    return direcionado ? total_arestas : total_arestas / 2;
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

// grafo_lista.cpp
// grafo_lista.cpp
bool GrafoLista::eh_conexo() const {
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

        for (const auto& vizinho : lista_adj[atual]) {
            if (!visitado[vizinho.first]) {
                visitado[vizinho.first] = true;
                count_visitados++;
                pilha.push(vizinho.first);
            }
        }
    }

    return count_visitados == num_vertices;
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
    if (!entrada) {
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }

    int vertices, dummy1, dummy2, dummy3;
    entrada >> vertices >> dummy1 >> dummy2 >> dummy3;
    num_vertices = vertices;
    lista_adj.clear();
    lista_adj.resize(vertices);

    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        origem--; destino--;

        if (origem == destino) {
            throw std::runtime_error("Laços não são permitidos!");
        }

        for (const auto& a : lista_adj[origem]) {
            if (a.first == destino) {
                throw std::runtime_error("Aresta múltipla!");
            }
        }

        lista_adj[origem].push_back({destino, peso});
        if (!direcionado) {
            lista_adj[destino].push_back({origem, peso});
        }
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
// Adicione esta função ao seu arquivo GrafoLista.h ou .cpp
double GrafoLista::calcular_custo_caminho_parcial(const std::vector<int>& caminho) {
    if (caminho.size() <= 1) return std::numeric_limits<double>::infinity();
    
    double custo = 0.0;
    for (size_t i = 0; i < caminho.size() - 1; i++) {
        int atual = caminho[i];
        int proximo = caminho[i+1];
        
        // Procura a aresta entre atual e próximo
        bool aresta_encontrada = false;
        for (const auto& aresta : lista_adj[atual]) {
            if (aresta.first == proximo) {
                custo += aresta.second;
                aresta_encontrada = true;
                break;
            }
        }
        
        if (!aresta_encontrada) {
            std::cout << "ERRO: Aresta não encontrada entre " << atual << " e " << proximo << std::endl;
            return std::numeric_limits<double>::infinity();
        }
    }
    
    return custo;
}

// Versão modificada do tsp_randomizado_controlado
std::vector<int> GrafoLista::tsp_randomizado_controlado(int iteracoes, int N) {
    std::vector<int> melhor_caminho;
    double menor_custo = std::numeric_limits<double>::max();
    int maior_tamanho = 0;
    
    // DEBUG: Informações sobre o grafo
    std::cout << "DEBUG: Verificando grafo - Vértices: " << num_vertices << std::endl;
    int arestas_totais = 0;
    for (int i = 0; i < num_vertices; i++) {
        arestas_totais += lista_adj[i].size();
    }
    std::cout << "DEBUG: Total de arestas: " << arestas_totais << std::endl;
    
    for (int it = 0; it < iteracoes; it++) {
        std::vector<int> caminho;
        std::vector<bool> visitado(num_vertices, false);
        int atual = rand() % num_vertices;
        visitado[atual] = true;
        caminho.push_back(atual);
        bool bloqueado = false;
        
        for (int i = 1; i < num_vertices && !bloqueado; i++) {
            std::vector<std::pair<double, int>> opcoes;
            for (const auto& aresta : lista_adj[atual]) {
                int destino = aresta.first;
                if (!visitado[destino]) {
                    opcoes.push_back({aresta.second, destino});
                }
            }
            
            if (opcoes.empty()) {
                bloqueado = true;
                // std::cout << "DEBUG: Bloqueado no vértice " << atual << " após visitar " 
                //          << caminho.size() << " de " << num_vertices << " vértices" << std::endl;
                break;
            }
            
            std::sort(opcoes.begin(), opcoes.end()); // Ordena por menor custo
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
            std::cout << "DEBUG: Novo melhor caminho com " << caminho.size() << "/" << num_vertices 
                     << " vértices. Custo: " << menor_custo << std::endl;
        }
        // Se mesmo tamanho, escolher o de menor custo
        else if (caminho.size() == maior_tamanho) {
            double custo = calcular_custo_caminho_parcial(caminho);
            if (custo < menor_custo) {
                melhor_caminho = caminho;
                menor_custo = custo;
                std::cout << "DEBUG: Novo melhor caminho (mesmo tamanho) com custo: " << menor_custo << std::endl;
            }
        }
        
        // Verifica se é possível formar um ciclo (útil para logging)
        bool ciclo_completo = false;
        if (caminho.size() == num_vertices) {
            // Verifica se o último vértice pode voltar ao primeiro
            for (const auto& aresta : lista_adj[caminho.back()]) {
                if (aresta.first == caminho[0]) {
                    ciclo_completo = true;
                    break;
                }
            }
        }
        
        if (it % 10 == 0 || it == iteracoes - 1) {
            std::cout << "Teste " << (it+1) << ": Caminho com " << caminho.size() << "/" << num_vertices 
                     << " vértices. Ciclo completo: " << (ciclo_completo ? "Sim" : "Não") << std::endl;
        }
    }
    
    if (melhor_caminho.empty()) {
        std::cout << "ALERTA: Não foi possível encontrar nenhum caminho!" << std::endl;
    } else {
        std::cout << "Melhor resultado: Caminho com " << melhor_caminho.size() << "/" << num_vertices 
                 << " vértices. Custo: " << menor_custo << std::endl;
    }
    
    return melhor_caminho;  // Sempre retorna o melhor caminho parcial encontrado
}

std::vector<int> GrafoLista::tsp_reativo(int max_iteracoes) {
    int N = 3;
    const double taxa_limite_inferior = 0.05;
    const double taxa_limite_superior = 0.20;
    std::vector<double> custos_historicos;
    std::vector<int> melhor_caminho;
    double menor_custo = std::numeric_limits<double>::max();

    for (int it = 0; it < max_iteracoes; ++it) {
        // Passo 1: Executa o randomizado com N atual (1 iteração por ciclo)
        std::vector<int> caminho = this->tsp_randomizado_controlado(1, N);
        
        // Passo 2: Calcula custo e atualiza histórico
        double custo = this->calcular_custo(caminho);
        custos_historicos.push_back(custo);
        
        if (custo < menor_custo) {
            menor_custo = custo;
            melhor_caminho = caminho;
        }

        // Passo 3: Ajuste dinâmico de N (a cada 10 iterações)
        if (it % 10 == 0 && it >= 10) {
            // Calcula média do período anterior (5 iterações entre -10 e -5)
            double custo_medio_anterior = std::accumulate(
                custos_historicos.begin() + (it - 10),
                custos_historicos.begin() + (it - 5), 0.0) / 5;

            // Calcula média do período atual (5 últimas iterações)
            double custo_medio_atual = std::accumulate(
                custos_historicos.begin() + (it - 5),
                custos_historicos.begin() + it, 0.0) / 5;

            // Calcula taxa de melhoria relativa
            double taxa = (custo_medio_anterior - custo_medio_atual) / custo_medio_anterior;

            // Ajusta N conforme a taxa
            if (taxa < taxa_limite_inferior) {
                N = std::min(5, N + 1);  // Aumenta diversificação
            } else if (taxa > taxa_limite_superior) {
                N = std::max(2, N - 1);  // Aumenta intensificação
            }
        }
    }

    return melhor_caminho;
}

// Adicione backtracking e fallbacks
std::vector<int> GrafoLista::tsp_guloso_densidade() {
    std::vector<int> melhor_caminho;
    double menor_custo = std::numeric_limits<double>::max();

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
                    if (!visitado[aresta.first]) {
                        opcoes.push_back(aresta.first);
                    }
                }
                if (!opcoes.empty()) {
                    proxima = opcoes[rand() % opcoes.size()];
                }
            }

            // Fallback 2: Backtracking
            if (proxima == -1 && backtrack_limit > 0) {
                int steps_back = std::min(2, (int)caminho.size() - 1);
                for (int s = 0; s < steps_back; ++s) {
                    int last = caminho.back();
                    caminho.pop_back();
                    visitado[last] = false;
                }
                atual = caminho.back();
                i -= steps_back;
                backtrack_limit--;
                continue;
            }

            if (proxima == -1) {
                bloqueado = true;
                break;
            }

            visitado[proxima] = true;
            caminho.push_back(proxima);
            atual = proxima;
        }

        if (!bloqueado) {
            bool ciclo_valido = false;
            for (const auto& aresta : lista_adj[caminho.back()]) {
                if (aresta.first == caminho[0]) {
                    ciclo_valido = true;
                    break;
                }
            }
            if (ciclo_valido) {
                double custo = calcular_custo(caminho);
                if (custo < menor_custo) {
                    menor_custo = custo;
                    melhor_caminho = caminho;
                }
            }
        }
    }
    return melhor_caminho.empty() ? std::vector<int>() : melhor_caminho;
}


double GrafoLista::calcular_custo(const std::vector<int>& caminho) {
    if (caminho.empty()) {
        std::cerr << "[ERRO] Caminho vazio!" << std::endl;
        return std::numeric_limits<double>::infinity();
    }
    
    if (caminho.size() == 1) {
        std::cout << "Caminho tem apenas 1 vértice." << std::endl;
        return 0; // Custo zero para um único vértice
    }

    double custo_total = 0;
    
    // Somando o custo de todas as arestas do caminho
    for (size_t i = 0; i < caminho.size() - 1; i++) {
        int u = caminho[i];
        int v = caminho[i + 1];
        
        bool aresta_encontrada = false;
        for (const auto& aresta : lista_adj[u]) {
            if (aresta.first == v) {
                custo_total += aresta.second;
                aresta_encontrada = true;
                break;
            }
        }
        
        if (!aresta_encontrada) {
            std::cerr << "[ERRO] Aresta não encontrada entre " << u << " e " << v << std::endl;
            return std::numeric_limits<double>::infinity();
        }
    }
    
    // Verifica se existe aresta do último para o primeiro (fechando o ciclo)
    bool ciclo_fechado = false;
    for (const auto& aresta : lista_adj[caminho.back()]) {
        if (aresta.first == caminho[0]) {
            custo_total += aresta.second;
            ciclo_fechado = true;
            break;
        }
    }
    
    // Se não tiver ciclo, apenas relata mas não considera um erro fatal
    if (!ciclo_fechado) {
        std::cout << "Ciclo não fechado (sem aresta de " << caminho.back() << " para " << caminho[0] << ")" << std::endl;
    }
    
    return custo_total;
}

void GrafoLista::diagnosticar_grafo() {
    std::cout << "=== DIAGNÓSTICO DO GRAFO ===" << std::endl;
    std::cout << "Número de vértices: " << num_vertices << std::endl;
    
    int total_arestas = 0;
    for (int i = 0; i < num_vertices; i++) {
        total_arestas += lista_adj[i].size();
    }
    std::cout << "Total de arestas: " << total_arestas << std::endl;
    
    // Verificar conectividade
    int max_grau = 0, min_grau = num_vertices;
    for (int i = 0; i < num_vertices; i++) {
        int grau = lista_adj[i].size();
        max_grau = std::max(max_grau, grau);
        min_grau = std::min(min_grau, grau);
    }
    std::cout << "Grau mínimo: " << min_grau << ", Grau máximo: " << max_grau << std::endl;
    
    // Verificar ciclos hamiltonianos parciais aleatórios
    for (int teste = 0; teste < 5; teste++) {
        int inicio = rand() % num_vertices;
        std::vector<bool> visitado(num_vertices, false);
        std::vector<int> caminho;
        int atual = inicio;
        visitado[atual] = true;
        caminho.push_back(atual);
        
        bool bloqueado = false;
        for (int i = 1; i < num_vertices && !bloqueado; i++) {
            bool encontrou_proximo = false;
            for (const auto& aresta : lista_adj[atual]) {
                int proximo = aresta.first;
                if (!visitado[proximo]) {
                    visitado[proximo] = true;
                    caminho.push_back(proximo);
                    atual = proximo;
                    encontrou_proximo = true;
                    break;
                }
            }
            if (!encontrou_proximo) {
                bloqueado = true;
            }
        }
        
        bool ciclo_completo = false;
        if (!bloqueado) {
            for (const auto& aresta : lista_adj[caminho.back()]) {
                if (aresta.first == caminho[0]) {
                    ciclo_completo = true;
                    break;
                }
            }
        }
        
        std::cout << "Teste " << teste+1 << ": ";
        std::cout << "Caminho com " << caminho.size() << "/" << num_vertices << " vértices. ";
        std::cout << "Ciclo completo: " << (ciclo_completo ? "Sim" : "Não") << std::endl;
    }
    std::cout << "===========================" << std::endl;
}