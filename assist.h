#pragma once
#ifndef _ASSIST_H
#define _ASSIST_H
/*define*/
#define SUCCESS 1
#define FAILED 0
#define EMPTY 2
#define WHITE_CHAR "\n\t\n\v\f\r "
#define MAX_CHAR 512

/*func*/
int binarToHex(char * binar);
void littleEnding(lineT line, FILE *file);
int littleEndingCode(lineT line, FILE *file, int i);
char * copy_str(const char *text);
int dospace(char ** str);
int input_exist_check(char * text, char * arr[]);
int input_length(char * input);
char * getWord(char ** text);
char * decimalToBinar(int a, int bit);
int isRegister(char  ch);
int isSemico(char ch);
int isQuomark(char  ch);
int getRegister(char ** text);
int getnumber(char ** text, int type);
int allIsInt(char * text);
#endif
