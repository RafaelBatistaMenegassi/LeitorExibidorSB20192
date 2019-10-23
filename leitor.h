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
manipualão de arquivos e definição de estruturas*/
#include <stdio.h>
#include <stdlib.h>
#include "exibidor.h"
#include "ClassFile.h"

ClassFile* lerArquivo (ClassFile * , char *);
cp_info* lerConstantPool (FILE *fp, cp_info *cp, u2 cp_count);
u2* lerInterfaces (FILE *fp, u2* interface, u2 i_count);
field_info* lerFields (FILE *fp, field_info *field, u2 f_count, cp_info *cp);
method_info* lerMetodos (FILE *fp, method_info* metodo, u2 m_count, cp_info *cp);
code_attribute* lerCode (FILE * fp, cp_info *cp);
line_number_table* lerLineNumberTable(FILE * fp, cp_info * cp);
exception_info* lerExceptionTable (FILE * fp, u2 size) ;
attribute_info* lerAtributos (FILE *fp, attribute_info* a, cp_info *cp);
exception_attribute* lerExceptionsAttribute (FILE *fp);
constant_value_attribute* lerConstantValue (FILE *fp);
signature_attribute* lerSignature (FILE *fp);
inner_classes_attribute* lerInnerClasses (FILE *fp, cp_info *cp);
classes* lerClasses (FILE *fp);
stack_map_attribute* lerStackMapTable (FILE *fp);
stack_map_frame* lerStackMapFrame (FILE *fp);
verification_type_info* lerVerificationTypeInfo (FILE *fp);
source_file_attribute* lerSourceFile (FILE  *fp);

#endif
