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
#include "classfile.h"
#include "leitor.h"

void imprimirClassfile (Classfile *);
void imprimeConstantPool (Classfile *);
void imprimeFields (Classfile *);
void imprimeMetodos (Classfile *);
void imprimeAtributos (Classfile *);

char* decodificarCode(Cp_info *cp, u2 sizeCP, u1 *code, u4 length,instrucao *instrucoes);
char* buscaNomeTag(u1 tag);
char* decodificaStringUTF8(Cp_info *cp);
char* decodificarOperandoInstrucao(Cp_info *cp, u2 index, u2 sizeCP);
char* decodificaNIeNT(Cp_info *cp, u2 index, u1 tipo);
char* decodificaAccessFlags(u2 flag);
char* organizandoFlags(char *flagsOrdemInversa);
double decodificaDoubleInfo (Cp_info *cp);
uint64_t decodificaLongInfo (Cp_info *cp);
int decodificaIntegerInfo (Cp_info * cp);
float decodificaFloatInfo (Cp_info * cp);
int setaOffsetImpressao (int posicao, u1 offset);


#endif
