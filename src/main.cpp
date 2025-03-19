#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>  // Necessário para atexit()
#include "grafo_matriz.h"
#include "grafo_lista.h"
#include "util.h"
// Inclua este cabeçalho para Windows
#ifdef _WIN32
#include <windows.h>
#endif

// Função que imprime a orientação de compilação e execução
void imprimir_orientacao() {
    std::cout << "\nPara criar as instâncias use:" << std::endl;
    std::cout << "g++ -o testes.out testes.cpp src/grafo.cpp src/grafo_lista.cpp src/grafo_matriz.cpp src/util.cpp -Iinclude -Wall -Werror -std=c++17" << std::endl;
    std::cout << "e execute com ./testes.out" << std::endl;
}

// Função para imprimir as propriedades do grafo
void testar_grafo(Grafo& grafo) {
    std::cout << "Ordem do grafo: " << grafo.get_ordem() << std::endl;
    std::cout << "É direcionado: " << (grafo.eh_direcionado() ? "Sim" : "Não") << std::endl;
    std::cout << "É bipartido: " << (grafo.eh_bipartido() ? "Sim" : "Não") << std::endl;
    std::cout << "Número de componentes conexas: " << grafo.n_conexo() << std::endl;
    std::cout << "É completo: " << (grafo.eh_completo() ? "Sim" : "Não") << std::endl;
    std::cout << "É uma árvore: " << (grafo.eh_arvore() ? "Sim" : "Não") << std::endl;
    std::cout << "Possui vértices de articulação: " << (grafo.possui_articulacao() ? "Sim" : "Não") << std::endl;
    std::cout << "Possui arestas ponte: " << (grafo.possui_ponte() ? "Sim" : "Não") << std::endl;
}

// Função para calcular a "Maior menor distância" entre quaisquer dois nós
void imprimir_maior_menor_distancia(Grafo& grafo) {
    double max_distance = 0;
    int id_u = -1, id_v = -1;
    for (int u = 0; u < grafo.get_ordem(); u++) {
        for (int v = 0; v < grafo.get_ordem(); v++) {
            double dist = grafo.menor_distancia(u, v);
            if (dist < std::numeric_limits<double>::infinity() && dist > max_distance) {
                max_distance = dist;
                id_u = u;
                id_v = v;
            }
        }
    }
    if (id_u == -1 || id_v == -1) {
        std::cout << "Maior menor distância: Nenhum caminho encontrado." << std::endl;
    } else {
        std::cout << "Maior menor distância: (" << id_u + 1 << "-" << id_v + 1 << ") " << max_distance << std::endl;
    }
}

int main(int argc, char* argv[])  {
    // Registra a função para imprimir a orientação ao final, independente do fluxo
    atexit(imprimir_orientacao);

    // Configura a codificação para UTF-8 (Windows)
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // Opção de gerar grafo aleatório
    if (argc > 1 && std::string(argv[1]) == "-g") {
        if (argc < 5) {
            std::cerr << "Uso: " << argv[0] << " -g <num_vertices> <num_arestas> <arquivo_saida>" << std::endl;
            return 1;
        }
        try {
            int vertices = std::stoi(argv[2]);
            int arestas = std::stoi(argv[3]);
            std::string arquivo_saida = argv[4];
            Util::gerar_grafo_aleatorio(vertices, arestas, arquivo_saida);
            std::cout << "Grafo aleatório gerado em " << arquivo_saida << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro: " << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    // Novo modo: Converter TSP para formato do programa
    if (argc > 1 && std::string(argv[1]) == "-converter_tsp") {
        if (argc != 4) {
            std::cerr << "Uso: " << argv[0] << " -converter_tsp <arquivo_tsp> <arquivo_saida>\n";
            return 1;
        }
        std::string arquivo_tsp = argv[2];
        std::string arquivo_saida = argv[3];
        Util::converter_TSPLIB_para_formato_esperado(arquivo_tsp, arquivo_saida);
        std::cout << "Arquivo TSP convertido: " << arquivo_saida << "\n";
        return 0;
    }
    
    // Fluxo tradicional: uso de -d (descrição) ou -p (NP-completos) com -m (matriz) ou -l (lista)
    if (argc < 4) {
        std::cerr << "Uso: " << argv[0] << " [-d|-p] [-m|-l] <arquivo_grafo>" << std::endl;
        return 1;
    }

    std::string modo = argv[1];      // -d para descrição, -p para NP (parte 3)
    std::string estrutura = argv[2];   // -m para matriz, -l para lista
    std::string arquivo_grafo = argv[3];

    try {
        if (estrutura == "-m") {
            GrafoMatriz grafo(0, false, false, false);
            grafo.carrega_grafo(arquivo_grafo);
            std::cout << "Excluindo nó 1..." << std::endl;
            grafo.deleta_no(0); // remove nó com id 1 (0-based)
            std::cout << "Excluindo primeira aresta do nó 2..." << std::endl;
            bool removido = false;
            // Para a matriz, procura a primeira aresta existente no nó 2 (índice 1)
            for (int j = 0; j < grafo.get_ordem(); j++) {
                try {
                    if (grafo.menor_distancia(1, j) < std::numeric_limits<double>::infinity()) {
                        grafo.deleta_aresta(1, j);
                        removido = true;
                        break;
                    }
                } catch (...) { }
            }
            if (!removido)
                std::cout << "Nenhuma aresta encontrada no nó 2 para excluir." << std::endl;

            testar_grafo(grafo);
            imprimir_maior_menor_distancia(grafo);
        } else if (estrutura == "-l") {
            GrafoLista grafo(0, false, false, false);
            grafo.carrega_grafo(arquivo_grafo);
            std::cout << "Excluindo nó 1..." << std::endl;
            grafo.deleta_no(0); // remove nó com id 1 (0-based)
            std::cout << "Excluindo primeira aresta do nó 2..." << std::endl;
            bool removido = false;
            // Tenta remover a primeira aresta de nó 2 (índice 1) para qualquer destino
            for (int target = 0; target < grafo.get_ordem(); target++) {
                try {
                    grafo.deleta_aresta(1, target);
                    removido = true;
                    break;
                } catch (...) { }
            }
            if (!removido)
                std::cout << "Nenhuma aresta encontrada no nó 2 para excluir." << std::endl;

            testar_grafo(grafo);
            imprimir_maior_menor_distancia(grafo);
        } else {
            std::cerr << "Estrutura inválida! Use -m ou -l." << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

