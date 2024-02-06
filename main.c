#include <stdio.h>
#include "sistema.h"

void imprimir_curso(Curso* c) {
    if(c == NULL) {
        printf("Estrutura vazia.\n");
        return;
    }
    printf("****** CURSO ********\n");
    printf("Codigo: %d\n"
            "Nome: %s\n"
            "Area: %c\n\n"
            , c->cod, c->nome, c->area);
}

void imprimir_disciplina(Disciplina* d) {
    if(d == NULL) {
        printf("Estrutura vazia.\n");
        return;
    }
    printf("******** DISCIPLINA *********\n");
    printf("Codigo: %d\n"
            "Nome: %s\n"
            "Codigo do Curso: %d\n"
            "Serie: %d\n\n"
            , d->cod, d->nome, d->codcurso, d->serie);
}

void imprimir_professor(Professor* p) {
    if(p == NULL) {
        printf("Estrutura vazia.\n");
        return;
    }
    printf("******** PROFESSOR *********\n");
    printf("Codigo: %d\n"
            "Nome: %s\n\n"
            , p->cod, p->nome);
}

void imprimir_associacao(Associacao* a) {
    if(a == NULL) {
        printf("Estrutura vazia.\n");
        return;
    }
    printf("******** DISTRIBUICAO DE DISCIPLINA *********\n");
    printf("Codigo da Disciplina: %d\n"
            "Ano Letivo: %d\n"
            "Codigo do Professor: %d\n\n"
            ,a->coddisciplina, a->anoletivo, a->codprofessor);
}


int main() {
    FILE *arq_disciplinas = open_arq("arq_disciplinas.txt");
    Disciplina *d0 = criar_disciplina(2, "banco de dados2", 11, 3);
    Disciplina *d1 = criar_disciplina(1, "banco de dados1", 10, 3);
    Disciplina *d2 = criar_disciplina(3, "banco de dados3", 10, 4);

    inserir_disciplina(arq_disciplinas, d0);
    inserir_disciplina(arq_disciplinas, d1);
    inserir_disciplina(arq_disciplinas, d2);

    print_inordem(arq_disciplinas);

    return 0;
}