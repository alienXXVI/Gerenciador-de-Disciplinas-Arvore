#include <stdio.h>
#include "menu.h"
#include "lotes.h"
#include "sistema.h"

void logo() {
    printf("\n\nGERENCIADOR DE DISCIPLINAS\n"
           "''''''''''''''''''''''''''\n");
}

void menu_principal(FILE *arq_cursos, FILE *arq_disciplinas, FILE *arq_professores, FILE *arq_associacoes) {
    int i = -1;

    while(i != 5) {
        printf("\n1 - Fazer insercao\n"
                "2 - Fazer impressao\n"
                "3 - Fazer remocao de uma associacao de disciplina\n"
                "4 - Carregar arquivo em lote\n"
                "5 - Finalizar o programa\n"
                "> ");

        scanf("%d%*c", &i);
        if(i == 1)
            cadastrar(arq_cursos, arq_disciplinas, arq_professores, arq_associacoes);
        if(i == 2)
            imprimir(arq_cursos, arq_disciplinas, arq_professores, arq_associacoes);
        if(i == 3)
            remover(arq_associacoes);
        if(i == 4)
            carregar_lote(arq_cursos, arq_disciplinas, arq_professores, arq_associacoes);
        if(i != 1 && i != 2 && i != 3 && i != 4 && i != 5)
            printf("(Digito invalido)\n");
    }
}

void cadastrar(FILE *arq_cursos, FILE *arq_disciplinas, FILE *arq_professores, FILE *arq_associacoes) {
    int i = -1;

    while (i != 5)
    {
        printf("\n1 - Cadastrar curso\n"
               "2 - Cadastrar disciplina\n"
               "3 - Cadastrar professor\n"
               "4 - Cadastrar associacao de disciplina\n"
               "5 - Voltar\n"
               "> ");
        scanf("%d%*c", &i);

        if(i == 1)
            cadastrar_curso(arq_cursos);
        if(i == 2)
            cadastrar_disciplina(arq_disciplinas);
        if(i == 3)
            cadastrar_professor(arq_professores);
        if(i == 4)
            cadastrar_associacao(arq_associacoes);
        if(i != 1 && i != 2 && i != 3 && i != 4 && i != 5)
            printf("(Digito invalido)\n");
    }
    
}

void cadastrar_curso(FILE *arq_cursos) {
    Curso *c = NULL;
    int codigo;
    char nome[100];
    char area;

    printf("\nDigite o codigo do curso\n"
           "> ");
    scanf("%d%*c", &codigo);

    printf("Digite o nome do curso\n"
           "> ");
    scanf("%s%*c", &nome);

    printf("Digite a area do curso\n"
           "> ");
    scanf("%c%*c", &area);

    c = criar_curso(codigo, nome, area);
    inserir_curso(arq_cursos, c);
    printf("(Curso inserido)\n");
}

void cadastrar_disciplina(FILE *arq_disciplinas) {
    Disciplina *d = NULL;
    int codigo;
    char nome[100];
    int codcurso;
    int serie;

    printf("\nDigite o codigo da disciplina\n"
           "> ");
    scanf("%d%*c", &codigo);

    printf("Digite o nome da disciplina\n"
           "> ");
    scanf("%s%*c", &nome);

    printf("Digite o codigo do curso da disciplina\n"
           "> ");
    scanf("%d%*c", &codcurso);

    printf("Digite a serie da disciplina\n"
           "> ");
    scanf("%d%*c", &serie);

    d = criar_disciplina(codigo, nome, codcurso, serie);
    inserir_disciplina(arq_disciplinas, d);
    printf("(Disciplina inserida)\n");
}

void cadastrar_professor(FILE* arq_professores) {
    Professor *p = NULL;
    int codigo;
    char nome[1000];

    printf("\nDigite o codigo do professor\n"
           "> ");
    scanf("%d%*c", &codigo);

    printf("Digite o nome do professor\n"
           "> ");
    scanf("%s%*c", &nome);

    p = criar_professor(codigo, nome);
    inserir_professor(arq_professores, p);
    printf("(Professor inserido)\n");
}

void cadastrar_associacao(FILE *arq_associacoes) {
    Associacao *a = NULL;
    int coddisciplina;
    int anoletivo;
    int codprofessor;

    printf("Digite o codigo da disciplina\n"
           "> ");
    scanf("%d%*c", &coddisciplina);

    printf("Digite o ano letivo\n"
           "> ");
    scanf("%d%*c", &anoletivo);

    printf("Digite o codigo do professor\n"
           "> ");
    scanf("%d%*c", &codprofessor);

    a = criar_associacao(coddisciplina, anoletivo, codprofessor);
    inserir_associacao(arq_associacoes, a);
    printf("(Distribuicao inserida)\n");
}

void imprimir(FILE *arq_cursos, FILE *arq_disciplinas, FILE *arq_professores, FILE *arq_associacoes) {
    int i = -1;

    while(i != 7) {
        printf("\n1 - Imprimir cursos\n"
               "2 - Imprimir disciplinas\n"
               "3 - Imprimir professores\n"
               "4 - Imprimir associacoes de disciplinas por níveis\n"
               "5 - Imprimir associacoes de disciplinas in-ordem\n"
               "6 - Imprimir registros livres\n"
               "7 - Voltar\n"
               "> ");
        scanf("%d%*c", &i);

        if(i == 1)
            print_inordem_cursos(arq_cursos);
        if(i == 2)
            print_inordem_disciplinas(arq_disciplinas);
        if(i == 3)
            print_inordem_professores(arq_professores);
        if(i == 4)
            print_arvore(arq_associacoes);
        if(i == 5)
            print_inordem_associacoes(arq_associacoes);
        if(i == 6)
            print_livres(arq_associacoes);
        if(i != 1 && i != 2 && i != 3 && i != 4 && i != 5 && i != 6 && i != 7)
            printf("(Digito invalido)\n");
    }
}

void remover(FILE *arq_associacoes) {
    char coddisciplina[6];
    char anoletivo[4];
    char* chave;

    printf("\nDigite o codigo da disciplina\n"
           "> ");
    scanf("%s%*c", &coddisciplina);

    printf("Digite o ano letivo\n"
           "> ");
    scanf("%s%*c", &anoletivo);

    sprintf(chave, "%d%d", anoletivo, coddisciplina);

    remover_associacao(arq_associacoes, chave);
    printf("(Associacao removida)\n");
}

void carregar_lote(FILE *arq_cursos, FILE *arq_disciplinas, FILE *arq_professores, FILE *arq_associacoes) {
    char arquivo[100];

    printf("\nDigite o nome do arquivo\n"
           "> ");
    scanf("%s%*c", &arquivo);

    ler_arq_txt(arquivo, arq_disciplinas,  arq_cursos, arq_professores, arq_associacoes);
    printf("(Arquivo lido)\n");
}