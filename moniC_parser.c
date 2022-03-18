//includes
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "moniC_definitions.h"

//macros
#define match_check_advance(token) match(token);if(get_compiler_error_flag())return;advance();if(get_compiler_error_flag())return;

//definitions
typedef enum {ENDFILE, //book-keeping tokens
    MAINTOKEN,IF,ELSE,DO,WHILE,FOR,RETURN,READ,WRITE,AND,OR,NOT,TRUE,FALSE,LOCAL,ARRAY,SWITCH, //reserved words
	CASE,BREAK,DEFAULT,GOTO,NOPR, //reserved words
    ID,NUM, //multicharacter tokens
    //special symbols
    PLUS,MINUS,MULT,DIV,LT,LTE,GT,GTE,EQ,NEQ,ASSIGN,COLON,SEMI,COMMA,LPAREN,RPAREN,LBRACKET,RBRACKET,
	LBRACE,RBRACE,INC,DEC,ADDR
} TokenType;

//external variables
extern char lexemeString[MAXLEXEMELEN+1]; //in moniC_scanner.c

//global variables
static _Bool varDecFlag;
static uint16_t literalTableIndex;
static uint32_t literalArrayIndex;

static char globalVarFunctionIDArray[MAXGVFIDARRAY];
static uint16_t globalVarFunctionIDArrayIndex;

static uint16_t prgReleaseIndex;
uint16_t prgReleasePointer=MAXBYTECODES-1;

static uint16_t prgDebugIndex;
uint16_t prgDebugPointer=MAXBYTECODES-1;

static uint16_t statementTolineNoArrayIndex;

///////////////////////////////////

static uint16_t jumpArray1ReleaseIndexGlobal;
static uint16_t jumpArray2ReleaseIndexGlobal;

static uint16_t jumpArray1DebugIndexGlobal;
static uint16_t jumpArray2DebugIndexGlobal;

//////////////////////////////////

char *functionLibrary[MAXLIBRARYFUNCTIONS+1]={0,"exp","log","sin","cos","tan"};
uint16_t functionTableIndex;

//////////////////////////////////

static char *parameterTable[MAXPARAMETERS+1];
static uint16_t parameterTableIndex;

//////////////////////////////////

static char *localVarTable[MAXLOCALVARS+1];
static uint16_t localVarTableIndex;

//////////////////////////////////

static TokenType token;

//prototypes

void moniC_parser(void);

void code(void);
void var_declaration(void);
void dimensions(void);
void function_declaration(void);
void main_function_declaration(void);
void params(void);
void param(void);
void local_declarations(void);
void statement_list(void);
void statement(void);
void compound_stmt(void);
void if_stmt(void);
void while_stmt(void);
void do_stmt(void);
void for_stmt(void);
void switch_stmt(void);
void return_stmt(void);
void read_stmt(void);
void write_stmt(void);
void assignment_stmt(void);
void expression_stmt(void);
void goto_stmt(void);
void label_stmt(void);
void nop_stmt(void);
void assignment(void);
void expression(void);
void simple_expression(void);
void additive_expression(void);
void term(void);
void factor(void);
void args(void);

void advance(void);
void match(TokenType);

void save_literal(void);
uint32_t push_double(uint32_t *array, uint32_t arrayIndex, double double_precision_value);

void pop_variable(char *lexeme);
void push_variable(char *lexeme);

////////////////////////////////////////////

uint16_t lookup_function_library_table(char *string);
uint16_t lookup_function_table(char *string);
void insert_function_table(char *string, uint16_t prgRIndex, uint16_t prgDIndex );
void insert_undeclared_function_table(char *string); 

////////////////////////////////////////////

uint16_t lookup_parameterTable(char *string);

uint16_t lookup_localVarTable(char *string);

uint16_t lookup_globalVarTable(char *string);
void insert_globalVarTable(char *string, uint8_t dimension, uint16_t length);

void moniC_parser_variable_init(void);

//program_utilities.c prototypes

void set_sourceIndex(uint32_t value);
uint32_t get_sourceIndex(void);

void set_source_index_previous(uint32_t sourceIndexValue);
void set_source_index_next(uint32_t sourceIndexValue);

void set_main_release_index(uint16_t prgReleaseIndex);
void set_main_debug_index(uint16_t prgDebugIndex);

void set_lineNo(uint16_t value);
uint16_t get_lineNo(void);

//////////////////////////////////////////////////

void set_prg_release(uint16_t prgReleaseIndex, uint32_t code);
uint32_t get_prg_release_code(uint16_t prgRIndex);
void set_prg_release_size(uint16_t prgReleaseIndex);

//////////////////////////////////////////////////

void set_statement_to_line_number_array(uint16_t statementTolineNoArrayIndex, uint16_t lineNumber);
void set_statement_to_line_number_array_size(uint16_t index);

/////////////////////////////////////////////////

void set_compiler_error_flag(_Bool flag);
_Bool get_compiler_error_flag(void);

//////////////////////////////////////////////////

void set_prg_debug(uint16_t prgDebugIndex, uint32_t code);
void set_prg_debug_size(uint16_t prgDebugIndex);
uint32_t get_prg_debug_code(uint16_t prgDIndex);
/////////////////////////////////////////////////

void set_prg_index_virtual_array(uint16_t statementTolineNoArrayIndex, uint16_t prgReleaseIndex);

void set_jump_array1_release(uint16_t jumpArray1ReleaseIndexLocal, uint16_t prgReleaseIndex);
void set_jump_array1_release_size(uint16_t size);

void set_jump_array2_release(uint16_t jumpArray2ReleaseIndexLocal, uint16_t prgReleaseIndex);
void set_jump_array2_release_size(uint16_t size);

