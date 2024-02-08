#ifndef LOTES
#define LOTES
#include <stdio.h>

// Insere uma disciplina na árvore (arquivo)
// Pré-condição: arq_disciplinas aberto para leitura
// Pós-condição: disciplina inserida no arquivo
void inserir_lote_disciplina(FILE *arq_disciplinas);

// Insere um curso na árvore (arquivo)
// Pré-condição: arq_cursos aberto para leitura
// Pós-condição: curso inserido no arquivo
void inserir_lote_curso(FILE *arq_cursos);

// Insere um professor na árvore (arquivo)
// Pré-condição: arq_professores aberto para leitura
// Pós-condição: professor inserido no arquivo
void inserir_lote_professor(FILE *arq_professores);

// Insere uma associação na árvore (arquivo)
// Pré-condição: arq_associacoes aberto para leitura
// Pós-condição: associacao inserida no arquivo
void inserir_lote_associacao(FILE *arq_associacoes);

// Remove uma associação da árvore
// Pré-condição: arq_associacoes aberto para escrita
// Pós-condição: associação removida do arquivo
void remover_lote_associacao(FILE *arq_associacoes);

// Lê o arquivo inteiro dado pela string
// Pré-condições: string contendo o nome do arquivo.extensão e arquivos abertos de cada estrutura
// Pós-condições: nenhuma
void ler_arq_txt(char *str, FILE *arq_disciplinas, FILE *arq_cursos, FILE *arq_professores, FILE *arq_associacoes);

#endif