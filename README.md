<p align="center">
  <img src="https://github.com/AntoniaTurcatto/GrafoMood/blob/13387b2dfed32bbd57e5f06fbe7dd95a256c7ef3/docs/images/GrafoMood.jpg" width="350" alt="Grafo">
</p>

**GrafoMood** é um sistema baseado em grafos desenvolvido como trabalho
da disciplina de Estrutura de Dados e Programação (EDP).\
O objetivo do projeto é permitir a criação de personagens, o
gerenciamento das conexões entre eles e a execução de ações capazes de
modificar os vínculos individualizados.

## 👥 Integrantes

-   [AntoniaTurcatto](https://github.com/AntoniaTurcatto)
-   [Tássia J. M. Machado](https://github.com/machadotassia1-max)
-   [Kauany da Costa](https://github.com/kauanycosta)
-   [Helen Muniz](https://github.com/helenzinha661)

## 🎯 Objetivo do Projeto

Criar uma ferramenta que represente personagens e suas
relações, permitindo visualizar, alterar e analisar esses
vínculos por meio de operações em um grafo orientado e valorado. 
Também possibilitar realizar ações em rede que afetam os vínculos e causam outas ações em cascata.

## ⚙️Funcionalidades

### 🆕 Criar e gerenciar personagens
-   Adicionar novos personagens 
-   Excluir personagens
-   Listar todos os vértices existentes

### 🧑‍🤝‍🧑 Conexões entre personagens
-   Criar uma relação orientada entre dois personagens
-   Remover uma conexão existente
-   Atualizar o peso do vínculo, simbolizando a relação da origem para com o destino

### ↔️ Ações modificadoras de vínculo
-   Aplicar uma ação realizada por um personagem que afeta seus
    relacionamentos
-   A modificação pode alterar pesos das arestas relacionadas
-   Peso base da ação _X_  peso do vínculo com o emissor
-   Após cada ação, os vínculos não interagidos a 5 turnos tem seu peso decaído gradualmente
  
### 📡Propagação de ações em cascata
-   Inicia desativado, ativa-se no menu do programa 
-   Esse sistema pode ser desativado entre as ações
-   Uma ação pode gerar outras ações
-   Após cada ação, os vínculos não interagidos a 5 turnos tem seu peso decaído gradualmente.
-   A probabilidade de geração de uma nova ação decai conforme aumenta a distância do emissor em relação à ação original.
  
### ⬇️ Salvar grafo em arquivo `.dot`
Representação visual do estado atual do grafo usando Graphviz.
As cores das conexões ilustram o tipo e intensidade dos vínculos a partir de um personagem.

### 📁 Carregar grafo de arquivo
Permite restaurar um grafo previamente salvo em um arquivo.
    
### 🔎 Verificar conexão entre dois personagens
Permite verificar se dois personagens estão conectados direto ou indiretamente.

## 🛠️ Escolhas Técnicas
-   **Estrutura do grafo:**
    -   Grafo orientado (as relações têm direção)
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
