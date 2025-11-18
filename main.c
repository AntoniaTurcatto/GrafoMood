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

///Se houver apenas uma conexao: ult == NULL
typedef struct{
    unsigned short int quant_conex;
    Conexao *prim;
    Conexao *ult;
}DescrConexoes;

typedef struct PERS_NODO{
    int id_personagem;
    Personagem info;
    struct PERS_NODO *prox;//proximo da lista de adjacencia
    DescrConexoes desc_conexoes;
}PersonagNodo;

/// Caso exista apenas um personagem: ult_nodo == NULL
typedef struct{
    unsigned short int quant_personagens;
    unsigned int proximo_id;
    PersonagNodo *raiz;
    PersonagNodo *ult_nodo;
}RedeConexao;

typedef struct{
    PersonagNodo *buscado;
    PersonagNodo *pai;
    bool encontrado;
}PersonagBuscado;

RedeConexao* cria_rede();

bool adiciona_personagem_rd(RedeConexao *rd, Personagem pers);

/// atualiza o vínculo entre personagens. Caso não exista o vínculo, cria.
/// retorno: false caso não exista um dos personagens
bool atualiza_vinculo(PersonagNodo *orig, unsigned int iddest, unsigned int peso, RedeConexao *rd);

Personagem cria_personagem();

///função para ler input textual do stdin tratando suas vulnerabilidades inerentes.
bool ler_texto_stdin(char buffer[]);

PersonagBuscado busca_personag(unsigned int id, RedeConexao *rd);

/// funções de remoção 
bool remove_conexao_rd(PersonagNodo *orig, unsigned int iddest, RedeConexao *rd, bool validar);
bool remove_personagem_rd(RedeConexao *rd, unsigned int idrem);

int main(){
    return 0;
}

RedeConexao* cria_rede(){
    RedeConexao rede;
    rede.quant_personagens = 0;
    rede.proximo_id = 0;
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



bool adiciona_personagem_rd(RedeConexao *rd, Personagem pers){
    PersonagNodo *p_novop;
    if(rd == NULL)
        return false;

    p_novop = (PersonagNodo*)malloc(sizeof(PersonagNodo)); 
    p_novop->id_personagem = rd->proximo_id++; //incrementa após uso
    p_novop->desc_conexoes.prim = NULL;
    p_novop->desc_conexoes.prim = NULL;
    p_novop->desc_conexoes.quant_conex = 0;
    p_novop->prox = NULL;
    p_novop->info = pers;
    
    switch (rd->quant_personagens){
        case 0:
            rd->raiz = p_novop;
            break;
        case 1:
            rd->ult_nodo   = p_novop;
            rd->raiz->prox = rd->ult_nodo;
            break; 
        default:
            rd->ult_nodo->prox = p_novop;
    }

    rd->quant_personagens++;
    return true;    
}

bool atualiza_vinculo(PersonagNodo *orig, unsigned int iddest, unsigned int peso, RedeConexao *rd){
    bool dest_presente = false;
    Conexao *connov;
      
    if(!busca_personag(orig->id_personagem, rd).encontrado || !busca_personag(iddest, rd).encontrado){
        return false;
    }

    connov = (Conexao*)malloc(sizeof(Conexao));
    connov->id_personagem = iddest;
    connov->prox_conexao  = NULL; 
    connov->peso          = peso;

    switch(orig->desc_conexoes.quant_conex){
        case 0:
            orig->desc_conexoes.prim = connov;
            break;
        case 1: 
            orig->desc_conexoes.ult = connov;
            orig->desc_conexoes.prim->prox_conexao = orig->desc_conexoes.ult;
            break;
        default:
            orig->desc_conexoes.ult->prox_conexao = connov;
    }

    orig->desc_conexoes.quant_conex++;
    return true;
}

PersonagBuscado busca_personag(unsigned int id, RedeConexao *rd){
    PersonagBuscado pb;

    pb.pai        = NULL;
    pb.buscado    = rd->raiz;
    pb.encontrado = false;

    while(!pb.encontrado && pb.buscado != NULL){
        if(pb.buscado->id_personagem == id){
            pb.encontrado = true;
        } else {
            pb.pai = pb.buscado;
            pb.buscado  = pb.buscado->prox;
        }
    }
    return pb;
}

bool remove_conexao_rd(PersonagNodo *orig, unsigned int iddest, RedeConexao *rd, bool validar){
    Conexao *ant = NULL, *atual;

    if(orig == NULL || rd == NULL)
        return false;

    if (validar){    
        if(!busca_personag(iddest, rd).encontrado)
            return false;
    }

    atual = orig->desc_conexoes.prim;
    while(atual != NULL){
        if(atual->id_personagem == iddest){

            // removendo primeiro item da lista
            if(ant == NULL){
                orig->desc_conexoes.prim = atual->prox_conexao;
            }
            else{
                ant->prox_conexao = atual->prox_conexao;

                if(atual == orig->desc_conexoes.ult && orig->desc_conexoes.quant_conex > 2)
                    orig->desc_conexoes.ult = ant;
            }

            if(orig->desc_conexoes.quant_conex == 2)
                orig->desc_conexoes.ult = NULL;

            free(atual);
            orig->desc_conexoes.quant_conex--;
            return true;
        }
        ant = atual;
        atual = atual->prox_conexao;
    }
    return false;
}

bool remove_personagem_rd(RedeConexao *rd, unsigned int idrem){
    PersonagNodo *aux;
    PersonagBuscado pb;

    //  remover o personagem da lista
    pb = busca_personag(idrem, rd);
    if(!pb.encontrado)
        return false;

    // remover conexões dos outros personagens APONTANDO para ele
    aux = rd->raiz;
    while(aux != NULL){
        remove_conexao_rd(aux, idrem, rd, false);
        aux = aux->prox;
    }

    //pai NULL = pb.buscado é raiz
    if(pb.pai == NULL){
        rd->raiz = pb.buscado->prox;

        if(rd->quant_personagens == 2)
            rd->ult_nodo = NULL;
    }
    else{
        pb.pai->prox = pb.buscado->prox;

        if(pb.buscado == rd->ult_nodo)
            rd->ult_nodo = pb.pai;
    }

    // liberar as conexões dele
    Conexao *c = pb.buscado->desc_conexoes.prim;
    Conexao *tmp;

    while(c != NULL){
        tmp = c->prox_conexao;
        free(c);
        c = tmp;
    }

    free(pb.buscado);
    rd->quant_personagens--;    

    return true;
}
