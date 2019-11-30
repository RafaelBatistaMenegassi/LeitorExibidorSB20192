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
    "leitor.c"

*/

#include "leitor.h"
#include <string.h>

/*Macro para leitura de um tipo u1 do arquivo .class*/
#define u1READ(arq) getc(arq)
/*Macro para leitura de um tipo u2 do arquivo .class*/
#define u2READ(arq) (getc(arq) << 8) | getc(arq)
/*Macro para leitura de um tipo u4 do arquivo .class*/
#define u4READ(arq) (getc(arq) << 24) | (getc(arq) << 16 | getc(arq) << 8 | getc(arq))

ClassFile* lerArquivo (ClassFile* cf, char *nomeArquivo)
{
    FILE *fp = fopen(nomeArquivo, "rb"); // abre o arquivo .class em modo leitura binario

    if(!fp){ // se nao encontrou arquivo ou nao conseguiu abrir, encerra o programa
      printf("Erro na abertura do arquivo .class, o programa sera encerrado...\n");
      exit(0);
    }

    cf->magic = u4READ(fp);
    cf->minor_version = u2READ(fp);
    cf->major_version = u2READ(fp);

    cf->constant_pool_count = u2READ(fp);
    cf->constant_pool = lerConstantPool(fp, cf->constant_pool, cf->constant_pool_count);

    cf->access_flags = u2READ(fp);
    cf->this_class = u2READ(fp);
    cf->super_class = u2READ(fp);

    cf->interfaces_count = u2READ(fp);
    cf->interfaces = (cf->interfaces_count > 0) ?
                     lerInterfaces(fp, cf->interfaces, cf->interfaces_count) : NULL;

    cf->fields_count = u2READ(fp);
    cf->fields = (cf->fields_count > 0) ?
                 lerFields(fp, cf->fields, cf->fields_count, cf->constant_pool) : NULL;

    /*Leitura do valor 'methods_count' = quantidade de entradas na tabela Method*/
    cf->methods_count = u2READ(fp);
    cf->methods = (cf->methods_count > 0) ?
                   lerMetodos(fp, cf->methods, cf->methods_count, cf->constant_pool) : NULL;  // ASS: (FILE *fp, method_info* metodo, u2 m_count, cp_info *cp)

    cf->attributes_count = u2READ(fp);
    if(cf->attributes_count > 0)
    {
      cf->attributes = (attribute_info **) malloc(cf->attributes_count * sizeof(attribute_info*));
      for (int i = 0; i < cf->attributes_count; i++) {
        *(cf->attributes + i) = lerAtributos(fp, *(cf->attributes + i), cf->constant_pool);
      }
    }

    fclose(fp);

    return cf;
}

