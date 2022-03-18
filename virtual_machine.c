//includes

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"
#include "moniC_definitions.h"
#include <math.h>

//structs

struct literalTableStruct {
	uint32_t index;
	uint16_t length;
};

struct functionTableStruct {
	_Bool decFlag; //set to false when a function is called in code before it is declared. Used for error checking
	char *lexptr; //address of function name in globalVarFunctionIDArray	
	uint16_t prgIndex; //program index at function declaration
};

struct globalVarTableStruct {
	char *lexptr;
	uint8_t dimension;
	uint32_t index; //start index of global variable in array used to store global variables at runtime (globalVar)
	uint16_t length; //length of global variable in memory blocks
};

//external variables

extern uint32_t stackPointer[STACKPOINTERDEPTH];
extern uint16_t stackPointerIndex;

extern uint32_t stack[STACKDEPTH];
extern uint32_t stackIndex;

//////////////////////////////////

extern uint32_t prgRelease[MAXBYTECODES];
extern uint16_t prgIndex;
extern uint16_t mainReleaseIndex;

////////////////////////////////////

extern uint16_t mainDebugIndex;

////////////////////////////////////

extern struct literalTableStruct literalTable[MAXLITERALS];
extern uint32_t literalArray[MAXLITERALARRAY];

///////////////////////////////////

extern struct globalVarTableStruct globalVarTable[MAXGLOBALVARS+1];
extern uint32_t globalVarArray[MAXGLOBALVARARRAY]; 
extern uint32_t globalVarArrayIndex;
extern uint16_t globalVariableLength[MAXGLOBALVARS]; 
extern uint16_t globalVarArrayStartIndex;

///////////////////////////////////

extern struct functionTableStruct functionReleaseTable[MAXFUNCTIONS+1]; 
extern struct functionTableStruct functionDebugTable[MAXFUNCTIONS+1]; 

///////////////////////////////////

extern uint16_t jumpArray1Release[JUMPSTACK];
extern uint16_t jumpArray2Release[JUMPSTACK];

extern uint16_t jumpArray1Debug[JUMPSTACK];
extern uint16_t jumpArray2Debug[JUMPSTACK];

//global variables

_Bool execFlag;

uint16_t returnStack[RETURNSTACK];
uint16_t returnStackIndex=0;

double addend1; //can be placed in function (see below) but for some reason uVision debugger doesn't display values in that case
double addend2;
double sum;

double minuend;
double subtrahend;
double difference;

double factor1;
double factor2;
double product;

double dividend;
double divisor;
double quotient;

//prototypes

void execute(uint32_t *prg, _Bool checkStackOverflowFlag);

//program_utilities prototypes

void set_lineNoDebug(uint16_t value);
void set_prg_index_virtual(uint16_t value);
void set_breakpoint_flag(_Bool flag);
void set_step_over_flag(_Bool);
_Bool get_step_over_flag(void);
uint16_t get_break_point_destination(void);

//math.c prototypes

_Bool double_test_less_than_or_equal(uint32_t *stack, uint32_t stackIndex);
_Bool double_test_less_than(uint32_t *stack, uint32_t stackIndex);
_Bool double_test_greater_than_or_equal(uint32_t *stack, uint32_t stackIndex);
_Bool double_test_greater_than(uint32_t *stack, uint32_t stackIndex);
_Bool double_test_equal(uint32_t *stack, uint32_t stackIndex);
_Bool double_test_not_equal(uint32_t *stack, uint32_t stackIndex);

//assembly prototypes

extern double thirty_two_bit_to_double(uint32_t lowerWord, uint32_t upperWord);
extern uint32_t lower_word_to_32bit(double value);
extern uint32_t upper_word_to_32bit(double value);

//functions

void virtual_machine(_Bool checkStackOverflowFlag) {
	prgIndex=0;
	execFlag=true;

	while (execFlag) {
		execute(prgRelease,checkStackOverflowFlag);
	}
}

