//includes

#include <stdint.h>
#include <stdbool.h>
#include "moniC_definitions.h"

//definitions
//compiler error types
typedef enum {LEXEMELEN
} CompilerErrorType;

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
	char *lexptr; //address of global variable name in globalVarFunctionIDArray
	uint8_t dimension; //scalar=0, one-dimensional array=1 etc.
	uint32_t startIndex; //start index of global variable in globalVar array, used to store global variables at runtime 
	uint16_t length; //length of global variable in memory blocks
};

//global variables

char nullProgram[1]={0};

char *source=nullProgram;
uint16_t sourceLines=0;

_Bool compileFlag=false;
_Bool compilerErrorFlag=false;
CompilerErrorType compilerError;

uint16_t sourcePageNumber;
uint16_t bytecodeReleasePageNumber=0;
uint16_t bytecodeDebugPageNumber=0;

uint32_t sourceIndex;

uint32_t sourceIndexPrevious; //source index at beginning of token
uint32_t sourceIndexNext; //source index after token when compiler error

uint16_t lineNo;
uint16_t lineNoDebug;

uint32_t prgRelease[MAXBYTECODES];
uint16_t prgReleaseSize;

uint32_t prgDebug[MAXBYTECODES];
uint16_t prgDebugSize;

uint16_t mainReleaseIndex;
uint16_t mainDebugIndex;

uint16_t statementTolineNoArray[MAXLINENO];
uint16_t statementTolineNoArraySize;
uint16_t prgIndexVirtual;
uint16_t prgIndexVirtualArray[MAXLINENO];

////////////////////////////////////////

uint16_t jumpArray1Release[JUMPSTACK];
uint16_t jumpArray1ReleaseSize;

uint16_t jumpArray2Release[JUMPSTACK];
uint16_t jumpArray2ReleaseSize;

uint16_t jumpArray1Debug[JUMPSTACK];
uint16_t jumpArray1DebugSize;

uint16_t jumpArray2Debug[JUMPSTACK];
uint16_t jumpArray2DebugSize;

uint16_t jumpArrayReleaseRaw[2*JUMPSTACK];
uint16_t jumpArrayRelease[2*JUMPSTACK];
uint16_t jumpArrayReleaseSize;

uint16_t jumpArrayDebugRaw[2*JUMPSTACK];
uint16_t jumpArrayDebug[2*JUMPSTACK];
uint16_t jumpArrayDebugSize;

//////////////////////////////////////////////

struct literalTableStruct literalTable[MAXLITERALS];
uint32_t literalArray[MAXLITERALARRAY];

//////////////////////////////////////////////

struct functionTableStruct functionReleaseTable[MAXFUNCTIONS+1]; //runtime variable (extern)
struct functionTableStruct functionDebugTable[MAXFUNCTIONS+1]; //runtime variable 

//////////////////////////////////////////////

struct globalVarTableStruct globalVarTable[MAXGLOBALVARS+1]; //runtime variable
uint16_t globalVarTableIndex;

uint32_t globalVarArray[MAXGLOBALVARARRAY]; //runtime variable
uint32_t globalVarArrayIndex; //runtime variable
uint16_t globalVariableLength[MAXGLOBALVARS]; //runtime variable
uint16_t globalVarArrayStartIndex; //runtime variable

//////////////////////////////////////////////

uint16_t prgIndex;
_Bool breakpointFlag;
_Bool stepOverFlag;
uint16_t breakpointDestination;

//functions

void set_source(char *sourceValue, uint16_t sourceLinesValue) {
	source=sourceValue;
	sourceLines=sourceLinesValue;
}

/////////////////////////////

void set_compiler_error_flag(_Bool flag) {
	compilerErrorFlag=flag;
}

_Bool get_compiler_error_flag(void) {
	return compilerErrorFlag;	
}

void set_compiler_error_type(CompilerErrorType error) {
	compilerError=error;	
}

