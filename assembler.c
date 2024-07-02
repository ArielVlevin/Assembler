#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "files.h"
#include "files.c"
#include "assist.c"	
#include "analyze.c"
#include "struct.c"
int main(int argc, char *argv[]){
	FILE *fp;
	char fName[MAX_LINE]; 
	int i = 1;
	for ( ; i < argc ; i++ ){
		if( !fileDataReset() )	break;

		fileD->fileName = argv[i];
		strcat(strcpy(fName, fileD->fileName), AS);
        	printf("%s:\n", fName);

        	fp = fopen(fName, "r");
        	if (!fp)	printf("The file %s cannot opened\n", fName);/*check if continue work*/
		else{
		fileD->file = fp;
		if( firstRun() )	
			if( secondRun() ){
				crateFiles();
				printf("Files created successfully!\n");
			}
		fclose(fp);
		if( fileD->errorFlag )	break;	/* Memmory error */
		}/*else - when file open*/	
	}/*for*/
	freeStructs();
	return 0;
}
/* The first pass */
int firstRun(){

	lineT ptr;	
	int fail = 0;
	symbolhead = NULL;
	counterReset();
	fail = textInput();	 /* textInput - analyze the text line by line, return failed only when memmory error */
	if( fileD->errorFlag )	return FAILED;
	ptr = linehead;
	
	while( ptr ){	/* this loop process the data textInput func analyzed by types and make the first pass rules */

		if( ptr->command <= ASCIZ ) {
			ptr->address = fileD->dc;
			if( ptr->command == DB ||  ptr->command == ASCIZ )	fileD->dc += ( ptr->datacount - 1 );
			else if( ptr->command == DH )	fileD->dc += ( SIZESHORT * ( ptr->datacount -1 ) );
			else /*dw*/	fileD->dc += ( SIZEINT * ( ptr->datacount - 1 ) );

		}else if( ptr->command == EXTERN ) {
			if( !addExtern(ptr) )	fail = 1;

		}else if( ptr->command >= ADD ){
			ptr->address = fileD->ic;		
			if( ptr->labelFlag ){
				if( symbolExistInTable(ptr->symbol) )	fail = 1;
				addSymbol(ptr);
			}
			fileD->ic += SIZEINT;
		}/* ptr->command >= ADD */
		ptr = ptr->next;
	}/* while */
	if  ( !addDataToTable() )	fail = 1; /* adding the ic to dc type commmand and add the data to symbol table */

	if( fail )	return FAILED;	/* If one of the lines is incorrect do not continue to second pass */
	return SUCCESS;
}

/* The second pass */
int secondRun(){

	int tmp, fail = 0;
	lineT  ptr = linehead;

	while( ptr ){	/* As first pass just following the second pass rules */

		if( ptr->command == ENTRY ) {
			if( !symbolExistInTable(ptr->parameter) )	{
				fail = 1;
				printf("This symbol do not exist in symbol's table: %s \n", ptr->parameter);
			}
			if( !addEntry(ptr) )	fail = 1;
	
		}else if( ptr->command >= BNE && ptr->command < JMP ) {
			if( !symbolExistInTable(ptr->parameter) || isExt(ptr) )	{
				fail = 1;
				if( !symbolExistInTable(ptr->parameter) )	printf("This symbol do not exist in symbol's table: %s \n", ptr->parameter);	
				else if( isExt(ptr) )	printf("Error! This type of command do not work with extern!\n"); 
			}else if( (tmp = returnAddress(ptr->parameter)) ) {
				ptr->data[2] = (tmp - ptr->address  );
				i_binar(ptr);
			}

		}else if( ptr->command >= JMP ){
			fail = j_binar(ptr);
		}
		ptr = ptr->next;

	}/* while */
	if( fail )	return FAILED;	 /* If one of the lines is incorrect do not continue to crating files */
	
	return SUCCESS;
}

		