void execute(uint32_t *prg,_Bool checkStackOverflowFlag) {
	_Bool flag;
	uint32_t i;

	switch (prg[prgIndex]) {
	case JUMPTOMAIN:
		if(prg==prgRelease) prgIndex=mainReleaseIndex;
		else prgIndex=mainDebugIndex;
		break;
	case PUSHLITERAL: //push object starting at literalTable[i].index onto the stack
	case PUSHLITERAL+1:
	case PUSHLITERAL+2:
	case PUSHLITERAL+3:
	case PUSHLITERAL+4:
	case PUSHLITERAL+5:
	case PUSHLITERAL+6:
		if (checkStackOverflowFlag) {
		}
		i=literalTable[prg[prgIndex]-PUSHLITERAL].index;
		stackPointer[++stackPointerIndex]=stackIndex;		 	
		while(i<literalTable[prg[prgIndex]-PUSHLITERAL].index+literalTable[prg[prgIndex]-PUSHLITERAL].length) {
			stack[stackIndex++]=literalArray[i++];
		}	
		break;
	case PUSHONE: //push 1 onto the stack
		stackPointer[++stackPointerIndex]=stackIndex;
		stack[stackIndex++]=DBL;
		stack[stackIndex++]=0;
		stack[stackIndex++]=0x3FF00000;
		break;
	case POPGLOBALVAR:
	case POPGLOBALVAR+1:
	case POPGLOBALVAR+2:
	case POPGLOBALVAR+3: //pop stack and store in global variable i
		globalVarArrayStartIndex=globalVarTable[prg[prgIndex]-POPGLOBALVAR+1].index;
		switch(globalVarTable[prg[prgIndex]-POPGLOBALVAR+1].dimension) {
		case 1: //one-dimensional array
			//decrement stack
			stackIndex = stackPointer[stackPointerIndex--];
			break;
		case 2: //two-dimensional array
			//decrement stack twice
			stackIndex = stackPointer[--stackPointerIndex];
			stackPointerIndex--;
			break;
		default: //scalar
			__asm volatile ("nop");
		}
		globalVarArrayIndex=globalVarArrayStartIndex*OBJECTLENGTH;
		i=stackPointer[stackPointerIndex];
		globalVariableLength[prg[prgIndex]-POPGLOBALVAR]=stackIndex-i;
		while(i<stackIndex) {
			globalVarArray[globalVarArrayIndex++]=stack[i++];
		}
		//decrement stack
		stackIndex=stackPointer[stackPointerIndex--];
		break;
	case PUSHGLOBALVAR:
	case PUSHGLOBALVAR+1:
	case PUSHGLOBALVAR+2:
	case PUSHGLOBALVAR+3: //push global variable i onto the stack
		globalVarArrayStartIndex=globalVarTable[prg[prgIndex]-PUSHGLOBALVAR+1].index;
		switch(globalVarTable[prg[prgIndex]-PUSHGLOBALVAR+1].dimension) {
		case 1: //one-dimensional array
			//decrement stack
			stackIndex = stackPointer[stackPointerIndex--];
			break;
		case 2: //two-dimensional array
			//decrement stack twice
			stackIndex = stackPointer[--stackPointerIndex];
			stackPointerIndex--;
			break;
		default: //scalar
			__asm volatile ("nop");
		}
		globalVarArrayIndex = globalVarArrayStartIndex*OBJECTLENGTH;
		i=globalVarArrayIndex+globalVariableLength[prg[prgIndex]-PUSHGLOBALVAR];
		stackPointer[++stackPointerIndex]=stackIndex;
		while(globalVarArrayIndex<i) {
			stack[stackIndex++]=globalVarArray[globalVarArrayIndex++];
		}
		break;
	case TESTGT:
	case TESTLT:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case APQ: //data second from the top of the stack is an apq
				break;
			}
			break;
			case DBL: //data at the top of the stack is a double
				switch (stack[stackPointer[stackPointerIndex-1]]) {
				case DBL: //data second from the top of the stack is a double
					switch (prg[prgIndex]) {
						case TESTLTE:
							flag=double_test_less_than_or_equal(stack,stackPointer[stackPointerIndex-1]+1);
							break;
						case TESTLT:
							flag=double_test_less_than(stack,stackPointer[stackPointerIndex-1]+1);
							break;
						case TESTGTE:
							flag=double_test_greater_than_or_equal(stack,stackPointer[stackPointerIndex-1]+1);
							break;
						case TESTGT:
							flag=double_test_greater_than(stack,stackPointer[stackPointerIndex-1]+1);
							break;
						case TESTEQ:
							flag=double_test_equal(stack,stackPointer[stackPointerIndex-1]+1);
              break;
            case TESTNEQ:
							flag=double_test_not_equal(stack,stackPointer[stackPointerIndex-1]+1);
              break;
					}					
					stackIndex = stackPointer[--stackPointerIndex];
					break;
				}
				break;
				case APF: //data at the top of the stack is an apf
					break;
		}
		if (flag) {//push true (double 1) onto the stack
			stack[stackIndex++]=DBL;
			stack[stackIndex++]=0;
			stack[stackIndex++]=0x3FF00000;
		}
		else {//push false (double 0) onto the stack
			stack[stackIndex++]=DBL;
			stack[stackIndex++]=0;
			stack[stackIndex++]=0;
		}
		break;
	case FALSEJUMPRELEASE:	
	case FALSEJUMPRELEASE+1:
	case FALSEJUMPRELEASE+2:
	case FALSEJUMPRELEASE+3:
	case FALSEJUMPRELEASE+4:
	case FALSEJUMPRELEASE+5: //pop top value from stack and, if it is 0, set prgIndex to jumpArray2Release[i]
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			break;
		case DBL: //data at the top of the stack is a double
			if((!stack[stackPointer[stackPointerIndex]+1]) && (!stack[stackPointer[stackPointerIndex]+2]))
				prgIndex=jumpArray2Release[prg[prgIndex]-FALSEJUMPRELEASE];
			break;
		}
		//decrement stack
		stackIndex = stackPointer[stackPointerIndex--];
		break;	
	case JUMPRELEASE:
	case JUMPRELEASE+1:	
	case JUMPRELEASE+2:
	case JUMPRELEASE+3:
	case JUMPRELEASE+4:	
	case JUMPRELEASE+5: //set prgIndex to jumpArray1Release[i]
		prgIndex = jumpArray1Release[prg[prgIndex]-JUMPRELEASE];
		break;
  case TRUEJUMPRELEASE:	
	case TRUEJUMPRELEASE+1:
	case TRUEJUMPRELEASE+2:
	case TRUEJUMPRELEASE+3:
	case TRUEJUMPRELEASE+4:
	case TRUEJUMPRELEASE+5: //pop top value from stack and, if it is not 0, set prgIndex to jumpArray2Release[i]
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			break;
		case DBL: //data at the top of the stack is a double
			if((stack[stackPointer[stackPointerIndex]+1]) || (stack[stackPointer[stackPointerIndex]+2])) 
				prgIndex=jumpArray2Release[prg[prgIndex]-TRUEJUMPRELEASE];
			break;
		}
		//decrement stack
		stackIndex = stackPointer[stackPointerIndex--];
		break;	
	case FALSEJUMPDEBUG:
	case FALSEJUMPDEBUG+1: 
	case FALSEJUMPDEBUG+2: 
	case FALSEJUMPDEBUG+3: 
	case FALSEJUMPDEBUG+4: 
	case FALSEJUMPDEBUG+5: //pop top value from stack and, if it is 0, set prgIndex to jumpArray2Debug[i]
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			break;
		case DBL: //data at the top of the stack is a double
			if((!stack[stackPointer[stackPointerIndex]+1]) && (!stack[stackPointer[stackPointerIndex]+2]))
				prgIndex=jumpArray2Debug[prg[prgIndex]-FALSEJUMPDEBUG];
			break;
		}
		//decrement stack
		stackIndex = stackPointer[stackPointerIndex--];
		break;
	case JUMPDEBUG:
	case JUMPDEBUG+1:
	case JUMPDEBUG+2:
	case JUMPDEBUG+3:
	case JUMPDEBUG+4:
	case JUMPDEBUG+5: //set prgIndex to jumpArray1Debug[i]
		 prgIndex = jumpArray1Debug[prg[prgIndex]-JUMPDEBUG];
		 break;
	case TRUEJUMPDEBUG:
	case TRUEJUMPDEBUG+1: 
	case TRUEJUMPDEBUG+2: 
	case TRUEJUMPDEBUG+3: 
	case TRUEJUMPDEBUG+4: 
	case TRUEJUMPDEBUG+5: //pop top value from stack and, if it is not 0, set prgIndex to jumpArray2Debug[i]
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			break;
		case DBL: //data at the top of the stack is a double
			if((stack[stackPointer[stackPointerIndex]+1]) || (stack[stackPointer[stackPointerIndex]+2]))
				prgIndex=jumpArray2Debug[prg[prgIndex]-TRUEJUMPDEBUG];
			break;
		}
		//decrement stack
		stackIndex = stackPointer[stackPointerIndex--];
		break;
	case CALLFUNCRELEASE:
	case CALLFUNCRELEASE+1:
	case CALLFUNCRELEASE+2:
	case CALLFUNCRELEASE+3:
	case CALLFUNCRELEASE+4:
	case CALLFUNCRELEASE+5:
		returnStack[returnStackIndex++]=prgIndex;
		prgIndex=functionReleaseTable[prg[prgIndex]-CALLFUNCRELEASE+1].prgIndex;
		break;
	case CALLFUNCDEBUG:
	case CALLFUNCDEBUG+1:
	case CALLFUNCDEBUG+2:
	case CALLFUNCDEBUG+3:
	case CALLFUNCDEBUG+4:
	case CALLFUNCDEBUG+5:
		returnStack[returnStackIndex++]=prgIndex;
		prgIndex=functionDebugTable[prg[prgIndex]-CALLFUNCDEBUG+1].prgIndex;
		break;
	case RETURNFROMFUNC:
		prgIndex=returnStack[--returnStackIndex];
		break;
	case ADDCODE:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case APQ: //data second from the top of the stack is an apq
				break;
			}
			break;
		case DBL: //data at the top of the stack is a double
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case DBL: //data second from the top of the stack is a double
				stackIndex=stackPointer[stackPointerIndex--];
				addend1=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);
				addend2=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				sum=addend1+addend2;
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(sum);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(sum);
				break;
			}
			break;
		}
		break;
	case SUBTRACTCODE:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case APQ: //data second from the top of the stack is an apq
				break;
			}
			break;
		case DBL: //data at the top of the stack is a double
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case DBL: //data second from the top of the stack is a double
				stackIndex=stackPointer[stackPointerIndex--];
				subtrahend=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);
				minuend=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				difference=minuend-subtrahend;
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(difference);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(difference);
				break;
			}
			break;
		}
		break;
	case MULTIPLYCODE:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case APQ: //data second from the top of the stack is an apq
				break;
			}
			break;
		case DBL: //data at the top of the stack is a double
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case DBL: //data second from the top of the stack is a double
				stackIndex=stackPointer[stackPointerIndex--];
				factor1=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);
				factor2=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				product=factor1*factor2;
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(product);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(product);
				break;
			}
			break;
		}
		break;
	case DIVIDECODE:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case APQ: //data second from the top of the stack is an apq
				break;
			}
			break;
		case DBL: //data at the top of the stack is a double
			switch (stack[stackPointer[stackPointerIndex-1]]) {
			case DBL: //data second from the top of the stack is a double
				stackIndex=stackPointer[stackPointerIndex--];
				divisor=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);
				dividend=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				quotient=dividend/divisor;
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(quotient);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(quotient);
				break;
			}
			break;
		}
		break;
	case NEGATECODE:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq			
			break;
		case DBL: //data at the top of the stack is a double				
				factor1=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				factor1=-factor1;
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(factor1);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(factor1);			
			break;
		}
		break;
	case TORAD:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq			
			break;
		case DBL: //data at the top of the stack is a double				
				factor1=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				factor1=factor1*PI/180;
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(factor1);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(factor1);			
			break;
		}
		break;
	case EXP:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq			
			break;
		case DBL: //data at the top of the stack is a double				
				factor1=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				factor1=exp(factor1);
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(factor1);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(factor1);			
			break;
		}
		break;
	case LOG:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq			
			break;
		case DBL: //data at the top of the stack is a double				
				factor1=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				factor1=log(factor1);
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(factor1);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(factor1);			
			break;
		}
		break;
