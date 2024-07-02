#pragma once
#ifndef _FILES_H
#define _FILES_H

/*define*/
#define SIZECHAR 1
#define SIZESHORT 2
#define SIZEINT 4

#define MAX_LINE 81 
#define SYMBOL_LENGTH 32
#define BIGGEST_BIT 31

#define AS ".as"
#define EXT ".ext"
#define ENT ".ent"
#define OB ".ob"

/* structs */
typedef struct symboltab * symbolT;
typedef struct symboltab{
	int address;
	char *symbol;
	int type[2];
	symbolT next;
}Symboltab;

typedef struct linetab * lineT;
typedef struct linetab{
	int address;
	char *linetext;
	int labelFlag;
	char *symbol; /* label name */
	char* binar_code;
	char* binar_code_s[MAX_LINE];
	int type[2];
	int command; /*command number from *all[] */
	char datat[MAX_LINE];
	int data[MAX_LINE];
	
	int datacount;
	char *parameter;
	lineT next;
}Linetab;


typedef struct filedata * fileT;
typedef struct filedata{
	FILE *file;
	char *fileName;
	int ic;
	int dc;
	int errorFlag;
	int address;
}Filedata;


/* struct's heads */
lineT linehead;
symbolT symbolhead;
fileT fileD;


/* strcut's func */
int isExt(lineT line);
int textInput();
int symbolExistInTable(char * word);
int returnAddress(char * word);
int addEntry(lineT line);
int addExtern(lineT line);
void addLine(lineT line);
void addSymbol(lineT line);
void freeSymbol();
void freeLine();
void freeStructs();
int addDataToTable();

/*file's funs*/
void crateEnt();
void crateExt();
void crateFiles();
void crateOb();
void counterReset();
void entryExist();
void externExist();
int fileDataReset();

/*assembler's func*/
int firstRun();
int secondRun();
#endif
