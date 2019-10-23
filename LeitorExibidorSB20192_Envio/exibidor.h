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
    "exibidor.h"

*/

#ifndef EXIBIDOR_INIT
#define EXIBIDOR_INIT

#include <stdio.h>
#include <stdlib.h>
#include "ClassFile.h"
#include "leitor.h"

char* buscaNomeTag(u1 tag);
char* decodificaStringUTF8(cp_info *cp);
char* decodificaNIeNT(cp_info *cp, u2 index, u1 tipo);
char* decodificaAccessFlags(u2 flag);
char* organizandoFlags(char* flagsOrdemInversa);
int decodificaIntegerInfo (cp_info * cp);
double decodificaDoubleInfo (cp_info * cp);
uint64_t decodificaLongInfo (cp_info * cp);
float decodificaFloatInfo (cp_info * cp);
int setaOffsetImpressao (int posicao, u1 offset);
void imprimirClassFile (ClassFile *);

#endif
