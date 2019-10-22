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

#include "exibidor.h"
#include <string.h>
#include "instrucoes.h"


char* buscaNomeTag(u1 tag){
	char *nometag = malloc(40*sizeof(char));
	switch(tag){
		case CONSTANT_Class:
			strcpy(nometag,"CONSTANT_Class_Info");
			break;
		case CONSTANT_Fieldref:
			strcpy(nometag,"CONSTANT_Fieldref_Info");
			break;
		case CONSTANT_Methodref:
			strcpy(nometag,"CONSTANT_Methodref_Info");
			break;
		case CONSTANT_InterfaceMethodref:
			strcpy(nometag,"CONSTANT_InterfaceMethodref_Info");
			break;
		case CONSTANT_String:
			strcpy(nometag,"CONSTANT_String_Info");
			break;
		case CONSTANT_Integer:
			strcpy(nometag,"CONSTANT_Integer_Info");
			break;
		case CONSTANT_Float:
			strcpy(nometag,"CONSTANT_Float_Info");
			break;
		case CONSTANT_Long:
			strcpy(nometag,"CONSTANT_Long_Info");
			break;
		case CONSTANT_Double:
			strcpy(nometag,"CONSTANT_Double_Info");
			break;
		case CONSTANT_NameAndType:
			strcpy(nometag,"CONSTANT_NameAndType_Info");
			break;
		case CONSTANT_Utf8:
			strcpy(nometag,"CONSTANT_Utf8_Info");
			break;
		case CONSTANT_MethodHandle:
			strcpy(nometag,"CONSTANT_MethodHandle_Info");
			break;
		case CONSTANT_MethodType:
			strcpy(nometag,"CONSTANT_MethodType_Info");
			break;
		case CONSTANT_InvokeDynamic:
			strcpy(nometag,"CONSTANT_InvokeDynamic_Info");
			break;
		default:
			return NULL;
			break;
	}
	return(nometag);
}

char* decodificaStringUTF8(cp_info *cp){
	char* retorno = malloc((cp->UnionCP.UTF8.length+1)*sizeof(char));
	char *auxretorno = retorno;

	for (u1 *aux=cp->UnionCP.UTF8.bytes;aux<cp->UnionCP.UTF8.bytes+cp->UnionCP.UTF8.length;aux++){
		*auxretorno = (char) *aux;
		auxretorno++;
	}

	*auxretorno = '\0';

	return(retorno);
}

char* decodificarOperandoInstrucao(cp_info *cp,u2 index, u2 sizeCP){

	char *retorno = malloc(100*sizeof(char));
	char *stringNomeClasse;// = malloc(100*sizeof(char));
	char *stringNomeMetodo;// = malloc(100*sizeof(char));
	char *stringGeral;// = malloc(100*sizeof(char));
	char *ponteiro2pontos;// = malloc(100*sizeof(char));
	cp_info *cp_aux = cp+index-1;


	if (index < sizeCP) {
		switch(cp_aux->tag){
			case CONSTANT_Methodref:

				// String do Class Name do Methodref
				// Concatenar com .
				// String do name do Name_and_type do Methodref

				stringNomeClasse = decodificaNIeNT(cp,cp_aux->UnionCP.Methodref.class_index,NAME_INDEX);

				stringNomeMetodo = decodificaNIeNT(cp,cp_aux->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);

				ponteiro2pontos = strchr(stringNomeMetodo,':');
				*ponteiro2pontos = '\0';


				strcpy(retorno,"<");
				strcat(retorno,stringNomeClasse);
				strcat(retorno,".");
				strcat(retorno,stringNomeMetodo);
				strcat(retorno,">");
			break;

			case CONSTANT_Fieldref:

				stringNomeClasse = decodificaNIeNT(cp,cp_aux->UnionCP.Fieldref.class_index,NAME_INDEX);
				stringGeral = decodificaNIeNT(cp,cp_aux->UnionCP.Fieldref.name_and_type_index,NAME_AND_TYPE);
				// printf("String Geral: %s\n",stringGeral);


				ponteiro2pontos = strchr(stringGeral,':');
				*ponteiro2pontos = '\0';

				// stringGeral = strncpy(strstr(stringGeral,&stringGeral[1]),stringGeral,strlen(stringGeral));

				strcpy(retorno,"<");
				strcat(retorno,stringNomeClasse);
				strcat(retorno,".");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;

			case CONSTANT_String:

				stringGeral = decodificaNIeNT(cp,cp_aux->UnionCP.String.string_index,STRING_INDEX);

				strcpy(retorno,"<");
				strcat(retorno,stringGeral);
				strcat(retorno,">");
			break;
			default:
				strcpy(retorno,"undefined");
				break;
		}
	} else {
		sprintf(retorno,"%d",index);
	}
	return(retorno);
}

