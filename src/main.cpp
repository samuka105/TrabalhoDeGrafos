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

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Uso: " << argv[0] << " -d [-m|-l] <arquivo_grafo>" << std::endl;
        return 1;
    }

    std::string modo = argv[1];
    std::string estrutura = argv[2];
    std::string arquivo_grafo = argv[3];

    if (modo != "-d" || (estrutura != "-m" && estrutura != "-l")) {
        std::cerr << "Argumentos inválidos!" << std::endl;
        return 1;
    }

    try {
        if (estrutura == "-m") {
            GrafoMatriz grafo(0, false, false, false);
            grafo.carrega_grafo(arquivo_grafo);
            testar_grafo(grafo);

            // Testando novos métodos da Parte 2
            grafo.novo_no();
            grafo.nova_aresta(1, 2, 5);
            std::cout << "Menor distância entre 1 e 2: " << grafo.menor_distancia(1, 2) << std::endl;
        } else if (estrutura == "-l") {
            GrafoLista grafo(0, false, false, false);
            grafo.carrega_grafo(arquivo_grafo);
            testar_grafo(grafo);

            // Testando novos métodos da Parte 2
            grafo.novo_no();
            grafo.nova_aresta(1, 2, 5);
            std::cout << "Menor distância entre 1 e 2: " << grafo.menor_distancia(1, 2) << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}