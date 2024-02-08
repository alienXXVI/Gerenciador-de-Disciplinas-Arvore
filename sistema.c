#include <stdlib.h>
#include <string.h>
#include "sistema.h"


// ---------------------------- ARQUIVO ----------------------------


// Verifica se o cabeçalho (a árvore) é vazia
// Entrada: cabeçalho
// Saída: é vazia (1) ou não é vazia (0)
int vazia(Cabecalho *cab) {
    return cab->pos_raiz == -1;
}

// Cria uma árvore nova em arquivo, podendo ser de Curso, Disciplina,
// Professor ou Associação
// Pré-condição: arquivo aberto para leitura/escrita
// Pós-condição: arquivo é inicializado com uma árvore vazia
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

// Imprime informações do cabeçalho
// Entrada: cabeçalho a ser impresso
// Saída: nenhuma
void imprimir_cabecalho(Cabecalho *cab) {
    printf("---- CABECALHO ----\n");
    printf("Cabeca: %d\n"
            "Topo: %d\n"
            "Livre: %d\n",
        cab->pos_raiz, cab->pos_topo, cab->pos_livre);
}

// Abre arquivo binário para leitura e escrita
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


// --------------------------- SISTEMA ---------------------------


//                             DISCIPLINA


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

// Conecta um nó à árvore de acordo com seu código
// Entrada: arquivo binário da árvore, raiz da árvore, posição do nó no arquivo, código do nó
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: o nó será conectado corretamente na árvore, atualizando as informações dos pais
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

// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
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

// Busca o nó dada sua raiz e seu código
// Entrada: arquivo da árvore, raiz atual e código
// Saída: posição do nó buscado ou -1 se ele não existir
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

// Busca uma disciplina com código especificado no arquivo
// Entrada: arquivo binário da árvore, código da disciplina a ser buscada
// Retorno: posição do nó com o código especificado, -1 se não existir
// Pré-condição: o arquivo deve ser válido
// Pós-condição: será retornada a posição do nó procurado ou -1 se ele não existir
int buscar_disciplina(FILE* arq, int codigo){
    Cabecalho* cab = ler_cabecalho(arq);

    int pos = buscar_disciplina_aux(arq, cab->pos_raiz, codigo);
    free(cab);
    return pos;
}

// Imprime informações do nó recursivamente
// Entrada: arquivo da árvore e raiz atual
// Saída: nenhuma
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

// Imprime a árvore in-ordem
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: os nós terão seus códigos e nomes impressos in-ordem
void print_inordem_disciplinas(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_inordem_disciplinas_aux(arq, cab->pos_raiz);
}


//                             CURSO


// Cria um Curso contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro curso,
//               área deve ser 'E' (Exatas), 'H' (Humanas) ou 'B' (Biológicas)
// Pós-condição: retorna ponteiro para Curso
Curso* criar_curso(int codigo, char *nome, char area) {
    Curso *c = (Curso*) malloc(sizeof(Curso));

    c->cod = codigo;
    strcpy(c->nome, nome);
    c->area = area;
    return c;
}

// Conecta um nó à árvore de acordo com seu código
// Entrada: arquivo binário da árvore, raiz da árvore, posição do nó no arquivo, código do nó
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: o nó será conectado corretamente na árvore, atualizando as informações dos pais
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

// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
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

// Busca o nó dada sua raiz e seu código
// Entrada: arquivo da árvore, raiz atual e código
// Saída: posição do nó buscado ou -1 se ele não existir
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

// Busca um curso com código especificado no arquivo
// Entrada: arquivo binário da árvore, código do curso a ser buscado
// Retorno: posição do nó com o código especificado, -1 se não existir
// Pré-condição: o arquivo deve ser válido
// Pós-condição: será retornada a posição do nó procurado ou -1 se ele não existir
int buscar_curso(FILE* arq, int codigo){
    Cabecalho* cab = ler_cabecalho(arq);

    int pos = buscar_curso_aux(arq, cab->pos_raiz, codigo);
    free(cab);
    return pos;
}

// Imprime informações do nó recursivamente
// Entrada: arquivo da árvore e raiz atual
// Saída: nenhuma
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

// Imprime a árvore in-ordem
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: os nós terão seus códigos e nomes impressos in-ordem
void print_inordem_cursos(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_inordem_cursos_aux(arq, cab->pos_raiz);
}


//                             PROFESSOR


// Cria um Professor contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro professor
// Pós-condição: retorna ponteiro para Professor
Professor* criar_professor(int codigo, char *nome) {
    Professor *p = (Professor*) malloc(sizeof(Professor));

    p->cod = codigo;
    strcpy(p->nome, nome);
    return p;
}

