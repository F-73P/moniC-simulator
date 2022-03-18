////includes
//#include <stdint.h>
//#include "moniC_definitions.h"

////global variables
//static uint32_t sourceIndexArray[MAXLINENO];

////prototypes
//void gen_sourceIndexArray(char *sourceLocal);

////functions
//void gen_sourceIndexArray(char *sourceLocal) { //generate sourceIndexArray, where sourceIndexArray[i] = source index at beginning
//	//of program line i
//	uint16_t lineNumber=0;
//	uint16_t sourceIndexLocal=0;

//	while(sourceLocal[sourceIndexLocal]) {
//		sourceIndexArray[lineNumber]=sourceIndexLocal;
//		while((sourceLocal[sourceIndexLocal] != '\n') && sourceLocal[sourceIndexLocal]) sourceIndexLocal++;
//		sourceIndexLocal++;
//		lineNumber++;
//	}
//	sourceIndexArray[lineNumber]=sourceIndexLocal;
//}