void set_jump_array1_debug(uint16_t jumpArray1DebugIndexLocal, uint16_t prgDebugIndex);
void set_jump_array1_debug_size(uint16_t size);

void set_jump_array2_debug(uint16_t jumpArray2DebugIndexLocal, uint16_t prgDebugIndex);
void set_jump_array2_debug_size(uint16_t size);

void set_literal_table_index(uint16_t literalTableIndex, uint32_t literalArrayIndex);
uint32_t get_literal_table_index(uint16_t literalTableIndex);

void set_literal_table_length(uint16_t literalTableIndex, uint32_t length);

uint32_t * get_literal_array(void);

///////////////////////////////////////

void set_function_table_lexptr(uint16_t index, char *address);
char * get_function_table_lxptr(uint16_t index);
void set_function_table_program_index(uint16_t index,uint16_t prgRIndex, uint16_t prgDIndex);
void set_function_table_dec_flag(uint16_t index,_Bool flag);

///////////////////////////////////////

void set_global_var_table_index(uint16_t index);
uint16_t get_global_var_table_index(void);
void inc_global_var_table_index(void);

void set_global_var_table_lexptr(uint16_t globalVarTableIndexValue, char *address);
void set_global_var_table_dimension(uint16_t globalVarTableIndexValue, uint8_t dimension);
void set_global_var_table_length(uint16_t globalVarTableIndexValue, uint16_t length);
void set_global_var_table_start_index(uint16_t globalVarTableIndexValue,uint32_t value);

uint32_t get_global_var_table_start_index(uint16_t globalVarTableIndexValue);
uint16_t get_global_var_table_length(uint16_t globalVarTableIndexValue);
char * get_global_var_table_lxptr(uint16_t globalVarTableIndexValue);

//moniC_scanner.c prototype

TokenType get_token(void);
	
//push_double.c prototype

uint32_t push_double(uint32_t *array, uint32_t arrayIndex, double doublePrecisionValue);

//functions

void moniC_parser(void) {
	moniC_parser_variable_init();  
	code();
	if(get_compiler_error_flag()) {
		set_source_index_next(get_sourceIndex());
	}	
}

void code(void) {
	advance(); if(get_compiler_error_flag()) return;
	while((token==ARRAY||token==MULT||token==ID) && varDecFlag) {
		var_declaration();if(get_compiler_error_flag()) return;
	}
	while(token==ID) {
		function_declaration();if(get_compiler_error_flag()) return;
	}
	main_function_declaration();
	match(ENDFILE); if(get_compiler_error_flag()) return;
	
	set_prg_release(prgReleaseIndex,ENDPRG);
	set_prg_release_size(prgReleaseIndex+1);

	set_prg_debug(prgDebugIndex,ENDPRG);
	set_prg_debug_size(prgDebugIndex+1);
	
	set_statement_to_line_number_array_size(statementTolineNoArrayIndex);
	
	set_jump_array1_release_size(jumpArray1ReleaseIndexGlobal);
	set_jump_array2_release_size(jumpArray2ReleaseIndexGlobal);	
	set_jump_array1_debug_size(jumpArray1DebugIndexGlobal);
	set_jump_array2_debug_size(jumpArray2DebugIndexGlobal);
}

void var_declaration(void) {
	uint32_t sourceIndexStart;
	char lexemeStringCopy[MAXLEXEMELEN+1];
	
	switch(token) {
	case ARRAY:
		advance();
		match_check_advance(ID)
		dimensions(); if(get_compiler_error_flag()) return;
		match_check_advance(SEMI)
		break;
	case MULT: //pointer
		advance();
		match_check_advance(ID)
		match_check_advance(SEMI)
		break;
	default: //ID - extra lookahead required to determine whether variable or function declaration
		sourceIndexStart=get_sourceIndex(); //save current sourceIndex
	  strcpy(lexemeStringCopy,lexemeString); //save lexeme before lookahead
		advance();
		switch(token) {
		case SEMI: //variable declaration
			advance();
			break;
		default: //function declaration
			match(LPAREN); if(get_compiler_error_flag()) return;
			token=ID;
			set_sourceIndex(sourceIndexStart);// restore sourceIndex
			varDecFlag=false;
		}
		strcpy(lexemeString,lexemeStringCopy); //restore lexeme
	}
}

void dimensions(void) {
//	match_check_advance(LBRACKET)
//	match_check_advance(NUM)
//	match_check_advance(RBRACKET)
//	while(token==LBRACKET){
//		advance();
//		match_check_advance(NUM)
//		match_check_advance(RBRACKET)
//	}
}

void function_declaration(void) {
	uint16_t i;  
	
	i=lookup_function_table(lexemeString);
	if (i) set_function_table_program_index(functionTableIndex,prgReleaseIndex-1,prgDebugIndex-1);
	//above occurs when function called in code before function is declared
	else insert_function_table(lexemeString,prgReleaseIndex-1,prgDebugIndex-1); 
	
	match_check_advance(ID)
	match_check_advance(LPAREN)
	params(); if(get_compiler_error_flag()) return;
	match_check_advance(RPAREN)
	match_check_advance(LBRACE)
	local_declarations(); if(get_compiler_error_flag()) return;
	statement_list(); if(get_compiler_error_flag()) return;
	
	if(get_prg_release_code(prgReleaseIndex-1)!=RETURNFROMFUNC) {
		set_prg_release(prgReleaseIndex,RETURNFROMFUNC); /*return to caller*/
	}		
	match_check_advance(RBRACE)
}

