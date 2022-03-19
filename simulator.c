//MIT license

//moniC simulator v1.00

//includes

#include <stdint.h>
#include <stdlib.h>
#include "definitions.h"
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "moniC_programs.h"

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

char *stackValues[]={"0.6","0.8","0.005"}; //enter the values to be pushed onto the stack
uint8_t noOfStackValues=3; //enter the number of values to be pushed onto the stack 

char prgString1[COMMANDLINELENGTH+9];
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
	uint8_t i;  
	
	//push "noOfStackValues" value(s) onto the stack (algebraic entry mode)	
	for(i=0;i<noOfStackValues;i++) {		
		strcpy(prgString1,"main(){");
		strcat(prgString1,stackValues[i]);
		strcat(prgString1,prgString2);
		
		set_source(prgString1,1);	
	
		moniC_parser();  
		if(!get_compiler_error_flag()) {
			virtual_machine(true);		
		} 	
	}  	
	
	//select program "testSource" from moniC_programs.h		
  set_source(testSource,1);	
	
  //gen_sourceIndexArray(source); //in tools.c, used to generate sourceIndexArray, where sourceIndexArray[i] = source index at beginning
  //of program line i. Used for debugging parser	
	
	//parse and execute program "testSource"
	moniC_parser(); 
	if(!get_compiler_error_flag()) {
		virtual_machine(false);		
	}
	
	//pop value(s) off stack and convert back to double 
	while(stackIndex) {
		stackIndex=stackPointer[stackPointerIndex--];
		stackLevel1Value=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);
	}	
	
	/*remainder of code is for testing parts of the debugger on the actual calculator*/
	
	//push "noOfStackValues" value(s) back onto the stack (algebraic entry mode)	
	for(i=0;i<noOfStackValues;i++) {		
		strcpy(prgString1,"main(){");
		strcat(prgString1,stackValues[i]);
		strcat(prgString1,prgString2);
		
		set_source(prgString1,1);	
	
		moniC_parser(); 
		if(!get_compiler_error_flag()) {
			virtual_machine(true);		
		} 	
	}	

  //select program "testSource" from moniC_programs.h	 	
	set_source(testSource,1);

  //parse program "testSource"
	moniC_parser(); 

	create_jump_array_release(); //for testing the debugger
	create_jump_array_debug(); //for testing the debugger
	
	//start the debugger
  moniC_debugger();			
	
	//pop value(s) off stack and convert back to double 
	while(stackIndex) {
		stackIndex=stackPointer[stackPointerIndex--];
		stackLevel1Value=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);
	}		
}
