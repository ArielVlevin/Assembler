#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "analyze.h"
#include "assist.h"
/* Analyze the current line and sort by his type */
int lineProcessing(char ** text, lineT line){

	int func_result;
	char *labelname;
	line->linetext =  *text, line->labelFlag = 0; 

	if( ( !(dospace(&line->linetext)) ) || isSemico(*line->linetext) ){	/* when its notes or empty line */
		return EMPTY;

	}else if((func_result = label_check(line->linetext))){		/*if this is label*/
		if( !input_exist_check(line->linetext, allnames) )	labelname = labelExtract(line, func_result);
		else{	printf("Illgal label\n");
			return FAILED;
		}
	}
	if( dospace(&line->linetext) && (line->command = input_exist_check(line->linetext, all)) ){
		(line->linetext) += strlen(all[line->command]);

		if(line->command < ENTRY){	/*DATA check*/
			return isData(line, labelname);

		}else if(( line->command == ENTRY ) || ( line->command == EXTERN )){	/* ENTRY and EXTERN */
			return isENEX(line);

		}else if( line->command > EXTERN ){	/* actions */
			return isCode(line, labelname);
		}
	}/* when doesn't found an action */
	printf("There not such action\n");
	return FAILED;
	
}
/*If the line is ENTRY or  EXTERN line do : */
int isENEX(lineT line){
	line->labelFlag = 0;
	if( !(dospace(&line->linetext)) )	return FAILED;
	line->parameter = getWord(&line->linetext);
	return SUCCESS;
}
/*If the line is code line do : */
int isCode(lineT line, char * labelname){
	if( line->labelFlag ){	/* label */
		line->symbol = labelname;
		line->type[0] = _CODE;
	}
	if( line->command >= JMP ){	/* J actions */
		if( line->command == STOP )	line->command = _STOP;
		else if( dospace(&line->linetext) ){
			line->parameter = getWord(&line->linetext);
		}
		return SUCCESS;
	}else if( codeExtract(&line->linetext, line) ){
		if( line->command <  ADDI )	r_binar(line);
		else if( line->command <  BNE )	 i_binar(line);
	}
	return SUCCESS;

}
/* If the line is data line do :- */
int isData(lineT line, char * labelname){

	int (*conv_func[3])(lineT t) = {halfwordConv, wordConv, byteConv};
	if(line->labelFlag){
		line->symbol = labelname;
		line->type[0] = _DATA;
	}
	
	if( line->command == ASCIZ ){
		if(ascizExstract(&line->linetext, line))	return (*conv_func[2])(line);
	}else{
		if(numExtract(&line->linetext, line))	return (*conv_func[line->command-1])(line);		
	}
	return FAILED;
}



/* DATA checking and extracting from the string */
int numExtract(char ** text, lineT line){

	int i = 1, numcheck, size = 0;
	char * p = copy_str(*text);
	
	if( line->command == DB )	size = DBSIZE;
	else if( line->command == DH )	size = DHSIZE;
	else if( line->command == DW )	size = DWSIZE;
	if( allIsInt(p) ){
		while( dospace(&p) ){
			if( ( numcheck = getnumber(&p, size) ) == ( size + 1 ) )	return FAILED;
			line->data[i++] = numcheck;
		}
		line->datacount = i;
		return SUCCESS;
	}else	printf("There are invalid chars!\n");
	return FAILED;
}
/* Exstracting  the text when the command type is asciz */
int ascizExstract(char ** text, lineT line){

	int i = 1;
	char *p = copy_str(*text);
	
	if( dospace(&p) ){
		if( !isQuomark(*p) ){	/* Check if the first mark exist */
			printf("Mark do not exist\n");
			return FAILED;
		}
		p++;
		while( *p && !isQuomark(*p) ){
			line->data[i++] = *p; 
			p++;
		} 
		if( isQuomark(*p) ){	/* If we get to the last mark */
			p++;
			if( dospace(&p) ){
				printf("Extraneous text after end of command\n");
				return FAILED;
			}
			line->data[i] = '\0';
			line->datacount = ++i;
			return SUCCESS;	
		}
	}
	printf("Text doesn't found!\n");
	return FAILED;
}	
/* converstion to binar when size is word( dw ) */
int wordConv(lineT line){
	
	word w;
	char  *binar; 
	int* p = (int*)&w, j=0, length = line->datacount-1;
	*p = 0;
	 
	
	
	while( j < length ){
		w.ch = line->data[j+1];
		binar = decimalToBinar(*p,sizeof(int) * 8);
		line->binar_code_s[j] = binar;
		j++;
	}
	line->binar_code_s[j] = '\0';	
	return SUCCESS;
}
/* converstion to binar when size is halfword( dh ) */
int halfwordConv(lineT line){

	halfword s;
	char  *binar;
	int* p = (int*)&s, j=0, length = line->datacount-1;  
	*p = 0;
	
	

	while( j < length ){
		s.ch = line->data[j+1];
		binar = decimalToBinar(*p,  sizeof(short) * 8);
		line->binar_code_s[j] = binar;
		j++;
	}
	/*line->binar_code_s[j] = '\0';*/
	return SUCCESS;
}
/* converstion to binar when size is byte ( asciz or db ) */
int byteConv(lineT line){

	byte z;
	char   *binar; 
	int* p = (int*)&z, j=0, length = line->datacount-1; 
	*p = 0;

	while( j < length ){
		z.ch = line->data[j+1];
		binar = decimalToBinar(*p,  8);
		line->binar_code_s[j] = binar;
		j++;
	}
	line->binar_code_s[j] = '\0';
	return SUCCESS;
}