void main_function_declaration(void) {
	set_main_release_index(prgReleaseIndex-1);
	set_main_debug_index(prgDebugIndex-1);
	match_check_advance(MAINTOKEN)
	match_check_advance(LPAREN)
	params(); if(get_compiler_error_flag()) return;
	match_check_advance(RPAREN)
	match_check_advance(LBRACE)

	local_declarations(); if(get_compiler_error_flag()) return;
	statement_list(); if(get_compiler_error_flag()) return;
	match_check_advance(RBRACE)
}

void params(void) {
//	if (token==MULT || token==ID) {
//		param(); if(get_compiler_error_flag()) return;
//		while (token==COMMA) {
//			advance();
//			param(); if(get_compiler_error_flag()) return;
//		}
//	}
}

void param(void){
//	switch(token) {
//	case MULT: //pointer
//		advance();
//		match_check_advance(ID)
//		break;
//	default: //ID
//		match_check_advance(ID)
//		while(token==LBRACKET) {
//			advance();
//			match_check_advance(NUM)
//			match_check_advance(RBRACKET)
//		}
//	}
}

void local_declarations(void){
//	while(token==LOCAL){
//		advance();
//		var_declaration(); if(get_compiler_error_flag()) return;
//	}
}

void statement_list(void) {
	while(token!=RBRACE) {
		statement(); if(get_compiler_error_flag()) return;
	}
}

void statement(void) {
	uint32_t sourceIndexStart;
	TokenType lookaheadToken;
	char lexemeStringCopy[MAXLEXEMELEN+1];

	switch(token) {
	case LBRACE:
		compound_stmt();
		break;
	case IF:
		if_stmt();
		break;
	case WHILE:
		while_stmt();
		break;
	case DO:
		do_stmt();
		break;
	case FOR:
		for_stmt();
		break;
	case SWITCH:
		switch_stmt();
		break;
	case RETURN:
		return_stmt();
		break;
	case READ:
		read_stmt();
		break;
	case WRITE:
		write_stmt();
		break;
	case GOTO:
		goto_stmt();
		break;
	case NOPR:
		nop_stmt();
		break;
	case ID: //extra lookahead required to determine whether assignment, label or expression statement
		sourceIndexStart=get_sourceIndex(); //save current sourceIndex
		strcpy(lexemeStringCopy,lexemeString); //save lexeme before lookahead
		advance();
		lookaheadToken=token;
		set_sourceIndex(sourceIndexStart);// restore sourceIndex
		strcpy(lexemeString,lexemeStringCopy); //restore lexeme
		token=ID; //restore ID
		switch(lookaheadToken) {
		case ASSIGN: //assignment statement
			assignment_stmt();
			break;
		case COLON: //label statement
			label_stmt();
		  break;
		default: //expression
			expression_stmt();
		}
		break;
	default: //expression statement
		expression_stmt();
	}
}

void compound_stmt(void) {
	match_check_advance(LBRACE)
	statement_list(); if(get_compiler_error_flag()) return;
	match_check_advance(RBRACE)
}

void if_stmt(void) {
	uint16_t jumpArray1ReleaseIndexLocal;
	uint16_t jumpArray2ReleaseIndexLocal;

	uint16_t jumpArray1DebugIndexLocal;
	uint16_t jumpArray2DebugIndexLocal;
	
	/*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/	
	
	match_check_advance(IF)
	match_check_advance(LPAREN)	
	expression(); if(get_compiler_error_flag()) return;	
	match_check_advance(RPAREN)
	
	/*jump to jumpArray2[jumpArray2IndexLocal] if condition is false*/
	jumpArray2ReleaseIndexLocal = jumpArray2ReleaseIndexGlobal++;	
	set_prg_release(prgReleaseIndex++,FALSEJUMPRELEASE+jumpArray2ReleaseIndexLocal);
	
	jumpArray2DebugIndexLocal = jumpArray2DebugIndexGlobal++;
	set_prg_debug(prgDebugIndex++,FALSEJUMPDEBUG+jumpArray2DebugIndexLocal);
	/************************************/
	
	statement(); if(get_compiler_error_flag()) return;
	if (token==ELSE) {
		advance();
		
		/*jump to jumpArray1[jumpArray1IndexLocal] to exit ELSE block when condition is true*/
		jumpArray1ReleaseIndexLocal = jumpArray1ReleaseIndexGlobal++;	
	  set_prg_release(prgReleaseIndex++,JUMPRELEASE+jumpArray1ReleaseIndexLocal);
		
		jumpArray1DebugIndexLocal = jumpArray1DebugIndexGlobal++;
	  set_prg_debug(prgDebugIndex++,JUMPDEBUG+jumpArray1DebugIndexLocal);
    /************************************/
		
		set_jump_array2_release(jumpArray2ReleaseIndexLocal,prgReleaseIndex-1); //program jumps here if condition is false and ELSE block present	
	  set_jump_array2_debug(jumpArray2DebugIndexLocal,prgDebugIndex-1);
		
		statement();
		
		set_jump_array1_release(jumpArray1ReleaseIndexLocal,prgReleaseIndex-1); //program jumps here if condition is true and ELSE block present	
	  set_jump_array1_debug(jumpArray1DebugIndexLocal,prgDebugIndex-1);
	}
	else {
		set_jump_array2_release(jumpArray2ReleaseIndexLocal,prgReleaseIndex-1); //program jumps here if condition is false and ELSE block not present	
	  set_jump_array2_debug(jumpArray2DebugIndexLocal,prgDebugIndex-1);
	}
}