cp_info* lerConstantPool (FILE *fp, cp_info *cp, u2 cp_count)
{
  cp = (cp_info *) malloc(cp_count * sizeof(cp_info));

  for(cp_info *p = cp; p < cp + cp_count - 1; p++){
    p->tag = u1READ(fp);

    switch (p->tag) {
      case CONSTANT_Class:
				p->cp_union.Class.name = u2READ(fp);
				break;

			case CONSTANT_Field:
				p->cp_union.Field.class_name = u2READ(fp);
				p->cp_union.Field.name_and_type = u2READ(fp);
				break;

			case CONSTANT_Method:
				p->cp_union.Method.class_name = u2READ(fp);
				p->cp_union.Method.name_and_type = u2READ(fp);
				break;

			case CONSTANT_InterfaceMethod:
				p->cp_union.InterfaceMethod.class_name = u2READ(fp);
				p->cp_union.InterfaceMethod.name_and_type = u2READ(fp);
				break;

			case CONSTANT_String:
				p->cp_union.String.string = u2READ(fp);
				break;

			case CONSTANT_Integer:
				p->cp_union.Integer.bytes = u4READ(fp);
				break;

			case CONSTANT_Float:
				p->cp_union.Float.bytes = u4READ(fp);
				break;

			case CONSTANT_Long:
				p->cp_union.Long.hi_bytes = u4READ(fp);
				p->cp_union.Long.lo_bytes = u4READ(fp);
				p++;
				break;

			case CONSTANT_Double:
				p->cp_union.Double.hi_bytes = u4READ(fp);
				p->cp_union.Double.lo_bytes = u4READ(fp);
				p++;
				break;

			case CONSTANT_NameAndType:
				p->cp_union.NameAndType.name = u2READ(fp);
				p->cp_union.NameAndType.descriptor = u2READ(fp);
				break;

			case CONSTANT_Utf8:
				p->cp_union.Utf8.length = u2READ(fp);
				p->cp_union.Utf8.bytes = malloc(p->cp_union.Utf8.length*sizeof(u1));
				for (u1 *i=p->cp_union.Utf8.bytes;i<p->cp_union.Utf8.bytes+p->cp_union.Utf8.length;i++){
					*i = u1READ(fp);
				}
				break;

			case CONSTANT_MethodHandle:
				p->cp_union.MethodHandle.reference_kind = u1READ(fp);
				p->cp_union.MethodHandle.reference = u2READ(fp);
				break;

			case CONSTANT_MethodType:
				p->cp_union.MethodType.descriptor = u2READ(fp);
				break;

			case CONSTANT_InvokeDynamic:
				p->cp_union.InvokeDynamic.bootstrap_method_attr = u2READ(fp);
				p->cp_union.InvokeDynamic.name_and_type = u2READ(fp);
				break;

			default:
				break;
    }
  }

  return cp;
}

u2* lerInterfaces (FILE *fp, u2* interface, u2 i_count)
{
    interface = (u2 *) malloc(i_count * sizeof(u2));
    for (u2 *p = interface; p < interface + i_count; p++){
      *interface = u2READ(fp);
    }

    return interface;
}

field_info* lerFields (FILE *fp, field_info *field, u2 f_count, cp_info *cp)
{
    field = (field_info *) malloc(f_count * sizeof(field_info));

    for(field_info *p = field; p < field + f_count; p++){
      p->access_flags = u2READ(fp);
  		p->name = u2READ(fp);
  		p->descriptor = u2READ(fp);
  		p->attributes_count = u2READ(fp);

  		if (p->attributes_count > 0) {
  			p->attributes = (attribute_info **) malloc(p->attributes_count * sizeof(attribute_info*));
  			for (int i = 0; i < p->attributes_count; p++) {
  				*(p->attributes + i) = lerAtributos(fp, *(p->attributes + i), cp); // ASS: (FILE *fp, attribute_info* a, cp_info *cp)
  			}
  		}
    }

    return field;
}

method_info* lerMetodos (FILE *fp, method_info* metodo, u2 m_count, cp_info *cp)
{
  metodo = (method_info *) malloc(m_count * sizeof(method_info));

  for(method_info *p = metodo; p < metodo + m_count; p++){
    p->access_flags = u2READ(fp);
    p->name = u2READ(fp);
    p->descriptor = u2READ(fp);
    p->attributes_count = u2READ(fp);

    //printf("access_flags: %02x\n",i->access_flags);
		//printf("Name index method: %d\n",i->name);

		/*Estrutura condicional que avalia se a quantidade de atributos
		do método é maior que zero. Se for, prossegue com a leitura dos
		atributos do método*/
		attribute_info *atributos = (attribute_info *) malloc(p->attributes_count * sizeof(attribute_info));

		if (p->attributes_count > 0) {
			p->attributes = (attribute_info **) malloc(p->attributes_count * sizeof(attribute_info*));
			for (int i = 0; i < p->attributes_count; i++) {
				*(p->attributes + i) = lerAtributos(fp,	*(p->attributes + i), cp);
			}
		}
  }

  return metodo;
}

