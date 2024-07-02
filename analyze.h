#pragma once
#ifndef _SEARCH_H
#define _SEARCH_H
#include "files.h"

/* define */
#define VAR 3
#define DECIMAL 10
#define F_ADDRESS 100
#define ADDRESS_JUMP 4
#define MAX_ADDRESS 9999
#define IF_ACT 17

#define BIGGEST_NUM 65535
#define SMALLEST_NUM -32768


/* enums*/
enum  acts { DH = 1, DW, DB, ASCIZ, ENTRY, EXTERN, ADD, SUB, AND, OR, NOR, MOVE, MVHI, MVLO, ADDI, SUBI, ANDI, ORI, NORI, LB, SB, LW, SW, LH, SH, BNE, BEQ, BLT, BGT, JMP, LA, CALL, STOP, _STOP = 63};
enum symboltypes {_DATA = 101, _CODE, _ENTRY, _EXTERMAL};
enum textlength {DBSIZE = 127, DHSIZE = 32767, DWSIZE = 2147483647};


/* arrays */
char (*allnames[]) = {"","add:","sub:","and:","or:","nor:","move:","mvhi:","mvlo:","addi:","subi:","andi:","ori:","nori:","bne:","beq:","blt:","bgt:","lb:","sb:","lw:","sw:","lh:","sh:","jmp:","la:","call:","stop:",".dh:",".dw:",".db:",".asciz:", ".entry:",".extern:", NULL};
char (*all[])={"",".dh",".dw",".db",".asciz",".entry",".extern","add","sub","and","or","nor","move","mvhi","mvlo","addi","subi","andi","ori","nori","lb","sb","lw","sw","lh","sh","bne","beq","blt","bgt","jmp","la","call","stop",NULL};


typedef struct R_Action{
	unsigned int  notisuse:6 ; 
	unsigned int  funct:5 ;  
	unsigned int  rd:5 ;
	unsigned int  rt:5 ;
	unsigned int  rs:5 ; 
	unsigned int  opcode:6 ;
}R_Action;

typedef struct I_Action{
	unsigned int  immed:16 ; 
	unsigned int  rt:5 ;  
	unsigned int  rs:5 ; 
	unsigned int  opcode:6 ;
}I_Action;

typedef struct J_Action{
	unsigned  int  address:25 ; 
	unsigned  int  reg:1 ;   
	unsigned  int  opcode:6 ;
}J_Action;

typedef struct byte{	unsigned int ch:8;	}byte;

typedef struct word{	unsigned int ch:32;	}word;

typedef struct halfword{	unsigned  int ch:16;	}halfword;


/* funcs */

int lineProcessing(char ** text, lineT line);
void r_binar(lineT line);
void i_binar(lineT line);
int j_binar(lineT line);
int codeExtract(char ** text, lineT line);
int label_check(char * text);
char * labelExtract(lineT line, int length);
int ascizExstract(char ** text, lineT line);
int numExtract(char ** text, lineT line);
int wordConv(lineT line);
int halfwordConv(lineT line);
int byteConv(lineT line);
int isData(lineT line, char * labelname);
int isCode(lineT line, char * labelname);
int isENEX(lineT line);
#endif
