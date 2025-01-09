#include <iostream>
#include <string>
#include "grafo_lista.h"
#include "grafo_matriz.h"
#include "util.h"

using namespace std;
using namespace Util; // Usa as funções do namespace Util diretamente.

void testar_grafo(Grafo& grafo) {
    cout << "Ordem do grafo: " << grafo.get_ordem() << endl;
    cout << "É direcionado: " << (grafo.eh_direcionado() ? "Sim" : "Não") << endl;
    cout << "É bipartido: " << (grafo.eh_bipartido() ? "Sim" : "Não") << endl;
    cout << "Número de componentes conexas: " << grafo.n_conexo() << endl;
    cout << "É completo: " << (grafo.eh_completo() ? "Sim" : "Não") << endl;
    cout << "É uma árvore: " << (grafo.eh_arvore() ? "Sim" : "Não") << endl;
    cout << "Possui vértices de articulação: " << (grafo.possui_articulacao() ? "Sim" : "Não") << endl;
    cout << "Possui arestas ponte: " << (grafo.possui_ponte() ? "Sim" : "Não") << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " [-d|-c] <arquivo_config> <arquivo_grafo>" << endl;
        return 1;
    }

    string modo = argv[1];
    string arquivo_config = argv[2];
    string arquivo_grafo = argc > 3 ? argv[3] : "";

    try {
        if (modo == "-d") {
            cout << "=== Testando GrafoLista ===" << endl;
            GrafoLista grafoLista(0, false, false, false);
            
            // Usa a função Util::ler_arquivo para carregar o grafo.
            vector<string> linhas = ler_arquivo(arquivo_grafo);
            for (const string& linha : linhas) {
                cout << "Linha lida: " << linha << endl; // Apenas para validação
            }
            grafoLista.carrega_grafo(arquivo_grafo);
            testar_grafo(grafoLista);

            cout << "\n=== Testando GrafoMatriz ===" << endl;
            GrafoMatriz grafoMatriz(0, false, false, false);
            grafoMatriz.carrega_grafo(arquivo_grafo);
            testar_grafo(grafoMatriz);

        } else if (modo == "-c") {
            cout << "=== Criando grafo aleatório ===" << endl;
            // Gera um grafo aleatório e salva em um arquivo usando Util::gerar_grafo_aleatorio.
            gerar_grafo_aleatorio(5, 10, arquivo_config); // Gera um grafo com 5 vértices e 10 arestas.
            cout << "Grafo aleatório gerado e salvo em: " << arquivo_config << endl;

            // Carrega o grafo gerado para testar as funcionalidades.
            GrafoLista grafoLista(0, false, false, false);
            grafoLista.carrega_grafo(arquivo_config);
            testar_grafo(grafoLista);
        } else {
            cerr << "Modo inválido! Use -d para carregar grafo ou -c para criar grafo aleatório." << endl;
            return 1;
        }
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}
