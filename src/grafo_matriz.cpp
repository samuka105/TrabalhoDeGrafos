#include grafo_matriz.h
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

GrafoMatriz::GrafoMatriz(int vertices, bool eh_direcionado, bool ponderado_vertices, bool ponderado_arestas)
    : num_vertices(vertices), direcionado(eh_direcionado), peso_vertices(ponderado_vertices), peso_arestas(ponderado_arestas) {
    matriz.resize(vertices, std::vector<int>(vertices, 0));
}

bool GrafoMatriz::eh_bipartido() const {
    // Implementa BFS para verificar bipartição.
    std::vector<int> cor(num_vertices, -1);
    for (int i = 0; i < num_vertices; ++i) {
        if (cor[i] == -1) {
            std::queue<int> fila;
            fila.push(i);
            cor[i] = 0;
            while (!fila.empty()) {
                int atual = fila.front();
                fila.pop();
                for (int j = 0; j < num_vertices; ++j) {
                    if (matriz[atual][j] > 0) {
                        if (cor[j] == -1) {
                            cor[j] = 1 - cor[atual];
                            fila.push(j);
                        } else if (cor[j] == cor[atual]) {
                            return false; // Não é bipartido.
                        }
                    }
                }
            }
        }
    }
    return true;
}

void GrafoMatriz::carrega_grafo(const std::string& arquivo) {
    std::ifstream entrada(arquivo);
    if (!entrada) {
        throw std::runtime_error("Erro ao abrir o arquivo!");
    }
    entrada >> num_vertices >> direcionado >> peso_vertices >> peso_arestas;
    matriz.assign(num_vertices, std::vector<int>(num_vertices, 0));
    int origem, destino, peso;
    while (entrada >> origem >> destino >> peso) {
        matriz[origem - 1][destino - 1] = peso;
        if (!direcionado) {
            matriz[destino - 1][origem - 1] = peso;
        }
    }
}
