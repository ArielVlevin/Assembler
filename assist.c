#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "assist.h"
/*helpfull func*/

/* DEC-Number to bits, return string */
char * decimalToBinar(int num, int bitsize){
	int i, j;
	char *bitarray = (char*) malloc( bitsize * sizeof(char) ); ;
	
	for(i = bitsize-1, j= 0; i >= 0; i-- , j++){
		if( ( num & ( 1<<i ) ) !=  0 )	bitarray[j] = '1';
		else	bitarray[j] = '0';
	}
	return bitarray;
}
/* binar code to hex code */
int binarToHex(char * binar){
	int hex = 0, p;
	while (*binar){
		if( *binar == '1' ) p = 1;
		else	p = 0;
      		hex = (hex<<1)|p;
      		binar++;
	} 
	return hex;
}
/* take the binar code from the line struct and print as hex as requested */
void littleEnding(lineT line, FILE *file){
	int len = strlen(line->binar_code), i;
	char *ch;
	ch = (char*) malloc(8 * sizeof(char));
	fprintf(file, "%04d ", fileD->address);
	while(len > 0){	
		for(len -= 8, i = 0 ; i < 8 ; i++, len++)	ch[i] = line->binar_code[len];
		len -= 8;
		fprintf(file, "%02X ", binarToHex(ch));
	}
	fprintf(file, "\n");
	fileD->address += 4;
}
/* take the binar code from the line struct and print as hex as requested for code type*/
int littleEndingCode(lineT line, FILE *file, int i){
	int h = 0,j = 0, len;
	char *ch;
	ch = (char*) malloc(8 * sizeof(char));
	if( !i )	fprintf(file, "%04d ", fileD->address);
	for( ; h < line->datacount-1 ; h++){
		len = strlen(*line->binar_code_s);
		while(len > 0){	
			for(len -= 8, j = 0  ;j < 8; j++, len++)	ch[j] = line->binar_code_s[h][len];
			len -= 8;
			fprintf(file, "%02X ", binarToHex(ch));
			i++;
			if( i % 4 == 0 )	fprintf(file, "\n%04d ", fileD->address += 4);
			
		}
	}
	return i;
}
/* Making copy of string */
char * copy_str(const char *text){
	size_t len = strlen(text) +1;
	char * p = malloc(len);
	if(!p) return NULL;
	memcpy(p, text, len);
	return p;
}
/* Cut and return the next word in the string */
char * getWord(char ** text){
	char *p, *token;
	dospace(text);
	p = copy_str(*text);
	token =  strtok(p, WHITE_CHAR);
	(*text) += strlen(token);
	return token;
}
/* return SUCCESS if the char is quotation mark*/
int isQuomark(char  ch){
	if( ch == '"' )	return SUCCESS;
	return FAILED;
}
/* return SUCCESS if the char is dollar sign*/
int isRegister(char  ch){
	if( ch == '$' )	return SUCCESS;
	return FAILED;
}
/* return SUCCESS if the char is semicolon sign*/
int isSemico(char ch){
	if( ch == ';' )	return SUCCESS;
	return FAILED;
}


/* checking if there register in the text. @return the register number(int)*/
int getRegister(char ** text){
	int reg, spaces;
	char *p, *token;
	if ( !dospace(text) )	return BIGGEST_BIT+1;
	p = copy_str(*text);
	if( isRegister(*p) ){
		p++;
		token =  strtok(p, ",");
		spaces = strlen(token);
		token =  strtok(p, " ");	/*delete spaces if exist */
		reg = atoi(token);
		if( 0 <= reg && reg <= BIGGEST_BIT){
			(*text) += (spaces +2);
			return reg;	
		}
	}
	printf("register doesn't exist! Error!\n");
	return BIGGEST_BIT+1;
}
/* Func check if all the vars in the string are int */
int allIsInt(char * text){
	while(*text){
		dospace(&text);
		if(*text == '+' || *text == '-' || isdigit(*text)){
			text++;
			while(*text && isdigit(*text))	text++;
			if(!(dospace(&text)))	return SUCCESS;
			else if(*text == ',')	text++;
			else	return FAILED;
		}else	break;
	}
	return FAILED;
}

/* check if the next number in the string is anable to use */
int getnumber(char ** text, int type){

	char *p, *token;
	int num, negative = -(type+1) , positive = type, spaces;
	if ( !dospace(text) )	return type + 1;
	p = copy_str(*text);
	token =  strtok(p, ",");
	spaces = strlen(token);
	token =  strtok(p, "\n\t "); 
	num = atoi(token);
	if(num <= positive && negative <= num){
		(*text) += (spaces +1);
		return num;
	}
	printf("can't use this number! Error!\n");
	return type + 1;
}

/*dospace - kill the space - if there no chars return 0, else return positive number */
int dospace( char ** str ){
	if(isspace(**str))
		while(isspace(**str))
			(*str)++;
	return **str;
}
/* checking if word from array existing in text, return the index of the array if found*/
int input_exist_check(char * text, char * arr[]){
	int i;
	char *p, *token;
	p = copy_str(text);
	token = strtok(p, WHITE_CHAR);
	for( i = 1; arr[i] != NULL; i++ )
		if( !(strcmp(token, arr[i])) )
			return i;
	return 0;
}

/*checking if the input length is ok, if its too long - print a message*/
int input_length(char * input){
	if(strlen(input) <= MAX_LINE)	return 0;
	printf("The Input is too long!\n");
	return 1;
}