// LEMBRAR QUE CP INFO COMEÇA DE 1, POR ISSO QUE SUBTRAI 1 NA SOMA
// Decodifica Name Index e Name Type
char* decodificaNIeNT(cp_info *cp, u2 index, u1 tipo){

	char *retorno = malloc(100*sizeof(u1));

	cp_info *aux;
	cp_info *aux2;
	cp_info *aux3;

	aux = cp+index-1;

	switch(tipo){
		case NAME_INDEX:

			aux2 = cp+(aux->UnionCP.Class.name_index-1);

			retorno = decodificaStringUTF8(aux2);
		break;

		case NAME_AND_TYPE:

			aux2 = cp+(aux->UnionCP.NameAndType.name_index-1);
			aux3 = cp+(aux->UnionCP.NameAndType.descriptor_index-1);

			strcpy(retorno,"");
			strcat(retorno,decodificaStringUTF8(aux2));
			strcat(retorno,":");
			strcat(retorno,decodificaStringUTF8(aux3));
		break;

		case STRING_INDEX:
		case CLASS_INDEX:
			retorno = decodificaStringUTF8(aux);
		break;

		case NAME_AND_TYPE_INFO_NAME_INDEX:
			retorno = decodificaStringUTF8(aux);
		break;

		case NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX:
			retorno = decodificaStringUTF8(aux);
		break;
	}

	return(retorno);
}

char* decodificaAccessFlags(u2 flag){

	char *retorno = malloc(100*sizeof(char));
	strcpy(retorno,"");

	while(flag){

		if(flag>=SYNTHETIC){
			flag-=SYNTHETIC;
			strcat(retorno,"SYNTHETIC;");
		}
		if(flag>=TRANSIENT){
			flag-=TRANSIENT;
			strcat(retorno,"TRANSIENT;");
		}
		if(flag>=VOLATILE){
			flag-=VOLATILE;
			strcat(retorno,"VOLATILE;");
		}

		if(flag>=FINAL){
			flag-=FINAL;
			strcat(retorno,"FINAL;");
		}

		if(flag>=STATIC){
			flag-=STATIC;
			strcat(retorno,"STATIC;");
		}

		if(flag>=PROTECTED){
			flag-=PROTECTED;
			strcat(retorno,"PROTECTED;");
		}

		if(flag>=PRIVATE){
			flag-=PRIVATE;
			strcat(retorno,"PRIVATE;");
		}

		if(flag>=PUBLIC){
			flag-=PUBLIC;
			strcat(retorno,"PUBLIC;");
		}
	}

	retorno = organizandoFlags(retorno);
	return(retorno);

}

char* organizandoFlags(char* flagsOrdemInversa){

	const char s[2] = ";"; // Delimitador que vai dividir a string;

	char* flags = strtok(flagsOrdemInversa,s);
	char* velho = (char*)malloc(100*sizeof(char));
	char* novo = (char*)malloc(100*sizeof(char));

	int contador = 0;

	strcpy(novo, "");
	strcpy(velho, "");

	printf("--------------------------------------\n");
	while(flags != NULL){

		if(contador == 0)
			strcpy(velho, flags);

		if(contador > 0){

			strcpy(novo, flags);
			strcat(novo," ");
			strcat(novo, velho);
			strcpy(velho, novo);
			contador++;
		}
		else
			contador++;

		flags = strtok(NULL, s);
	}

	if(contador == 1){
		free(novo);
		return velho;
	}
	else{
		free(velho);
		return novo;
	}
}

double decodificaDoubleInfo (cp_info * cp) {
	 uint64_t valor = ((uint64_t)cp->UnionCP.Double.high_bytes<<32) | (uint64_t)cp->UnionCP.Double.low_bytes;
	 int sinal = ((valor>>63) == 0) ? 1 : -1;
	 int expon = ((valor>>52) & 0x7ffL);
	 long mant = (expon == 0) ? ((valor & 0xfffffffffffffL) << 1) : ((valor & 0xfffffffffffffL) | 0x10000000000000L);

	double retorno = sinal*mant*(pow(2,expon-1075));
	return retorno;
}

uint64_t decodificaLongInfo (cp_info * cp) {
	return ((((uint64_t)cp->UnionCP.Long.high_bytes)<<32) | ((uint64_t)cp->UnionCP.Long.low_bytes));
}

