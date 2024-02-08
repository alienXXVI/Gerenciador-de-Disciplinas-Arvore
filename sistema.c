#include <stdlib.h>
#include <string.h>
#include "sistema.h"


// ------------------------------- Arquivo -------------------------------


// Verifica se o cabeçalho (a árvore) é vazia
// Entrada: cabeçalho
// Saída: é vazia (1) ou não é vazia (0)
int vazia(Cabecalho *cab) {
    return cab->pos_raiz == -1;
}

// Cria uma árvore (cabeçalho) nova em arquivo, podendo ser de Curso, Disciplina,
// CadastroProfessor ou CadastroProfessorDisciplina
// Pré-condição: arquivo aberto para leitura/escrita
// Pós-condição: arquivo é inicializado com uma lista vazia
void criar_cabecalho_vazio(FILE* arq){
    Cabecalho *cab = (Cabecalho*) malloc(sizeof(Cabecalho));

    cab->pos_raiz = -1;
    cab->pos_topo = 0;
    cab->pos_livre = -1;
    escrever_cabecalho(arq, cab);
    free(cab);
}

// Lê o cabeçalho do arquivo contendo as informações da árvore
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// Pós-condição: retorna o ponteiro para o cabeçalho lido
Cabecalho* ler_cabecalho(FILE *arq) {
    Cabecalho *cab = (Cabecalho*) malloc(sizeof(Cabecalho));
    
    fseek(arq, 0, SEEK_SET); // posiciona no início do arquivo
    fread(cab, sizeof(Cabecalho), 1, arq);
    return cab;
}

// Escreve no arquivo o cabeçalho contendo as informações da árvore
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// Pós-condição: cabeçalho escrito no arquivo
void escrever_cabecalho(FILE *arq, Cabecalho *cab){
    fseek(arq, 0, SEEK_SET); // posiciona no início do arquivo
    fwrite(cab, sizeof(Cabecalho), 1, arq);
}

void imprimir_cabecalho(Cabecalho *cab) {
    printf("---- CABECALHO ----\n");
    printf("Cabeca: %d\n"
            "Topo: %d\n"
            "Livre: %d\n",
        cab->pos_raiz, cab->pos_topo, cab->pos_livre);
}

// Abre arquivo
// Pré-condição: string contendo o nome do arquivo.extensão
// Pós-condição: retorna arquivo aberto
FILE* open_arq(char* str){
    FILE* arq = fopen(str, "r+b");

    if(arq == NULL){
        arq = fopen(str, "w+b");
        Cabecalho* cab = (Cabecalho*) malloc(sizeof(Cabecalho));
        cab->pos_raiz = -1;
        cab->pos_livre = -1;
        cab->pos_topo = 0;
        escrever_cabecalho(arq, cab);
        free(cab);
    }
    return arq;
}


// --------------------------- Sistema ---------------------------


// Cria uma Disciplina contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente para o mesmo curso
// Pós-condição: retorna ponteiro para Disciplina
Disciplina* criar_disciplina(int codigo, char *nome, int codcurso, int serie) {
    Disciplina *d = (Disciplina*) malloc(sizeof(Disciplina));

    d->cod = codigo;
    strcpy(d->nome, nome);
    d->codcurso = codcurso;
    d->serie = serie;
    return d;
}

// Cria um Curso contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro curso,
//      area deve ser 'E' (Exatas), 'H' (Humanas) ou 'B' (Biológicas)
// Pós-condição: retorna ponteiro para Curso
Curso* criar_curso(int codigo, char *nome, char area) {
    Curso *c = (Curso*) malloc(sizeof(Curso));

    c->cod = codigo;
    strcpy(c->nome, nome);
    c->area = area;
    return c;
}

// Cria um Professor contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro professor
// Pós-condição: retorna ponteiro para Professor
Professor* criar_professor(int codigo, char *nome) {
    Professor *p = (Professor*) malloc(sizeof(Professor));

    p->cod = codigo;
    strcpy(p->nome, nome);
    return p;
}

// Cria uma Associacao contendo os dados fornecidos
// Pré-condição: os códigos fornecidos devem ser válidos
// Pós-condição: retorna ponteiro para Associacao
Associacao* criar_associacao(int coddisciplina, int anoletivo, int codprofessor) {
    Associacao *a = (Associacao*) malloc(sizeof(Associacao));
    char ano[4], disciplina[10];

    sprintf(a->chave, "%d%d", anoletivo, coddisciplina);
    a->coddisciplina = coddisciplina;
    a->anoletivo = anoletivo;
    a->codprofessor = codprofessor;
    return a;
}