code_attribute* lerCode (FILE * fp, cp_info *cp)
{
	code_attribute * code_attributes = NULL;
	code_attributes = (code_attribute*) malloc(sizeof(code_attribute));
	code_attributes->max_stack = u2READ(fp);
	code_attributes->max_locals = u2READ(fp);
	code_attributes->code_length = u4READ(fp);
	if(code_attributes->code_length > 0) {
		code_attributes->code = malloc(code_attributes->code_length*sizeof(u1));
		for(u1 *p = code_attributes->code; p < code_attributes->code + code_attributes->code_length; p++){
			*p = u1READ(fp);
		}
	}

	code_attributes->exception_info_length = u2READ(fp);

	if(code_attributes->exception_info_length > 0){
		code_attributes->ex_info = NULL;//malloc(code_attributes->exception_info_length*sizeof(exception_info));
		code_attributes->ex_info = lerExceptionTable(fp, code_attributes->exception_info_length);
	}

	code_attributes->attributes_count = u2READ(fp);

	if (code_attributes->attributes_count > 0){
		code_attributes->attributes = (attribute_info**)malloc(code_attributes->attributes_count*sizeof(attribute_info*));
		for (int i = 0; i < code_attributes->attributes_count; i++) {
			*(code_attributes->attributes + i) = lerAtributos(fp, *(code_attributes->attributes + i), cp);
		}
	}

	return code_attributes;
}

line_number_table* lerLineNumberTable(FILE * fp, cp_info * cp)
{
	line_number_table * lnt = (line_number_table*)malloc(sizeof(line_number_table));
	lnt->line_number_length = u2READ(fp);
	if (lnt->line_number_length > 0) {
		lnt->info = (line_number_info*)malloc(lnt->line_number_length*sizeof(line_number_info));
		for (line_number_info * linfo = lnt->info; linfo < lnt->info+lnt->line_number_length; linfo++) {
			linfo->start_pc = u2READ(fp);
			linfo->line_number = u2READ(fp);
		}
	}
	return lnt;
}

exception_info* lerExceptionTable (FILE * fp, u2 size)
{
	exception_info * exception_tableAux = (exception_info*) malloc(size*sizeof(exception_info));
	for(exception_info * e = exception_tableAux; e < exception_tableAux + size; e++){
		e->start_pc = u2READ(fp);
		e->end_pc = u2READ(fp);
		e->handler_pc = u2READ(fp);
		e->catch_type = u2READ(fp);
	}
	return exception_tableAux;
}

attribute_info* lerAtributos (FILE *fp, attribute_info* a, cp_info *cp)
{
  a = (attribute_info *) malloc(sizeof(attribute_info));

  a->attribute_name = u2READ(fp);
  a->attribute_length = u4READ(fp);

  //printf("att name index: %d\n",attributes->attribute_name);
	//printf("attr length: %d\n",attributes->attribute_length);

  if (a->attribute_length > 0) {
			char *string_name;
			string_name = decodificaStringUTF8(cp + a->attribute_name-1);

			if(strcmp(string_name, "SourceFile") == 0){

				source_file_attribute *SourceFile = NULL;//(code_attribute*) malloc(i->attributes_count*sizeof(code_attribute));
				SourceFile = lerSourceFile(fp);
				a->info = NULL;//malloc(i->attributes_count*sizeof(code_attribute));
				a->info = (source_file_attribute*) SourceFile;
			}
      else if (strcmp(string_name,"Code") == 0) {
				printf("Entrou code\n");
				code_attribute *code_attr = NULL;
				code_attr = lerCode(fp ,cp);
				a->info = (code_attribute*) code_attr;
			}
      else if (strcmp(string_name,"LineNumberTable") == 0) {
				line_number_table *lnt = NULL;
				printf("Entrou lnt\n");
				lnt = lerLineNumberTable(fp, cp);
				a->info = (line_number_table*)lnt;
			}
      else if (strcmp(string_name,"StackMapTable") == 0) {
				printf("Entrou stack map table\n");
				stack_map_attribute *stackMapTable = NULL;
				stackMapTable = lerStackMapTable(fp);
				printf("Saiu de smt\n");
				a->info = (stack_map_attribute*)stackMapTable;
			}
      else if (strcmp(string_name, "InnerClasses") == 0) {
				printf("Entrou em inner class\n");
				inner_classes_attribute *innerClasses = NULL;
				innerClasses = lerInnerClasses(fp, cp);
				a->info = (inner_classes_attribute*)innerClasses;
			}
      else if (strcmp(string_name,"Signature") == 0) {
				printf("Entrou em signature\n");
				signature_attribute *signatureR = NULL;
				signatureR = lerSignature(fp);
				a->info = (signature_attribute*)signatureR;
			}
      else if (strcmp(string_name,"ConstantValue") == 0) {
				constant_value_attribute *constantV = NULL;
				constantV = lerConstantValue(fp);
				a->info = (constant_value_attribute*)constantV;
			}
      else if (strcmp(string_name,"Exceptions") == 0) {
				exception_attribute *exceptions = NULL;
				exceptions = lerExceptionsAttribute(fp);
				a->info = (exception_attribute*) exceptions;
			}
	}

	return a;
}

