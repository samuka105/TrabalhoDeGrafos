
# Projeto de Grafos em C++

## Descrição

Propor e implementar algoritmos para o problema NP-Completo escolhido ou designado.

## Funcionalidades

### Parte 3
- Guloso
- Randomizado
- Reativo


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
1. Diretório principal do projeto
2. Compile o projeto usando o seguinte comando:
   ```

   g++ -o main.exe src/main.cpp src/grafo.cpp src/grafo_lista.cpp src/grafo_matriz.cpp src/util.cpp -Iinclude/ -Wall -Werror

   (ou main.out), caso esteja no Linux

   g++ -o testes.exe testes.cpp src/grafo.cpp src/grafo_lista.cpp src/grafo_matriz.cpp src/util.cpp -Iinclude -Wall -Werror -std=c++17

   (ou testes.out), caso esteja no Linux
   ```
3. Execute o programa:
   ```
      ./testes.exe

      Isso rodará os testes relacionados aos algoritmos Guloso, Randomizado e Reativo
   ```



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


## Equipe de Desenvolvimento
- Samuel Paiva Bernardes


---