// --------------------------- Disciplina ---------------------------


void link_no_disciplina(FILE* arq, int raiz, int pos, int codigo){
    Disciplina* aux = (Disciplina*) malloc(sizeof(Disciplina));

    fseek(arq, sizeof(Cabecalho) + sizeof(Disciplina) * raiz, SEEK_SET);
    fread(aux, sizeof(Disciplina), 1, arq);

    // Código à esquerda do nó atual
    if(codigo < aux->cod){
        // O novo nó será filho esquerdo do nó atual
        if(aux->esq == -1){
            aux->esq = pos;
            fseek(arq, sizeof(Cabecalho) + sizeof(Disciplina) * raiz, SEEK_SET);
            fwrite(aux, sizeof(Disciplina), 1, arq);
            free(aux);
        }
        else{ // já tem filhos à esquerda
            int pos_aux = aux->esq;
            free(aux);
            link_no_disciplina(arq, pos_aux, pos, codigo);
        }
    }

    // Código à direita do nó atual
    if(codigo > aux->cod){
        // O novo nó será filho direito do nó atual
        if(aux->dir == -1){
            aux->dir = pos;
            fseek(arq, sizeof(Cabecalho) + sizeof(Disciplina) * raiz, SEEK_SET);
            fwrite(aux, sizeof(Disciplina), 1, arq);
            free(aux);
        }
        else{
            int pos_aux = aux->dir;
            free(aux);
            link_no_disciplina(arq, pos_aux, pos, codigo);
        }

    }

    if(codigo == aux->cod) free(aux);
}

void inserir_disciplina(FILE* arq, Disciplina* d){
    if(buscar_disciplina(arq, d->cod) == -1){ // se produto já não existir
        Cabecalho* cab = ler_cabecalho(arq);

        d->dir = -1;
        d->esq = -1;

        int pos;
        Disciplina* aux = (Disciplina*) malloc(sizeof(Disciplina));

        // Há nós livres no arquivo
        if(cab->pos_livre != -1){
            fseek(arq, sizeof(Cabecalho) + sizeof(Disciplina) * cab->pos_livre, SEEK_SET);
            fread(aux, sizeof(Disciplina), 1, arq);
            pos = cab->pos_livre; // guarda a posição no produto a ser inserido
            cab->pos_livre = aux->dir; // atualiza o pos livre para o proximo
        }

        else
            pos = (cab->pos_topo)++; // cria um novo topo

        // Novo nó será a raiz da árvore
        if(cab->pos_raiz == -1)
            cab->pos_raiz = pos;

        fseek(arq, sizeof(Cabecalho) + sizeof(Disciplina) * pos, SEEK_SET);
        fwrite(d, sizeof(Disciplina), 1, arq);
        escrever_cabecalho(arq, cab);
        link_no_disciplina(arq, cab->pos_raiz, pos, d->cod);
        free(aux);
        free(d);
        free(cab);
    }
}

int buscar_disciplina_aux(FILE* arq, int raiz, int codigo){
    if(raiz == -1)
        return -1;

    Disciplina* aux = (Disciplina*) malloc(sizeof(Disciplina));
    fseek(arq, sizeof(Cabecalho) + sizeof(Disciplina) * raiz, SEEK_SET);
    fread(aux, sizeof(Disciplina), 1, arq);

    if(aux->cod == codigo) {
        free(aux);
        return raiz;
    }

    else {
        int pos;
        if(aux->cod > codigo)
            pos = aux->esq;
        else
            pos = aux->dir;
        free(aux);
        return buscar_disciplina_aux(arq, pos, codigo);
    }
}

int buscar_disciplina(FILE* arq, int codigo){
    Cabecalho* cab = ler_cabecalho(arq);

    int pos = buscar_disciplina_aux(arq, cab->pos_raiz, codigo);
    free(cab);
    return pos;
}

void print_inordem_disciplinas_aux(FILE* arq, int raiz){
    if(raiz == -1)
        return;
    Disciplina* aux = (Disciplina*) malloc(sizeof(Disciplina));

    fseek(arq, sizeof(Cabecalho) + sizeof(Disciplina) * raiz, SEEK_SET);
    fread(aux, sizeof(Disciplina), 1, arq);
    print_inordem_disciplinas_aux(arq, aux->esq);
    printf("COD: %03d | NOME: %s | COD CURSO: %03d | SERIE: %03d\n\n", aux->cod, aux->nome, aux->codcurso, aux->serie);
    print_inordem_disciplinas_aux(arq, aux->dir);
    free(aux);
}

