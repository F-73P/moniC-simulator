//includes

#include <stdint.h>
#include <stdbool.h>
#include "moniC_definitions.h"

//external variables

extern uint32_t prgDebug[MAXBYTECODES];
extern uint16_t prgIndex;

extern uint32_t globalVarArray[MAXGLOBALVARARRAY];
extern _Bool execFlag;

//variables

double globalVarValue;

//prototypes

void execute(uint32_t *prg);

//program_utilities prototypes

uint16_t get_global_var_table_index(void);
char * get_global_var_table_lxptr(uint16_t globalVarTableIndexValue);
uint32_t * get_global_var_array(void);
uint16_t is_release_program_step_a_label(uint16_t step);
uint16_t is_debug_program_step_a_label(uint16_t step);
void set_breakpoint_flag(_Bool flag);
_Bool get_breakpoint_flag(void);
void set_step_over_flag(_Bool flag);
void set_break_point_destination(uint16_t lineNoDest);
uint16_t get_lineNoDebug(void);

//assembly prototypes

extern double thirty_two_bit_to_double(uint32_t lowerWord, uint32_t upperWord);

//functions

void moniC_debugger(void) {
	uint16_t globalVarTableIndexLocal=get_global_var_table_index();
	uint32_t *globalVarArrayLocal=get_global_var_array();
	uint16_t count=0;
	uint16_t singleStepButtonPress=3;
	
	prgIndex=0;
  set_breakpoint_flag(false);
  set_step_over_flag(false);	
	execFlag=true;
	
	do //run to main
	{ execute(prgDebug);
	} while(!get_breakpoint_flag() && execFlag);
	set_breakpoint_flag(false);
  //refresh debug windows - arrow at "lineNoDebug","prgIndex" and "prgIndexVirtual" in the C,debug bytecode and 
  //release bytecode windows on the actual calculator 
		
	while(execFlag) {
    if(count==singleStepButtonPress) { //simulate step over button pressed after single step pressed singleStepButtonPress times
			set_step_over_flag(true);
			set_break_point_destination(get_lineNoDebug()+1);
		}
				
		do
		{ execute(prgDebug);
		} while(!get_breakpoint_flag() && execFlag);
		set_breakpoint_flag(false);
    //refresh debug windows		

    count++;		
	}  
}

