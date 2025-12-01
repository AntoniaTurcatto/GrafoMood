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
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_NOME 75
#define MAX_ERRO 100
#define MAX_PERSONAGEM 20
#define FLAG_TESTE "--test"
#define NA_ALCANCADO -1
#define PROF_PRIM_RECEPTOR 1
#define PESO_NORMAL 50
#define CANCELAR_OP_MENU 0

///Macro para validação de NOTNULL
#define ISNULL(xxxxp) (xxxxp != NULL) 

typedef struct{
    char nome[MAX_NOME];
    unsigned short int idade;    
}Personagem;

typedef struct PERS_NODO PersonagNodo;

typedef struct CONEC{
    int peso;
    PersonagNodo *personagem;
    struct CONEC *prox_conexao;//prox conexao do personagem de onde originou a primeira CONEC
}Conexao;

///Se houver apenas uma conexao: ult == prim
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
    bool visitado; //variavel auxiliar para dfs/bfs
}PersonagNodo;

/// Caso exista apenas um personagem: ult_nodo == prim
typedef struct{
    unsigned short int quant_personagens;
    unsigned int proximo_id;
    PersonagNodo *raiz;
    PersonagNodo *ult_nodo;
}RedeConexao;

/// pai = NULL -> buscado = raiz
typedef struct{
    PersonagNodo *buscado;
    PersonagNodo *pai;
    bool encontrado;
}PersonagBuscado;

typedef struct{
    Conexao *buscado;
    Conexao *pai;
    bool encontrado;
}ConexaoBusc;

typedef struct{
    bool erro;
    char detalhes_erro[MAX_ERRO];
}DetalhesTeste;

typedef enum {
    ACAO_ELOGIO,   
    ACAO_INSULTO
} TipoAcao;

typedef struct{
    PersonagNodo *emissor;
    PersonagNodo *receptor;
    TipoAcao acao;
    RedeConexao *pers_informados;
}DetalhesAcao;

RedeConexao cria_rede();

void limpa_rede_rd(RedeConexao *rd);

void desvisita_todos(RedeConexao *rd);

DescrConexoes cria_descr_conex();

/// Adiciona personagem ao grafo, manipulando seu id conforme argumento
bool adiciona_personagem_rd(RedeConexao *rd, PersonagNodo *pers, bool id_auto);

/// atualiza o vínculo entre personagens. Caso não exista o vínculo, cria.
/// retorno: false caso não exista um dos personagens
bool atualiza_conexao_rd(DescrConexoes *desc_conexoes, PersonagNodo *dest, unsigned int peso, bool atualiza);

Personagem cria_personagem();

PersonagNodo *cria_nodo_pers(Personagem p);

///função para ler input textual do stdin tratando suas vulnerabilidades inerentes.
bool ler_texto_stdin(char buffer[]);

///função para garantir que o buffer seja limpo após scanf
void safe_scanf(const char *fmt, void *var);

/// Função para buscar o pai de um personagem e se esse personagem existe na rede
PersonagBuscado busca_personag(unsigned int id, RedeConexao *rd);
/// Função para buscar o pai de uma conexão e se essa conexão existe na rede
ConexaoBusc busca_conex(unsigned int id, DescrConexoes p);

bool remove_conexao_rd(DescrConexoes *desc_conexoes, PersonagNodo *remov);
bool remove_personagem_rd(RedeConexao *rd, PersonagNodo *p);

/// função para exibir a rede completa
bool exibir_rede(RedeConexao *rd);

/// função para exibir BFS
bool exibir_bfs(RedeConexao *rd, unsigned int id_inicio);
/// função para exibir DFS
bool exibir_dfs(RedeConexao *rd, unsigned int id_inicio);

void dfs_rec(PersonagNodo *p);

int run();
int menu_ini();
void menu_acoes(RedeConexao *rd);
PersonagNodo *menu_selec_pers(RedeConexao *rd);
bool menu_atualiza_conex(RedeConexao *rd);
bool menu_remove_conex(RedeConexao *rd);

DetalhesTeste test();

DetalhesTeste testar_ligamento_grafo(RedeConexao *rd, DetalhesTeste dt);

/// Função de teste para conexão entre Conexões e se Personagem x está conectado a personagem y com peso z
DetalhesTeste testar_conex_geral(RedeConexao *rd, DetalhesTeste dt, DescrConexoes dc, int id_pers_conexao, int peso, bool deve_existir_conex);

/// Verifica se a conexão na lista de Conexões é íntegra, com elementos válidos
DetalhesTeste testar_integr_conex_personag(RedeConexao *rd, DetalhesTeste dt, DescrConexoes desc_orig);

/// Verifica se existe a conexão entre personagem x a personagem y, com peso z
DetalhesTeste testar_conex_entre_pers(DetalhesTeste dt, DescrConexoes dc, int id_pers_conexao, int peso, bool deve_existir_conex);

DetalhesTeste testar_remocao_intercal(DetalhesTeste dt, bool erro_prev, RedeConexao *rd);

void printa_suceso_erro(bool erro, bool erro_prev);

bool salvar_grafo_dot(RedeConexao *rd, const char *nomeArquivo);

bool carrega_grafo_rd(RedeConexao *rd, const char *nomeArquivo);

