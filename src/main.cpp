#include <iostream>
#include <string>
#include "grafo_matriz.h"
#include "grafo_lista.h"
#include "util.h"


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

/void executar_algoritmos_np(Grafo& grafo) {
    // Exemplo para TSP (implemente seus métodos)
    auto sol_gulosa = grafo.solucao_gulosa();
    auto sol_rand = grafo.solucao_randomizada(0.5);
    auto sol_reativa = grafo.solucao_reativa(1000);
    // ... processar soluções ...
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Uso: " << argv[0] << " [-d|-p] [-m|-l] <arquivo_grafo>" << std::endl;
        return 1;
    }

    std::string modo = argv[1];
    std::string estrutura = argv[2];
    std::string arquivo_grafo = argv[3];

    try {
        if (estrutura == "-m") {
            GrafoMatriz grafo(0, false, false, false);
            grafo.carrega_grafo(arquivo_grafo);
            if (modo == "-d") testar_grafo(grafo);
            else if (modo == "-p") executar_algoritmos_np(grafo);
        } else if (estrutura == "-l") {
            GrafoLista grafo(0, false, false, false);
            grafo.carrega_grafo(arquivo_grafo);
            if (modo == "-d") testar_grafo(grafo);
            else if (modo == "-p") executar_algoritmos_np(grafo);
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