//	case POWER:
//		switch (stack[stackPointer[stackPointerIndex]]) {
//		case APQ: //data at the top of the stack is an apq
//			switch (stack[stackPointer[stackPointerIndex-1]]) {
//			case APQ: //data second from the top of the stack is an apq
//				break;
//			}
//			break;
//		case DBL: //data at the top of the stack is a double
//			switch (stack[stackPointer[stackPointerIndex-1]]) {
//			case DBL: //data second from the top of the stack is a double
//				stackIndex=stackPointer[stackPointerIndex--];
//				factor1=thirty_two_bit_to_double(stack[stackIndex+1],stack[stackIndex+2]);
//				factor2=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
//				quotient=pow(factor2,factor1);
//				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(quotient);
//				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(quotient);
//				break;
//			}
//			break;
//		}
//		break;
	case SIN:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq			
			break;
		case DBL: //data at the top of the stack is a double				
				factor1=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				factor1=sin(factor1);		
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(factor1);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(factor1);			
			break;
		}
		break;
	case COS:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq			
			break;
		case DBL: //data at the top of the stack is a double				
				factor1=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				factor1=cos(factor1);		
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(factor1);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(factor1);			
			break;
		}
		break;
	case TAN:
		switch (stack[stackPointer[stackPointerIndex]]) {
		case APQ: //data at the top of the stack is an apq			
			break;
		case DBL: //data at the top of the stack is a double				
				factor1=thirty_two_bit_to_double(stack[stackPointer[stackPointerIndex]+1],stack[stackPointer[stackPointerIndex]+2]);
				factor1=tan(factor1);		
				stack[stackPointer[stackPointerIndex]+1]=lower_word_to_32bit(factor1);
				stack[stackPointer[stackPointerIndex]+2]=upper_word_to_32bit(factor1);			
			break;
		}
		break;
	case BKPT:
	case BKPT+1:
	case BKPT+2:
	case BKPT+3:
	case BKPT+4:
	case BKPT+5:	
	case BKPT+6:
	case BKPT+7:
	case BKPT+8:
	case BKPT+9:
	case BKPT+10:
	case BKPT+11:
    if(get_step_over_flag()) {
			if((prg[prgIndex]-BKPT)==get_break_point_destination()) {
				set_step_over_flag(false);
				set_lineNoDebug(prg[prgIndex]-BKPT);
				set_prg_index_virtual(prg[prgIndex]-BKPT);
				set_breakpoint_flag(true);	
			}
		}
    else {		
			set_lineNoDebug(prg[prgIndex]-BKPT);
			set_prg_index_virtual(prg[prgIndex]-BKPT);
			set_breakpoint_flag(true);
		}			
		break;
	case ENDPRG:
		execFlag=false;
		break;
	}
	prgIndex++;
}


