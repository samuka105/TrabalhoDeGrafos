#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace Util {
    std::vector<std::string> ler_arquivo(const std::string& arquivo);
    bool verificar_biparticao(const std::vector<std::vector<int>>& grafo);
    void gerar_grafo_aleatorio(int vertices, int arestas, const std::string& arquivo_saida);
}

#endif // UTIL_H