void realiza_acao(PersonagNodo *emissor, PersonagNodo *receptor, TipoAcao acao);

void propaga_acao(DetalhesAcao detalhes, PersonagNodo *emissor, int profundidade);

/// Verifica vinculo de dois personagens 
int diagnostica_vinculo_completo_rd(RedeConexao *rd, unsigned int id_origem, unsigned int id_destino);
void imprime_vinculo_rd(RedeConexao *rd, unsigned int idA, unsigned int idB);

int main(int argc, char **argv){
    DetalhesTeste dt;
    if (argc == 2){
        if(strcmp(argv[1], FLAG_TESTE) == 0){
            printf("===TESTE ATIVADO===\n\n");
            setvbuf(stdout, NULL, _IONBF, 0); // sem buffering no stdout para testes
            dt = test();
            if(dt.erro){
                printf(dt.detalhes_erro);
                return -1;
            }
            printf("\n\n===Sucesso ao testar!!===\n");
            return 0;
        }
    }
    return run();
}

int run(){
    RedeConexao rd = cria_rede();

    printf(" == GRAFOMOOD ==\n"
    "Escolha uma opção:\n");
    
    int opc_menu_ini = menu_ini();
    while(opc_menu_ini != CANCELAR_OP_MENU){
        switch(opc_menu_ini){
        case 1:
            adiciona_personagem_rd(&rd, cria_nodo_pers(cria_personagem()), true);
            break;
        case 2:
            if (remove_personagem_rd(&rd, menu_selec_pers(&rd)))
                printf("Personagem removido com sucesso\n");
            else    
                printf("Erro ao remover pesonagem\n");
            break;
        case 3:
            if(menu_atualiza_conex(&rd)){
                printf("Conexão atualizada ou criada com sucesso\n");
            } else {
                printf("Erro ao atualizar ou criar conexão\n");
            }
            break;
        case 4:
            if(menu_remove_conex(&rd)){
                printf("Conexão removida com sucesso\n");
            } else {
                printf("Erro ao remover conexão\n");
            }
            break;
        case 5:
            menu_ini();
            break;
        default:
            printf("Opção inválida\n");
        }

        opc_menu_ini = menu_ini();
    }
    return 0;
}

int menu_ini(){
    int opc;
    printf("1. Adicionar personagem\n"
           "2. Remover personagem\n"
           "3. Atualizar/criar conexão\n"
           "4. Remover conexão\n"
           "5. Realizar ações entre personagens\n"
           "0. Sair\n");
    safe_scanf("%d", &opc);
    return opc;
}

void menu_acoes(RedeConexao *rd){
    PersonagNodo *pnod, *pnod2;
    TipoAcao acao;
    int opc, opacao;
    
    //loop infinito, feito desta forma para evitar repetir reescrita do printf
    for(;;){
        printf("1. Realizar nova ação\n"
               "0. Voltar ao menu principal\n");
        safe_scanf("%d", &opc);

        if(opc == 0)
            break;

        if(opc != 1){
            printf("Opção inválida\n");
            continue;
        }    

        pnod = menu_selec_pers(rd);
        if(ISNULL(pnod))
            continue;
            
        pnod2 = menu_selec_pers(rd);
        if(ISNULL(pnod2))
            continue;    

        printf("\nAção a ser realizada:\n"
        "1. Elogio\n"
        "2. Insulto\n");
        safe_scanf("%d", &opacao);    
        if (opacao == 1){
            acao = ACAO_ELOGIO;
        } else if(opacao == 2){
            acao = ACAO_INSULTO;
        } else {
            printf("Opção inválida\n");
            continue;
        }

        realiza_acao(pnod, pnod2, acao);
    }

}

PersonagNodo *menu_selec_pers(RedeConexao *rd){
    int id;
    PersonagBuscado pb;

    printf("Informe o ID do personagem: ");
    safe_scanf("%d", &id);
    return busca_personag(id, rd).buscado;
}

bool menu_atualiza_conex(RedeConexao *rd){
    PersonagNodo *pnod, *pnod2;
    int peso;

    printf("Personagem de onde partirá a conexão (que irá conhecer o outro personagem):\n");
    pnod = menu_selec_pers(rd);
    if(pnod == NULL){
        printf("Personagem inexistente\n");
        return false;
    }
    printf("Personagem a ser conectado:\n");
    pnod2 = menu_selec_pers(rd);
    if(pnod2 == NULL){
        printf("Personagem inexistente\n");
        return false;
    }

    printf("Peso da conexão do primeiro personagem para com o primero: \n"
    "0   = ódio\n"
    "50  = neutro\n"
    "100 = amor\n");
    safe_scanf("%d", &peso);

    return atualiza_conexao_rd(&pnod->desc_conexoes, pnod2, peso, true);
}

bool menu_remove_conex(RedeConexao *rd){
    PersonagNodo *pnod, *pnod2;
    printf("Personagem de onde parte a conexão:\n");
    pnod = menu_selec_pers(rd);
    if(pnod == NULL){
        printf("Personagem inexistente\n");
        return false;
    }
    printf("Personagem a ser desconectado:\n");
    pnod2 = menu_selec_pers(rd);
    if(pnod2 == NULL){
        printf("Personagem inexistente\n");
        return false;
    }

    return remove_conexao_rd(&pnod->desc_conexoes, pnod2);
}

