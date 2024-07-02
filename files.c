#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "files.h"
#include "assist.h"	
#include "analyze.h"

/* Reset fileD for the next file */
int fileDataReset(){
	fileD = NULL;
	fileD = (fileT)calloc(1, sizeof(Filedata));
	if(!fileD){
		printf("Error! memory not allocated.\n");
		return FAILED;
	}
	return SUCCESS;
}

/* reset ic and dc counter */
void counterReset(){
	fileD->ic = 100;
	fileD->dc = 0;
}

/* Crate the 3 required files */
void crateFiles(){
	entryExist();
	externExist();
	crateOb();
}

/* Check if there entry type in symbol table, if yes - crate ent file */
void entryExist(){
	symbolT ptr = symbolhead;
	while( ptr ){
		if( ptr->type[1] == _ENTRY ){
			crateEnt();
			break;
		}
		ptr = ptr->next;
	}
}

/* Check if there extern type in symbol table, if yes - crate ext file */
void externExist(){
	lineT ptr = linehead;
	while (ptr){
		if( isExt(ptr) && ptr->address ){
			crateExt();
			break;
		}
		ptr = ptr->next;
	}
}

/* crate Ent  file */
void crateEnt(){
	char fName[MAX_LINE] = "";
	FILE *file = NULL;
	symbolT ptr = symbolhead;
	strcat((strcat(fName, fileD->fileName)), ENT);
	file = fopen(fName, "w+");
	if ( !file ){
        	printf("Error! File can't be created!\n");
		fileD->errorFlag = 1;
		return;
	}
	while ( ptr ){
		if(ptr->type[1] == _ENTRY)
			fprintf(file, "%s %04d\n", ptr->symbol, ptr->address);
        	ptr = ptr->next;
	}
	fclose(file); 
}

/* crate Ext file */
void crateExt(){
	char fName[MAX_LINE];
	FILE *file = NULL;
	lineT ptr = linehead;
	strcat((strcpy(fName, fileD->fileName)), EXT);
	file = fopen(fName, "w+");
	if (!file){
        	printf("Error! File can't be created!\n");
		fileD->errorFlag = 1;
		return;
	}
    
	while (ptr){
		if( isExt(ptr) && ptr->address )
			fprintf(file, "%s %04d\n", ptr->parameter, ptr->address);
        	ptr = ptr->next;
	}
	fclose(file); 
}

/* crate Ob file */
void crateOb(){
	int  i= 0 ;
	char fName[MAX_LINE];
	FILE *file = NULL;
	lineT ptr = linehead;	
	strcat((strcpy(fName, fileD->fileName)), OB);
	
	file = fopen(fName, "w+");
	if(!file){
		printf("Error! File can't be created!\n");
		fileD->errorFlag = 1;
		return;
    	}
	fileD->address = 100;
	fprintf(file, "%d %d\n", fileD->ic - fileD->address, fileD->dc);
	while (ptr){	
		if(DH <= ptr->command && ptr->command <= ASCIZ ){
			i = littleEndingCode(ptr, file, i); /* i count the words, every 4 words its jump to next line */
		}else if(ptr->address >= 100){
			littleEnding(ptr, file);
		}
		ptr = ptr->next;
	}
	fclose(file);
}
