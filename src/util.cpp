#include "util.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

namespace Util {

std::vector<std::string> ler_arquivo(const std::string& arquivo) {
    std::ifstream entrada(arquivo);
    if (!entrada) {
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }
    std::vector<std::string> linhas;
    std::string linha;
    while (std::getline(entrada, linha)) {
        linhas.push_back(linha);
    }
    return linhas;
}

bool verificar_biparticao(const std::vector<std::vector<int>>& grafo) {
    int n = grafo.size();
    std::vector<int> cor(n, -1);

    for (int i = 0; i < n; ++i) {
        if (cor[i] == -1) {
            std::queue<int> fila;
            fila.push(i);
            cor[i] = 0;
            while (!fila.empty()) {
                int atual = fila.front();
                fila.pop();
                for (int vizinho = 0; vizinho < n; ++vizinho) {
                    if (grafo[atual][vizinho] > 0) {
                        if (cor[vizinho] == -1) {
                            cor[vizinho] = 1 - cor[atual];
                            fila.push(vizinho);
                        } else if (cor[vizinho] == cor[atual]) {
                            return false; // Não é bipartido.
                        }
                    }
                }
            }
        }
    }
    return true;
}

void gerar_grafo_aleatorio(int vertices, int arestas, const std::string& arquivo_saida) {
    std::ofstream saida(arquivo_saida);
    if (!saida) {
        throw std::runtime_error("Erro ao criar o arquivo de saída!");
    }

    srand(time(0)); // Inicializa o gerador de números aleatórios.
    saida << vertices << " " << arestas << std::endl;

    for (int i = 0; i < arestas; ++i) {
        int origem = rand() % vertices + 1;
        int destino = rand() % vertices + 1;
        while (destino == origem) { // Evitar laços
            destino = rand() % vertices + 1;
        }
        int peso = rand() % 10 + 1; // Pesos aleatórios entre 1 e 10.
        saida << origem << " " << destino << " " << peso << std::endl;
    }
}
}