// Conecta um nó à árvore de acordo com seu código
// Entrada: arquivo binário da árvore, raiz da árvore, posição do nó no arquivo, código do nó
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: o nó será conectado corretamente na árvore, atualizando as informações dos pais
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

// Cria um Professor contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro professor
// Pós-condição: retorna ponteiro para Professor
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

// Busca o nó dada sua raiz e seu código
// Entrada: arquivo da árvore, raiz atual e código
// Saída: posição do nó buscado ou -1 se ele não existir
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

// Busca um professor com código especificado no arquivo
// Entrada: arquivo binário da árvore, código do professor a ser buscado
// Retorno: posição do nó com o código especificado, -1 se não existir
// Pré-condição: o arquivo deve ser válido
// Pós-condição: será retornada a posição do nó procurado ou -1 se ele não existir
int buscar_professor(FILE* arq, int codigo){
    Cabecalho* cab = ler_cabecalho(arq);

    int pos = buscar_professor_aux(arq, cab->pos_raiz, codigo);
    free(cab);
    return pos;
}

// Imprime informações do nó recursivamente
// Entrada: arquivo da árvore e raiz atual
// Saída: nenhuma
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

// Imprime a árvore in-ordem
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: os nós terão seus códigos e nomes impressos in-ordem
void print_inordem_professores(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_inordem_professores_aux(arq, cab->pos_raiz);
}


//                             ASSOCIAÇÃO


// Cria uma Associação contendo os dados fornecidos
// Pré-condição: os códigos fornecidos devem ser válidos
// Pós-condição: retorna ponteiro para Associacao
Associacao* criar_associacao(int coddisciplina, int anoletivo, int codprofessor) {
    Associacao *a = (Associacao*) malloc(sizeof(Associacao));

    sprintf(a->cod, "%d%d", anoletivo, coddisciplina);
    a->coddisciplina = coddisciplina;
    a->anoletivo = anoletivo;
    a->codprofessor = codprofessor;
    return a;
}

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

// Conecta um nó à árvore de acordo com seu código
// Entrada: arquivo binário da árvore, raiz da árvore, posição do nó no arquivo, código do nó
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: o nó será conectado corretamente na árvore, atualizando as informações dos pais
void link_no_associacao(FILE* arq, int raiz, int pos, char* codigo){
    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);

    // Código à esquerda do nó atual
    if(maior(aux->cod, codigo) == 1){ // codigo < aux->cod
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
    if(maior(codigo, aux->cod) == 1){ // codigo > aux->cod
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

    if(maior(codigo, aux->cod) == -1) // codigo == aux->cod
        free(aux);
}

// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_associacao(FILE* arq, Associacao* p){
    if(buscar_associacao(arq, p->cod) == -1){ // se produto não existir ainda
        Cabecalho* cab = ler_cabecalho(arq);

        p->dir = -1;
        p->esq = -1;

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
        fwrite(p, sizeof(Associacao), 1, arq);
        escrever_cabecalho(arq, cab);
        link_no_associacao(arq, cab->pos_raiz, pos, p->cod);
        free(aux);
        free(p);
        free(cab);
    }
}

// Busca o nó dada sua raiz e seu código
// Entrada: arquivo da árvore, raiz atual e código
// Saída: posição do nó buscado ou -1 se ele não existir
int buscar_associacao_aux(FILE* arq, int raiz, char* codigo){
    if(raiz == -1)
        return -1;

    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));
    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);

    if(maior(aux->cod, codigo) == -1) { // aux->cod == codigo
        free(aux);
        return raiz;
    }

    else {
        int pos;
        if(maior(aux->cod, codigo) == 1) // aux->cod > codigo
            pos = aux->esq;
        else
            pos = aux->dir;
        free(aux);
        return buscar_associacao_aux(arq, pos, codigo);
    }
}

// Busca uma associação com a chave especificada no arquivo
// Entrada: arquivo binário da árvore, chave da associação a ser buscada
// Retorno: posição do nó com a chave especificada, -1 se não existir
// Pré-condição: o arquivo deve ser válido
// Pós-condição: será retornada a posição do nó procurado ou -1 se ele não existir
int buscar_associacao(FILE* arq, char* codigo){
    Cabecalho* cab = ler_cabecalho(arq);

    int pos = buscar_associacao_aux(arq, cab->pos_raiz, codigo);
    free(cab);
    return pos;
}

