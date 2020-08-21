#include "conjuntoInt.h"
#include <stdio.h>
#include <stdlib.h>
#define DEBUG(x) printf("%ld\n", x);

struct conjuntoInt
{
  long *vetor;
  u_long tam;
};

// tentativa de algoritmo bisseccao baseado em calculo numérico 2ºsem 2019
u_long retornaIndexDoVetor(long *vetor, u_long limiteSuperior, long numeroProcurado)
{
  u_long limiteInferior = 0;
  if (numeroProcurado < vetor[limiteInferior] || vetor[limiteSuperior] < numeroProcurado)
    return 0;
  else if (vetor[limiteInferior] == numeroProcurado)
    return limiteInferior;
  else if (vetor[limiteSuperior] == numeroProcurado)
    return limiteSuperior;
  else
  {
    while (TRUE)
    {
      u_long mediana = limiteInferior + (limiteSuperior - limiteInferior) / 2;
      if (numeroProcurado < vetor[mediana] && vetor[limiteInferior] != numeroProcurado)
        limiteSuperior = mediana;
      else if (vetor[mediana] < numeroProcurado && vetor[limiteSuperior] != numeroProcurado)
        limiteInferior = mediana;
      else if (vetor[mediana] == numeroProcurado)
        return mediana;
      if ((limiteInferior + 1) == limiteSuperior)
        return 0;
    }
  }
}

void insertVector(long *vector, u_long tam, long *item)
{
  u_long index = retornaIndexDoVetor(vector, tam, *item);
  printf("tam vector %ld index %ld vector value %ld value add %ld\n", tam, index, vector[index], *item);
  if (tam > 1)
  {
    for (u_long i = tam; index < i; i--)
    {
      vector[i] = vector[1 - i];
    }
    vector[index] = *item;
  }
  else
  {
    vector[0] = *item;
  }
}

void insere(CONJUNTO *conjunto, long valorAdicionar)
{
  conjunto->vetor = realloc(conjunto->vetor, (conjunto->tam + 1) * sizeof(long));
  if (conjunto->vetor == NULL)
  {
    perror("Erro na alocacao do conjunto\n");
    exit(EXIT_FAILURE);
  }
  insertVector(conjunto->vetor, conjunto->tam++, &valorAdicionar);
}

CONJUNTO *criaConjuntoVazio()
{
  CONJUNTO *ptr = malloc(sizeof(CONJUNTO));
  ptr->vetor = malloc(sizeof(long));
  if (ptr == NULL || ptr->vetor == NULL)
  {
    perror("Erro na criação do conjunto");
    exit(EXIT_FAILURE);
  }
  ptr->tam = 0;
  return ptr;
}

void finalizaCopiaParaUniao(CONJUNTO *conjuntoUniao, CONJUNTO *aFinalizar, u_long indice)
{
  while (indice < aFinalizar->tam)
    insere(conjuntoUniao, aFinalizar->vetor[indice++]);
}

CONJUNTO *uniao(CONJUNTO *conj1, CONJUNTO *conj2)
{
  CONJUNTO *uniaoConjuntos = criaConjuntoVazio();
  u_long i, tam = conj1->tam < conj2->tam ? conj1->tam : conj2->tam;
  for (i = 0; i < tam; i++)
  {
    insere(uniaoConjuntos, conj1->vetor[i]);
    insere(uniaoConjuntos, conj2->vetor[i]);
  }
  conj1->tam > conj2->tam ? finalizaCopiaParaUniao(uniaoConjuntos, conj1, i) : finalizaCopiaParaUniao(uniaoConjuntos, conj2, i);
  return uniaoConjuntos;
}

int removeItemConjunto(CONJUNTO *conj, long itemRetirar)
{
  u_long index = retornaIndexDoVetor(conj->vetor, conj->tam, itemRetirar);
  if (index)
  {
    while (index < conj->tam)
    {
      conj->vetor[index] = conj->vetor[index + 1];
      index++;
    }
    long *tmp = realloc(conj->vetor, (--conj->tam) * sizeof(long));
    if (tmp == NULL)
    {
      fprintf(stderr, "Erro na remoção do item '%ld' no conjunto\n", itemRetirar);
      exit(EXIT_FAILURE);
    }
    conj->vetor = tmp;
    return TRUE;
  }
  return FALSE;
}

CONJUNTO *interseccao(CONJUNTO *conj1, CONJUNTO *conj2)
{
  CONJUNTO *interseccaoConjuntos = criaConjuntoVazio();
  for (u_long i = 0; i < conj1->tam; i++)
  {
    u_long tmp = retornaIndexDoVetor(conj2->vetor, conj2->tam, conj1->vetor[i]);
    if (tmp)
    {
      insere(interseccaoConjuntos, conj1->vetor[i]);
    }
  }
  return interseccaoConjuntos;
}

CONJUNTO *diferenca(CONJUNTO *conj1, CONJUNTO *conj2)
{
  CONJUNTO *conjDiferenca = uniao(conj1, conj2),
           *conjInterseccao = interseccao(conj1, conj2);

  for (u_long i = 0; i < conjInterseccao->tam; i++)
  {
    removeItemConjunto(conjDiferenca, conjInterseccao->vetor[i]);
  }
  liberaConjunto(conjInterseccao);
  return conjDiferenca;
}

int main(int argc, char const *argv[])
{
  CONJUNTO *ptr = criaConjuntoVazio(),
           *ptr2 = criaConjuntoVazio(),
           *ptrDiferenca = NULL;

  for (long i = 0; i < 5; i++)
  {
    insere(ptr, i);
    // insere(ptr2, i + 4);
  }

  printConjunto(ptr);
  // printConjunto(ptr2);

  // ptrDiferenca = uniao(ptr, ptr2);
  // printConjunto(ptrDiferenca);

  return 0;
}

long menor(CONJUNTO *conjunto)
{
  return conjunto->vetor[0];
}

long maior(CONJUNTO *conjunto)
{
  return conjunto->vetor[conjunto->tam];
}

int testaSeOsConjuntosSaoIguais(CONJUNTO *conjunto1, CONJUNTO *conjunto2)
{
  long tam;
  if ((tam = conjunto1->tam) == conjunto2->tam)
  {
    for (u_long i = 0; i < tam; i++)
    {
      if (conjunto1->vetor[i] != conjunto2->vetor[i])
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

u_long tamanho(CONJUNTO *conjunto)
{
  return conjunto->tam;
}

int testaSeoConjuntoVazio(CONJUNTO *conjunto)
{
  if (!conjunto->tam)
  {
    return TRUE;
  }
  return FALSE;
}

void liberaConjunto(CONJUNTO *conj)
{
  free(conj->vetor);
  free(conj);
}

void printConjunto(CONJUNTO *conj)
{
  for (u_long i = 0; i < conj->tam; i++)
  {
    printf("%ld,", conj->vetor[i]);
  }
  putchar('\n');
}