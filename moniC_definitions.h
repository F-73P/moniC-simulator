//program definitions

#define OBJECTLENGTH 5 //memory object length in words
#define SCALAR 0 //object is a scalar, i.e. occupies one block of memory
#define ARRAYONEDIM 1 //object is a one-dimensional array, i.e. occupies more than one block of memory

#define PARAMETER 1
#define LOCALVARIABLE 2
#define GLOBALVARIABLE 3

#define RESERVED 22 //the number of reserved words
#define MAXLEXEMELEN 40 //maximum length of lexeme
#define MAXLINENO 200 //maximum number of program lines

#define MAXLITERALS 100 //maximum number of literals (constants) in program
#define MAXLITERALARRAY 1000 //length of array used to store literals

#define MAXPARAMETERS 10 //maximum number of parameters per function
#define MAXPIDARRAY 500 //length of array used to store names of parameters in a given function

#define MAXLOCALVARS 10 //maximum number of local variables per function
#define MAXLVIDARRAY 500 //length of array used to store names of local variables in a given function

#define MAXLIBRARYFUNCTIONS 5 //maximum number of library functions in program

#define MAXFUNCTIONS 100 //maximum number of functions in program

#define RETURNSTACK 100 //length of run-time stack to store return address (value of prgIndex) when a function is called

#define MAXGLOBALVARS 100 //maximum number of global variables in program
#define MAXGLOBALVARARRAY 1000 //length of run-time array used to store global variables

#define MAXGVFIDARRAY 1000 //length of array used to store names of global variables and functions

#define MAXBYTECODES 200 //maximum number of bytecodes in translated program

#define JUMPSTACK 100 //length of jumpStack1Array and jumpStack2Array 

//program codes

#define EXP 1
#define LOG 2
#define SIN 3
#define COS 4
#define TAN 5

#define JUMPTOMAIN 1001
#define ENDPRG 1002
#define ADDCODE 1003
#define SUBTRACTCODE 1004
#define MULTIPLYCODE 1005
#define DIVIDECODE 1006
#define NEGATECODE 1007
#define PUSHONE 1008
#define TESTLTE 1009
#define TESTLT 1010
#define TESTGTE 1011
#define TESTGT 1012
#define TESTEQ 1013
#define TESTNEQ 1014
#define RETURNFROMFUNC 1015
#define TORAD 1016

#define BKPT 10001 

#define PUSHLITERAL 20001 

#define POPPARAMETER 30001
#define PUSHPARAMETER 40001

#define POPLOCALVAR 50001
#define PUSHLOCALVAR 60001

#define POPGLOBALVAR 70001
#define PUSHGLOBALVAR 80001

#define FALSEJUMPRELEASE 90001
#define JUMPRELEASE 100001
#define TRUEJUMPRELEASE 110001

#define FALSEJUMPDEBUG 120001
#define JUMPDEBUG 130001
#define TRUEJUMPDEBUG 140001

#define CALLFUNCRELEASE 150001

#define CALLFUNCDEBUG 160001