void print_inordem_disciplinas(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_inordem_disciplinas_aux(arq, cab->pos_raiz);
}


// --------------------------- Curso ---------------------------


void link_no_curso(FILE* arq, int raiz, int pos, int codigo){
    Curso* aux = (Curso*) malloc(sizeof(Curso));

    fseek(arq, sizeof(Cabecalho) + sizeof(Curso) * raiz, SEEK_SET);
    fread(aux, sizeof(Curso), 1, arq);

    // Código à esquerda do nó atual
    if(codigo < aux->cod){
        // O novo nó será filho esquerdo do nó atual
        if(aux->esq == -1){
            aux->esq = pos;
            fseek(arq, sizeof(Cabecalho) + sizeof(Curso) * raiz, SEEK_SET);
            fwrite(aux, sizeof(Curso), 1, arq);
            free(aux);
        }
        else{ // já tem filhos à esquerda
            int pos_aux = aux->esq;
            free(aux);
            link_no_curso(arq, pos_aux, pos, codigo);
        }
    }

    // Código à direita do nó atual
    if(codigo > aux->cod){
        // O novo nó será filho direito do nó atual
        if(aux->dir == -1){
            aux->dir = pos;
            fseek(arq, sizeof(Cabecalho) + sizeof(Curso) * raiz, SEEK_SET);
            fwrite(aux, sizeof(Curso), 1, arq);
            free(aux);
        }
        else{
            int pos_aux = aux->dir;
            free(aux);
            link_no_curso(arq, pos_aux, pos, codigo);
        }

    }

    if(codigo == aux->cod) free(aux);
}

void inserir_curso(FILE* arq, Curso* c){
    if(buscar_curso(arq, c->cod) == -1){ // se produto já não existir
        Cabecalho* cab = ler_cabecalho(arq);

        c->dir = -1;
        c->esq = -1;

        int pos;
        Curso* aux = (Curso*) malloc(sizeof(Curso));

        // Há nós livres no arquivo
        if(cab->pos_livre != -1){
            fseek(arq, sizeof(Cabecalho) + sizeof(Curso) * cab->pos_livre, SEEK_SET);
            fread(aux, sizeof(Curso), 1, arq);
            pos = cab->pos_livre; // guarda a posição no produto a ser inserido
            cab->pos_livre = aux->dir; // atualiza o pos livre para o proximo
        }

        else
            pos = (cab->pos_topo)++; // cria um novo topo

        // Novo nó será a raiz da árvore
        if(cab->pos_raiz == -1)
            cab->pos_raiz = pos;

        fseek(arq, sizeof(Cabecalho) + sizeof(Curso) * pos, SEEK_SET);
        fwrite(c, sizeof(Curso), 1, arq);
        escrever_cabecalho(arq, cab);
        link_no_curso(arq, cab->pos_raiz, pos, c->cod);
        free(aux);
        free(c);
        free(cab);
    }
}

int buscar_curso_aux(FILE* arq, int raiz, int codigo){
    if(raiz == -1)
        return -1;

    Curso* aux = (Curso*) malloc(sizeof(Curso));
    fseek(arq, sizeof(Cabecalho) + sizeof(Curso) * raiz, SEEK_SET);
    fread(aux, sizeof(Curso), 1, arq);

    if(aux->cod == codigo) {
        free(aux);
        return raiz;
    }

    else {
        int pos;
        if(aux->cod > codigo)
            pos = aux->esq;
        else
            pos = aux->dir;
        free(aux);
        return buscar_curso_aux(arq, pos, codigo);
    }
}

int buscar_curso(FILE* arq, int codigo){
    Cabecalho* cab = ler_cabecalho(arq);

    int pos = buscar_curso_aux(arq, cab->pos_raiz, codigo);
    free(cab);
    return pos;
}

void print_inordem_cursos_aux(FILE* arq, int raiz){
    if(raiz == -1)
        return;
    Curso* aux = (Curso*) malloc(sizeof(Curso));

    fseek(arq, sizeof(Cabecalho) + sizeof(Curso) * raiz, SEEK_SET);
    fread(aux, sizeof(Curso), 1, arq);
    print_inordem_cursos_aux(arq, aux->esq);
    printf("COD: %03d | NOME: %s | AREA: %c\n\n", aux->cod, aux->nome, aux->area);
    print_inordem_cursos_aux(arq, aux->dir);
    free(aux);
}

