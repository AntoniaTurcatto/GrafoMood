<p align="center">
  <img src="https://github.com/AntoniaTurcatto/GrafoMood/blob/450a85d86638c732d70c14449fe34a1e1662574d/docs/images/grafomood.jpg" width="350" alt="Grafo">
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
Também possibilitar realizar ações em rede que afetam os vínculos e causam outras ações em cascata.

## 📝 Documentação
[Página principal da documentação](/docs/DOCS.md)

## ⚙️ Como compilar
Usando `gcc`

    gcc maic.c -o grafomood  

## 🎏 Flags opcionais
- `--test`  
  Executa a suíte de testes da aplicação e exibe o resultado (sucesso ou falha). 
  Em caso de erro conhecido, também mostra uma descrição detalhada do problema.
  
##  Formato de Saída `.dot`
O sistema exporta o grafo no formato compatível com Graphviz, tornando
possível gerar visualizações como:

    dot -Tpng grafo.dot -o grafo.png
