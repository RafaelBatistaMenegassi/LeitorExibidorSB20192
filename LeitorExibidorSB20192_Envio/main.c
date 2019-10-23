/*
Universidade de Brasília - 02/2019
Software Básico - Turma A

Alunos:
    - Brenda Barbosa de Souza - 120111918
    - Jéssica da Silva Oliveira - 13/0028983
    - Rafael Batista Menegassi - 14/0159355
    - Rafael Silva de Alencar - 130130834
    - Rodrigo Neris Ferreira Cardoso - 140161597

Projeto:
    Leitor/Exibidor de arquivos .class.

Arquivo:
    "main.c"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassFile.h"
#include "leitor.h"
#include "exibidor.h"

int main (int argc, char *argv[])
{
  ClassFile *class_file;
  char *nomeArquivo; 

  if(argc > 1) {
    nomeArquivo = (char *) malloc(strlen(argv[1]) * sizeof(char));
    strcpy(nomeArquivo, argv[1]);
  } else {
    printf("O nome do arquivo .class nao foi incluido.\nO programa sera encerrado.......\n");
    exit(0);
  }

  class_file = (ClassFile *) malloc(sizeof(ClassFile));

  class_file = lerArquivo(class_file, nomeArquivo);

  imprimirClassFile(class_file);

  return 0;
}