void print_inordem_cursos(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_inordem_cursos_aux(arq, cab->pos_raiz);
}


// --------------------------- Professor ---------------------------


void link_no_professor(FILE* arq, int raiz, int pos, int codigo){
    Professor* aux = (Professor*) malloc(sizeof(Professor));

    fseek(arq, sizeof(Cabecalho) + sizeof(Professor) * raiz, SEEK_SET);
    fread(aux, sizeof(Professor), 1, arq);

    // Código à esquerda do nó atual
    if(codigo < aux->cod){
        // O novo nó será filho esquerdo do nó atual
        if(aux->esq == -1){
            aux->esq = pos;
            fseek(arq, sizeof(Cabecalho) + sizeof(Professor) * raiz, SEEK_SET);
            fwrite(aux, sizeof(Professor), 1, arq);
            free(aux);
        }
        else{ // já tem filhos à esquerda
            int pos_aux = aux->esq;
            free(aux);
            link_no_professor(arq, pos_aux, pos, codigo);
        }
    }

    // Código à direita do nó atual
    if(codigo > aux->cod){
        // O novo nó será filho direito do nó atual
        if(aux->dir == -1){
            aux->dir = pos;
            fseek(arq, sizeof(Cabecalho) + sizeof(Professor) * raiz, SEEK_SET);
            fwrite(aux, sizeof(Professor), 1, arq);
            free(aux);
        }
        else{
            int pos_aux = aux->dir;
            free(aux);
            link_no_professor(arq, pos_aux, pos, codigo);
        }

    }

    if(codigo == aux->cod) free(aux);
}

void inserir_professor(FILE* arq, Professor* p){
    if(buscar_professor(arq, p->cod) == -1){ // se produto já não existir
        Cabecalho* cab = ler_cabecalho(arq);

        p->dir = -1;
        p->esq = -1;

        int pos;
        Professor* aux = (Professor*) malloc(sizeof(Professor));

        // Há nós livres no arquivo
        if(cab->pos_livre != -1){
            fseek(arq, sizeof(Cabecalho) + sizeof(Professor) * cab->pos_livre, SEEK_SET);
            fread(aux, sizeof(Professor), 1, arq);
            pos = cab->pos_livre; // guarda a posição no produto a ser inserido
            cab->pos_livre = aux->dir; // atualiza o pos livre para o proximo
        }

        else
            pos = (cab->pos_topo)++; // cria um novo topo

        // Novo nó será a raiz da árvore
        if(cab->pos_raiz == -1)
            cab->pos_raiz = pos;

        fseek(arq, sizeof(Cabecalho) + sizeof(Professor) * pos, SEEK_SET);
        fwrite(p, sizeof(Professor), 1, arq);
        escrever_cabecalho(arq, cab);
        link_no_professor(arq, cab->pos_raiz, pos, p->cod);
        free(aux);
        free(p);
        free(cab);
    }
}

int buscar_professor_aux(FILE* arq, int raiz, int codigo){
    if(raiz == -1)
        return -1;

    Professor* aux = (Professor*) malloc(sizeof(Professor));
    fseek(arq, sizeof(Cabecalho) + sizeof(Professor) * raiz, SEEK_SET);
    fread(aux, sizeof(Professor), 1, arq);

    if(aux->cod == codigo) {
        free(aux);
        return raiz;
    }

    else {
        int pos;
        if(aux->cod > codigo)
            pos = aux->esq;
        else
            pos = aux->dir;
        free(aux);
        return buscar_professor_aux(arq, pos, codigo);
    }
}

int buscar_professor(FILE* arq, int codigo){
    Cabecalho* cab = ler_cabecalho(arq);

    int pos = buscar_professor_aux(arq, cab->pos_raiz, codigo);
    free(cab);
    return pos;
}

void print_inordem_professores_aux(FILE* arq, int raiz){
    if(raiz == -1)
        return;
    Professor* aux = (Professor*) malloc(sizeof(Professor));

    fseek(arq, sizeof(Cabecalho) + sizeof(Professor) * raiz, SEEK_SET);
    fread(aux, sizeof(Professor), 1, arq);
    print_inordem_professores_aux(arq, aux->esq);
    printf("COD: %03d | NOME: %s\n\n", aux->cod, aux->nome);
    print_inordem_professores_aux(arq, aux->dir);
    free(aux);
}

