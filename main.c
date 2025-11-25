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

#define MAX_NOME 75
#define MAX_ERRO 100
#define MAX_PERSONAGEM 20
#define FLAG_TESTE "--test"

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

RedeConexao cria_rede();

bool adiciona_personagem_rd(RedeConexao *rd, Personagem pers);

/// atualiza o vínculo entre personagens. Caso não exista o vínculo, cria.
/// retorno: false caso não exista um dos personagens
bool atualiza_conexao_rd(PersonagNodo *orig,  PersonagNodo *dest, unsigned int peso, RedeConexao *rd);

Personagem cria_personagem();

///função para ler input textual do stdin tratando suas vulnerabilidades inerentes.
bool ler_texto_stdin(char buffer[]);

/// Função para buscar o pai de um personagem e se esse personagem existe na rede
PersonagBuscado busca_personag(unsigned int id, RedeConexao *rd);
/// Função para buscar o pai de uma conexão e se essa conexão existe na rede
ConexaoBusc busca_conex(unsigned int id, DescrConexoes p);

bool remove_conexao_rd(PersonagNodo *orig, ConexaoBusc cb, RedeConexao *rd);
bool remove_personagem_rd(RedeConexao *rd, PersonagBuscado prem);

/// função para exibir a rede completa
bool exibir_rede(RedeConexao *rd);

/// função para exibir BFS
bool exibir_bfs(RedeConexao *rd, unsigned int id_inicio);
/// função para exibir DFS
bool exibir_dfs(RedeConexao *rd, unsigned int id_inicio);

void dfs_rec(PersonagNodo *p, bool visitado[]);

int run();
DetalhesTeste test();

DetalhesTeste testar_ligamento_grafo(RedeConexao *rd, DetalhesTeste dt);

/// Função de teste para conexão entre Conexões e se Personagem x está conectado a personagem y com peso z
DetalhesTeste testar_conex_geral(RedeConexao *rd, DetalhesTeste dt, DescrConexoes dc, int id_pers_conexao, int peso, bool deve_existir_conex);

/// Verifica se a conexão na lista de Conexões é íntegra, com elementos válidos
DetalhesTeste testar_integr_conex_personag(RedeConexao *rd, DetalhesTeste dt, DescrConexoes desc_orig);

/// Verifica se existe a conexão entre personagem x a personagem y, com peso z
DetalhesTeste testar_conex_entre_pers(DetalhesTeste dt, DescrConexoes dc, int id_pers_conexao, int peso, bool deve_existir_conex);

void printa_suceso_erro(bool erro, bool erro_prev);