void while_stmt(void) {
	uint16_t jumpArray1ReleaseIndexLocal;
	uint16_t jumpArray2ReleaseIndexLocal;

	uint16_t jumpArray1DebugIndexLocal;
	uint16_t jumpArray2DebugIndexLocal;
	
	/*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/	

	match_check_advance(WHILE)
	match_check_advance(LPAREN)

	jumpArray1ReleaseIndexLocal = jumpArray1ReleaseIndexGlobal++;
	set_jump_array1_release(jumpArray1ReleaseIndexLocal,prgReleaseIndex-1); //start of while loop, program jumps
	//here to test the condition again	 

	jumpArray1DebugIndexLocal = jumpArray1DebugIndexGlobal++;
	set_jump_array1_debug(jumpArray1DebugIndexLocal,prgDebugIndex-2); //step before start of while loop,
	//program jumps to breakpoint, display the line to be executed and test the condition again	

	expression();if(get_compiler_error_flag()) return;
	match_check_advance(RPAREN)

	/*jump to jumpArray2[jumpArray2IndexLocal] if condition is false*/
	jumpArray2ReleaseIndexLocal = jumpArray2ReleaseIndexGlobal++;	
	set_prg_release(prgReleaseIndex++,FALSEJUMPRELEASE+jumpArray2ReleaseIndexLocal);
	
	jumpArray2DebugIndexLocal = jumpArray2DebugIndexGlobal++;
	set_prg_debug(prgDebugIndex++,FALSEJUMPDEBUG+jumpArray2DebugIndexLocal);
	/************************************/

	statement();if(get_compiler_error_flag()) return;

	/*jump to jumpArray1[jumpArray1Index-1] and test condition again*/
	set_prg_release(prgReleaseIndex++,JUMPRELEASE+jumpArray1ReleaseIndexLocal);	
	set_prg_debug(prgDebugIndex++,JUMPDEBUG+jumpArray1DebugIndexLocal);
	/************************************/

	set_jump_array2_release(jumpArray2ReleaseIndexLocal,prgReleaseIndex-1); //end of while loop, program jumps here if 
	//condition is false
	set_jump_array2_debug(jumpArray2DebugIndexLocal,prgDebugIndex-1); //end of while loop, program jumps here if
	//condition is false
}

void do_stmt(void) {	
	uint16_t jumpArray2ReleaseIndexLocal;	
	uint16_t jumpArray2DebugIndexLocal;
	
	/*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/	
	
	match_check_advance(DO)
	
	jumpArray2ReleaseIndexLocal = jumpArray2ReleaseIndexGlobal++;
	set_jump_array2_release(jumpArray2ReleaseIndexLocal,prgReleaseIndex-1); //start of do while loop, program jumps
	//here to execute again

  jumpArray2DebugIndexLocal = jumpArray2DebugIndexGlobal++;
	set_jump_array2_debug(jumpArray2DebugIndexLocal,prgDebugIndex-2); //step before start of do while loop, 
	//program jumps to breakpoint, display the line to be executed and execute again	
	
	statement(); if(get_compiler_error_flag()) return;
	match_check_advance(WHILE)
	match_check_advance(LPAREN)
	expression(); if(get_compiler_error_flag()) return;
	match_check_advance(RPAREN)
	
	/*jump to jumpArray2[jumpArray2Index-1] and test condition again*/
	set_prg_release(prgReleaseIndex++,TRUEJUMPRELEASE+jumpArray2ReleaseIndexLocal);	
	set_prg_debug(prgDebugIndex++,TRUEJUMPDEBUG+jumpArray2DebugIndexLocal);		
	/************************************/
	
	match(SEMI); advance();	
}

void for_stmt(void){
	uint16_t jumpArray1ReleaseIndexLocal;
	uint16_t jumpArray2ReleaseIndexLocal;

	uint16_t jumpArray1DebugIndexLocal;
	uint16_t jumpArray2DebugIndexLocal;
	
	uint16_t i;
	uint16_t prgReleaseIndexUpdateCtrlStart; 
	uint16_t prgDebugIndexUpdateCtrlStart; 
	uint16_t noOfUpdateCtrlInstructionsRelease;
  uint16_t noOfUpdateCtrlInstructionsDebug; 	
	
	match_check_advance(FOR)
	match_check_advance(LPAREN)

	assignment(); if(get_compiler_error_flag()) return; //initialise control expression e.g. i=0
	match_check_advance(SEMI)
	
	/*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/	
	
	jumpArray1ReleaseIndexLocal = jumpArray1ReleaseIndexGlobal++;
	set_jump_array1_release(jumpArray1ReleaseIndexLocal,prgReleaseIndex-1); //start of for loop, program jumps
	//here to test the condition again	 

	jumpArray1DebugIndexLocal = jumpArray1DebugIndexGlobal++;
	set_jump_array1_debug(jumpArray1DebugIndexLocal,prgDebugIndex-2); //step before start of for loop,
	//program jumps to breakpoint, display the line to be executed and test the condition again

  expression();	if(get_compiler_error_flag()) return; //test control expression e.g. i<10
	match_check_advance(SEMI)
	
	/*jump to jumpArray2[jumpArray2IndexLocal] if condition is false*/
	jumpArray2ReleaseIndexLocal = jumpArray2ReleaseIndexGlobal++;	
	set_prg_release(prgReleaseIndex++,FALSEJUMPRELEASE+jumpArray2ReleaseIndexLocal);
	
	jumpArray2DebugIndexLocal = jumpArray2DebugIndexGlobal++;
	set_prg_debug(prgDebugIndex++,FALSEJUMPDEBUG+jumpArray2DebugIndexLocal);
	/************************************/
	
	prgReleaseIndexUpdateCtrlStart = prgReleaseIndex;
  prgDebugIndexUpdateCtrlStart = prgDebugIndex;	
	
	expression();	if(get_compiler_error_flag()) return; //update control expression e.g. i++
	match_check_advance(RPAREN)
	
	/*push update control expression instructions onto the end of the program stack*/
	noOfUpdateCtrlInstructionsRelease = prgReleaseIndex-prgReleaseIndexUpdateCtrlStart; 
	for(i=noOfUpdateCtrlInstructionsRelease;i>0;i--) 
	set_prg_release(prgReleasePointer--,get_prg_release_code(prgReleaseIndexUpdateCtrlStart+i-1));
	
	noOfUpdateCtrlInstructionsDebug = prgDebugIndex-prgDebugIndexUpdateCtrlStart; 
	for(i=noOfUpdateCtrlInstructionsDebug;i>0;i--) 
	set_prg_debug(prgDebugPointer--,get_prg_debug_code(prgDebugIndexUpdateCtrlStart+i-1));
	/***********************/
	
	prgReleaseIndex=prgReleaseIndexUpdateCtrlStart;
	prgDebugIndex=prgDebugIndexUpdateCtrlStart; 
	
	statement(); if(get_compiler_error_flag()) return;
	
	/*push update control expression instructions into the correct position on the program stack*/
	for(i=noOfUpdateCtrlInstructionsRelease;i>0;i--) 
	set_prg_release(prgReleaseIndex++,get_prg_release_code(++prgReleasePointer));
	
	for(i=noOfUpdateCtrlInstructionsDebug;i>0;i--) 
	set_prg_debug(prgDebugIndex++,get_prg_debug_code(++prgDebugPointer));
	/***********************/
	
	/*jump to jumpArray1[jumpArray1Index-1] and test condition again*/
	set_prg_release(prgReleaseIndex++,JUMPRELEASE+jumpArray1ReleaseIndexLocal);	
	set_prg_debug(prgDebugIndex++,JUMPDEBUG+jumpArray1DebugIndexLocal);
	/************************************/
	
	set_jump_array2_release(jumpArray2ReleaseIndexLocal,prgReleaseIndex-1); //end of for loop, program jumps here if 
	//condition is false
	set_jump_array2_debug(jumpArray2DebugIndexLocal,prgDebugIndex-1); //end of while loop, program jumps here if
	//condition is false
}

