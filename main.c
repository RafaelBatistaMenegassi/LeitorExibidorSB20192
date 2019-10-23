#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char const *argv[])
{
  //Classfile *class_file;
  char *nomeArquivo; // ponteiro para o arquivo .class

  if(argc > 1) {
    nomeArquivo = (char *) malloc(strlen(argv[1]) * sizeof(char));
    strcpy(nomeArquivo, argv[1]);
  } else {
    printf("O nome do arquivo .class nao foi incluido.\nO programa sera encerrado.......\n");
    exit(0);
  }

  class_file = (Classfile *) malloc(sizeof(Classfile));

  class_file = leArquivo(class_file, nomeArquivo);

  imprimeClassfile(class_file);

  return 0;
}
