# TrabalhoDeGrafos
 
# Projeto: Implementação de uma Classe Abstrata de Grafo em C++

Este projeto implementa uma classe abstrata chamada **`grafo`** em C++, com duas classes derivadas para diferentes representações de grafos:

- **`grafo_matriz`**: Representa o grafo usando uma matriz de adjacência.
- **`grafo_lista`**: Representa o grafo usando listas encadeadas para vértices e arestas.

## Estruturas de Armazenamento

### `grafo_matriz`
- Representação estática, usando matriz de adjacência.
- Útil para grafos densos (muitos vértices conectados entre si).
- Quando o grafo é não direcionado, utiliza a matriz triangular para economizar espaço.

### `grafo_lista`
- Usa alocação dinâmica com listas encadeadas para representar vértices e arestas.
- Ideal para grafos esparsos (poucos vértices conectados).
- Mais eficiente em termos de memória para representações onde há poucas conexões.

---

## Funcionalidades Implementadas

A classe abstrata **`grafo`** define as seguintes funções essenciais, que as classes derivadas devem implementar:

### Análise Estrutural do Grafo
- **`eh_bipartido`**  
  Verifica se o grafo é bipartido (ou seja, pode ser dividido em dois conjuntos de vértices sem arestas internas a cada conjunto).  
  _Método: Força bruta._

- **`n_conexo`**  
  Retorna a quantidade de componentes conexas no grafo.

- **`get_grau`**  
  Retorna o grau de um vértice (quantidade de arestas conectadas a ele).

- **`get_ordem`**  
  Retorna a ordem do grafo (número total de vértices).

- **`eh_direcionado`**  
  Retorna se o grafo é direcionado ou não.

---

### Propriedades Avançadas
- **`vertice_ponderado`**  
  Indica se os vértices possuem peso associado.

- **`aresta_ponderada`**  
  Indica se as arestas possuem peso associado.

- **`eh_completo`**  
  Determina se o grafo é completo (todos os vértices estão conectados a todos os outros).

- **`eh_arvore`**  
  Indica se o grafo é uma árvore (estrutura sem ciclos e conexa).

---

### Conectividade
- **`possui_articulacao`**  
  Verifica se existe pelo menos um vértice de articulação (remover esse vértice desconecta o grafo).

- **`possui_ponte`**  
  Verifica se existe pelo menos uma aresta ponte (remover essa aresta desconecta o grafo).

---

### Entrada e Saída de Dados
- **`carrega_grafo`**  
  Lê um grafo de um arquivo `.txt` e o carrega na estrutura.

- **`novo_grafo`**  
  Gera um grafo aleatório a partir de um arquivo de configuração `.txt`.

---

## Como Usar o Projeto

1. **Configuração Inicial**  
   Compile os arquivos de implementação com um compilador C++ compatível (ex.: g++, clang++).
   ```bash
   g++ main.cpp grafo_matriz.cpp grafo_lista.cpp -o grafo
