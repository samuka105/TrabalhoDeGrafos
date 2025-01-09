#include "util.h" // Inclui o cabeçalho que contém declarações de funções utilitárias.
#include <queue> // Inclui a biblioteca de fila para uso posterior.
#include <fstream> // Inclui a biblioteca para manipulação de arquivos.
#include <sstream> // Inclui a biblioteca para manipulação de strings via stream.
#include <stdexcept> // Inclui a biblioteca para manipulação de exceções.
#include <cstdlib> // Inclui a biblioteca para funções de geração de números aleatórios.
#include <ctime> // Inclui a biblioteca para manipulação do tempo, utilizada na semente de aleatoriedade.
using namespace std;

namespace Util { // Define um namespace para encapsular as funções utilitárias.

vector<string> ler_arquivo(const string& arquivo) {
    // Abre o arquivo passado como parâmetro para leitura.
    ifstream entrada(arquivo);
    // Verifica se o arquivo foi aberto corretamente.
    if (!entrada) {
        // Lança uma exceção caso o arquivo não possa ser aberto.
        throw runtime_error("Erro ao abrir o arquivo!");
    }
    // Cria um vetor para armazenar as linhas do arquivo.
    vector<string> linhas;
    string linha;
    // Lê cada linha do arquivo e a armazena no vetor.
    while (getline(entrada, linha)) {
        linhas.push_back(linha);
    }
    // Retorna o vetor contendo todas as linhas do arquivo.
    return linhas;
}

bool verificar_biparticao(const vector<vector<int>>& grafo) {
    // Determina o número de vértices no grafo.
    int n = grafo.size();
    // Cria um vetor para armazenar a cor de cada vértice (-1 indica não colorido).
    vector<int> cor(n, -1);

    // Itera sobre todos os vértices do grafo.
    for (int i = 0; i < n; ++i) {
        // Verifica se o vértice ainda não foi colorido.
        if (cor[i] == -1) {
            // Cria uma fila para auxiliar na BFS (Busca em Largura).
            queue<int> fila;
            // Adiciona o vértice atual à fila e o colore com 0.
            fila.push(i);
            cor[i] = 0;
            // Executa a BFS enquanto a fila não estiver vazia.
            while (!fila.empty()) {
                // Obtém o vértice atual da fila.
                int atual = fila.front();
                fila.pop();
                // Itera sobre todos os vértices adjacentes ao vértice atual.
                for (int vizinho = 0; vizinho < n; ++vizinho) {
                    // Verifica se há uma aresta entre o vértice atual e o vizinho.
                    if (grafo[atual][vizinho] > 0) {
                        // Se o vizinho ainda não estiver colorido, colore-o com a cor oposta do vértice atual.
                        if (cor[vizinho] == -1) {
                            cor[vizinho] = 1 - cor[atual];
                            fila.push(vizinho);
                        } else if (cor[vizinho] == cor[atual]) {
                            // Se o vizinho já estiver colorido com a mesma cor do vértice atual, o grafo não é bipartido.
                            return false;
                        }
                    }
                }
            }
        }
    }
    // Se todos os vértices puderem ser coloridos com duas cores sem conflitos, o grafo é bipartido.
    return true;
}

void gerar_grafo_aleatorio(int vertices, int arestas, const string& arquivo_saida) {
    // Abre o arquivo de saída para escrever o grafo gerado.
    ofstream saida(arquivo_saida);
    // Verifica se o arquivo de saída foi criado corretamente.
    if (!saida) {
        // Lança uma exceção caso o arquivo de saída não possa ser criado.
        throw runtime_error("Erro ao criar o arquivo de saída!");
    }

    // Inicializa o gerador de números aleatórios com o tempo atual como semente.
    srand(time(0));
    // Escreve o número de vértices e arestas no arquivo de saída.
    saida << vertices << " " << arestas << endl;

    // Gera arestas aleatórias para o grafo.
    for (int i = 0; i < arestas; ++i) {
        // Gera um vértice de origem aleatório.
        int origem = rand() % vertices + 1;
        // Gera um vértice de destino aleatório, garantindo que não seja igual ao de origem.
        int destino = rand() % vertices + 1;
        while (destino == origem) {
            destino = rand() % vertices + 1;
        }
        // Gera um peso aleatório entre 1 e 10 para a aresta.
        int peso = rand() % 10 + 1;
        // Escreve a aresta e seu peso no arquivo de saída.
        saida << origem << " " << destino << " " << peso << endl;
    }
}
}

