#ifndef SISTEMA
#define SISTEMA
#include <stdio.h>


// ---------------------------- ESTRUTURAS ----------------------------

// Estrutura de cabeçalho do arquivo
typedef struct {
    int pos_raiz; // posição do início da árvore
    int pos_topo; // 1a posição não usada no fim do arquivo
    int pos_livre; // posição de início da lista de nós livres
} Cabecalho;

// Estrutura para Disciplina
typedef struct {
    int cod;
    char nome[50];
    int codcurso;
    int serie;
    int esq;
    int dir;
} Disciplina;

// Estrutura para Curso
typedef struct {
    int cod;
    char nome[50];
    char area; // Exatas, Humanas, Biológicas
    int esq;
    int dir;
} Curso;

// Estrutura para Professor
typedef struct {
    int cod;
    char nome[50];
    int esq;
    int dir;
} Professor;

// Estrutura para Associação de Disciplina
typedef struct {
    char cod[10];
    int coddisciplina;
    int anoletivo;
    int codprofessor;
    int esq;
    int dir;
} Associacao;

// Estrutura de lista para impressão por níveis de Associação
typedef struct lista{
    char cod[10];
    int coddisciplina;
    int anoletivo;
    int codprofessor;
    int pos;
    struct lista* prox;
} lista;


// ---------------------------- ARQUIVO ----------------------------


// Verifica se o cabeçalho (a árvore) é vazia
// Entrada: cabeçalho
// Saída: é vazia (1) ou não é vazia (0)
int vazia(Cabecalho *cab);

// Cria uma árvore nova em arquivo, podendo ser de Curso, Disciplina,
// Professor ou Associação
// Pré-condição: arquivo aberto para leitura/escrita
// Pós-condição: arquivo é inicializado com uma árvore vazia
void criar_cabecalho_vazio(FILE* arq);

// Lê o cabeçalho do arquivo contendo as informações da árvore
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// Pós-condição: retorna o ponteiro para o cabeçalho lido
Cabecalho* ler_cabecalho(FILE *arq);

// Escreve no arquivo o cabeçalho contendo as informações da árvore
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// Pós-condição: cabeçalho escrito no arquivo
void escrever_cabecalho(FILE *arq, Cabecalho *cab);

// Imprime informações do cabeçalho
// Entrada: cabeçalho a ser impresso
// Saída: nenhuma
void imprimir_cabecalho(Cabecalho *cab);

// Abre arquivo binário para leitura e escrita
// Pré-condição: string contendo o nome do arquivo.extensão
// Pós-condição: retorna arquivo aberto
FILE* open_arq(char *str);


// --------------------------- SISTEMA ---------------------------


//                             DISCIPLINA


// Cria uma Disciplina contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente para o mesmo curso
// Pós-condição: retorna ponteiro para Disciplina
Disciplina* criar_disciplina(int codigo, char *nome, int codcurso, int serie);

// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_disciplina(FILE* arq, Disciplina* d);

// Busca uma disciplina com código especificado no arquivo
// Entrada: arquivo binário da árvore, código da disciplina a ser buscada
// Retorno: posição do nó com o código especificado, -1 se não existir
// Pré-condição: o arquivo deve ser válido
// Pós-condição: será retornada a posição do nó procurado ou -1 se ele não existir
int buscar_disciplina(FILE* arq, int codigo);

// Imprime a árvore in-ordem
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: os nós terão seus códigos e nomes impressos in-ordem
void print_inordem_disciplinas(FILE* arq);


//                             CURSO


// Cria um Curso contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro curso,
//               área deve ser 'E' (Exatas), 'H' (Humanas) ou 'B' (Biológicas)
// Pós-condição: retorna ponteiro para Curso
Curso* criar_curso(int codigo, char *nome, char area);

// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_curso(FILE* arq, Curso* d);

// Busca um curso com código especificado no arquivo
// Entrada: arquivo binário da árvore, código do curso a ser buscado
// Retorno: posição do nó com o código especificado, -1 se não existir
// Pré-condição: o arquivo deve ser válido
// Pós-condição: será retornada a posição do nó procurado ou -1 se ele não existir
int buscar_curso(FILE* arq, int codigo);

// Imprime a árvore in-ordem
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: os nós terão seus códigos e nomes impressos in-ordem
void print_inordem_cursos(FILE* arq);


//                             PROFESSOR


// Cria um Professor contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro professor
// Pós-condição: retorna ponteiro para Professor
Professor* criar_professor(int codigo, char *nome);

// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_professor(FILE *arq, Professor* p);

// Busca um professor com código especificado no arquivo
// Entrada: arquivo binário da árvore, código do professor a ser buscado
// Retorno: posição do nó com o código especificado, -1 se não existir
// Pré-condição: o arquivo deve ser válido
// Pós-condição: será retornada a posição do nó procurado ou -1 se ele não existir
int buscar_professor(FILE* arq, int codigo);

// Imprime a árvore in-ordem
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: os nós terão seus códigos e nomes impressos in-ordem
void print_inordem_professores(FILE* arq);


//                             ASSOCIAÇÃO


// Cria uma Associação contendo os dados fornecidos
// Pré-condição: os códigos fornecidos devem ser válidos
// Pós-condição: retorna ponteiro para Associacao
Associacao* criar_associacao(int coddisciplina, int anoletivo, int codprofessor);

// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_associacao(FILE* arq, Associacao* a);

// Busca uma associação com a chave especificada no arquivo
// Entrada: arquivo binário da árvore, chave da associação a ser buscada
// Retorno: posição do nó com a chave especificada, -1 se não existir
// Pré-condição: o arquivo deve ser válido
// Pós-condição: será retornada a posição do nó procurado ou -1 se ele não existir
int buscar_associacao(FILE* arq, char* codigo);

// Imprime a árvore in-ordem
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: os nós terão seus códigos e nomes impressos in-ordem
void print_inordem_associacoes(FILE* arq);

// Remove uma associação da árvore de associações
// Entrada: código da disciplina e ano letivo
// Saída: nenhuma
// Pré-condição: o código deve ser uma string formata pelo ano letivo e código da disciplina
// Pós-condição: nenhuma
void remover_associacao(FILE* f, char* codigo);

// Imprime a lista de nós livres
// Entrada: arquivo binário da árvore
// Retorno: nenhum
// Pré-condição: o arquivo deve ser válido
// Pós-condição: serão impressas todas as posições livres
void print_livres(FILE* arq);

// Imprime a árvore em níveis
// Entrada: arquivo da árvore
// Saída: nenhuma
void print_arvore(FILE* arq);

#endif