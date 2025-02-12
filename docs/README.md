Aqui está o conteúdo reformatado para um arquivo `README.md`:

```markdown
# Projeto de Grafos em C++

## Descrição
Este projeto implementa grafos utilizando **lista de adjacência** e **matriz de adjacência**, com foco na exploração de propriedades e algoritmos fundamentais em teoria dos grafos. O projeto foi desenvolvido seguindo uma abordagem **orientada a objetos**, onde:
- A classe base `Grafo` define a interface e métodos comuns.
- As classes derivadas `GrafoLista` e `GrafoMatriz` implementam as estruturas específicas.

A **Parte 2** do trabalho expande as funcionalidades da **Parte 1**, incluindo métodos para adicionar/remover nós e arestas, além de calcular a menor distância entre dois vértices usando o algoritmo de Dijkstra.

---

## Funcionalidades
### Parte 1
- Leitura de grafos a partir de arquivos.
- Geração de grafos aleatórios.
- Verificação de propriedades como:
  - Bipartição.
  - Conexidade.
  - Grau dos vértices.
  - Identificação de pontes e articulações.

### Parte 2
- Adição e remoção de nós e arestas.
- Cálculo da menor distância entre dois vértices (algoritmo de Dijkstra).
- Manipulação dinâmica de grafos.

---

## Estrutura do Projeto

```
TrabalhoGrafosGrupoX/
├── include/
│   ├── grafo.h
│   ├── grafo_matriz.h
│   ├── grafo_lista.h
│   └── util.h
├── src/
│   ├── grafo.cpp
│   ├── grafo_matriz.cpp
│   ├── grafo_lista.cpp
│   ├── util.cpp
│   └── main.cpp
├── entradas/
│   └── grafo.txt
└── README.md
```

### Descrição dos Arquivos
- **`grafo.h`**: Define a classe abstrata `Grafo`, que contém métodos comuns a todos os tipos de grafos.
- **`grafo_matriz.h` e `grafo_matriz.cpp`**: Implementam a classe `GrafoMatriz`, que representa grafos usando matriz de adjacência.
- **`grafo_lista.h` e `grafo_lista.cpp`**: Implementam a classe `GrafoLista`, que representa grafos usando lista de adjacência.
- **`util.h` e `util.cpp`**: Contêm funções utilitárias, como leitura de arquivos e geração de grafos aleatórios.
- **`main.cpp`**: Contém a função principal que testa as funcionalidades implementadas.
- **`grafo.txt`**: Exemplo de arquivo de entrada para carregar um grafo.

---

## Requisitos
- **Compilador C++** (g++ ou similar).
- **CMake** (opcional, para compilação automatizada).

---

## Como Compilar e Executar

### Compilação Manual
1. Navegue até o diretório `src/`.
2. Compile o projeto usando o seguinte comando:
   ```bash
   g++ -o main main.cpp grafo.cpp grafo_matriz.cpp grafo_lista.cpp util.cpp
   ```
3. Execute o programa:
   ```bash
   ./main -d -m entradas/grafo.txt  # Usando matriz de adjacência
   ./main -d -l entradas/grafo.txt  # Usando lista de adjacência
   ```

### Compilação com CMake
1. Crie um diretório para a compilação:
   ```bash
   mkdir build
   cd build
   ```
2. Execute o CMake para configurar o projeto:
   ```bash
   cmake ..
   ```
3. Compile o projeto:
   ```bash
   make
   ```
4. Execute o programa:
   ```bash
   ./main -d -m entradas/grafo.txt  # Usando matriz de adjacência
   ./main -d -l entradas/grafo.txt  # Usando lista de adjacência
   ```

---

## Formato do Arquivo de Entrada
O arquivo de entrada (`grafo.txt`) deve seguir o seguinte formato:

```
3 1 1 1 // número de nós, direcionado, ponderado vértices, ponderado arestas
2 3 7   // peso dos nós (apenas se ponderado nos vértices)
1 2 6   // origem, destino, peso (peso apenas se ponderado na aresta)
2 1 4   // origem, destino, peso (peso apenas se ponderado na aresta)
2 3 -5  // origem, destino, peso (peso apenas se ponderado na aresta)
```

---

## Exemplos de Uso

### Adicionar Novo Nó
```cpp
GrafoMatriz grafo(0, false, false, false);
grafo.novo_no();
```

### Adicionar Nova Aresta
```cpp
grafo.nova_aresta(1, 2, 5); // Adiciona aresta entre 1 e 2 com peso 5
```

### Remover Nó
```cpp
grafo.deleta_no(2); // Remove o nó com ID 2
```

### Remover Aresta
```cpp
grafo.deleta_aresta(1, 2); // Remove a aresta entre 1 e 2
```

### Calcular Menor Distância
```cpp
double distancia = grafo.menor_distancia(1, 2);
std::cout << "Menor distância entre 1 e 2: " << distancia << std::endl;
```

---

## Status Atual
O projeto está em desenvolvimento ativo. Algumas funcionalidades podem ainda apresentar problemas, e estou trabalhando para corrigi-los e melhorar a estabilidade da aplicação.


---

## Equipe de Desenvolvimento
- Samuel Paiva Bernardes


---