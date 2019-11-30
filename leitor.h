/*
Universidade de Brasília - 02/2019
Software Básico - Turma A
Leitor/Exibidor de arquivo .class

Alunos:
    Brenda Souza
    Jéssica Oliveira
    Rafael Alencar
    Rafael Batista
    Rodrigo Cardoso

*/

/*Macro para verificação de inclusão de arquivos de estrutura,
bibliotecas de leitura/escrita de dados e manipulação de arquivos
*/
#ifndef LEITURA_INIT
#define LEITURA_INIT

/*Inclusão de bibliotecas para leitura/escrita de dados,
manipulação de arquivos e definição de estruturas*/
#include <stdio.h>
#include <stdlib.h>
#include "exibidor.h"
#include "classfile.h"

/* FUNÇÃO PRINCIPAL PARA LEITURA DO ARQUIVO .CLASS */
Classfile* lerArquivo (Classfile * , char *);

/* FUNÇÕES AUXILIARES PARA LEITURA DAS PARTES ESPECÍFICAS DO ARQUIVO .CLASS */
Cp_info* lerConstantPool (FILE *, Cp_info *, u2);
u2* lerInterfaces (FILE *fp, u2 *interface, u2 i_count);
Field_info* lerFields (FILE *fp, Field_info *field, u2 f_count, Cp_info *cp);
Method_info* lerMetodos (FILE *fp, Method_info* metodo, u2 m_count, Cp_info *cp);
Attribute_info* lerAtributos (FILE *fp, Attribute_info* a, Cp_info *cp);
Code_attribute* lerCode (FILE * fp, Cp_info *cp);
Line_number_table* lerLineNumberTable(FILE * fp, Cp_info * cp);
Exception_info* lerExceptionTable (FILE * fp, u2 size);
Exception_attribute* lerExceptionsAttribute (FILE *fp);
Constant_value_attribute* lerConstantValue (FILE *fp);
Signature_attribute* lerSignature (FILE *fp);
Inner_classes_attribute* lerInnerClasses (FILE *fp, Cp_info *cp);
Classes* lerClasses (FILE *fp);
Stack_map_attribute* lerStackMapTable (FILE *fp);
Stack_map_frame* lerStackMapFrame (FILE *fp);
Verification_type_info* lerVerificationTypeInfo (FILE *fp);
Source_file_attribute* lerSourceFile (FILE  *fp);

#endif
