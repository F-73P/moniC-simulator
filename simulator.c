//v1.00

//includes

#include <stdint.h>
#include <stdlib.h>
#include "definitions.h"
#include <string.h>
#include <stdbool.h>
#include <math.h>

//external variables - in program_utilities.c

extern char *source; //current program

//global variables

uint32_t stackPointer[STACKPOINTERDEPTH];
uint16_t stackPointerIndex=0;

uint32_t stack[STACKDEPTH];
uint32_t stackIndex=0;

double commandLineValue;
double stackLevel1Value;
double stackLevel2Value;

char commandLine[COMMANDLINELENGTH]="30";

char prgString1[COMMANDLINELENGTH+9]="main(){";
char prgString2[2]=";}";

//prototypes

void moniC_editor(uint8_t keyNumber);
void moniC_parser(void);
_Bool get_compiler_error_flag(void);
void virtual_machine(_Bool checkStackOverflowFlag);
void moniC_debugger(void);
void create_jump_array_release(void);
void create_jump_array_debug(void);

//program/program_utilities

void set_source(char *sourceValue, uint16_t sourceLinesValue);

//prototypes tools

void gen_sourceIndexArray(char *sourceLocal);

//push_double

uint32_t push_double(uint32_t *array, uint32_t arrayIndex, double doublePrecisionValue);

//assembly prototypes

extern double thirty_two_bit_to_double(uint32_t lowerWord, uint32_t upperWord);

//functions

int main(void) {
	//push command line value onto the stack (algebraic entry mode)
	strcat(prgString1,commandLine);
	strcat(prgString1,prgString2);
	
	set_source(prgString1,1);	
	
	moniC_parser(); 
	if(!get_compiler_error_flag()) {
		virtual_machine(true);		
	}  
	
	//push another value onto the stack (algebraic entry mode) - comment out if only one value pushed onto the stack
	strcpy(prgString1,"main(){");
	strcat(prgString1,"sin(60)");
	strcat(prgString1,prgString2);
	
	set_source(prgString1,1);	
	
	moniC_parser(); 
	if(!get_compiler_error_flag()) {
		virtual_machine(true);		
	}  
	
	//select program
	moniC_editor(1);

  //gen_sourceIndexArray(source); //in tools.c, used to generate sourceIndexArray, where sourceIndexArray[i] = source index at beginning
  //of program line i	
	
	//execute program
	moniC_parser(); 
	if(!get_compiler_error_flag()) {
		virtual_machine(false);		
	}

  //pop value off stack and convert to double 
  stackIndex=stackPointer[stackPointerIndex--];
	stackLevel1Value=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);		

  //pop value off stack and convert to double - if more than one value on the stack, comment out otherwise 
  stackIndex=stackPointer[stackPointerIndex--];
	stackLevel2Value=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);		
	
	//push double value(s) onto the stack for debugging
	commandLineValue=atof("5");
	stackIndex=push_double(stack,stackIndex,commandLineValue);
	stackPointer[++stackPointerIndex]=0;	
	
	create_jump_array_release(); //for debugging
	create_jump_array_debug(); //for debugging
	
	//debug
  moniC_debugger();	
	
	//pop value off stack and convert to double 
  stackIndex=stackPointer[stackPointerIndex--];
	stackLevel1Value=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);		
}