int main(int argc, char **argv){
    DetalhesTeste dt;
    if (argc == 2){
        if(strcmp(argv[1], FLAG_TESTE) == 0){
            printf("===TESTE ATIVADO===\n\n");
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
    return 0;
}

RedeConexao cria_rede(){
    RedeConexao rede;
    rede.quant_personagens = 0;
    rede.proximo_id = 0;
    rede.raiz = NULL;
    rede.ult_nodo = NULL;
    return rede;
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
    p_novop->desc_conexoes.ult = NULL;
    p_novop->desc_conexoes.quant_conex = 0;
    p_novop->prox = NULL;
    p_novop->info = pers;
    

    if(rd->quant_personagens == 0){
        rd->raiz = rd->ult_nodo =  p_novop;
    } else{
        if(rd->quant_personagens == 1){
            rd->raiz->prox = p_novop;
        } else {
            rd->ult_nodo->prox = p_novop;
        }
        rd->ult_nodo = p_novop;
    }

    rd->quant_personagens++;
    return true;    
}

bool atualiza_conexao_rd(PersonagNodo *orig, PersonagNodo *dest, unsigned int peso, RedeConexao *rd){
    ConexaoBusc conexb;
      
    if(orig == NULL|| dest == NULL){
        return false;
    }

    //todo: fazer busca_personagS
    if(!busca_personag(orig->id_personagem, rd).encontrado || !busca_personag(dest->id_personagem, rd).encontrado) {
        return false;
    }

    conexb = busca_conex(dest->id_personagem, orig->desc_conexoes);

    if(!conexb.encontrado){
        conexb.buscado = (Conexao*)malloc(sizeof(Conexao));
        conexb.buscado->personagem    = dest;
        conexb.buscado->prox_conexao  = NULL;

        if(orig->desc_conexoes.quant_conex == 0){
            orig->desc_conexoes.prim = orig->desc_conexoes.ult = conexb.buscado;
        } else {
            if (orig->desc_conexoes.quant_conex == 1){
                orig->desc_conexoes.prim->prox_conexao = orig->desc_conexoes.ult;
            } else {
                orig->desc_conexoes.ult->prox_conexao = conexb.buscado;
            }
            orig->desc_conexoes.ult = conexb.buscado;
        }

        orig->desc_conexoes.quant_conex++;
    }
    conexb.buscado->peso = peso;
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

bool remove_conexao_rd(PersonagNodo *orig, ConexaoBusc cb, RedeConexao *rd){

    if(orig == NULL || rd == NULL || !cb.encontrado)
        return false;    

    if(orig->desc_conexoes.quant_conex == 1){   
        orig->desc_conexoes.prim = orig->desc_conexoes.ult = NULL;
    }else if(cb.pai == NULL){// removendo primeiro item da lista
        orig->desc_conexoes.prim = cb.buscado->prox_conexao;

        if(orig->desc_conexoes.quant_conex == 2)
            orig->desc_conexoes.ult = orig->desc_conexoes.prim;
    }
    else{
        cb.pai->prox_conexao = cb.buscado->prox_conexao;

        if(cb.buscado == orig->desc_conexoes.ult)
            orig->desc_conexoes.ult = cb.pai;

        if(orig->desc_conexoes.quant_conex == 2)
            orig->desc_conexoes.prim = orig->desc_conexoes.ult;    
    }

    free(cb.buscado);
    orig->desc_conexoes.quant_conex--;
    return true;
}

bool remove_personagem_rd(RedeConexao *rd, PersonagBuscado pb){
    PersonagNodo *aux;

    if(!pb.encontrado)
        return false;

    // remover conexões dos outros personagens APONTANDO para ele
    aux = rd->raiz;
    while(aux != NULL){
        if(aux->id_personagem != pb.buscado->id_personagem){
            remove_conexao_rd(aux, busca_conex(pb.buscado->id_personagem, aux->desc_conexoes), rd);
        }
        aux = aux->prox;
    }

    if (rd->quant_personagens == 1) {
        rd->raiz = NULL;
        rd->ult_nodo = NULL;
    }
    else if(pb.pai == NULL){ //pai NULL = pb.buscado é raiz
        rd->raiz = pb.buscado->prox;

        if(rd->quant_personagens == 2)
            rd->ult_nodo = rd->raiz;
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
            printf("\n");
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

    bool visitado[MAX_PERSONAGEM] = {false};
    PersonagNodo *fila[MAX_PERSONAGEM];
    int ini = 0, fim = 0;

    fila[fim++] = pb.buscado;
    visitado[pb.buscado->id_personagem] = true;

    printf("=== BFS a partir de %u ===\n", id_inicio);

    while (ini < fim) {
        PersonagNodo *atual = fila[ini++];

        printf("Visitando ID %d\n", atual->id_personagem);

        Conexao *c = atual->desc_conexoes.prim;

        while (c != NULL) {
            unsigned int id_dest = c->personagem->id_personagem;
            if (!visitado[id_dest]) {
                fila[fim++] = c->personagem;
                visitado[id_dest] = true;
            }

            c = c->prox_conexao;
        }
    }

    return true;
}

void dfs_rec(PersonagNodo *p, bool visitado[]) {
    visitado[p->id_personagem] = true;
    printf("Visitando ID %d\n", p->id_personagem);

    Conexao *c = p->desc_conexoes.prim;

    while (c != NULL) {
        unsigned int id_dest = c->personagem->id_personagem;
        if (!visitado[id_dest]) {
            dfs_rec(c->personagem, visitado);
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

    bool visitado[MAX_PERSONAGEM] = {false};

    printf("=== DFS a partir de %u ===\n", id_inicio);

    dfs_rec(pb.buscado, visitado);

    return true;
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
    adiciona_personagem_rd(&rd, per);
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    //validando demais inserção
    printf("TESTANDO SEGUNDA INSERÇÃO...");
    per.idade = 200;
    strncpy(per.nome, "teste\0", MAX_NOME);
    adiciona_personagem_rd(&rd, per);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);        

    printf("TESTANDO TERCEIRA INSERÇÃO...");
    per.idade = 199;
    strncpy(per.nome, "teste3\0", MAX_NOME);
    adiciona_personagem_rd(&rd, per);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO QUARTA INSERÇÃO...");
    per.idade = 198;
    strncpy(per.nome, "teste4\0", MAX_NOME);
    adiciona_personagem_rd(&rd, per);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO QUINTA INSERÇÃO...");
    per.idade = 197;
    strncpy(per.nome, "teste5\0", MAX_NOME);
    adiciona_personagem_rd(&rd, per);
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    //inserção conexão......................

    printf("CONECTANDO PRIMEIRO E 3o PERSONAGEM...");
    atualiza_conexao_rd(rd.raiz, rd.raiz->prox->prox, 20, &rd);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.raiz->desc_conexoes, rd.raiz->prox->prox->id_personagem, 20, true);  
    printa_suceso_erro(dt.erro, erro_prev);

    printf("CONECTANDO 3o E PRIMEIRO PERSONAGEM...");
    atualiza_conexao_rd(rd.raiz->prox->prox, rd.raiz , 33, &rd);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.raiz->prox->prox->desc_conexoes, rd.raiz->id_personagem,33, true);  
    printa_suceso_erro(dt.erro, erro_prev);

    printf("CONECTANDO ULT E PRIMEIRO PERSONAGEM...");
    atualiza_conexao_rd(rd.ult_nodo, rd.raiz, 31, &rd);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.ult_nodo->desc_conexoes, rd.raiz->id_personagem,31, true);  
    printa_suceso_erro(dt.erro, erro_prev);

    //remoção conexao ........................

    printf("REMOVENDO CONEXAO ENTRE ULT E PRIM...");
    remove_conexao_rd(rd.ult_nodo, busca_conex(rd.raiz->id_personagem, rd.ult_nodo->desc_conexoes), &rd);
    erro_prev = erro_prev || dt.erro;
    dt = testar_conex_geral(&rd, dt, rd.ult_nodo->desc_conexoes, rd.raiz->id_personagem,31, false);  
    printa_suceso_erro(dt.erro, erro_prev);

    //remoção personag........................

    printf("TESTANDO REMOÇÃO DE 1° ELEMENTO...");
    aux = *(rd.raiz); // para usar depois
    remove_personagem_rd(&rd, busca_personag(rd.raiz->id_personagem, &rd));
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    // teste adicionar conexão inválida
    if(atualiza_conexao_rd(rd.raiz, &aux, 2, &rd) && !erro_prev){
        dt.erro = true;
        snprintf(dt.detalhes_erro, MAX_ERRO, "Realizou conexão com elemento não existente na lista de vertices");
    }

    printf("TESTANDO REMOÇÃO DE ULT ELEMENTO...");
    remove_personagem_rd(&rd, busca_personag(rd.ult_nodo->id_personagem, &rd));
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DE 2° ELEMENTO...");
    remove_personagem_rd(&rd, busca_personag(rd.raiz->prox->id_personagem, &rd));
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DA RAIZ...");
    remove_personagem_rd(&rd, busca_personag(rd.raiz->id_personagem, &rd));
    erro_prev = erro_prev || dt.erro;
    dt = testar_ligamento_grafo(&rd, dt);
    printa_suceso_erro(dt.erro, erro_prev);

    printf("TESTANDO REMOÇÃO DA RAIZ...");
    remove_personagem_rd(&rd, busca_personag(rd.raiz->id_personagem, &rd));
    erro_prev = erro_prev || dt.erro;
    if((rd.raiz != NULL || rd.ult_nodo != NULL) && !erro_prev){
        dt.erro = true;
        snprintf(dt.detalhes_erro, MAX_ERRO, "raiz ou ultnodo não NULL quando vazio");
    }

    if(remove_personagem_rd(&rd, busca_personag(1, &rd)) && !erro_prev){
        dt.erro = true;
        snprintf(dt.detalhes_erro, MAX_ERRO, "Deixou remover sem nenhum elemento");
    }
    printa_suceso_erro(dt.erro, erro_prev);

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