RedeConexao cria_rede(){
    RedeConexao rede;
    rede.quant_personagens = 0;
    rede.proximo_id = 0;
    rede.raiz = NULL;
    rede.ult_nodo = NULL;
    return rede;
}

DescrConexoes cria_descr_conex(){
    DescrConexoes dc;
    dc.prim = NULL;
    dc.ult  = NULL;
    dc.quant_conex = 0;
    return dc;
}

Personagem cria_personagem(){
    Personagem p;
    printf("Informe o nome do personagem [até %d caracteres]: ", MAX_NOME);
    if (!ler_texto_stdin(p.nome)){
        strcpy(p.nome, "Erro");
    }
    
    printf("Informe a idade do personagem: ");
    safe_scanf("%hu", &p.idade);
    
    return p;
}

PersonagNodo *cria_nodo_pers(Personagem p){
    PersonagNodo *p_novop;
    p_novop = (PersonagNodo*)malloc(sizeof(PersonagNodo)); 
    p_novop->id_personagem = -1; 
    p_novop->desc_conexoes.prim = NULL;
    p_novop->desc_conexoes.ult = NULL;
    p_novop->desc_conexoes.quant_conex = 0;
    p_novop->prox = NULL;
    p_novop->info = p;
    p_novop->visitado = false;
    return p_novop;
}

void limpa_rede_rd(RedeConexao *rd){
    while(remove_personagem_rd(rd,rd->raiz));
}

