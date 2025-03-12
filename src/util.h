#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace Util {
    /// Lê todas as linhas de um arquivo e retorna como vetor de strings
    std::vector<std::string> ler_arquivo(const std::string& arquivo);
    
    /// Verifica se um grafo representado por matriz de adjacência é bipartido
    bool verificar_biparticao(const std::vector<std::vector<int>>& grafo);
    
    /// Gera um grafo aleatório e salva em arquivo
    void gerar_grafo_aleatorio(int vertices, int arestas, const std::string& arquivo_saida);
}

#endif // UTIL_H