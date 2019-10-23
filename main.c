#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassFile.h"
#include "leitor.h"
#include "exibidor.h"

int main (int argc, char const *argv[])
{
  ClassFile *class_file;
  char *nomeArquivo; // ponteiro para o arquivo .class

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