void switch_stmt(void){
//	match_check_advance(SWITCH)
//	match_check_advance(LPAREN)
//	expression(); if(get_compiler_error_flag()) return;
//	match_check_advance(RPAREN)
//	match_check_advance(LBRACE)
//	while(token==CASE) {
//		advance();
//		match_check_advance(NUM)
//		match_check_advance(COLON)
//		statement_list(); if(get_compiler_error_flag()) return;
//		match_check_advance(BREAK)
//		match_check_advance(SEMI)
//	}
//	if (token==DEFAULT) {
//		advance();
//		match_check_advance(COLON)
//		statement_list(); if(get_compiler_error_flag()) return;
//	}
//	match_check_advance(RBRACE)
}

void return_stmt(void){
	/*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/	
	
	match_check_advance(RETURN)	
	if (token!=SEMI) {
		expression(); if(get_compiler_error_flag()) return;		
	}
	match_check_advance(SEMI)
	set_prg_release(prgReleaseIndex++,RETURNFROMFUNC); /*return to caller*/
	set_prg_debug(prgDebugIndex++,RETURNFROMFUNC); 
}

void read_stmt(void) {
	char lexemeStringCopy[MAXLEXEMELEN+1];
	_Bool array=false;
	
	/*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/	

	match_check_advance(READ)

	switch(token){
	case MULT: //pointer
		break;
	default: //extra lookahead required to determine if array
		strcpy(lexemeStringCopy,lexemeString); //save lexeme before lookahead
		match_check_advance(ID)

		while(token==LBRACKET){
			array=true;
			advance();
			expression();if(get_compiler_error_flag()) return;
			match_check_advance(RBRACKET)
		}
		if(array) {
			//check that lexemeStringCopy is an array (declared at beginning of program)
			//push array indices onto the stack
		}
	}

	pop_variable(lexemeStringCopy);

	match_check_advance(SEMI)
}

void write_stmt(void) {
	/*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/	
	
	match_check_advance(WRITE)
	expression(); if(get_compiler_error_flag()) return;
	match_check_advance(SEMI)
}

void assignment_stmt(void) {
  /*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/		
	
	assignment(); if(get_compiler_error_flag()) return;
	match_check_advance(SEMI)
}

void expression_stmt(void) {
	/*set breakpoint for debugging*/
	set_statement_to_line_number_array(statementTolineNoArrayIndex,get_lineNo());
	set_prg_index_virtual_array(statementTolineNoArrayIndex,prgReleaseIndex);	
	set_prg_debug(prgDebugIndex++,BKPT+statementTolineNoArrayIndex++);	
	/****************/	
	
	expression();
	if(get_compiler_error_flag()) return;
	match_check_advance(SEMI)
}

void goto_stmt(void) {
//	match_check_advance(SEMI)
//	match_check_advance(ID)
//	match_check_advance(SEMI)
}

void label_stmt(void) {
//	match_check_advance(ID)
//	match_check_advance(COLON)
//	statement_list();
}

void nop_stmt(void) {
//	match_check_advance(NOPR)
//	match_check_advance(SEMI)
}

void assignment(void) {
	char lexemeStringCopy[MAXLEXEMELEN+1];
	_Bool array=false;

	switch(token){
	case MULT: //pointer
		break;
	default: //extra lookahead required to determine if array
		strcpy(lexemeStringCopy,lexemeString); //save lexeme before lookahead
		match_check_advance(ID)

		while(token==LBRACKET){
			array=true;
			advance();
			expression();if(get_compiler_error_flag()) return;
			match_check_advance(RBRACKET)
		}
		if(array) {
			//check that lexemeStringCopy is an array (declared at beginning of program)
			//push array indices onto the stack
		}
	}

	match_check_advance(ASSIGN)

	expression();if(get_compiler_error_flag()) return;

	pop_variable(lexemeStringCopy);
}