//////////////////////////////

void set_sourceIndex(uint32_t value) {
	sourceIndex=value;
}

uint32_t get_sourceIndex(void) {
	return sourceIndex;
}

void set_source_index_previous(uint32_t sourceIndexValue) {
	sourceIndexPrevious=sourceIndexValue;
}

void set_source_index_next(uint32_t sourceIndexValue) {
	sourceIndexNext=sourceIndexValue;
}

///////////////////////////////

void set_main_release_index(uint16_t prgReleaseIndex) {
	mainReleaseIndex=prgReleaseIndex;
}

void set_main_debug_index(uint16_t prgDebugIndex) {
	mainDebugIndex= prgDebugIndex;
}

///////////////////////////////

void set_lineNo(uint16_t value) {
	lineNo=value;
}

uint16_t get_lineNo(void) {
	return lineNo;
}

///////////////////////////////

void set_prg_release(uint16_t prgReleaseIndex, uint32_t code) {
	prgRelease[prgReleaseIndex]=code;
}

uint32_t get_prg_release_code(uint16_t prgReleaseIndex) {
	return prgRelease[prgReleaseIndex];
}

uint32_t * get_prg_release(void) {
	return prgRelease;
}

void set_prg_release_size(uint16_t prgReleaseIndex) {
	prgReleaseSize=prgReleaseIndex;
}

uint16_t get_prg_release_size(void) {
	return prgReleaseSize;
}

////////////////////////////

void set_prg_debug(uint16_t prgDebugIndex, uint32_t code) {
	prgDebug[prgDebugIndex]=code;
}

uint32_t get_prg_debug_code(uint16_t prgDIndex) {
	return prgDebug[prgDIndex];
}

uint32_t * get_prg_debug(void) {
	return prgDebug;
}

void set_prg_debug_size(uint16_t prgDebugIndex) {
	prgDebugSize=prgDebugIndex;
}

uint16_t get_prg_debug_size(void) {
	return prgDebugSize;
}

//////////////////////////////

void set_statement_to_line_number_array(uint16_t statementTolineNoArrayIndex, uint16_t lineNumber) {
	statementTolineNoArray[statementTolineNoArrayIndex]=lineNumber;	
}

void set_statement_to_line_number_array_size(uint16_t index) {
	statementTolineNoArraySize=index;
}

void set_prg_index_virtual_array(uint16_t statementTolineNoArrayIndex, uint16_t prgReleaseIndex) {
	prgIndexVirtualArray[statementTolineNoArrayIndex]=prgReleaseIndex;
}

////////////////////////////////

void set_jump_array1_release(uint16_t jumpArray1ReleaseIndexLocal, uint16_t prgReleaseIndex) {
	jumpArray1Release[jumpArray1ReleaseIndexLocal]=prgReleaseIndex;
}

void set_jump_array1_release_size(uint16_t size) {
	jumpArray1ReleaseSize=size;
}

uint16_t get_jump_array1_release_size(void) {
	return jumpArray1ReleaseSize;
}

/////////////////////////////////

void set_jump_array2_release(uint16_t jumpArray2ReleaseIndexLocal, uint16_t prgReleaseIndex) {
	jumpArray2Release[jumpArray2ReleaseIndexLocal]=prgReleaseIndex;
}

void set_jump_array2_release_size(uint16_t size) {
	jumpArray2ReleaseSize=size;
}

uint16_t get_jump_array2_release_size(void) {
	return jumpArray2ReleaseSize;
}

/////////////////////////////

void set_jump_array1_debug(uint16_t jumpArray1DebugIndexLocal, uint16_t prgDebugIndex) {
	jumpArray1Debug[jumpArray1DebugIndexLocal]=prgDebugIndex;
}

void set_jump_array1_debug_size(uint16_t size) {
	jumpArray1DebugSize=size;
}

uint16_t get_jump_array1_debug_size(void) {
	return jumpArray1DebugSize;
}

