#ifndef SISTEMA
#define SISTEMA
#include <stdio.h>

// *
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

// Estrutura para Associação
typedef struct {
    char chave[10];
    int coddisciplina;
    int anoletivo;
    int codprofessor;
    int esq;
    int dir;
} Associacao;


// ---------------------------- ARQUIVO ----------------------------


// Verifica se o cabeçalho (a arvore) é vazia
// Entrada: cabeçalho
// Saída: é vazia (1) ou não é vazia (0)
int vazia(Cabecalho *cab);

// Cria uma arvore nova em arquivo, podendo ser de Curso, Disciplina,
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

// Abre arquivo
// Pré-condição: string contendo o nome do arquivo.extensão
// Pós-condição: retorna arquivo aberto
FILE* open_arq(char *str);


// --------------------------- Sistema ---------------------------


// Cria uma Disciplina contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente para o mesmo curso
// Pós-condição: retorna ponteiro para Disciplina
Disciplina* criar_disciplina(int codigo, char *nome, int codcurso, int serie);

// Cria um Curso contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro curso,
//      area deve ser 'E' (Exatas), 'H' (Humanas) ou 'B' (Biológicas)
// Pós-condição: retorna ponteiro para Curso
Curso* criar_curso(int codigo, char *nome, char area);

// Cria um Professor contendo os dados fornecidos
// Pré-condição: o código fornecido deve ser diferente de outro professor
// Pós-condição: retorna ponteiro para CadastroProfessor
Professor* criar_professor(int codigo, char *nome);

// Cria uma Associação contendo os dados fornecidos
// Pré-condição: os códigos fornecidos devem ser válidos
// Pós-condição: retorna ponteiro para CadastroProfessorDisciplina
Associacao* criar_associacao(int coddisciplina, int anoletivo, int codprofessor);

// ***************************************

// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_disciplina(FILE* arq, Disciplina* d);

int buscar_disciplina(FILE* arq, int codigo);

void print_inordem_disciplinas(FILE* arq);



// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_curso(FILE* arq, Curso* d);

int buscar_curso(FILE* arq, int codigo);

void print_inordem_cursos(FILE* arq);


// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_professor(FILE *arq, Professor* p);

int buscar_professor(FILE* arq, int codigo);

void print_inordem_professores(FILE* arq);


// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
void inserir_associacao(FILE* arq, Associacao* a);

int buscar_associacao(FILE* arq, char* codigo);

void print_inordem_associacoes(FILE* arq);

// ********************************************


// Insere o nó na árvore do arquivo
// Pré-condição: arquivo deve estar aberto para escrita
// Pós-condição: arquivo com novo nó
// void inserir_associacao(FILE *arq_associacao, FILE *arq_disciplinas, Associacao* a);

// Busca por uma disciplina através do seu código
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// Pós-condição: ponteiro para nó lido é retornado
//int buscar_disciplina(FILE* arq, int codigo);

// Busca por um curso através do seu código
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// Pós-condição: ponteiro para nó lido é retornado
// Curso* buscar_curso(FILE* arq, int codigo);

// Busca por um professor através do seu código
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// Pós-condição: ponteiro para nó lido é retornado
// Professor* buscar_professor(FILE* arq, int codigo);

// Busca por uma associação através do seu código
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// Pós-condição: ponteiro para nó lido é retornado
// Associacao* buscar_associacao(FILE* arq, int codigo);

// Imprime a árvore de disciplinas
// Pré-condição: arquivo aberto para leitura
// Pós-condição: nenhuma
void imprimir_disciplinas(FILE *arq);

// Imprime a árvore de cursos
// Pré-condição: arquivo aberto para leitura
// Pós-condição: nenhuma
void imprimir_cursos(FILE *arq);

// Imprime a árvore de professores
// Pré-condição: arquivo aberto para leitura
// Pós-condição: nenhuma
void imprimir_professores(FILE *arq);

// Imprime a árvore de associações
// Pré-condição: arquivo aberto para leitura
// Pós-condição: nenhuma
void imprimir_associacoes(FILE *arq);

// Imprime a árvore de associações organizada por cursos
// Pré-condição: arquivos de associações e de cursos abertos para leitura
// Pós-condição: nenhuma
void imprimir_associacao2(FILE *arq_associacao, FILE *arq_cursos);

// Imprime a árvore de associações organizada por cursos
// Pré-condição: arquivos de associações e de cursos abertos para leitura
// Pós-condição: nenhuma
void imprimir_associacoes_organizada(FILE *arq_associacao, FILE *arq_cursos);

// Escreve um nó em uma determinada posição do arquivo
// Pré-condição: arquivo deve estar aberto e ser um arquivo de árvore
// pos deve ser uma posição válida do arquivo
// Pós-condição: nó escrito no arquivo
void escrever_associacao(FILE* arq, Associacao *a, int pos);

// Busca por uma associação dada uma posição
// Pré-condição: arquivo de associação aberto para leitura
// Pós-condição: nó encontrado é retornado, caso não encontre, retorna NULL
Associacao* ler_associacao(FILE* arq, int pos);

// Remove uma associação dado seu código e seu ano letivo
// Pré-condição: arquivo de associação deve estar aberto para escrita
// Pós-condição: nó removido
void remover_associacao(FILE *arq, int coddisciplina, int ano_letivo);


#endif