void expression(void) {
	simple_expression(); if(get_compiler_error_flag()) return;
	while(token==AND || token==OR || token==NOT) {
		advance();
		simple_expression(); if(get_compiler_error_flag()) return;
	}
}

void simple_expression(void){
	additive_expression();if(get_compiler_error_flag()) return;
	if (token==LTE || token==LT || token==GT || token==GTE || token==EQ || token==NEQ) {
		switch(token) {
		case LTE:
			advance();
			additive_expression();
			/*pop top two levels from stack, push 1 if stack[2]<=stack[1], push 0 otherwise*/
		  set_prg_release(prgReleaseIndex++,TESTLTE);	
	    set_prg_debug(prgDebugIndex++,TESTLTE);		
			/************************************/
			break;
		case LT:
			advance();
			additive_expression();
			/*pop top two levels from stack, push 1 if stack[2]<stack[1], push 0 otherwise*/
		  set_prg_release(prgReleaseIndex++,TESTLT);	
	    set_prg_debug(prgDebugIndex++,TESTLT);
			/************************************/
			break;
		case GT:
			advance();
			additive_expression();
			/*pop top two levels from stack, push 1 if stack[2]>stack[1], push 0 otherwise*/
		  set_prg_release(prgReleaseIndex++,TESTGT);	
	    set_prg_debug(prgDebugIndex++,TESTGT);		
			/************************************/
			break;
		case GTE:
			advance();
			additive_expression();
			/*pop top two levels from stack, push 1 if stack[2]>=stack[1], push 0 otherwise*/
		  set_prg_release(prgReleaseIndex++,TESTGTE);	
	    set_prg_debug(prgDebugIndex++,TESTGTE);			
			/************************************/
			break;
		case EQ:
			advance();
			additive_expression();
			/*pop top two levels from stack, push 1 if stack[2]=stack[1], push 0 otherwise*/
		  set_prg_release(prgReleaseIndex++,TESTEQ);	
	    set_prg_debug(prgDebugIndex++,TESTEQ);			
			/************************************/
			break;
		default: //NEQ
			advance();
			additive_expression();
			/*pop top two levels from stack, push 1 if stack[2]!=stack[1], push 0 otherwise*/
		  set_prg_release(prgReleaseIndex++,TESTNEQ);	
	    set_prg_debug(prgDebugIndex++,TESTNEQ);		
			/************************************/
		}
	}
}

void additive_expression(void) {
	term();if(get_compiler_error_flag()) return;
	while(token==PLUS || token==MINUS) {
		switch(token) {
		case PLUS:
			advance();
			term();if(get_compiler_error_flag()) return;
			/*pop top two levels from stack, add, then push sum onto stack*/
			set_prg_release(prgReleaseIndex++,ADDCODE);	
	    set_prg_debug(prgDebugIndex++,ADDCODE);	
			/************************************/
			break;
		default:
			advance();
			term();if(get_compiler_error_flag()) return;
			/*pop top two levels from stack, subtract, then push difference onto stack*/
			set_prg_release(prgReleaseIndex++,SUBTRACTCODE);	
	    set_prg_debug(prgDebugIndex++,SUBTRACTCODE);
			/************************************/
			break;
		}
	}
}

void term(void) {
	_Bool negateFlag=false;
	
	if(token==MINUS) {
		negateFlag=true;
		advance();
	}
	factor();if(get_compiler_error_flag()) return;
	while(token==MULT || token==DIV) {
		switch(token) {
		case MULT:
			advance();
			factor();if(get_compiler_error_flag()) return;
			/*pop top two levels from stack, add, then push sum onto stack*/
			set_prg_release(prgReleaseIndex++,MULTIPLYCODE);	
	    set_prg_debug(prgDebugIndex++,MULTIPLYCODE);
			/************************************/
			break;
		default:
			advance();
			factor();if(get_compiler_error_flag()) return;
			/*pop top two levels from stack, subtract, then push difference onto stack*/
			set_prg_release(prgReleaseIndex++,DIVIDECODE);	
	    set_prg_debug(prgDebugIndex++,DIVIDECODE);
			/************************************/
			break;
		}
	}
	if(negateFlag) {
		set_prg_release(prgReleaseIndex++,NEGATECODE);	
	  set_prg_debug(prgDebugIndex++,NEGATECODE);
	}		
}

