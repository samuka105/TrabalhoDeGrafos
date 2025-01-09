#ifndef UTIL_H
#define UTIL_H
using namespace std;

#include <string>
#include <vector>

// Funções utilitárias para manipulação de grafos.

namespace Util {
    // Lê um arquivo `.txt` e retorna os dados do grafo.
    vector<string> ler_arquivo(const string& arquivo);

    // Verifica se um grafo é bipartido (usado em força bruta).
    bool verificar_biparticao(const vector<vector<int>>& grafo);

    // Gera um grafo aleatório com base em parâmetros.
    void gerar_grafo_aleatorio(int vertices, int arestas, const string& arquivo_saida);
}

#endif // UTIL_H

