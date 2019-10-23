/*
Universidade de Brasília - 02/2019
Software Básico - Turma A

Alunos:
    Brenda Souza
    Jéssica Oliveira
    Rafael Alencar
    Rafael Batista
    Rodrigo Cardoso

*/

#ifndef EXIBIDOR_INIT
#define EXIBIDOR_INIT

#include <stdio.h>
#include <stdlib.h>
#include "ClassFile.h"
#include "leitor.h"

char* buscaNomeTag(u1 tag);
char* decodificaStringUTF8(cp_info *cp);
//char* decodificarOperandoInstrucao(cp_info *cp, u2 index, u2 sizeCP);
char* decodificaNIeNT(cp_info *cp, u2 index, u1 tipo);
char* decodificaAccessFlags(u2 flag);
char* organizandoFlags(char* flagsOrdemInversa);
double decodificaDoubleInfo (cp_info * cp);
uint64_t decodificaLongInfo (cp_info * cp);
//int decodificaIntegerInfo (cp_info * cp);
//float decodificaFloatInfo (cp_info * cp);
int setaOffsetImpressao (int posicao, u1 offset);
void imprimirClassFile (ClassFile *);

#endif
