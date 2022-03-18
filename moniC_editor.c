//includes

#include <stdint.h>
#include "keypad_definitions.h"
#include "moniC_programs.h"

//prototypes

//program/program_utilities
void set_source(char *sourceValue, uint16_t sourceLinesValue);

//functions

void moniC_editor(uint8_t keyNumber) {
	switch (keyNumber) {
		case ONE:
		set_source(testSource,testSourceLines);		
		break;
//	case ONE:
//		set_source(source1,source1Lines);		
//		break;
//	case TWO:
//		set_source(source2,source2Lines);		
//		break;
//	case THREE:
//		set_source(source3,source3Lines);		
//		break;
//	case FOUR:
//		set_source(source4,source4Lines);
//    break;	
//  case FIVE:
//		set_source(source5,source5Lines);		
//		break;
//	case SIX:
//		set_source(source6,source6Lines);		
//		break;
//	case SEVEN:
//		set_source(source7,source7Lines);		
//		break;
//	case EIGHT:
//		set_source(source8,source8Lines);		
//		break;
//	case NINE:
//		set_source(source9,source9Lines);		
//		break;
  }	
}