//////////////////////////////

void set_jump_array2_debug(uint16_t jumpArray2DebugIndexLocal, uint16_t prgDebugIndex) {
	jumpArray2Debug[jumpArray2DebugIndexLocal]=prgDebugIndex;
}

void set_jump_array2_debug_size(uint16_t size) {
	jumpArray2DebugSize=size;
}

uint16_t get_jump_array2_debug_size(void) {
	return jumpArray2DebugSize;
}

///////////////////////////////

void create_jump_array_release(void) {
	uint16_t i;
	uint16_t j;
	uint16_t k=0;
	uint16_t sizeRaw=jumpArray1ReleaseSize+jumpArray2ReleaseSize;	
	uint16_t temp;

	for(i=0;i<jumpArray1ReleaseSize;i++) {
		jumpArrayReleaseRaw[i]=jumpArray1Release[i];
	}

	for(j=i;j<i+jumpArray2ReleaseSize;j++) {
		jumpArrayReleaseRaw[j]=jumpArray2Release[k++];
	}
	
	if(sizeRaw) {	
		for(i=0;i<sizeRaw-1;i++) { //bubble sort from smallest to largest
			for(j=i+1;j<sizeRaw;j++) {
				if(jumpArrayReleaseRaw[i]>jumpArrayReleaseRaw[j]) {
					temp=jumpArrayReleaseRaw[i];
					jumpArrayReleaseRaw[i]=jumpArrayReleaseRaw[j];
					jumpArrayReleaseRaw[j]=temp;
				}
			}
		}		
		j=0;
		jumpArrayRelease[0]=jumpArrayReleaseRaw[0];
		for(i=1;i<sizeRaw;i++) { //remove repeated values 
			if(jumpArrayReleaseRaw[i]!=jumpArrayRelease[j]) 
				jumpArrayRelease[++j]=jumpArrayReleaseRaw[i]; 
		}	
    jumpArrayReleaseSize=j+1;		
  }	
}

uint16_t is_release_program_step_a_label(uint16_t step) {
	uint16_t i;	
	
	for(i=0;i<jumpArrayReleaseSize;i++) {
		if (jumpArrayRelease[i]==step-1) return i+1; 
	}
	return 0;
}

///////////////////////////////////

void create_jump_array_debug(void) {	
	uint16_t i;
	uint16_t j;
	uint16_t k=0;
	uint16_t sizeRaw=jumpArray1DebugSize+jumpArray2DebugSize;
	uint16_t temp;

	for(i=0;i<jumpArray1DebugSize;i++) {
		jumpArrayDebugRaw[i]=jumpArray1Debug[i];
	}

	for(j=i;j<i+jumpArray2DebugSize;j++) {
		jumpArrayDebugRaw[j]=jumpArray2Debug[k++];
	}
	
	if(sizeRaw) {
		for(i=0;i<sizeRaw-1;i++) { //bubble sort from smallest to largest
			for(j=i+1;j<sizeRaw;j++) {
				if(jumpArrayDebugRaw[i]>jumpArrayDebugRaw[j]) {
					temp=jumpArrayDebugRaw[i];
					jumpArrayDebugRaw[i]=jumpArrayDebugRaw[j];
					jumpArrayDebugRaw[j]=temp;
				}
			}
		}		
		j=0;
		jumpArrayDebug[0]=jumpArrayDebugRaw[0];
		for(i=1;i<sizeRaw;i++) { //remove repeated values
      if(jumpArrayDebugRaw[i]!=jumpArrayDebug[j]) 
				jumpArrayDebug[++j]=jumpArrayDebugRaw[i];			
		}
    jumpArrayDebugSize=j+1;			
  }
}

uint16_t is_debug_program_step_a_label(uint16_t step) {
	uint16_t i;	
	
	for(i=0;i<jumpArrayDebugSize;i++) {
		if (jumpArrayDebug[i]+1==step-1) return i+1; 
	}
	return 0;
}