void desvisita_todos(RedeConexao *rd){
    PersonagNodo *pnod = rd->raiz;
    while(pnod != NULL){
        pnod->visitado = false;
        pnod = pnod->prox;
    }
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

void safe_scanf(const char *fmt, void *var){
    int c;
    scanf(fmt, var);

    while((c = getchar()) != '\n' && c != EOF);
}

bool adiciona_personagem_rd(RedeConexao *rd, PersonagNodo *pers, bool id_auto){
    PersonagBuscado pb;

    if(rd == NULL)
        return false;

    if(rd->quant_personagens == MAX_PERSONAGEM){
        printf("Máximo de personagens atingido!\n");
        return false;
    }
        
    if(id_auto){
        pers->id_personagem = rd->proximo_id++; //incrementa após uso
    }

    if(rd->quant_personagens == 0){
        rd->raiz = pers;
    } else{
        if(rd->quant_personagens == 1){
            rd->raiz->prox = pers;
        } else {
            rd->ult_nodo->prox = pers;
        }
    }
    rd->ult_nodo = pers;

    rd->quant_personagens++;
    return true;    
}

bool atualiza_conexao_rd(DescrConexoes *desc_conexoes, PersonagNodo *dest, unsigned int peso, bool atualiza){
    ConexaoBusc conexb;
      
    if(dest == NULL){
        return false;
    }

    conexb = busca_conex(dest->id_personagem, *desc_conexoes);
    if(!conexb.encontrado){
        conexb.buscado = (Conexao*)malloc(sizeof(Conexao));
        conexb.buscado->personagem    = dest;
        conexb.buscado->prox_conexao  = NULL;
        conexb.buscado->peso = PESO_NORMAL;

        if(desc_conexoes->quant_conex == 0){
            desc_conexoes->prim = desc_conexoes->ult = conexb.buscado;
        } else {
            if (desc_conexoes->quant_conex == 1){
                desc_conexoes->prim->prox_conexao = conexb.buscado;
            } else {
                desc_conexoes->ult->prox_conexao = conexb.buscado;
            }
            desc_conexoes->ult = conexb.buscado;
        }

        desc_conexoes->quant_conex++;
    }
    if(atualiza){
        conexb.buscado->peso = conexb.buscado->peso + peso >= 0 ? conexb.buscado->peso + peso : 0;
    }else{    
        conexb.buscado->peso = peso;    
    }
    if(conexb.buscado->peso > 100)
        conexb.buscado->peso = 100;
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
    if(!pb.encontrado){
        pb.buscado = pb.pai = NULL;
    }
    return pb;
}

ConexaoBusc busca_conex(unsigned int id, DescrConexoes p){
    ConexaoBusc cb;
    cb.buscado    = p.prim;
    cb.pai        = NULL;
    cb.encontrado = false;
    while(!cb.encontrado && cb.buscado != NULL){
        if(cb.buscado->personagem == NULL){
            exit(1); // estado indesejado e irreparável
        }

        if(cb.buscado->personagem->id_personagem == id){
            cb.encontrado = true;
        } else {
            cb.pai = cb.buscado;
            cb.buscado = cb.buscado->prox_conexao;
        }
    }
    return cb;
}

bool remove_conexao_rd(DescrConexoes *desc_conexoes, PersonagNodo *remov){
    if(remov == NULL || desc_conexoes == NULL){
        return false;    
    }else if(desc_conexoes->quant_conex == 0)
        return false;

    ConexaoBusc cb = busca_conex(remov->id_personagem, *desc_conexoes); 
    if(!cb.encontrado)
        return false;

    if(desc_conexoes->quant_conex == 1){ 
        desc_conexoes->prim = desc_conexoes->ult = NULL;
    }else if(cb.pai == NULL){// removendo primeiro item da lista
        desc_conexoes->prim = cb.buscado->prox_conexao;

        if(desc_conexoes->quant_conex == 2)
            desc_conexoes->ult = desc_conexoes->prim;
    }
    else{
        cb.pai->prox_conexao = cb.buscado->prox_conexao;

        if(cb.buscado == desc_conexoes->ult)
            desc_conexoes->ult = cb.pai;

        if(desc_conexoes->quant_conex == 2)
            desc_conexoes->prim = desc_conexoes->ult;    
    }

    free(cb.buscado);
    desc_conexoes->quant_conex--;
    return true;
}

bool remove_personagem_rd(RedeConexao *rd, PersonagNodo *p){
    PersonagNodo *aux;
    PersonagNodo *pai;
    if(p == NULL || rd == NULL)
        return false;

    if(rd->quant_personagens == 0)
        return false;    

    pai = busca_personag(p->id_personagem, rd).pai; 
    // remover conexões dos outros personagens APONTANDO para ele
    aux = rd->raiz;
    while(aux != NULL){
        if(aux->id_personagem != p->id_personagem){
            remove_conexao_rd(&aux->desc_conexoes, p);
        }
        aux = aux->prox;
    }

    if (rd->quant_personagens == 1) {
        rd->raiz = NULL;
        rd->ult_nodo = NULL;
    }
    else if(pai == NULL){ //pai NULL = pb.buscado é raiz
        rd->raiz = p->prox;

        if(rd->quant_personagens == 2)
            rd->ult_nodo = rd->raiz;
    }
    else{
        pai->prox = p->prox;

        if(p == rd->ult_nodo)
            rd->ult_nodo = pai;
    }

    // liberar as conexões dele
    Conexao *c = p->desc_conexoes.prim;
    Conexao *tmp;
    while(c != NULL){
        tmp = c->prox_conexao;
        free(c);
        c = tmp;
    }

    free(p);
    rd->quant_personagens--;    

    return true;
}

bool exibir_rede(RedeConexao *rd) {
    if (rd == NULL || rd->raiz == NULL)
        return false;

    printf("=== EXIBINDO REDE DE CONEXOES ===\n");

    PersonagNodo *p = rd->raiz;

    while (p != NULL) {
        printf("Personagem %d: %s (%hu anos)\n",
               p->id_personagem,
               p->info.nome,
               p->info.idade);

        if (p->desc_conexoes.quant_conex == 0) {
            printf("  -> Sem conexoes\n");
        } else {
            printf("  -> Conexoes: ");
            Conexao *c = p->desc_conexoes.prim;
            while (c != NULL) {
                printf("[ID %d, peso %d] ",
                       c->personagem->id_personagem,
                       c->peso);
                c = c->prox_conexao;
            }
            printf("\n\n");
        }

        p = p->prox;
    }

    return true;
}

bool exibir_bfs(RedeConexao *rd, unsigned int id_inicio) {
    if (rd == NULL || rd->raiz == NULL)
        return false;

    PersonagBuscado pb = busca_personag(id_inicio, rd);
    if (!pb.encontrado)
        return false;

    PersonagNodo *fila[MAX_PERSONAGEM];
    int ini = 0, fim = 0;

    fila[fim++] = pb.buscado;//incrementa fim após 
    pb.buscado->visitado = true;

    printf("=== BFS a partir de %u ===\n", id_inicio);

    while (ini < fim) {
        PersonagNodo *atual = fila[ini++];

        printf("Visitando ID %d\n", atual->id_personagem);

        Conexao *c = atual->desc_conexoes.prim;

        while (c != NULL) {
            if (!c->personagem->visitado) {
                fila[fim++] = c->personagem;
                c->personagem->visitado = true;
            }

            c = c->prox_conexao;
        }
    }
    desvisita_todos(rd);
    return true;
}

void dfs_rec(PersonagNodo *p) {
    p->visitado = true;
    printf("Visitando ID %d\n", p->id_personagem);

    Conexao *c = p->desc_conexoes.prim;

    while (c != NULL) {
        if (!c->personagem->visitado) {
            dfs_rec(c->personagem);
        }
        c = c->prox_conexao;
    }
}

bool exibir_dfs(RedeConexao *rd, unsigned int id_inicio) {
    if (rd == NULL || rd->raiz == NULL)
        return false;

    PersonagBuscado pb = busca_personag(id_inicio, rd);
    if (!pb.encontrado)
        return false;

    printf("=== DFS a partir de %u ===\n", id_inicio);

    dfs_rec(pb.buscado);

    desvisita_todos(rd);
    return true;
}

int diagnostica_vinculo_completo_rd(RedeConexao *rd, unsigned int id_origem, unsigned int id_destino) {
    if (rd == NULL || rd->raiz == NULL)
        return NA_ALCANCADO;

    if (id_origem == id_destino)
        return 0;

    PersonagBuscado pb_origem = busca_personag(id_origem, rd);
    if (!pb_origem.encontrado)
        return NA_ALCANCADO;

    int distancia[MAX_PERSONAGEM];
    for (int i = 0; i < MAX_PERSONAGEM; i++)
        distancia[i] = NA_ALCANCADO;

    PersonagNodo *fila[MAX_PERSONAGEM];
    int ini = 0, fim = 0;

    fila[fim++] = pb_origem.buscado;
    distancia[pb_origem.buscado->id_personagem] = 0;

    while (ini < fim) {
        PersonagNodo *atual = fila[ini++];
        int dist_atual = distancia[atual->id_personagem];

        Conexao *c = atual->desc_conexoes.prim;
        while (c != NULL) {
            unsigned int id_vizinho = c->personagem->id_personagem;

            if (id_vizinho == id_destino) {
                return dist_atual + 1;
            }

            if (distancia[id_vizinho] == NA_ALCANCADO) {
                if (id_vizinho < MAX_PERSONAGEM) {
                    distancia[id_vizinho] = dist_atual + 1;
                    fila[fim++] = c->personagem;
                }
            }
            c = c->prox_conexao;
        }
    }

    return NA_ALCANCADO;
}

void imprime_vinculo_rd(RedeConexao *rd, unsigned int idA, unsigned int idB) {
    int grau = diagnostica_vinculo_completo_rd(rd, idA, idB);

    if (grau == NA_ALCANCADO) {
        printf("Personagem %d e Personagem %d não possuem vínculo.\n", idA, idB);
    } else if (grau == 0) {
        printf("Personagem %d e Personagem %d são a mesma pessoa.\n", idA, idB);
    } else if (grau == 1) {
        printf("Personagem %d e Personagem %d possuem vínculo DIRETO (grau 1).\n", idA, idB);
    } else {
        printf("Personagem %d e Personagem %d possuem vínculo INDIRETO (grau %d).\n", idA, idB, grau);
    }
}


// AÇÕES ................................................
void realiza_acao(PersonagNodo *emissor, PersonagNodo *receptor, TipoAcao acao){
    if(receptor == NULL)
        return;

    RedeConexao pers_informados = cria_rede();
    DetalhesAcao detalhes;
    detalhes.acao = acao;
    detalhes.emissor = emissor;
    detalhes.receptor = receptor;
    detalhes.pers_informados = &pers_informados;

    int peso;
    if(acao == ACAO_ELOGIO){
        peso = 10;
    } else {
        peso = -10;
    }
    
    atualiza_conexao_rd(&(receptor->desc_conexoes), emissor, peso, true); 

    adiciona_personagem_rd(&pers_informados, cria_nodo_pers(emissor->info), false); 
    adiciona_personagem_rd(&pers_informados, cria_nodo_pers(receptor->info), false); 
    propaga_acao(detalhes, receptor, PROF_PRIM_RECEPTOR);

    limpa_rede_rd(&pers_informados);
}

void propaga_acao(DetalhesAcao detalhes, PersonagNodo *emissor, int profundidade){
    if(detalhes.emissor == NULL)
        return; 

    Conexao *caux = emissor->desc_conexoes.prim;
    while(caux != NULL){
        if(caux->personagem == detalhes.emissor || caux->personagem == detalhes.receptor){
            goto PROXIMO;
        }

        if(caux->peso >= 50){
            float chance_distancia = 1.0f / profundidade;
            float chance_peso = (float)caux->peso / 100.0f; // normalizar de 100 para 1.0
            float chance_tot  = chance_distancia * chance_peso;
            float r = rand() / (float)RAND_MAX; // numero entre 0.0 e 1.0
            if(r <= chance_tot){
                printf("%s[%d] contou sobre a ação de %s[%d] para %s[%d]...\n", 
                    emissor->info.nome, emissor->id_personagem,
                    detalhes.emissor->info.nome, detalhes.emissor->id_personagem,
                    caux->personagem->info.nome, caux->personagem->id_personagem);
                int peso_base = detalhes.acao == ACAO_ELOGIO ? 10 : -10;

                atualiza_conexao_rd(&caux->personagem->desc_conexoes, detalhes.emissor, peso_base / profundidade, true);

                if(!busca_personag(caux->personagem->id_personagem, detalhes.pers_informados).encontrado){
                    adiciona_personagem_rd(detalhes.pers_informados, cria_nodo_pers(caux->personagem->info), false); 
                    propaga_acao(detalhes, caux->personagem, profundidade+1);
                }
                
            }
        }
        PROXIMO:
        caux = caux->prox_conexao;
    }
}

DetalhesTeste test(){
    RedeConexao rd = cria_rede();
    Personagem per = cria_personagem();
    DetalhesTeste dt;
    bool erro_prev = false;
    PersonagNodo aux;

    dt.erro = false;

    //validando primeira inserção e input
    printf("TESTANDO PRIMEIRA INSERÇÃO...");
    adiciona_personagem_rd(&rd, cria_nodo_pers(per), true);
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    //validando demais inserção
    printf("TESTANDO SEGUNDA INSERÇÃO...");
    per.idade = 200;
    strncpy(per.nome, "teste\0", MAX_NOME);
    adiciona_personagem_rd(&rd, cria_nodo_pers(per), true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);        

    printf("TESTANDO TERCEIRA INSERÇÃO...");
    per.idade = 199;
    strncpy(per.nome, "teste3\0", MAX_NOME);
    adiciona_personagem_rd(&rd, cria_nodo_pers(per), true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO QUARTA INSERÇÃO...");
    per.idade = 198;
    strncpy(per.nome, "teste4\0", MAX_NOME);
    adiciona_personagem_rd(&rd, cria_nodo_pers(per), true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO QUINTA INSERÇÃO...");
    per.idade = 197;
    strncpy(per.nome, "teste5\0", MAX_NOME);
    adiciona_personagem_rd(&rd, cria_nodo_pers(per), true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    //inserção conexão......................

    printf("CONECTANDO PRIMEIRO E 3o PERSONAGEM...");
    atualiza_conexao_rd(&rd.raiz->desc_conexoes, rd.raiz->prox->prox, 200, false);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.raiz->desc_conexoes, rd.raiz->prox->prox->id_personagem, 20, true);  
    printa_suceso_erro(dt.erro, erro_prev);

    printf("CONECTANDO 3o E PRIMEIRO PERSONAGEM...");
    atualiza_conexao_rd(&rd.raiz->prox->prox->desc_conexoes, rd.raiz , 830, false);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.raiz->prox->prox->desc_conexoes, rd.raiz->id_personagem,33, true);  
    printa_suceso_erro(dt.erro, erro_prev);

    printf("CONECTANDO 3o E 2o PERSONAGEM...");
    atualiza_conexao_rd(&rd.raiz->prox->prox->desc_conexoes, rd.raiz->prox , 830, false);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.raiz->prox->prox->desc_conexoes, rd.raiz->prox->id_personagem,33, true);  
    printa_suceso_erro(dt.erro, erro_prev);

    printf("CONECTANDO 2o E 3o PERSONAGEM...");
    atualiza_conexao_rd(&rd.raiz->prox->desc_conexoes, rd.raiz->prox->prox , 830, false);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.raiz->prox->desc_conexoes, rd.raiz->prox->prox->id_personagem,33, true);  
    printa_suceso_erro(dt.erro, erro_prev);

    printf("CONECTANDO ULT E PRIMEIRO PERSONAGEM...");
    atualiza_conexao_rd(&rd.ult_nodo->desc_conexoes, rd.raiz, 31, true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.ult_nodo->desc_conexoes, rd.raiz->id_personagem,31, true);  
    printa_suceso_erro(dt.erro, erro_prev);


    //salvando ..............................

    printf("Salvando...");
    erro_prev = erro_prev || dt.erro;
    dt.erro = !salvar_grafo_dot(&rd, "teste");
    if(dt.erro)
        snprintf(dt.detalhes_erro, MAX_ERRO, "Erro ao salvar arquivo. Erro: %d", errno);

    printa_suceso_erro(dt.erro, erro_prev);


    //exibir e ações .....
    realiza_acao(rd.ult_nodo, rd.raiz, ACAO_INSULTO);
    exibir_rede(&rd);

    
    //testar caminhamento grafo..............

    exibir_bfs(&rd, rd.raiz->id_personagem);
    exibir_dfs(&rd, rd.raiz->id_personagem);


    //remoção conexao ........................

    printf("REMOVENDO CONEXAO ENTRE ULT E PRIM...");
    remove_conexao_rd(&rd.ult_nodo->desc_conexoes, rd.raiz);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.ult_nodo->desc_conexoes, rd.raiz->id_personagem,31, false);  
    printa_suceso_erro(dt.erro, erro_prev);

    //remoção personag........................

    printf("TESTANDO REMOÇÃO DE 1° ELEMENTO...");
    aux = *(rd.raiz); // para usar depois
    remove_personagem_rd(&rd, rd.raiz);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DE ULT ELEMENTO...");
    remove_personagem_rd(&rd, rd.ult_nodo);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DE 2° ELEMENTO...");
    remove_personagem_rd(&rd, rd.raiz->prox);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    exibir_bfs(&rd, rd.raiz->id_personagem);
    exibir_dfs(&rd, rd.raiz->id_personagem);

    printf("TESTANDO REMOÇÃO DA RAIZ...");
    remove_personagem_rd(&rd, rd.raiz);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DA RAIZ...");
    remove_personagem_rd(&rd, rd.raiz);
    erro_prev = erro_prev || dt.erro;
    if((rd.raiz != NULL || rd.ult_nodo != NULL) && !erro_prev){
        dt.erro = true;
        snprintf(dt.detalhes_erro, MAX_ERRO, "raiz ou ultnodo não NULL quando vazio");
    }

    if(remove_personagem_rd(&rd, &aux) && !erro_prev){
        dt.erro = true;
        snprintf(dt.detalhes_erro, MAX_ERRO, "Deixou remover sem nenhum elemento");
    }
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO INSERÇÃO E REMOÇÃO INTERCALADAS...");
    RedeConexao rd2 = cria_rede();
    dt = testar_remocao_intercal(dt, erro_prev, &rd2);
    return dt;
}

