#include <stdio.h>
#include "sistema.h"
#include "lotes.h"
#include "menu.h"

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
    FILE *arq_disciplinas = open_arq("arq_disciplinas.bin");
    FILE *arq_cursos = open_arq("arq_cursos.bin");
    FILE *arq_professores = open_arq("arq_professores.bin");
    FILE *arq_associacoes = open_arq("arq_associacoes.bin");

    logo();
    menu_principal(arq_cursos, arq_disciplinas, arq_professores, arq_associacoes);

    fclose(arq_cursos);
    fclose(arq_disciplinas);
    fclose(arq_professores);
    fclose(arq_associacoes);

    // FILE *arq_associacoes = open_arq("arq_associacoes.bin");
    // Associacao *a = criar_associacao(10, 2020, 110);
    // Associacao *a1 = criar_associacao(11, 2021, 111);
    // Associacao *a2 = criar_associacao(11, 2021, 112);
    // Associacao *a3 = criar_associacao(13, 2023, 110);
    // Associacao *a4 = criar_associacao(14, 2023, 114);
    // Associacao *a5 = criar_associacao(15, 2025, 115);

    // inserir_associacao(arq_associacoes, a);
    // inserir_associacao(arq_associacoes, a1);
    // inserir_associacao(arq_associacoes, a2);
    // inserir_associacao(arq_associacoes, a3);
    // inserir_associacao(arq_associacoes, a4);
    // inserir_associacao(arq_associacoes, a5);

    // print_inordem_associacoes(arq_associacoes);

    return 0;
}