/*
Universidade de Brasília - 02/2019
Software Básico - Turma A
Leitor/Exibidor de arquivo .class

Alunos: 
    Brenda - 
    Jéssica - 
    Rafael Alencar - 
    Rafael Batista - 
    Rodrigo -

*/

#ifndef EXIBIDOR_H
#define EXIBIDOR_H

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "classFileStruct.h"
#include "instrucoes.h"

char* buscaNomeTag(u1 tag);
char* decodificaStringUTF8(cp_info *cp);
char* decodificarOperandoInstrucao(cp_info *cp, u2 index, u2 sizeCP);
char* decodificaNIeNT(cp_info *cp, u2 index,u1 tipo);
char* decodificaAccessFlags(u2 flag);
char* organizandoFlags(char* flagsOrdemInversa);
double decodificaDoubleInfo (cp_info * cp);
uint64_t decodificaLongInfo (cp_info * cp);
int decodificaIntegerInfo (cp_info * cp);
float decodificaFloatInfo (cp_info * cp);
int setaOffsetImpressao (int posicao, u1 offset);
void imprimirClassFile (ClassFile *, FILE *);


#endif