void print_inordem_professores(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_inordem_professores_aux(arq, cab->pos_raiz);
}


// --------------------------- Associação ---------------------------

// Determina qual das duas string é lexicograficamente maior
// Entrada: as duas string a serem comparadas
// Saída: 1 se o primeiro valor é maior, 0 se o segundo é maior e -1 se são iguais
int maior(const char* valor1, const char* valor2) {
    int i = 0;

    while(valor1[i] != '\0' || valor2[i] != '\0') {
        if(valor1[i] > valor2[i])
            return 1;
        
        if(valor1[i] < valor2[i])
            return 0;
        i++;
    }
    // são iguais
    return -1;
}

void link_no_associacao(FILE* arq, int raiz, int pos, char* codigo){
    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);

    // Código à esquerda do nó atual
    if(maior(aux->chave, codigo)){ // codigo < aux->chave
        // O novo nó será filho esquerdo do nó atual
        if(aux->esq == -1){
            aux->esq = pos;
            fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
            fwrite(aux, sizeof(Associacao), 1, arq);
            free(aux);
        }
        else{ // já tem filhos à esquerda
            int pos_aux = aux->esq;
            free(aux);
            link_no_associacao(arq, pos_aux, pos, codigo);
        }
    }

    // Código à direita do nó atual
    if(maior(codigo, aux->chave)){ // codigo > aux->chave
        // O novo nó será filho direito do nó atual
        if(aux->dir == -1){
            aux->dir = pos;
            fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
            fwrite(aux, sizeof(Associacao), 1, arq);
            free(aux);
        }
        else{
            int pos_aux = aux->dir;
            free(aux);
            link_no_associacao(arq, pos_aux, pos, codigo);
        }

    }

    if(maior(codigo, aux->chave) == -1) // codigo == aux->chave
        free(aux);
}

void inserir_associacao(FILE* arq, Associacao* a){
    if(buscar_associacao(arq, a->chave) == -1){ // se produto já não existir
        Cabecalho* cab = ler_cabecalho(arq);

        a->dir = -1;
        a->esq = -1;

        int pos;
        Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

        // Há nós livres no arquivo
        if(cab->pos_livre != -1){
            fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * cab->pos_livre, SEEK_SET);
            fread(aux, sizeof(Associacao), 1, arq);
            pos = cab->pos_livre; // guarda a posição no produto a ser inserido
            cab->pos_livre = aux->dir; // atualiza o pos livre para o proximo
        }

        else
            pos = (cab->pos_topo)++; // cria um novo topo

        // Novo nó será a raiz da árvore
        if(cab->pos_raiz == -1)
            cab->pos_raiz = pos;

        fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * pos, SEEK_SET);
        fwrite(a, sizeof(Associacao), 1, arq);
        escrever_cabecalho(arq, cab);
        link_no_associacao(arq, cab->pos_raiz, pos, a->chave);
        free(aux);
        free(a);
        free(cab);
    }
}

int buscar_associacao_aux(FILE* arq, int raiz, char* codigo){
    printf("raiz = %d\n", raiz);
    if(raiz == -1)
        return -1;

    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));
    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);

    printf("ok2\n");

    if(maior(aux->chave, codigo) == -1) { // aux->chave == codigo
        free(aux);
        return raiz;
    }

    else {
        int pos;
        if(maior(aux->chave, codigo)) {// aux->chave > codigo
            pos = aux->esq;
            printf("maior = %d\n", maior(aux->chave, codigo));
        }
        else
            pos = aux->dir;
        free(aux);
        printf("ok3\n");
        return buscar_associacao_aux(arq, pos, codigo);
    }
}

int buscar_associacao(FILE* arq, char* codigo){
    Cabecalho* cab = ler_cabecalho(arq);

    int pos = buscar_associacao_aux(arq, cab->pos_raiz, codigo);
    free(cab);
    return pos;
}

void print_inordem_associacoes_aux(FILE* arq, int raiz){
    if(raiz == -1)
        return;
    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);
    print_inordem_associacoes_aux(arq, aux->esq);
    printf("DISCIPLINA: %03d | ANO LETIVO: %d | PROFESSOR: %03d\n\n", aux->coddisciplina, aux->anoletivo, aux->codprofessor);
    print_inordem_associacoes_aux(arq, aux->dir);
    free(aux);
}

void print_inordem_associacoes(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_inordem_associacoes_aux(arq, cab->pos_raiz);
}