int decodificaIntegerInfo (cp_info * cp) {

	u4 valor = cp->UnionCP.Integer.bytes;
	return valor;
}

float decodificaFloatInfo (cp_info * cp) {
	u4 valor = cp->UnionCP.Float.bytes;
	float retorno;
	int sinal = ((valor>>31) == 0) ? 1 : -1;
	int expon = ((valor>>23) & 0xff);
	int mant = (expon == 0) ? (valor & 0x7fffff)<<1 : (valor & 0x7fffff) | 0x800000;
	retorno = (sinal)*(mant)*(pow(2,expon-150));
	return retorno;
}


int setaOffsetImpressao (int posicao, u1 offset) {

	if (posicao == 0) {
		return offset;
	} else {
		return (offset+1);
	}
}

void imprimirClassFile (ClassFile * arquivoClass){
    cp_info * aux;
	double valor;
	uint64_t longValue;
	method_info * auxMethod;
	field_info * auxField;
	attribute_info ** auxAttributeClasse;
	attribute_info ** fieldAttrAux;
	exception_table * exceptionTableAux;
	uint32_t contador = 1;
	char *ponteiroprint;

	printf("\n-----GENERAL INFORMATION-----\n\n");
	printf("Magic: %08x\n",arquivoClass->magic);
	printf("Minor Version: %04x\n",arquivoClass->minor_version);
	printf("Major Version: %04x\n",arquivoClass->major_version);
	printf("Constant Pool Count: %04x\n",arquivoClass->constant_pool_count);
	printf("Access Flags: %04x\n",arquivoClass->access_flags);
	printf("This Class: %04x\n",arquivoClass->this_class);
	printf("Super Class: %04x\n",arquivoClass->super_class);
	printf("Interfaces Count: %04x\n",arquivoClass->interfaces_count);
	printf("Fields Count: %04x\n",arquivoClass->fields_count);
	printf("Methods Count: %04x\n",arquivoClass->methods_count);
	printf("Atributes Count: %02x\n",arquivoClass->attributes_count);


	printf("\n\n-----CONSTANT POOL-----\n\n");
	
    for (aux = arquivoClass->constant_pool; aux < arquivoClass->constant_pool+arquivoClass->constant_pool_count-1; aux++) {
        printf("--------------Entrada [%d]--------------\n",contador);
        contador++;
        printf( "TAG: %02d: %s\n",aux->tag,buscaNomeTag(aux->tag));
		switch(aux->tag) {
            case CONSTANT_Class:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Class.name_index,CLASS_INDEX);
				printf("Class Name: cp_info#%02d <%s>\n",aux->UnionCP.Class.name_index,ponteiroprint);
				break;
            case CONSTANT_Fieldref:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Fieldref.class_index,NAME_INDEX);
				printf("Class Name: cp_info#%02d <%s>\n",aux->UnionCP.Fieldref.class_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Fieldref.name_and_type_index,NAME_AND_TYPE);
				printf("Name and Type: cp_info#%02d <%s>\n",aux->UnionCP.Fieldref.name_and_type_index,ponteiroprint);
				break;
            case CONSTANT_Methodref:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Methodref.class_index,NAME_INDEX);
				printf("Class Name: cp_info#%02d <%s>\n",aux->UnionCP.Methodref.class_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.Methodref.name_and_type_index,NAME_AND_TYPE);
				printf("Name and Type: cp_info#%02d <%s>\n",aux->UnionCP.Methodref.name_and_type_index,ponteiroprint);
				break;
            case CONSTANT_InterfaceMethodref:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.String.string_index,NAME_INDEX);
				printf("Class Name: cp_info#%02d <%s>\n",aux->UnionCP.InterfaceMethodref.class_index, ponteiroprint);
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.String.string_index,NAME_AND_TYPE);
				printf("Name and Type Index: cp_info#%02d <%s>\n",aux->UnionCP.InterfaceMethodref.name_and_type_index, ponteiroprint);
				break;
			case CONSTANT_String:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.String.string_index,STRING_INDEX);
				printf("String: cp_info#%02d <%s>\n",aux->UnionCP.String.string_index,ponteiroprint);
				break;
			case CONSTANT_Integer:
				printf("Bytes: %04x\n",aux->UnionCP.Integer.bytes);
				printf("Integer: %d\n",aux->UnionCP.Integer.bytes);
				break;
			case CONSTANT_Float:
				printf("Bytes: %04x\n",aux->UnionCP.Float.bytes);
				printf("Float: %d\n",aux->UnionCP.Float.bytes);
				break;
			case CONSTANT_Long:
				longValue = decodificaLongInfo(aux);
				printf("Long High Bytes: 0x%08x\n",aux->UnionCP.Long.high_bytes);
				printf("Long Low Bytes: 0x%08x\n",aux->UnionCP.Long.low_bytes);
				printf("Long: %lu\n",longValue);
				break;
			case CONSTANT_Double:
				valor = decodificaDoubleInfo(aux);
				printf("Double High Bytes: 0x%08x\n",aux->UnionCP.Double.high_bytes);
				printf("Double Low Bytes: 0x%08x\n",aux->UnionCP.Double.low_bytes);
				printf("Double: %lf\n",valor);
				break;
			case CONSTANT_NameAndType:
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.NameAndType.name_index,NAME_AND_TYPE_INFO_NAME_INDEX);
				printf("Name: cp_info#%02d <%s>\n",aux->UnionCP.NameAndType.name_index,ponteiroprint);
				ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,aux->UnionCP.NameAndType.descriptor_index,NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
				printf("Descriptor: cp_info#%02d <%s>\n",aux->UnionCP.NameAndType.descriptor_index,ponteiroprint);
				break;
			case CONSTANT_Utf8:
				printf("Length of byte array: %d\n",(int)aux->UnionCP.UTF8.length);
				printf("Length of string: %d\n",(int)aux->UnionCP.UTF8.length);
				printf("String: ");
				for (u1 * i = aux->UnionCP.UTF8.bytes; i < aux->UnionCP.UTF8.bytes + aux->UnionCP.UTF8.length; i++) {
					printf("%c",(char) (*i));
				}
				printf("\n");
				break;
			case CONSTANT_MethodHandle:
				printf("MethodHandle Reference Kind: %02x\n",aux->UnionCP.MethodHandle.reference_kind);
				printf("MethodHandle Reference Index: %04x\n",aux->UnionCP.MethodHandle.reference_index);
				break;
			case CONSTANT_MethodType:
				printf("MethodType Descriptor Index: %04x\n",aux->UnionCP.MethodType.descriptor_index);
				break;
			case CONSTANT_InvokeDynamic:
				printf("InvokeDynamic - Bootstrap Method Attr Index: %04x\n",aux->UnionCP.InvokeDynamicInfo.bootstrap_method_attr_index);
				printf("InvokeDynamic - Name and Type Index: %04x\n",aux->UnionCP.InvokeDynamicInfo.name_and_type_index);
				break;
			default:
				printf("DefaultImpressao\n");
				break;
        }
    }

	printf("\n\n-----INTERFACES-----\n\n");
	contador = 0;
	for (u2 * auxInterfaces = arquivoClass->interfaces; auxInterfaces < arquivoClass->interfaces+arquivoClass->interfaces_count; auxInterfaces++) {
		ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,*auxInterfaces,NAME_INDEX);
		printf("Interface: cp_info#%d <%s>\n",*auxInterfaces, ponteiroprint);
	}


	printf("\n\n-----FIELDS-----\n\n");
	contador = 0;
	for (auxField = arquivoClass->fields; auxField < arquivoClass->fields + arquivoClass->fields_count; auxField++,contador++) {
		printf("--------------Field [%d]--------------\n\n",contador);
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+auxField->name_index);
		printf("Name: cp_info#%d <%s>\n",auxField->name_index,ponteiroprint);
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+auxField->descriptor_index);
		printf("Descriptor: cp_info#%d <%s>\n",auxField->descriptor_index,ponteiroprint);
		ponteiroprint = decodificaAccessFlags(auxField->access_flags);
		printf("Access Flags: 0x%04x [%s]\n",auxField->access_flags,ponteiroprint);
		printf("Attributes Count: %d\n",auxField->attributes_count);
		if (auxField->attributes_count > 0) {
			fieldAttrAux = auxField->attributes;
			for (int posicaoFields = 0; posicaoFields < auxField->attributes_count; posicaoFields++) {
				ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+(*(fieldAttrAux+posicaoFields))->attribute_name_index);
				printf("Attribute Name Index: cp_info#%d <%s>\n",(*(fieldAttrAux+posicaoFields))->attribute_name_index,ponteiroprint);
				printf("Attribute Length: %d\n",(*(fieldAttrAux+posicaoFields))->attribute_length);
				if (strcmp(ponteiroprint, "ConstantValue") == 0) {
					constantValue_attribute * cvAux = (constantValue_attribute*)(*(fieldAttrAux+posicaoFields))->info;
					cp_info * cpInfoAux = arquivoClass->constant_pool-1+cvAux->constantvalue_index;
					//FLOAT
					if (cpInfoAux->tag == 4) {
						float valorCV = decodificaFloatInfo(arquivoClass->constant_pool-1+cvAux->constantvalue_index);
						printf("Constant Value Index: cp_info#%d <%f>\n",cvAux->constantvalue_index,valorCV);
					//Integer-Byte-Boolean-Short-Char
					} else if (cpInfoAux->tag == 3) {
						int valorRetorno = decodificaIntegerInfo (arquivoClass->constant_pool-1+cvAux->constantvalue_index);
						printf("Constant Value Index: cp_info#%d <%d>\n",cvAux->constantvalue_index,valorRetorno);
					//STRING
					} else if (cpInfoAux->tag == 8) {
						char * stringEntrada = decodificaNIeNT(arquivoClass->constant_pool,cvAux->constantvalue_index,NAME_INDEX);
						printf("Constant Value Index: cp_info#%d <%s>\n",cvAux->constantvalue_index,stringEntrada);
					//DOUBLE
					} else if (cpInfoAux->tag == 6) {
						double valorDB = decodificaDoubleInfo(arquivoClass->constant_pool-1+cvAux->constantvalue_index);
						printf("Constant Value Index: cp_info#%d <%lf>\n",cvAux->constantvalue_index,valorDB);
					} else if (cpInfoAux->tag == 5) {
						uint64_t valorL = decodificaLongInfo(arquivoClass->constant_pool-1+cvAux->constantvalue_index);
						printf("Constant Value Index: cp_info#%d <%lu>\n",cvAux->constantvalue_index,valorL);
					}
				} else if (strcmp(ponteiroprint,"Signature") == 0) {
					signature_attribute * sig = (signature_attribute*)(*(fieldAttrAux+posicaoFields))->info;
					char * Signature_Index = decodificaStringUTF8(arquivoClass->constant_pool-1+sig->signature_index);
					printf("Signature index: cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
				}
			}
		}
	}

	printf("\n\n-----METODOS-----\n\n");

	contador = 0;

    for (auxMethod = arquivoClass->methods; auxMethod < arquivoClass->methods + arquivoClass->methods_count; auxMethod++,contador++) {
		printf("--------------Metodo [%d]--------------\n\n",contador);
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+auxMethod->name_index);
		printf("Name: cp_info#%d <%s>\n",auxMethod->name_index,ponteiroprint);
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+auxMethod->descriptor_index);
		printf("Descriptor: cp_info#%d <%s>\n",auxMethod->descriptor_index,ponteiroprint);
		ponteiroprint = decodificaAccessFlags(auxMethod->access_flags);
		printf("Access Flags: 0x%04x [%s]\n",auxMethod->access_flags,ponteiroprint);
		printf("Attributes Count: %d\n",auxMethod->attributes_count);

		printf("Atributos:\n");
		attribute_info ** auxAttrCompleto = auxMethod->attributes;
		for (int posicao = 0; posicao < auxMethod->attributes_count; posicao++) {
			ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+(*(auxAttrCompleto+posicao))->attribute_name_index);
			printf("Attribute Name Index: cp_info#%d <%s>\n",(*(auxAttrCompleto+posicao))->attribute_name_index,ponteiroprint);
			printf("Attribute Length: %d\n",(*(auxAttrCompleto+posicao))->attribute_length);

			if (strcmp(ponteiroprint,"Code") == 0) {
				code_attribute * auxCodePontual = (code_attribute*)(*(auxAttrCompleto+posicao))->info;
				printf("Max Stack: %d\n",auxCodePontual->max_stack);
				printf("Max Locals: %d\n",auxCodePontual->max_locals);
				printf("Code length: %d\n",auxCodePontual->code_length);
				printf("\n\n----Code----\n\n");
				if(auxCodePontual->code_length > 0) {
					ponteiroprint = decodificarCode(arquivoClass->constant_pool,arquivoClass->constant_pool_count,auxCodePontual->code,auxCodePontual->code_length,instrucoes);
					printf("%s\n",ponteiroprint);
				}
				if(auxCodePontual->exception_table_length > 0) {
					printf("Exception Table:\n");
					printf("Nr.\t\tStart PC\tEnd PC\tHandler PC\tCatch Type\n");
					int contadorExceptionTable = 0;
					for(exceptionTableAux = auxCodePontual->table; exceptionTableAux < auxCodePontual->table + auxCodePontual->exception_table_length; exceptionTableAux++){
						printf("%d\t\t%02x\t\t%02x\t\t%02x\t%02x\n",contadorExceptionTable,exceptionTableAux->start_pc,exceptionTableAux->end_pc,exceptionTableAux->handler_pc,exceptionTableAux->catch_type);
						contadorExceptionTable++;
					}
					printf("\n\n");
				}
				printf("Attributes Count: %d\n",auxCodePontual->attributes_count);
				if (auxCodePontual->attributes_count > 0) {
					int lntContador = 0;
					attribute_info ** auxAttributesFromCode = auxCodePontual->attributes;
					for (int posicaoDois = 0; posicaoDois < auxCodePontual->attributes_count; posicaoDois++) {
						ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool-1+(*(auxAttributesFromCode+posicaoDois))->attribute_name_index);
						printf("Attribute Name Index: cp_info#%d <%s>\n",(*(auxAttributesFromCode+posicaoDois))->attribute_name_index,ponteiroprint);
						printf("Attribute Length: %d\n",(*(auxAttributesFromCode+posicaoDois))->attribute_length);
						if (strcmp(ponteiroprint,"LineNumberTable") == 0) {
							line_number_table * lntAux = (line_number_table*)(*(auxAttributesFromCode+posicaoDois))->info;
							printf("Line Number Table Length: %d\n",(int)lntAux->line_number_table_length);
							printf("Attribute Info: \n");
							printf("Nr.\t\tStartPC\t\tLineNumber\n");
							for (line_number_tableInfo * linfo = lntAux->info; linfo < lntAux->info + lntAux->line_number_table_length; linfo++) {
								printf("%d\t\t%d\t\t%d\n",lntContador,linfo->start_pc,linfo->line_number);
								lntContador++;
							}
							printf("\n");
						} else if (strcmp(ponteiroprint,"StackMapTable") == 0) {
							int offsetImpressao = 0;
							stackMapTable_attribute * smt = (stackMapTable_attribute*)(*(auxAttributesFromCode+posicaoDois))->info;
							stack_map_frame ** smf = smt->entries;
							printf("Nr.\t\tStack Map Frame\n");
							for (int posicaoSMF = 0; posicaoSMF < smt->number_of_entries; posicaoSMF++) {
								if ((*(smf+posicaoSMF))->frame_type >= 0 && (*(smf+posicaoSMF))->frame_type <= 63) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->frame_type);
									printf("%d\t\tSAME(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,(*(smf+posicaoSMF))->frame_type);
								} else if ((*(smf+posicaoSMF))->frame_type >= 64 && (*(smf+posicaoSMF))->frame_type <= 127) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,((*(smf+posicaoSMF))->frame_type)-64);
									printf("%d\t\tSAME_LOCALS_1_STACK_ITEM(%d), Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,((*(smf+posicaoSMF))->frame_type-64));
									printf("\t\tStack verifications:\n");
									verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame.stack;
									switch ((*(VTIAux))->tag) {
										case 0:
											printf("\t\t\tTOP\n");
											break;
										case 1:
											printf("\t\t\tINTEGER\n");
											break;
										case 2:
											printf("\t\t\tFLOAT\n");
											break;
										case 3:
											printf("\t\t\tLONG\n");
											break;
										case 4:
											printf("\t\t\tDOUBLE\n");
											break;
										case 5:
											printf("\t\t\tNULL\n");
											break;
										case 6:
											printf("\t\t\tUNINITIALIZED THIS\n");
											break;
										case 7:
											ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux))->type_info.object_variable_info.cpool_index,NAME_INDEX);
											printf("\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux))->type_info.object_variable_info.cpool_index, ponteiroprint);
											break;
										case 8:
											printf("\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux))->type_info.uninitialized_variable_info.offset);
											break;
									}
								} else if ((*(smf+posicaoSMF))->frame_type == 247) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta);
									printf("%d\t\tSAME_LOCALS_1_STACK_ITEM_EXTENDED(%d), Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type,offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta);
									printf("\t\tStack verifications:\n");
									verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.same_locals_1_stack_item_frame_extended.stack;
									switch ((*(VTIAux))->tag) {
										case 0:
											printf("\t\t\tTOP\n");
											break;
										case 1:
											printf("\t\t\tINTEGER\n");
											break;
										case 2:
											printf("\t\t\tFLOAT\n");
											break;
										case 3:
											printf("\t\t\tLONG\n");
											break;
										case 4:
											printf("\t\t\tDOUBLE\n");
											break;
										case 5:
											printf("\t\t\tNULL\n");
											break;
										case 6:
											printf("\t\t\tUNINITIALIZED THIS\n");
											break;
										case 7:
											ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux))->type_info.object_variable_info.cpool_index,NAME_INDEX);
											printf("\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux))->type_info.object_variable_info.cpool_index, ponteiroprint);
											break;
										case 8:
											printf("\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux))->type_info.uninitialized_variable_info.offset);
											break;
									}
								} else if ((*(smf+posicaoSMF))->frame_type >= 248 && (*(smf+posicaoSMF))->frame_type <= 250) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.chop_frame.offset_delta);
									printf("%d\t\tCHOP(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.chop_frame.offset_delta);
								} else if ((*(smf+posicaoSMF))->frame_type == 251) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.same_frame_extended.offset_delta);
									printf("%d\t\tSAME_FRAME_EXTENDED(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao,(*(smf+posicaoSMF))->map_frame_type.same_frame_extended.offset_delta);
								} else if ((*(smf+posicaoSMF))->frame_type >= 252 && (*(smf+posicaoSMF))->frame_type <= 254) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.append_frame.offset_delta);
									printf("%d\t\tAPPEND(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao, (*(smf+posicaoSMF))->map_frame_type.append_frame.offset_delta);
									verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.append_frame.locals;
									printf("\t\t  Local verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < ((*(smf+posicaoSMF))->frame_type-251); posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												printf("\t\t\tTOP\n");
												break;
											case 1:
												printf("\t\t\tINTEGER\n");
												break;
											case 2:
												printf("\t\t\tFLOAT\n");
												break;
											case 3:
												printf("\t\t\tLONG\n");
												break;
											case 4:
												printf("\t\t\tDOUBLE\n");
												break;
											case 5:
												printf("\t\t\tNULL\n");
												break;
											case 6:
												printf("\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,NAME_INDEX);
												printf("\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												printf("\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
								} else if ((*(smf+posicaoSMF))->frame_type == 255) {
									offsetImpressao += setaOffsetImpressao(posicaoSMF,(*(smf+posicaoSMF))->map_frame_type.full_frame.offset_delta);
									printf("%d\t\tFULL_FRAME(%d),Offset: %d(+%d)\n",posicaoSMF,(*(smf+posicaoSMF))->frame_type, offsetImpressao, (*(smf+posicaoSMF))->map_frame_type.full_frame.offset_delta);
									verification_type_info ** VTIAux = (*(smf+posicaoSMF))->map_frame_type.full_frame.locals;
									printf("\t\t  Local verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < (*(smf+posicaoSMF))->map_frame_type.full_frame.number_of_locals; posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												printf("\t\t\tTOP\n");
												break;
											case 1:
												printf("\t\t\tINTEGER\n");
												break;
											case 2:
												printf("\t\t\tFLOAT\n");
												break;
											case 3:
												printf("\t\t\tLONG\n");
												break;
											case 4:
												printf("\t\t\tDOUBLE\n");
												break;
											case 5:
												printf("\t\t\tNULL\n");
												break;
											case 6:
												printf("\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,NAME_INDEX);
												printf("\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												printf("\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
									VTIAux = (*(smf+posicaoSMF))->map_frame_type.full_frame.stack;
									printf("\t\t  Stack verifications:\n");
									for (int posicaoVTI = 0; posicaoVTI < (*(smf+posicaoSMF))->map_frame_type.full_frame.number_of_stack_items; posicaoVTI++) {
										switch ((*(VTIAux+posicaoVTI))->tag) {
											case 0:
												printf("\t\t\tTOP\n");
												break;
											case 1:
												printf("\t\t\tINTEGER\n");
												break;
											case 2:
												printf("\t\t\tFLOAT\n");
												break;
											case 3:
												printf("\t\t\tLONG\n");
												break;
											case 4:
												printf("\t\t\tDOUBLE\n");
												break;
											case 5:
												printf("\t\t\tNULL\n");
												break;
											case 6:
												printf("\t\t\tUNINITIALIZED THIS\n");
												break;
											case 7:
												ponteiroprint = decodificaNIeNT(arquivoClass->constant_pool,(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index,NAME_INDEX);
												printf("\t\t\tOBJECT cp_info#%d <%s>\n",(*(VTIAux+posicaoVTI))->type_info.object_variable_info.cpool_index, ponteiroprint);
												break;
											case 8:
												printf("\t\t\tUNINITIALIZED Offset: %d\n",(*(VTIAux+posicaoVTI))->type_info.uninitialized_variable_info.offset);
												break;
										}
									}
								}
							}
						}
					}
				}
			} else if (strcmp(ponteiroprint,"Exceptions") == 0) {
				exceptions_attribute * excpAux = (exceptions_attribute*)(*(auxAttrCompleto+posicao))->info;
				int contadorExcp = 0;
				char * exceptionIndexString;
				printf("Nr.\t\tException\t\t\tVerbose\n");
				for (u2 * indexExcp = excpAux->exception_index_table; indexExcp < excpAux->exception_index_table + excpAux->number_of_exceptions; indexExcp++) {
					exceptionIndexString = decodificaNIeNT(arquivoClass->constant_pool,*indexExcp,NAME_INDEX);
					printf("%d\t\tcp_info#%d\t\t\t%s\n",contadorExcp,*indexExcp,exceptionIndexString);
					contadorExcp++;
				}
			} else if (strcmp(ponteiroprint,"Signature") == 0) {
				signature_attribute * sig = (signature_attribute*)(*(auxAttrCompleto+posicao))->info;
				char * Signature_Index = decodificaStringUTF8(arquivoClass->constant_pool-1+sig->signature_index);
				printf("Signature index: cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
			}
		}
	}

	printf("\n\n-----ATRIBUTOS DA CLASSE-----\n\n");
	auxAttributeClasse = arquivoClass->attributes;
	for (int posicao = 0; posicao < arquivoClass->attributes_count; posicao++) {
		ponteiroprint = decodificaStringUTF8(arquivoClass->constant_pool+(*(auxAttributeClasse+posicao))->attribute_name_index-1);
		printf("Attribute Name Index: cp_info#%d <%s>\n",(*(auxAttributeClasse+posicao))->attribute_name_index,ponteiroprint);
		printf("Attribute Length: %d\n",(int) (*(auxAttributeClasse+posicao))->attribute_length);
		if (strcmp(ponteiroprint,"SourceFile") == 0) {
		 	source_file_attribute * SourceFile = ((source_file_attribute*)((*(auxAttributeClasse+posicao))->info));
			printf("Source File Name Index: cp_info#%d <%s>\n",SourceFile->source_file_index,decodificaStringUTF8(arquivoClass->constant_pool+SourceFile->source_file_index-1));
		} else if (strcmp(ponteiroprint, "InnerClasses") == 0) {
			innerClasses_attribute * innerC = ((innerClasses_attribute*)((*(auxAttributeClasse+posicao))->info));
			printf("Nr.\t\tInner Class\t\t\tOuter Class\t\tInner Name\t\tAccess Flags\n");
			char * innerClassString, * outerClassString, * innerNameIndex, * accessFlagsInner;
			classes ** vetorClasses = innerC->classes_vector;
			for (int posicaoInncerC = 0; posicaoInncerC < innerC->number_of_classes; posicaoInncerC++) {
				innerClassString = decodificaNIeNT(arquivoClass->constant_pool,(*(vetorClasses+posicaoInncerC))->inner_class_info_index,NAME_INDEX);
				outerClassString = decodificaNIeNT(arquivoClass->constant_pool,(*(vetorClasses+posicaoInncerC))->outer_class_info_index,NAME_INDEX);
				innerNameIndex = decodificaStringUTF8(arquivoClass->constant_pool-1+(*(vetorClasses+posicaoInncerC))->inner_name_index);
				accessFlagsInner = decodificaAccessFlags((*(vetorClasses+posicaoInncerC))->inner_class_access_flags);
				printf("%d\t\tcp_info#%d\t\t\tcp_info#%d\t\tcp_info#%d\t\t0x%04x\n",posicaoInncerC,(*(vetorClasses+posicaoInncerC))->inner_class_info_index,(*(vetorClasses+posicaoInncerC))->outer_class_info_index,(*(vetorClasses+posicaoInncerC))->inner_name_index,(*(vetorClasses+posicaoInncerC))->inner_class_access_flags);
				printf("  \t\t%s\t\t%s\t\t%s\t\t\t%s\n",innerClassString,outerClassString,innerNameIndex,accessFlagsInner);
			}
		} else if (strcmp(ponteiroprint,"Signature") == 0) {
			signature_attribute * sig = (signature_attribute*)((*(auxAttributeClasse+posicao))->info);
			char * Signature_Index = decodificaStringUTF8(arquivoClass->constant_pool-1+sig->signature_index);
			printf("Signature index: cp_info#%d <%s>\n",sig->signature_index,Signature_Index);
		}
	}

}
