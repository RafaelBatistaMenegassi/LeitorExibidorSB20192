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
/*Macro para verificação de inclusão de arquivos de estrutura,
bibliotecas de leitura/escrita de dados e manipulação de arquivos
*/
#ifndef LEITURA_H
#define LEITURA_H

/*Inclusão de bibliotecas para leitura/escrita de dados,
manipualão de arquivos e definição de estruturas*/
#include <stdio.h>
#include <stdlib.h>
#include "exibidor.h"
#include "ClassFile.h"

Classfile* lerArquivo (Classfile * , char *);
Cp_info* lerConstantPool (FILE *fp, Cp_info *cp, u2 cp_count);
u2* lerInterfaces (FILE *fp, u2* interface, u2 i_count);
Field_info* lerFields (File *fp, Field_info *field, u2 f_count, Cp_info *cp);
Method_info* lerMetodos (FILE *fp, Method_info* metodo, u2 m_count, Cp_info *cp)
code_attribute* lerCode (FILE * fp, cp_info *cp);
line_number_table* lerLineNumberTable(FILE * fp, cp_info * cp);
exception_table* lerExceptionTable (FILE * fp, u2 size) ;
Attribute_info* lerAtributos (FILE *fp, Attribute_info* a, Cp_info *cp);
exceptions_attribute* lerExceptionsAttribute (FILE *fp);
constantValue_attribute* lerConstantValue (FILE *fp);
signature_attribute* lerSignature (FILE *fp);
innerClasses_attribute* lerInnerClasses (FILE *fp, cp_info *cp);
classes* lerClasses (FILE *fp);
stackMapTable_attribute* lerStackMapTable (FILE *fp);
stack_map_frame* lerStackMapFrame (FILE *fp)
verification_type_info* lerVerificationTypeInfo (FILE *fp);
source_file_attribute* lerSourceFile (FILE  fp);

#endif
