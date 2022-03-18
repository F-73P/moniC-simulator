//includes

#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "moniC_definitions.h"

//definitions
typedef enum {ENDFILE, //book-keeping tokens
    MAINTOKEN,IF,ELSE,DO,WHILE,FOR,RETURN,READ,WRITE,AND,OR,NOT,TRUE,FALSE,LOCAL,ARRAY,SWITCH, //reserved words
	CASE,BREAK,DEFAULT,GOTO,NOPR, //reserved words
    ID,NUM, //multicharacter tokens
    //special symbols
    PLUS,MINUS,MULT,DIV,LT,LTE,GT,GTE,EQ,NEQ,ASSIGN,COLON,SEMI,COMMA,LPAREN,RPAREN,LBRACKET,RBRACKET,
	LBRACE,RBRACE,INC,DEC,ADDR
} TokenType;

//compiler error types
typedef enum {LEXEMELEN
} CompilerErrorType;

//states in DFA scanner
typedef enum {START,INNUM,INEXP,INID,INLESSTHAN,INGREATERTHAN,INEQUAL,INNOTEQUAL,INDIV,INPLUS,INMINUS,
              INCOMMENT,OUTCOMMENT,DONE} StateType;

//structs

struct { //lookup table of reserved words
char* str;
TokenType tok;
} reservedWords[RESERVED] = {{"main",MAINTOKEN},{"if",IF},{"else",ELSE},{"do",DO},{"while",WHILE},{"for",FOR}, {"return",RETURN},
                             {"read",READ},{"write",WRITE},{"and",AND},{"or",OR},{"not",NOT},{"true",TRUE},
                             {"false",FALSE},{"local",LOCAL},{"array",ARRAY},{"switch",SWITCH},
                             {"case",CASE},{"break",BREAK},{"default",DEFAULT},{"goto",GOTO},{"nop",NOPR}};
							
//external variables - in program_utilities.c
extern char *source; //current program
extern uint32_t sourceIndex; //current position in source
extern uint16_t lineNo; //current line number in source

//global variables
char lexemeString[MAXLEXEMELEN+1];

//prototypes
TokenType reservedLookup(char *lexemeString);
														 
//program_utilities.c prototypes
void set_compiler_error_flag(_Bool flag);
void set_compiler_error_type(CompilerErrorType error);

