/*GRAFOMOOD
Integrantes: AntoniaTurcatto, Tássia J. M. Machado, Kauany da Costa, Helen Muniz.

Tema: Ligações entre Personagens
Grafo orientado (pesos nas relações individualizadas)
Tipo de representação usada: Lista de adjacência
Percurso: BFS/DFS [a ver]
Funcionalidades adicionais: 
    Salvar em arquivo .dot,
    Carregar grafo de arquivo, 
    Conexão entre dois vértices, 
    Ação modificadora de vínculo 
Observações:
*/
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <strings.h>
#include <stdbool.h>

#define MAX_NOME 75
#define MAX_PERSONAGEM 20

typedef struct{
    char nome[MAX_NOME];
    unsigned short int idade;    
}Personagem;

typedef struct CONEC{
    int peso;
    int id_personagem;//id para identificar PersonagNodo
    struct CONEC *prox_conexao;//prox conexao do personagem de onde originou a primeira CONEC
}Conexao;

typedef struct PERS_NODO{
    int id_personagem;
    Personagem info;
    struct PERS_NODO *prox;//proximo da lista de adjacencia
    Conexao *prim_conexao;
}PersonagNodo;

typedef struct{
    unsigned short int quant_personagens;
    PersonagNodo *raiz;
}RedeConexao;

RedeConexao* cria_rede();
bool adiciona_personagem_rd(RedeConexao *rd);
Personagem cria_personagem();

///função para ler input textual do stdin tratando suas vulnerabilidades inerentes.
bool ler_texto_stdin(char buffer[]);




int main(){
    return 0;
}

RedeConexao* cria_rede(){
    RedeConexao rede;
    rede.quant_personagens = 0;
    rede.raiz = NULL;
    return &rede;
}

Personagem cria_personagem(){
    Personagem p;
    printf("Informe o nome do personagem [até %d caracteres]: ", MAX_NOME);
    if (!ler_texto_stdin(p.nome)){
        strcpy(p.nome, "Erro");
    }
    
    printf("Informe a idade do personagem: ");
    scanf("%hu", &p.idade);
    
    return p;
}
 
bool ler_texto_stdin(char buffer[]){
    int c;
    size_t len; 
    if (fgets(buffer, MAX_NOME, stdin) == NULL) {
        buffer[0] = '\0';
        printf("\n");
        return false; // erro ou EOF
    }

    len = strlen(buffer);
    // se terminou com '\n', remove
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        // se não terminou com '\n', limpar o resto da linha do stdin
        while ((c = getchar()) != '\n' && c != EOF);
    }
    printf("\n");
    return true;
}



bool adiciona_personagem_rd(RedeConexao *rd){
    //todo: fazer
}