DetalhesTeste testar_ligamento_grafo(RedeConexao *rd, DetalhesTeste dt){
    PersonagNodo *p_aux;
    Conexao *c_aux;

    if(dt.erro)
        return dt;

    dt.erro = false;

    if(rd == NULL){
        strcpy(dt.detalhes_erro, "RedeConexao NULL");
        dt.erro = true;
        return dt;
    }
    else if(rd->raiz == NULL){
        strcpy(dt.detalhes_erro, "erro ligação elemento: raiz NULL");
        dt.erro = true;
        return dt;
    } 
    else if(rd->ult_nodo == NULL && rd->quant_personagens > 1){
        strcpy(dt.detalhes_erro, "Inserção de ligação errada: ult_nodo != NULL");
        dt.erro = true;
        return dt;
    }
    
    p_aux = rd->raiz;
    for(int i = 1; i <= rd->quant_personagens; i++){
        if(p_aux->prox == NULL && i != rd->quant_personagens){
            snprintf(dt.detalhes_erro, MAX_ERRO, "O próximo nodo do %d° nodo é NULL", i);
            dt.erro = true;
            break;
        }

        dt = testar_integr_conex_personag(rd, dt, p_aux->desc_conexoes);
        if(dt.erro)
            break;    
        if(i < rd->quant_personagens){
            p_aux = p_aux->prox;
        } else if(p_aux->prox != NULL) {
            snprintf(dt.detalhes_erro, MAX_ERRO, "O próximo personagem do ÚLTIMO não é NULL (quando deveria ser)");
            return dt;
        }
    }
    if(p_aux->id_personagem != rd->ult_nodo->id_personagem && !dt.erro){
        strcpy(dt.detalhes_erro, "Último personagem do descritor não foi atualizado");
        dt.erro = true;
    }

    dt.erro = false;
    return dt;
}