/*label_check*/
int label_check(char * text){
	int chars = 1; /*Counter of the label's chars*/
	if( !(isalpha(*text)) )	return FAILED;
	while( ( isalpha(*text) || ( ( isdigit(*text)) && chars <= BIGGEST_BIT ) ) ){
		chars++;
		text++;
	}
	if( *text == ':' )	return chars;
	else	return FAILED; /* Not a label */
}
/* Label extract from the string */
char * labelExtract(lineT line, int length){
	char *p, *par;
	line->labelFlag = 1;/*turn on label flag*/
	p = copy_str(line->linetext);
	par = strtok(p, ":"); /* the name if the label*/
	(line->linetext) += length;
	return  par;
}




/* I R J To Binar */

int codeExtract(char ** text, lineT line){
	int i, num, act_num = line->command;
	for( i = 0 ; i < VAR ; i++ ){
		if( i == 0 || ( i == 1 && act_num < NOR ) || ( ( i == 1 && ( act_num >= BNE )) || ( i == 2 && act_num < BNE ) ) ){
			if(  ( (num = getRegister(text) ) != ( BIGGEST_BIT + 1 ) )  )				line->data[i] = num;
			else	return FAILED;	

		}else if( i == 1 && act_num >= ADDI && act_num < BNE ){
			if(  ( (num = getnumber(text, BIGGEST_NUM) ) < ( BIGGEST_NUM + 1 ) )  )				line->data[i] = num;
			else	return FAILED;	
		}else if( i == 2 && act_num >= NOR ){
			line->parameter = getWord(text);
		}
	}
	return SUCCESS;
}


void i_binar(lineT line){
	/*set I*/
	I_Action i;
	int* p = (int*)&i;
	char *binar;         
	*p = 0; 
	/*placing data*/
	i.rs = line->data[0];
	if(line->command < BNE){
		if( line->command < LB )	i.opcode = line->command - 5;
		else	i.opcode = line->command - 1;
		i.immed = line->data[1];
		i.rt = line->data[2];
		
	}else{	/* >= BNE */
		i.opcode = line->command - 11;
		i.rt = line->data[1];
		i.immed = line->data[2];
		
	}
	binar = decimalToBinar(*p, sizeof(int) * 8);
	line->binar_code =  binar;
}

void r_binar(lineT line){
	/*set R*/
	R_Action r;
	int* p = (int*)&r;
	char *binar;         
	*p = 0;  
	/*placing data*/      
	r.rs = line->data[0];
	r.rd = line->data[2];  
	if(line->command < NOR){	
	r.funct = line->command - EXTERN;
	r.rt = line->data[1];
	}else{/* copy commands */
		r.opcode = 1;
		r.funct = line->command - NOR;
	}         
	binar = decimalToBinar(*p, sizeof(int) * 8);
	line->binar_code =  binar;
}

int j_binar(lineT line){ 
	/*set J*/
	J_Action j;
	int* p = (int*)&j;
	char *binar;
	*p = 0; 
	/*placing data*/
	j.opcode = line->command ;
	if(line->command != _STOP){ 
		if( ( isRegister(*(line->parameter) ) ) &&  line->command == JMP ) {
			j.reg = 1;
			j.address = getRegister(&line->parameter);
		}else if( symbolExistInTable(line->parameter) ){
			if( isExt(line)	)	j.address = 0;
			else 	j.address = returnAddress(line->parameter);
		}else{
			printf("This symbol do not exist in symbol's table: %s \n", line->parameter);
			return 1;
		}	
	}
	binar = decimalToBinar(*p, sizeof(int) * 8);
	line->binar_code =  binar;
	return 0;
}