// Imprime informações do nó recursivamente
// Entrada: arquivo da árvore e raiz atual
// Saída: nenhuma
void print_inordem_associacoes_aux(FILE* arq, int raiz){
    if(raiz == -1)
        return;
    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);
    print_inordem_associacoes_aux(arq, aux->esq);
    printf("raiz = %d\n", raiz);
    printf("COD: %s | PROFESSOR: %03d | ANO LETIVO: %d | DISCIPLINA: %03d\n\n", aux->cod, aux->codprofessor, aux->anoletivo, aux->coddisciplina);
    print_inordem_associacoes_aux(arq, aux->dir);
    free(aux);
}

// Imprime a árvore in-ordem
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: os nós terão seus códigos e nomes impressos in-ordem
void print_inordem_associacoes(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_inordem_associacoes_aux(arq, cab->pos_raiz);
}

// Encontra o maior nó entre os filhos esquerdos da raiz
// Entrada: arquivo da árvore e raiz atual
// Saída: posição do nó
int maximo(FILE* arq, int raiz){
    if(raiz == -1)
        return -1;

    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);

    if(aux->dir == -1){
        free(aux);
        return raiz;
    }

    int pos = aux->dir;
    free(aux);
    return maximo(arq, pos);
}

// Desconecta um nó (que não possui filhos à esquerda) de seus pais. Uso exclusivo da função remover que substitui o nó atual
// pelo nó máximo da esquerda.
// Entrada: arquivo binário da árvore, raiz da árvore, código do nó a ser desconectado
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido, o nó a ser desconectado não possui filhos à esquerda
// Pós-condição: os pais do nó receberão seus filhos e ele será desconectado
void deslink_no(FILE* arq, int raiz, char* codigo){
    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);

    // Código à esquerda do nó atual
    if(maior(aux->cod, codigo) == 1){ // codigo < aux->cod
        if(aux->esq != -1){
            Associacao* filho = (Associacao*) malloc(sizeof(Associacao));
            fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * aux->esq, SEEK_SET);
            fread(filho, sizeof(Associacao), 1, arq);

            // Nó com código informado é filho esquerdo do nó atual
            if(maior(filho->cod, codigo) == -1){ // filho->cod == codigo
                aux->esq = filho->dir;
                fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
                fwrite(aux, sizeof(Associacao), 1, arq);
                free(filho);
                free(aux);
            }

            else{
                int pos = aux->esq;
                free(filho);
                free(aux);
                deslink_no(arq, pos, codigo);
            }
        }
    }

    // Código à direita do nó atual
    if(maior(codigo, aux->cod) == 1){ // codigo > aux->cod
        if(aux->dir != -1){
            Associacao* filho = (Associacao*) malloc(sizeof(Associacao));

            fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * aux->dir, SEEK_SET);
            fread(filho, sizeof(Associacao), 1, arq);

            // Nó com código informado é filho direito do nó atual
            if(maior(filho->cod, codigo) == -1){ // filho->cod == codigo
                aux->dir = filho->dir;
                fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
                fwrite(aux, sizeof(Associacao), 1, arq);
                free(filho);
                free(aux);
            }

            else{
                int pos = aux->dir;
                free(filho);
                free(aux);
                deslink_no(arq, pos, codigo);
            }

        }
    }
    if(maior(codigo, aux->cod) == -1) // codigo == aux->cod
        free(aux);
}

// Remove recursivamente um nó
// Entrada: arquivo da árvore e posição do nó
// Saída: nenhuma
void remover_aux(FILE* arq, int pos){
    Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

    fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * pos, SEEK_SET);
    fread(aux, sizeof(Associacao), 1, arq);

    int maior_esq = maximo(arq, aux->esq);

    // REMOÇÃO EM UM NÓ COM FILHO(S) NA ESQUERDA
    if(maior_esq != -1){
        // ETAPA 1: Armazenar informações do filho (nó na POSIÇÃO que será removida)
        Associacao* filho = (Associacao*) malloc(sizeof(Associacao));
        fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * maior_esq, SEEK_SET);
        fread(filho, sizeof(Associacao), 1, arq);

        // ETAPA 2: Remover posição do filho
        remover_aux(arq, maior_esq);

        // ETAPA 3: Atualizar novos filhos do nó principal (nó com CÓDIGO que será "removido")
        //          Substituir informação do nó principal pelo nó do filho
        fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * pos, SEEK_SET);
        fread(aux, sizeof(Associacao), 1, arq);
        strcpy(aux->cod, filho->cod); //
        aux->coddisciplina = filho->coddisciplina;
        aux->anoletivo = filho->anoletivo;
        aux->codprofessor = filho->codprofessor;

        // ETAPA 4: Reescrever nó com informação substituída
        fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * pos, SEEK_SET);
        fwrite(aux, sizeof(Associacao), 1, arq);

        free(filho);
    }

    // REMOÇÃO EM UM NÓ SEM FILHOS NA ESQUERDA
    else{
        Cabecalho* cab = ler_cabecalho(arq);

        if(pos == cab->pos_raiz)
            cab->pos_raiz = aux->dir;
        else
            deslink_no(arq, cab->pos_raiz, aux->cod);

        aux->esq = -1;
        aux->dir = cab->pos_livre;

        fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * pos, SEEK_SET);
        fwrite(aux, sizeof(Associacao), 1, arq);

        cab->pos_livre = pos;
        escrever_cabecalho(arq, cab);
        free(cab);
    }

    free(aux);
}