DetalhesTeste testar_conex_geral(RedeConexao *rd, DetalhesTeste dt, DescrConexoes dc, int id_pers_conexao, int peso, bool deve_existir_conex){
    if(dt.erro)
        return dt;

    dt = testar_integr_conex_personag(rd, dt, dc);    
    if (!dt.erro)
        dt = testar_conex_entre_pers(dt, dc, id_pers_conexao, peso, deve_existir_conex);
    
    return dt;    
}

DetalhesTeste testar_integr_conex_personag(RedeConexao *rd, DetalhesTeste dt, DescrConexoes desc_orig){
    Conexao *aux;

    if(dt.erro || desc_orig.quant_conex == 0)
        return dt;
   
    aux = desc_orig.prim;

    dt.erro = true;
    for(int i = 1; i <= desc_orig.quant_conex; i++){
        if(aux->prox_conexao == NULL && i != desc_orig.quant_conex){
            snprintf(dt.detalhes_erro, MAX_ERRO, "A próxima conexão da %d° conexão é NULL quando não deveria ser", i);
            return dt;
        }

        if(aux->personagem == NULL){
            snprintf(dt.detalhes_erro, MAX_ERRO, "O personagem da %d° conexão é NULL", i);
            return dt;
        } else{
            if(!busca_personag(aux->personagem->id_personagem, rd).encontrado){
                snprintf(dt.detalhes_erro, MAX_ERRO, "O personagem da %d° conexão é não existe dentre os vértices", i);
                return dt;
            }
        }

        if(i < desc_orig.quant_conex)
            aux = aux->prox_conexao;
        else if (aux->prox_conexao != NULL){
            snprintf(dt.detalhes_erro, MAX_ERRO, "A próxima conexão da ÚLTIMA conexão não é NULL (quando deveria ser)");
            return dt;
        }   
    }

    dt.erro = false;
    return dt;
}