void factor(void) {
	uint16_t i;
	char lexemeStringCopy[MAXLEXEMELEN+1];

	switch(token) {
	case LPAREN:
		advance();
		expression();if(get_compiler_error_flag()) return;
		match_check_advance(RPAREN)
		break;
	case NUM:
		save_literal(); //convert tokenString to double and add to literal table
		advance();
		/*push object at literal[literalTable[literalTableIndex].index] onto the stack*/
	  set_prg_release(prgReleaseIndex++,PUSHLITERAL+literalTableIndex-1);	
	  set_prg_debug(prgDebugIndex++,PUSHLITERAL+literalTableIndex-1);
		/************************************/
		break;
	case MULT: //pointer
		break;
	case ID: //extra lookahead required to determine whether function call or one of var,var++,var--
		//sourceIndexStart=get_sourceIndex(); //save current sourceIndex
		strcpy(lexemeStringCopy,lexemeString); //save lexeme before lookahead
		advance();
		//lookaheadToken=token;
		//set_sourceIndex(sourceIndexStart);// restore sourceIndex
		switch(token) {
		case LPAREN: //function call
			advance();
			args();if(get_compiler_error_flag()) return;
			match_check_advance(RPAREN)
		  
		  i=lookup_function_library_table(lexemeStringCopy);	
			if(i) {				
				switch(i) {
					case EXP:
						set_prg_release(prgReleaseIndex++,EXP);	
				    set_prg_debug(prgDebugIndex++,EXP);
						break;
					case LOG:
						set_prg_release(prgReleaseIndex++,LOG);	
				    set_prg_debug(prgDebugIndex++,LOG);
						break;
					case SIN:
						//convert to radians
					  set_prg_release(prgReleaseIndex++,TORAD);	
				    set_prg_debug(prgDebugIndex++,TORAD);					  
						set_prg_release(prgReleaseIndex++,SIN);	
				    set_prg_debug(prgDebugIndex++,SIN);
						break;
					case COS:
						set_prg_release(prgReleaseIndex++,TORAD);	
				    set_prg_debug(prgDebugIndex++,TORAD);
						set_prg_release(prgReleaseIndex++,COS);	
				    set_prg_debug(prgDebugIndex++,COS);
						break;
					case TAN:
						set_prg_release(prgReleaseIndex++,TORAD);	
				    set_prg_debug(prgDebugIndex++,TORAD);
						set_prg_release(prgReleaseIndex++,TAN);	
				    set_prg_debug(prgDebugIndex++,TAN);
						break;
				}				
			}
		  else {
				i=lookup_function_table(lexemeStringCopy);
				if (!i) insert_undeclared_function_table(lexemeStringCopy);				
				set_prg_release(prgReleaseIndex++,CALLFUNCRELEASE+functionTableIndex-1);	
				set_prg_debug(prgDebugIndex++,CALLFUNCDEBUG+functionTableIndex-1);
			}				
			break;
		case INC: //var++
			advance();
		  push_variable(lexemeStringCopy);
			set_prg_release(prgReleaseIndex++,PUSHONE);	
	    set_prg_debug(prgDebugIndex++,PUSHONE);
			set_prg_release(prgReleaseIndex++,ADDCODE);	
	    set_prg_debug(prgDebugIndex++,ADDCODE);
			pop_variable(lexemeStringCopy);
			break;
		case DEC: //var--
			advance();
			push_variable(lexemeStringCopy);
			set_prg_release(prgReleaseIndex++,PUSHONE);	
	    set_prg_debug(prgDebugIndex++,PUSHONE);
			set_prg_release(prgReleaseIndex++,SUBTRACTCODE);	
	    set_prg_debug(prgDebugIndex++,SUBTRACTCODE);
			pop_variable(lexemeStringCopy);
			break;
		case LBRACKET: //variable is an array
			advance();
			while(token==LBRACKET){
				advance();
				expression();if(get_compiler_error_flag()) return;
				match_check_advance(RBRACKET)
			}
			break;
		default: //var
			push_variable(lexemeStringCopy);
		}
		break;
		case TRUE:
			advance();
			break;
		case FALSE:
			advance();
			break;
		default: //address operator
			match_check_advance(ADDR)
			match_check_advance(ID)
			while(token==LBRACKET){
				advance();
				expression();if(get_compiler_error_flag()) return;
				match_check_advance(RBRACKET)
			}
	}
}

void args(void) {
	if(token==MINUS || token==LPAREN || token==NUM || token==MULT || token==ID || token==ADDR ||
			token==TRUE || token==FALSE) {
		expression(); if(get_compiler_error_flag()) return;
		while(token==COMMA) {
			advance();
			expression(); if(get_compiler_error_flag()) return;
		}
	}
}

void advance(void) {
	set_source_index_previous(get_sourceIndex()); //save current sourceIndex
	token=get_token();
}

void match(TokenType expected) {
	if (expected!=token) set_compiler_error_flag(true);
}

void save_literal(void) {
	double doublePrecisionValue;

	//literalTableIndex++;
	set_literal_table_index(literalTableIndex,literalArrayIndex);	

	doublePrecisionValue = atof(lexemeString);
	literalArrayIndex = push_double(get_literal_array(),literalArrayIndex,doublePrecisionValue);

	set_literal_table_length(literalTableIndex,literalArrayIndex-get_literal_table_index(literalTableIndex));
	literalTableIndex++;
}

void pop_variable(char *lexeme) {
	uint16_t i;

	i=lookup_parameterTable(lexeme);
	if (i) { //parameter found
		/*pop into parameter*/
		set_prg_release(prgReleaseIndex++,POPPARAMETER+i-1);	
	  set_prg_debug(prgDebugIndex++,POPPARAMETER+i-1);
		/************************************/
	}
	else {
		i=lookup_localVarTable(lexeme);
		if (i) { //local variable found
			/*pop into local variable*/
			set_prg_release(prgReleaseIndex++,POPLOCALVAR+i-1);	
	    set_prg_debug(prgDebugIndex++,POPLOCALVAR+i-1);
			/************************************/
		}
		else {
		i=lookup_globalVarTable(lexeme);
			if (!i) { //declare global variable if not found
				insert_globalVarTable(lexeme,SCALAR,1);
				i=lookup_globalVarTable(lexeme);
			}
			/*pop into global variable*/
			set_prg_release(prgReleaseIndex++,POPGLOBALVAR+i-1);	
	    set_prg_debug(prgDebugIndex++,POPGLOBALVAR+i-1);
			/************************************/
		}
	}
}