//functions
TokenType get_token(void) { //returns the next token in source
	char character;	//current character in source
	uint16_t lexemeStringIndex = 0; //index for storing lexeme into lexemeString
	_Bool save; //flag to indicate save to lexemeString

	TokenType currentToken; //holds current token to be returned to the parser

	StateType state = START; //current state - always begins at START

  while (state!=DONE) {
		character=source[sourceIndex++]; //next character in program
		save = true;
		switch (state) { //begin switch (state)
			case START:
				if (isdigit(character))
					state = INNUM;
         else if (isalpha(character))
           state = INID;
				 else if (character == '<')
					 state = INLESSTHAN;
				 else if (character == '>')
					 state = INGREATERTHAN;
				 else if (character == '=')
					 state = INEQUAL;
				 else if (character == '!')
					 state = INNOTEQUAL;
				 else if (character == '/') {
					 save = false;
					 state = INDIV;
				 }
				 else if (character =='+') {
					 state = INPLUS;
				 }
				 else if (character =='-') {
					 state = INMINUS;
				 }
         else if ((character == ' ') || (character == '\t') || (character == '\n')) {
					 save = false;
					 if (character == '\n') lineNo++;
				 }
         else
         { state = DONE;
           switch (character) {
						 case ENDFILE:
               save = false;
               currentToken = ENDFILE;
               break;
             case '*':
               currentToken = MULT;
               break;
						 case ':':
               currentToken = COLON;
               break;
             case ';':
               currentToken = SEMI;
               break;
             case ',':
							 currentToken = COMMA;
               break;
             case '(':
               currentToken = LPAREN;
               break;
             case ')':
               currentToken = RPAREN;
               break;
						 case '[':
               currentToken = LBRACKET;
               break;
             case ']':
               currentToken = RBRACKET;
               break;
						 case '{':
               currentToken = LBRACE;
               break;
             case '}':
               currentToken = RBRACE;
               break;
						 case '&':
               currentToken = ADDR;
               break;
             default:
            	 //insert error code
            	 state = DONE;
           }
         }
         break;
			case INNUM:
         if (!isdigit(character)) { //backup in the input
					 if(character == 'e') {
						 state=INEXP;
					 }
           else if(character != '.') {
						 sourceIndex--;
						 save = false;
						 state = DONE;
						 currentToken = NUM;
					 }
         }
         break;
			case INEXP:
				state=INNUM;
				break;
      case INID: //ID is letters/digits (cannot start with a digit)
         if (!isalpha(character)) { //backup in the input
					 if (!isdigit(character)) {
						 sourceIndex--;
						 save = false;
						 state = DONE;
						 currentToken = ID;
					 }
         }
         break;
			case INLESSTHAN:
			  state = DONE;
			  if (character == '=') currentToken = LTE;
			  else {
          save = false;
					sourceIndex--;
					currentToken = LT;
        }
			  break;
			case INGREATERTHAN:
			  state = DONE;
			  if (character == '=') currentToken = GTE;
			  else {
          save = false;
					sourceIndex--;
					currentToken = GT;
        }
			  break;
			case INEQUAL:
				state = DONE;
			  if (character == '=') currentToken = EQ;
			  else {
					sourceIndex--;
					save=false;
					currentToken = ASSIGN;
				}
				break;
			case INNOTEQUAL:
				state = DONE;
			  if (character == '=') currentToken = NEQ;
			  else {
					//insert error code
					state = DONE;
				}
			  break;
			case INDIV:
        save = false;
			  if (character == '*') state = INCOMMENT;
			  else {
					state = DONE;
					sourceIndex--;
					currentToken = DIV;
					if (lexemeStringIndex < MAXLEXEMELEN) {
						lexemeString[lexemeStringIndex++] = '/';
						lexemeString[lexemeStringIndex] = '\0';
					}
					else {
						//insert error code
					}
        }
			  break;
			case INPLUS:
        state = DONE;
			  if (character == '+') currentToken = INC;
			  else {
          save = false;
					sourceIndex--;
					currentToken = PLUS;
        }
			  break;
			case INMINUS:
        state = DONE;
			  if (character == '-') currentToken = DEC;
			  else {
          save = false;
					sourceIndex--;
					currentToken = MINUS;
        }
			  break;
      case INCOMMENT:
			  save = false;
			  if (character == ENDFILE) {
					state = DONE;
				  currentToken = ENDFILE;
			  }
			  else if (character == '*')
				  state = OUTCOMMENT;
				else if (character == '\n') lineNo++;
			  break;
			case OUTCOMMENT:
				save = false;
				if (character == '/') state = START;
			  else { //backup in the input
           sourceIndex--;
           state = INCOMMENT;
        }
				break;
      case DONE:
      default: //should never happen
			   //insert error code
				 state = DONE;
         break;
    } //end switch (state)

		if (save) {
			if (lexemeStringIndex < MAXLEXEMELEN) lexemeString[lexemeStringIndex++] = character;
      else {
				set_compiler_error_flag(true);
				set_compiler_error_type(LEXEMELEN);
			}
		}

    if (state == DONE) {
			lexemeString[lexemeStringIndex] = '\0';
       if (currentToken == ID) currentToken = reservedLookup(lexemeString);
		}
  }
	return currentToken;
}

TokenType reservedLookup (char *lexemeString) { //lookup an identifier to see if it is a reserved word (linear search)
	uint16_t i;
	for (i=0;i<RESERVED;i++) {
		if (!strcmp(lexemeString,reservedWords[i].str)) return reservedWords[i].tok;
	}
	return ID;
}

