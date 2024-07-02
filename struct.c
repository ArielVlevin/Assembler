#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "analyze.h"
#include "assist.h"
#include "files.h"

/* This func analyze the text line by line with the func lineProcessing and crate more memmory for the struct every successful line */
int textInput(){
	int checker, fail =  0;
	char* input;
	lineT line = NULL;
	linehead = NULL;
	fileD->errorFlag = 0;
	
	
	input = malloc(MAX_CHAR * sizeof(char));
		
	while(fgets(input, MAX_CHAR, fileD->file) != NULL){
	   	if((input_length(input))){	/* too long */			
			fail = 1;
			continue;
		}
		if(!linehead){
			linehead = (lineT)calloc(1,sizeof(Linetab));
			if(!linehead){		
				printf("Error! memory not allocated.\n");
				fileD->errorFlag = 1;
				return FAILED;
			}
			if(!(checker = lineProcessing(&input, linehead)))	fail = 1;
			if( checker == EMPTY )	linehead = NULL;	/* EMPTY OR NOTE */
			continue;
		}else{
			line = (lineT)calloc(1,sizeof(Linetab));
			if( !line ){
				printf("Error! memory not allocated.\n");
				fileD->errorFlag = 1;	
				return FAILED; 
			}
			if(!(checker = lineProcessing(&input, line)))	fail = 1;
			if(checker == EMPTY)	line = NULL;	/* EMPTY OR NOTE */
			else	addLine(line);
			continue;
		}

	}
	return fail;
}
/* add Extern type to the symbol table */
int addExtern(lineT line){
	symbolT ptr = symbolhead;
	while( ptr ){
		if( !(strcmp(ptr->symbol,line->parameter)) ){
			if( line->type[1] == _ENTRY || line->type[0] == _DATA ||line->type[0] == _CODE )	return FAILED;
			else	return SUCCESS;
		}
		ptr = ptr->next;
	}
	line->type[0] = _EXTERMAL; 
	line->symbol = line->parameter;
	line->address = 0;
	addSymbol(line);
	return SUCCESS;

}
/* add Entry symbol type to the symbol table */
int addEntry(lineT line){	
	symbolT ptr = symbolhead;
	while( ptr ){
		if((!(strcmp(ptr->symbol,line->parameter)))){
			if( line->type[0] == _EXTERMAL ){
				printf("Entry and Extermal doesn't work toghter!\n");	
			}else{
				ptr->type[1] = _ENTRY; 
				return SUCCESS;
			}
		} ptr = ptr->next;
	}
	return FAILED;
}

/* return the address of the existing symbol*/
int returnAddress(char * word){
	lineT  ptr = linehead;	
	while( ptr ){		
		if( ptr->labelFlag && !(strcmp(ptr->symbol, word)) )	return ptr->address;  
		ptr = ptr->next;
	}
	return FAILED;
}


/* add the receive line to the the end of line table */
void addLine(lineT line){
    lineT tmp = linehead;
    while (tmp->next)	tmp = tmp->next;
    tmp->next = line;
    return;
}
/* add the receive symbol to the end of the symbol table */
void addSymbolToTable(symbolT symbol){
    symbolT tmp = symbolhead;
    while (tmp->next)	tmp = tmp->next;
    tmp->next = symbol;
    return;
}


/* return true if the symbol exist in synvol table */
int symbolExistInTable(char * word){
	symbolT ptr = symbolhead;	
	while( ptr ){
		if( !(strcmp(ptr->symbol, word)) )	return SUCCESS;
		ptr = ptr->next;
	}
	return FAILED;
}

/* add data symbol to symbol table and the the ic to address number */
int addDataToTable(){
	lineT ptr = linehead;
	for( ptr = linehead; ptr ; ptr = ptr->next ){
		if( ptr->command <= ASCIZ ) { 	ptr->address += fileD->ic;
			if( ptr->labelFlag ){
				if( symbolExistInTable(ptr->symbol) )	return FAILED;
				addSymbol(ptr);
			}
		}
	}
	return SUCCESS;

}
/* Take the info of the line and add to symbol table */
void addSymbol(lineT line){
	symbolT symbol = NULL;
	if(!symbolhead){
		symbolhead = (symbolT)calloc(1, sizeof(Symboltab));
		if(!symbolhead){
			printf("Error! memory not allocated.\n");
			fileD->errorFlag = 1;	
			return;
		}
		symbolhead->symbol = line->symbol;
		symbolhead->address = line->address;
		symbolhead->type[0] = line->type[0];
		symbolhead->type[1] = line->type[1];
	}else{
		symbol = (symbolT)calloc(1, sizeof(Symboltab));
		if(!symbol){
			printf("Error! memory not allocated.\n");
			fileD->errorFlag = 1;	
			return;
		}else	addSymbolToTable(symbol);
		symbol->symbol = line->symbol;
		symbol->address = line->address;
		symbol->type[0] = line->type[0];
		symbol->type[1] = line->type[1];
	}
}

/* Rerturn true if the symbol the parameter are in the symbol table and Extermal */
int isExt(lineT line){
	symbolT ptr = symbolhead;
	while( ptr ){
		if( ptr->type[0] == _EXTERMAL && line->parameter &&  !(strcmp(ptr->symbol, line->parameter)) )	return SUCCESS;
		ptr = ptr->next;
	}
	return FAILED;
}
/* Free symbol table */
void freeSymbol(){
    symbolT symbol = NULL;
    while (symbolhead){
        symbol = symbolhead;
        symbolhead = symbolhead->next;
        free(symbol);
    }
}
/* free line struct */
void freeLine(){
    lineT line = NULL;
    while (linehead){
        line = linehead;
        linehead = linehead->next;
        free(line);
    }
}
/* free all the structs we used */
void freeStructs(){
	if(fileD)	free(fileD);
	if(symbolhead)	freeSymbol();
	if(linehead)	freeLine();
}