exception_attribute* lerExceptionsAttribute (FILE *fp)
{
	exception_attribute * exceptions = (exception_attribute*)malloc(sizeof(exception_attribute));
	exceptions->num_exceptions = u2READ(fp);
	exceptions->exception_info = NULL;
	if (exceptions->num_exceptions > 0) {
		exceptions->exception_info = (u2*)malloc(exceptions->num_exceptions*sizeof(u2));
		for (u2 * excpAux = exceptions->exception_info; excpAux < exceptions->exception_info + exceptions->num_exceptions; excpAux++) {
			*excpAux = u2READ(fp);
		}
	}
	return exceptions;
}

constant_value_attribute* lerConstantValue (FILE *fp)
{
	constant_value_attribute * cv = (constant_value_attribute*)malloc(sizeof(constant_value_attribute));
	cv->constant_value = u2READ(fp);
	return cv;
}

signature_attribute* lerSignature (FILE *fp)
{
	signature_attribute * signature = (signature_attribute*)malloc(sizeof(signature_attribute));
	signature->signature = u2READ(fp);
	return signature;
}

inner_classes_attribute* lerInnerClasses (FILE *fp, cp_info *cp)
{
	inner_classes_attribute * innerClasses = (inner_classes_attribute*)malloc(sizeof(inner_classes_attribute));
	innerClasses->num_classes = u2READ(fp);
	if (innerClasses->num_classes > 0) {
		innerClasses->classes_array = (classes**)malloc(innerClasses->num_classes*sizeof(classes*));
		for (int posicao = 0; posicao < innerClasses->num_classes; posicao++) {
			*(innerClasses->classes_array+posicao) = lerClasses(fp);
		}
	}
	return innerClasses;
}

classes* lerClasses (FILE *fp)
{
	classes *classeRetorno = (classes*)malloc(sizeof(classes));
	classeRetorno->inner_class_info = u2READ(fp);
	classeRetorno->outer_class_info = u2READ(fp);
	classeRetorno->inner_name = u2READ(fp);
	classeRetorno->inner_class_access_flags = u2READ(fp);

	return classeRetorno;
}

stack_map_attribute* lerStackMapTable (FILE *fp)
{
	stack_map_attribute * stackMapTable = (stack_map_attribute*)malloc(sizeof(stack_map_attribute));
	stackMapTable->num_entries = u2READ(fp);
	if (stackMapTable->num_entries > 0) {
		stackMapTable->entries = (stack_map_frame**)malloc(stackMapTable->num_entries*sizeof(stack_map_frame*));
		for (int posicao = 0; posicao < stackMapTable->num_entries; posicao++) {
			*(stackMapTable->entries+posicao) = lerStackMapFrame(fp);
		}
	}
	return stackMapTable;
}

