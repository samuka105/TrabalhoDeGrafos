# Projeto de Grafos em C++

## Descrição
Este projeto implementa grafos utilizando **lista de adjacência** e **matriz de adjacência**, com foco na exploração de propriedades e algoritmos fundamentais em teoria dos grafos.

O projeto foi desenvolvido seguindo uma abordagem **orientada a objetos**, onde:
- A classe base `Grafo` define a interface e métodos comuns.
- As classes derivadas `GrafoLista` e `GrafoMatriz` implementam as estruturas específicas.

## Funcionalidades
- Leitura de grafos a partir de arquivos.
- Geração de grafos aleatórios.
- Verificação de propriedades como:
  - Bipartição.
  - Conexidade.
  - Grau dos vértices.
  - Identificação de pontes e articulações.

## Estrutura do Projeto
build/ # Diretório para os arquivos de compilação. docs/ # Documentação do projeto. src/ # Código-fonte principal. ├── grafo_lista.cpp ├── grafo_lista.h ├── grafo_matriz.cpp ├── grafo_matriz.h ├── grafo.cpp ├── grafo.h ├── main.cpp ├── util.cpp ├── util.h README.md # Este arquivo.


## Requisitos
- **CMake**
- **Compilador C++** (MinGW ou similar)

## Como Compilar e Executar
1. Clone este repositório.
2. Navegue até o diretório raiz do projeto.
3. Execute os seguintes comandos:

```bash
mkdir build
cd build
cmake ..
make


Observações
Configuração dos Grafos: O arquivo de configuração do grafo deve ser especificado no código principal (main.cpp). Certifique-se de que ele esteja no formato esperado.
Exemplos de arquivos de entrada podem ser encontrados na documentação (docs/).
Status Atual
Atualmente, o projeto ainda apresenta problemas de funcionamento. Estou trabalhando ativamente para corrigir os erros e melhorar a estabilidade da aplicação.

Agradeço pela compreensão! 🚀