void push_variable(char *lexeme) {
	uint16_t i;

	i=lookup_parameterTable(lexeme);
	if (i) { //parameter found
		/*push parameter*/
		set_prg_release(prgReleaseIndex++,PUSHPARAMETER+i-1);	
	  set_prg_debug(prgDebugIndex++,PUSHPARAMETER+i-1);
		/************************************/
	}
	else {
		i=lookup_localVarTable(lexeme);
		if (i) { //local variable found
			/*push local variable*/
			set_prg_release(prgReleaseIndex++,PUSHLOCALVAR+i-1);	
	    set_prg_debug(prgDebugIndex++,PUSHLOCALVAR+i-1);
			/************************************/
		}
		else {
		i=lookup_globalVarTable(lexeme);
			if (!i) { //declare global variable if not found
				insert_globalVarTable(lexeme,SCALAR,1);
				i=lookup_globalVarTable(lexeme);
			}
			/*push global variable*/
			set_prg_release(prgReleaseIndex++,PUSHGLOBALVAR+i-1);	
	    set_prg_debug(prgDebugIndex++,PUSHGLOBALVAR+i-1);
			/************************************/
		}
	}
}

uint16_t lookup_function_library_table(char *string) {
	uint16_t p;

	for (p=1;p<MAXLIBRARYFUNCTIONS+1;p++) {
		if (strcmp(functionLibrary[p],string)==0) return p;
	}
	return 0;
}

uint16_t lookup_function_table(char *string) {
	uint16_t p;

	for (p=functionTableIndex;p>0;p--) {
		if (strcmp(get_function_table_lxptr(p),string)==0) return p;
	}
	return 0;
}

void insert_function_table(char *string, uint16_t prgRIndex, uint16_t prgDIndex) {
	unsigned int stringLength;

	stringLength = strlen(string);
	if (functionTableIndex+1 > MAXFUNCTIONS) {
		set_compiler_error_flag(true);
		//insert error type code
	}
	if (globalVarFunctionIDArrayIndex+stringLength+1 >= MAXGVFIDARRAY) {
		set_compiler_error_flag(true);
		//insert error type code
	}	
	functionTableIndex++;
	set_function_table_dec_flag(functionTableIndex,true);
	set_function_table_lexptr(functionTableIndex,&globalVarFunctionIDArray[globalVarFunctionIDArrayIndex]);	
	set_function_table_program_index(functionTableIndex,prgRIndex,prgDIndex);
	
	strcpy(get_function_table_lxptr(functionTableIndex),string);
	
	globalVarFunctionIDArrayIndex += stringLength+1;	
}

void insert_undeclared_function_table(char *string) {
	unsigned int stringLength;

	stringLength = strlen(string);
	if (functionTableIndex+1 > MAXFUNCTIONS) {
		set_compiler_error_flag(true);
		//insert error type code
	}
	if (globalVarFunctionIDArrayIndex+stringLength+1 >= MAXGVFIDARRAY) {
		set_compiler_error_flag(true);
		//insert error type code
	}	
	functionTableIndex++;
	set_function_table_lexptr(functionTableIndex,&globalVarFunctionIDArray[globalVarFunctionIDArrayIndex]);
	set_function_table_dec_flag(functionTableIndex,false);
}

uint16_t lookup_parameterTable(char *string) {
	uint16_t p;

	for (p=parameterTableIndex;p>0;p--) {
		if (strcmp(parameterTable[p],string)==0) return p;
	}
	return 0;
}

uint16_t lookup_localVarTable(char *string) {
	uint16_t p;

	for (p=localVarTableIndex;p>0;p--) {
		if (strcmp(localVarTable[p],string)==0) return p;
	}
	return 0;
}

uint16_t lookup_globalVarTable(char *string) {
	uint16_t p;

	for (p=get_global_var_table_index();p>0;p--) {
		if (strcmp(get_global_var_table_lxptr(p),string)==0) return p;
	}
	return 0;
}

void insert_globalVarTable(char *string, uint8_t dimension, uint16_t length) {
	//uint16_t stringLength;
	unsigned int stringLength;

	stringLength = strlen(string);
	if (get_global_var_table_index()+1 > MAXGLOBALVARS) {
		set_compiler_error_flag(true);
		//insert error type code
	}
	if (globalVarFunctionIDArrayIndex+stringLength+1 >= MAXGVFIDARRAY) {
		set_compiler_error_flag(true);
		//insert error type code
	}
	inc_global_var_table_index();
	set_global_var_table_lexptr(get_global_var_table_index(),&globalVarFunctionIDArray[globalVarFunctionIDArrayIndex]);
	set_global_var_table_dimension(get_global_var_table_index(),dimension);
	set_global_var_table_length(get_global_var_table_index(),length);
	
	if (get_global_var_table_index()>1) 
	set_global_var_table_start_index(get_global_var_table_index(),
	get_global_var_table_start_index(get_global_var_table_index()-1)+
	get_global_var_table_length(get_global_var_table_index()-1));	
	
	strcpy(get_global_var_table_lxptr(get_global_var_table_index()),string);
	globalVarFunctionIDArrayIndex += stringLength+1;	
}

void moniC_parser_variable_init(void) {
	varDecFlag=true;

	literalTableIndex=0;
	literalArrayIndex=0;
	
	functionTableIndex=0;
	
	set_global_var_table_index(0);
	
	globalVarFunctionIDArrayIndex=0;
	
	prgReleaseIndex=0;
	prgDebugIndex=0;
	
	statementTolineNoArrayIndex=0;

	jumpArray1ReleaseIndexGlobal=0;
	jumpArray2ReleaseIndexGlobal=0;

	jumpArray1DebugIndexGlobal=0;
	jumpArray2DebugIndexGlobal=0;
	
	set_sourceIndex(0);
	set_lineNo(0);
	
	set_prg_release(prgReleaseIndex++,JUMPTOMAIN);
	set_prg_debug(prgDebugIndex++,JUMPTOMAIN);	
}