DetalhesTeste testar_conex_entre_pers(DetalhesTeste dt, DescrConexoes dc, int id_pers_conexao, int peso, bool deve_existir_conex){
    Conexao *aux;

    if(dt.erro){
        return dt;
    }

    aux = dc.prim;
    dt.erro = true; 
    while(aux != NULL){
        if(aux->personagem->id_personagem == id_pers_conexao && deve_existir_conex){
            dt.erro = false;
            if(aux->peso != peso){
                snprintf(dt.detalhes_erro, MAX_ERRO, "O peso da conexão não foi atualizado, sendo %d quando deveria ser %d", aux->peso, peso);
            }
            break;
        } 
        dt.erro = true;  
        aux = aux->prox_conexao;
    }

    //só existir erro de não encontrar se deve encontrar
    dt.erro = dt.erro && deve_existir_conex;
    if(dt.erro){
        snprintf(dt.detalhes_erro, MAX_ERRO, "Personagem não encontrado nas conexões");
    }
    return dt;
}

DetalhesTeste testar_remocao_intercal(DetalhesTeste dt, bool erro_prev, RedeConexao *rd){
    Personagem per = cria_personagem();

    printf("TESTANDO PRIMEIRA INSERÇÃO (intercal)...");
    adiciona_personagem_rd(rd, cria_nodo_pers(per), true);
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    //validando demais inserção
    printf("TESTANDO SEGUNDA INSERÇÃO (intercal)...");
    per.idade = 200;
    strncpy(per.nome, "teste\0", MAX_NOME);
    adiciona_personagem_rd(rd, cria_nodo_pers(per), true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);  
    
    printf("TESTANDO REMOÇÃO DE 1° ELEMENTO (intercal)...");
    remove_personagem_rd(rd, rd->raiz);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO TERCEIRA INSERÇÃO (intercal)...");
    per.idade = 199;
    strncpy(per.nome, "teste3\0", MAX_NOME);
    adiciona_personagem_rd(rd, cria_nodo_pers(per), true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DE ULT ELEMENTO (intercal)...");
    remove_personagem_rd(rd, rd->ult_nodo);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DE 2° ELEMENTO (intercal)...");
    remove_personagem_rd(rd, rd->raiz->prox);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO QUARTA INSERÇÃO (intercal)...");
    per.idade = 198;
    strncpy(per.nome, "teste4\0", MAX_NOME);
    adiciona_personagem_rd(rd, cria_nodo_pers(per), true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO QUINTA INSERÇÃO (intercal)...");
    per.idade = 197;
    strncpy(per.nome, "teste5\0", MAX_NOME);
    adiciona_personagem_rd(rd, cria_nodo_pers(per), true);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DE ULT ELEMENTO (intercal)...");
    remove_personagem_rd(rd, rd->ult_nodo);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DE RAIZ (intercal)...");
    remove_personagem_rd(rd, rd->raiz->prox);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    limpa_rede_rd(rd);

    return dt;
}


void printa_suceso_erro(bool erro, bool erro_prev){
    if(erro_prev){
        printf("Erro prévio\n");
    }
    else if(erro){
        printf("Erro\n");
    } else {
        printf("Sucesso\n");
    }
}

bool salvar_grafo_dot(RedeConexao *rd, const char *nomeArquivo) {
    char caminho[512];

    // snprintf evita overflow
    snprintf(caminho, sizeof(caminho), "%s.dot", nomeArquivo);

    FILE *arq = fopen(caminho, "w");
    if (!arq) {
        perror("Erro ao abrir arquivo");
        return false;
    }
    if (!arq) return false;

    fprintf(arq, "digraph G {\n");
    
    PersonagNodo *p = rd->raiz;
    while (p != NULL) {
        // nó
        fprintf(arq, "    %d [label=\"%s (%hu)\"];\n",
                p->id_personagem,
                p->info.nome,
                p->info.idade);

        // arestas
        Conexao *c = p->desc_conexoes.prim;
        while (c != NULL) {
            const char *cor;

            // definir cor baseada no peso
            if (c->peso >= 7) {
                cor = "red";
            } else if (c->peso >= 4) {
                cor = "orange";
            } else {
                cor = "blue";
            }

            fprintf(arq, "    %d -> %d [label=\"%d\", color=%s];\n",
                    p->id_personagem,
                    c->personagem->id_personagem,
                    c->peso,
                    cor);

            c = c->prox_conexao;
        }

        p = p->prox;
    }

    fprintf(arq, "}\n");
    fclose(arq);
    return true;
}

bool carrega_grafo_rd(RedeConexao *rd, const char *nome_arquivo) {
    if (rd == NULL) return false;

    limpa_rede_rd(rd); // limpa o grafo atual

    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s.dot", nome_arquivo);

    FILE *arq = fopen(caminho, "r");
    if (!arq) {
        perror("Erro ao abrir arquivo DOT");
        return false;
    }

    char linha[512];
    unsigned int id_lido;
    char nome_lido[MAX_NOME];
    unsigned short idade_lida;

    // mapa simples ID -> nodo
    PersonagNodo *mapa[MAX_PERSONAGEM] = {NULL};

    //PASSAGEM PARA LER NÓS
    while (fgets(linha, sizeof(linha), arq)) {
   
        if (sscanf(linha, " %u [label=\"%[^(\"] (%hu)\"];", 
                   &id_lido, nome_lido, &idade_lida) == 3) 
        {
            Personagem p;
            // Copiar string 
            strncpy(p.nome, nome_lido, MAX_NOME - 1);
            p.nome[MAX_NOME - 1] = '\0';
            p.idade = idade_lida;

            PersonagNodo *novo = cria_nodo_pers(p);
            novo->id_personagem = id_lido;

            adiciona_personagem_rd(rd, novo, false);

            if (id_lido < MAX_PERSONAGEM)
                mapa[id_lido] = novo;
        }
    }


    // LER ARESTAS
    rewind(arq); // voltar ao início do arquivo

    unsigned int origem, destino;
    int peso;

    while (fgets(linha, sizeof(linha), arq)) {
       
        if (sscanf(linha, " %u -> %u [label=\"%d\"];", &origem, &destino, &peso) == 3) 
        {
            // Validação básica
            if (origem < MAX_PERSONAGEM &&
                destino < MAX_PERSONAGEM &&
                mapa[origem] != NULL &&
                mapa[destino] != NULL &&
                peso >= 0)
            {
                atualiza_conexao_rd(
                    &(mapa[origem]->desc_conexoes),
                    mapa[destino],
                    peso,
                    false
                );
            }
        }
    }

    fclose(arq);

    // Ajustar proximo_id
    int maior_id = -1;
    for (PersonagNodo *aux = rd->raiz; aux != NULL; aux = aux->prox) {
        if (aux->id_personagem > maior_id)
            maior_id = aux->id_personagem;
    }
    rd->proximo_id = maior_id + 1;

    return true;
}