// Remove uma associação da árvore de associações
// Entrada: código da disciplina e ano letivo
// Saída: nenhuma
// Pré-condição: o código deve ser uma string formata pelo ano letivo e código da disciplina
// Pós-condição: nenhuma
void remover_associacao(FILE* f, char* codigo){
    int pos = buscar_associacao(f, codigo);
    if(pos != -1)
        remover_aux(f, pos);
}

// Imprime a lista de nós livres
// Entrada: arquivo binário da árvore e raiz atual
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: serão impressas todas as posições livres
void print_livres_aux(FILE* arq, int raiz){
    if(raiz != -1){
        Associacao* aux = (Associacao*) malloc(sizeof(Associacao));

        fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
        fread(aux, sizeof(Associacao), 1, arq);
        int pos = aux->dir;
        printf("POS LIVRE: %03d\n", raiz);
        free(aux);
        print_livres_aux(arq, pos);
    }
}

// Imprime a lista de nós livres
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: serão impressas todas as posições livres
void print_livres(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    print_livres_aux(arq, cab->pos_livre);
    free(cab);
}

// Insere um novo elemento na lista ordenada
// Entrada: ponteiro para a lista de associações, posição a ser inserida na lista e informações do nó
// Retorno: ponteiro para a lista atualizada
lista* inserir_lista(lista* l, int pos, char* chave, int coddisciplina, int anoletivo, int codprofessor){
    if(l == NULL || pos <= l->pos){
        lista* aux = (lista*) malloc(sizeof(lista));
        strcpy(aux->cod, chave);
        aux->coddisciplina = coddisciplina;
        aux->anoletivo = anoletivo;
        aux->codprofessor = codprofessor;
        aux->pos = pos;
        aux->prox = l;
        return aux;
    }
    l->prox = inserir_lista(l->prox, pos, chave, coddisciplina, anoletivo, codprofessor);
    return l;
}

// Função auxiliar para percorrer a árvore binária e montar uma lista de associações
// Entrada: arquivo binário da árvore, ponteiro para a lista de associações
//          posição da raiz da subárvore a ser percorrida e altura atual na árvore
// Retorno: nenhum
void print_arvore_aux(FILE* arq, lista** l, int raiz, int h){
    if(raiz != -1){
        Associacao* a = (Associacao*) malloc(sizeof(Associacao));
        
        fseek(arq, sizeof(Cabecalho) + sizeof(Associacao) * raiz, SEEK_SET);
        fread(a, sizeof(Associacao), 1, arq);
        *l = inserir_lista(*l, h, a->cod, a->coddisciplina, a->anoletivo, a->codprofessor);
        int esq = a->esq;
        int dir = a->dir;
        free(a);
        print_arvore_aux(arq, l, dir, h+1);
        print_arvore_aux(arq, l, esq, h+1);

    }
}

// Imprime a lista
// Entrada: ponteiro para a lista de associações, posição atual na lista
// Retorno: nenhum
void print_lista(lista* l, int pos){
    if(l == NULL) return;
    if(l->pos > pos){
        printf("\n[%s] ", l->cod);
        print_lista(l->prox, pos+1);
    }
    else{
        printf("[%s] ", l->cod);
        print_lista(l->prox, pos);
    }
} 

// Libera a memória alocada para a lista
// Entrada: ponteiro para a lista de associações.
// Retorno: nenhum
void free_lista(lista* l){
    if(l == NULL) return;
    free_lista(l->prox);
    free(l);
}

// Imprime a árvore em níveis
// Entrada: arquivo da árvore
// Saída: nenhuma
void print_arvore(FILE* arq){
    Cabecalho* cab = ler_cabecalho(arq);
    lista* l = NULL;

    print_arvore_aux(arq, &l, cab->pos_raiz, 0);
    print_lista(l, 0);
    printf("\n");
    free_lista(l);
    free(cab);
}