//////////////////////////////////

void set_literal_table_index(uint16_t literalTableIndex, uint32_t literalArrayIndex) {
	literalTable[literalTableIndex].index=literalArrayIndex;
}

uint32_t get_literal_table_index(uint16_t literalTableIndex) {
	return literalTable[literalTableIndex].index;
}

void set_literal_table_length(uint16_t literalTableIndex, uint32_t length) {
	literalTable[literalTableIndex].length=length;
}

uint32_t * get_literal_array(void) {
	return literalArray;
}

///////////////////////////////////////

char * get_function_table_lxptr(uint16_t index) {
	return functionReleaseTable[index].lexptr;	
}

void set_function_table_lexptr(uint16_t index, char *address) {
	functionReleaseTable[index].lexptr=address;
	functionDebugTable[index].lexptr=address;
}

void set_function_table_program_index(uint16_t index,uint16_t prgRIndex, uint16_t prgDIndex) {
	functionReleaseTable[index].prgIndex=prgRIndex;	
	functionDebugTable[index].prgIndex=prgDIndex;	
}

void set_function_table_dec_flag(uint16_t index,_Bool flag) {
	functionReleaseTable[index].decFlag=flag;	
	functionDebugTable[index].decFlag=flag;	
}

////////////////////////////////////////

void set_global_var_table_index(uint16_t index) {
	globalVarTableIndex=index;
}

uint16_t get_global_var_table_index(void) {
	return globalVarTableIndex;
}

void inc_global_var_table_index(void) {
	globalVarTableIndex++;
}

void set_global_var_table_lexptr(uint16_t globalVarTableIndexValue,char *address) {
	globalVarTable[globalVarTableIndexValue].lexptr=address;
}

char * get_global_var_table_lxptr(uint16_t globalVarTableIndexValue) {
	return globalVarTable[globalVarTableIndexValue].lexptr;
}

void set_global_var_table_dimension(uint16_t globalVarTableIndexValue,uint8_t dimension) {
	globalVarTable[globalVarTableIndexValue].dimension=dimension;
}

void set_global_var_table_length(uint16_t globalVarTableIndexValue, uint16_t length) {
	globalVarTable[globalVarTableIndexValue].length=length;	
}

uint16_t get_global_var_table_length(uint16_t globalVarTableIndexValue) {
	return globalVarTable[globalVarTableIndexValue].length;
}

void set_global_var_table_start_index(uint16_t globalVarTableIndexValue,uint32_t value) {
	globalVarTable[globalVarTableIndexValue].startIndex=value;
}

uint32_t get_global_var_table_start_index(uint16_t globalVarTableIndexValue) {
	return globalVarTable[globalVarTableIndexValue].startIndex;
}

uint32_t * get_global_var_array(void) {
	return globalVarArray;
}

//////////////////////////////////////////////////////

void set_lineNoDebug(uint16_t value) {
	lineNoDebug=statementTolineNoArray[value];
}

uint16_t get_lineNoDebug(void) {
	return lineNoDebug;
}

void set_prg_index_virtual(uint16_t value) {
	prgIndexVirtual=prgIndexVirtualArray[value];	
}

void set_breakpoint_flag(_Bool flag) {
	breakpointFlag=flag;
}

_Bool get_breakpoint_flag(void) {
	return breakpointFlag;
}

void set_step_over_flag(_Bool flag) {
	stepOverFlag=flag;
}

_Bool get_step_over_flag(void) {
	return stepOverFlag;
}

void set_break_point_destination(uint16_t lineNoDest) {
	uint16_t i;
	
	for(i=0;i<statementTolineNoArraySize;i++) {
		if(statementTolineNoArray[i]==lineNoDest) {
			breakpointDestination=i;
			break;
		}
	}	
}

uint16_t get_break_point_destination(void) {
	return breakpointDestination;
}

///////////////////////////////////////////////
