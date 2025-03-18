#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

namespace Util {
    std::vector<std::string> ler_arquivo(const std::string& arquivo);
    
    bool verificar_biparticao(const std::vector<std::vector<int>>& grafo);
    void gerar_grafo_aleatorio(int vertices, int arestas, const std::string& arquivo_saida);
    
    // Assinatura do método para converter um arquivo TSPLIB para o formato esperado
    void converter_TSPLIB_para_formato_esperado(
        const std::string& arquivo_TSPLIB, 
        const std::string& arquivo_saida, 
        int k_vizinhos = 20  // Default apenas aqui
    );
}

#endif // UTIL_H
