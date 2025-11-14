# 🌐 GrafoMood

**GrafoMood** é um sistema baseado em grafos desenvolvido como trabalho
da disciplina de Estrutura de Dados e Programação (EDP).\
O objetivo do projeto é permitir a criação de personagens, o
gerenciamento das conexões entre eles e a execução de ações capazes de
modificar os vínculos individualizados.

## 👥 Integrantes

-   **AntoniaTurcatto**\
-   **Tássia J. M. Machado**\
-   **Kauany da Costa**\
-   **Helen Muniz**

## 🎯 Objetivo do Projeto

Criar uma ferramenta que represente personagens como vértices e suas
relações como arestas, permitindo visualizar, alterar e analisar esses
vínculos por meio de operações em um grafo orientado e valorado.

## ⚙️Funcionalidades

###  Criar e gerenciar personagens

-   Adicionar novos personagens ao grafo\
-   Excluir personagens\
-   Listar todos os vértices existentes

###  Conexões entre personagens

-   Criar uma relação orientada entre dois personagens\
-   Remover uma conexão existente\
-   Atualizar o peso (força do vínculo)

###  Ações modificadoras de vínculo

-   Aplicar uma ação realizada por um personagem que afeta seus
    relacionamentos\
-   A modificação pode alterar pesos das arestas relacionadas
  
###   Salvar grafo em arquivo `.dot`
Representação visual do grafo usando Graphviz.\
As cores das conexões ilustram o tipo e intensidade dos vínculos.

###   Carregar grafo de arquivo
Permite restaurar um grafo previamente salvo.
    
###  Verificar conexão entre dois personagens
Permite verificar se dois personagens estão conectados direto ou indiretamente.


## 🛠️ Escolhas Técnicas

-   **Estrutura do grafo:**
    -   Grafo orientado (as relações têm direção)\
    -   Grafo valorado (arestas possuem peso representando intensidade
        do vínculo)
-   **Representação adotada:**
    -   Lista de Adjacência
-   **Percursos utilizados (a confirmar):**
    -   BFS / DFS

##  Formato de Saída `.dot`

O sistema exporta o grafo no formato compatível com Graphviz, tornando
possível gerar visualizações como:

    dot -Tpng grafo.dot -o grafo.png

##  Observações Finais

Este projeto aplica conceitos fundamentais de Estrutura de Dados com
foco em grafos, permitindo explorar técnicas de representação, percursos
e manipulação dinâmica de estruturas conectadas.
