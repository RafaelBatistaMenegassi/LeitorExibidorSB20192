#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Testar programa retirando aqui!
#include "classfile.h"
#include "leitor.h"
#include "exibidor.h"

int main (int argc, char *argv[])
{
  Classfile *class_file;
  char *nomeArquivo; // string que contem o nome do arquivo .class

  /* Verifica se o nome do arquivo .class foi passado na execução do programa.
   * Se foi, nomeArquivo contem essa string.
   */
  if(argc > 1) {
    nomeArquivo = (char *) malloc(strlen(argv[1]) * sizeof(char));
    strcpy(nomeArquivo, argv[1]);
  } else {
    printf("O nome do arquivo .class nao foi incluido.\nO programa sera encerrado.......\n");
    exit(0);
  }

  /* Cria estrutura do tipo class file*/
  class_file = (Classfile *) malloc(sizeof(struct classfile));

  /* Le o arquivo e armazena nas esttruturas correspondentes e retorna o ponteiro
   * para a estrutura do tipo classfile.
   */
  class_file = lerArquivo(class_file, nomeArquivo);

  /* Imprime as informacoes lidas do arquivo .class */
  imprimirClassfile(class_file);

  return 0;
}