stack_map_frame* lerStackMapFrame (FILE *fp)
{
	stack_map_frame * StackMapFrame = (stack_map_frame*)malloc(sizeof(stack_map_frame));
	StackMapFrame->frame_type = u1READ(fp);
	if (StackMapFrame->frame_type >= 0 && StackMapFrame->frame_type <= 63) {
	} else if (StackMapFrame->frame_type >= 64 && StackMapFrame->frame_type <= 127) {
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame.stack = (verification_type_info**)malloc(sizeof(verification_type_info*));
		*(StackMapFrame->map_frame_type.same_locals_1_stack_item_frame.stack) = lerVerificationTypeInfo(fp);
	} else if (StackMapFrame->frame_type == 247) {
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta = u2READ(fp);
		StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.stack = (verification_type_info**)malloc(sizeof(verification_type_info*));
		*(StackMapFrame->map_frame_type.same_locals_1_stack_item_frame_extended.stack) = lerVerificationTypeInfo(fp);
	} else if (StackMapFrame->frame_type >= 248 && StackMapFrame->frame_type <= 250) {
		StackMapFrame->map_frame_type.chop_frame.offset_delta = u2READ(fp);
	} else if (StackMapFrame->frame_type == 251) {
		StackMapFrame->map_frame_type.same_frame_extended.offset_delta = u2READ(fp);
	} else if (StackMapFrame->frame_type >= 252 && StackMapFrame->frame_type <= 254) {
		StackMapFrame->map_frame_type.append_frame.offset_delta = u2READ(fp);
		u2 sizeMalloc = (StackMapFrame->frame_type-251);
		StackMapFrame->map_frame_type.append_frame.locals = (verification_type_info**)malloc(sizeMalloc*sizeof(verification_type_info*));
		for (int posicao = 0; posicao < sizeMalloc; posicao++) {
			*(StackMapFrame->map_frame_type.append_frame.locals+posicao) = lerVerificationTypeInfo(fp);
		}
	} else if (StackMapFrame->frame_type == 255) {
		StackMapFrame->map_frame_type.full_frame.offset_delta = u2READ(fp);
		StackMapFrame->map_frame_type.full_frame.num_locals = u2READ(fp);
		if (StackMapFrame->map_frame_type.full_frame.num_locals > 0) {
			StackMapFrame->map_frame_type.full_frame.locals = (verification_type_info**)malloc(StackMapFrame->map_frame_type.full_frame.num_locals*sizeof(verification_type_info*));
			for (int posicao = 0; posicao < StackMapFrame->map_frame_type.full_frame.num_locals; posicao++) {
				*(StackMapFrame->map_frame_type.full_frame.locals+posicao) = lerVerificationTypeInfo(fp);
				if ((*(StackMapFrame->map_frame_type.full_frame.locals+posicao))->tag == 7) {
				}
			}
		}
		StackMapFrame->map_frame_type.full_frame.num_stack_items = u2READ(fp);
		if (StackMapFrame->map_frame_type.full_frame.num_stack_items > 0) {
			StackMapFrame->map_frame_type.full_frame.stack = (verification_type_info**)malloc(StackMapFrame->map_frame_type.full_frame.num_stack_items*sizeof(verification_type_info*));
			for (int posicao = 0; posicao < StackMapFrame->map_frame_type.full_frame.num_stack_items; posicao++) {
				*(StackMapFrame->map_frame_type.full_frame.stack+posicao) = lerVerificationTypeInfo(fp);
			}
		}
	}
	return StackMapFrame;
}

verification_type_info* lerVerificationTypeInfo (FILE *fp)
{
	verification_type_info * VTI = (verification_type_info*)malloc(sizeof(verification_type_info));
	VTI->tag = u1READ(fp);
	switch (VTI->tag) {
		case 7:
			VTI->type_info.object_variable_info.cp = u2READ(fp);
			break;
		case 8:
			VTI->type_info.uninitialized_variable_info.offset = u2READ(fp);
			break;
		default:
			break;
	}

	return VTI;
}

source_file_attribute* lerSourceFile (FILE *fp)
{
	source_file_attribute * SourceFile = NULL;
	SourceFile = (source_file_attribute*)malloc(sizeof(source_file_attribute));
	SourceFile->source_file = u2READ(fp);
	return SourceFile;
}
