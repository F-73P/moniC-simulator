//includes

#include <stdint.h>
#include "moniC_definitions.h"

//programs

char testSource[]= {
		"/*add*/\n" 
    "\n"
		"main()\n"
		"{ read x;\n" 
	  "  read y;\n"    
		"  write x+y;\n"	  
		"}\n"	     
};

uint16_t testSourceLines = 7;

//char testSource[]= {
//		"/*exponential program*/\n" 
//    "\n"
//		"main()\n"
//		"{ read x;\n" 
//		"  value1=exp(x/10)+1;\n"
//    "  value2=sin(x)-0.25;\n"
//    "  write value2;\n"	
//		"  write value1;\n"	  
//		"}\n"	     
//};

//uint16_t testSourceLines = 7;

//char source1[]= {
//		"/*factorial program*/\n" //sourceIndex at start of line=0
//		"\n" //sourceIndex at start of line=22 etc.
//		"main()\n" //23
//		"{ read x;\n" //30
//		"  factorial=1;\n" //40
//		"  while(x>0)\n" //55
//		"  { factorial=factorial*x;\n" //68
//		"    x--;\n" //95
//		"  }\n" //104
//		"  write factorial;\n" //108
//		"}\n" //127, //sourceIndex after last character=129
//};
//char source1[]={0x2F, 0x2A, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x2A, 0x2F, 0x0A, 0x0A, 0x6D, 0x61, 0x69, 0x6E, 0x28, 0x29, 0x0A, 0x7B, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x78, 0x3B, 0x0A, 0x20, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x3D, 0x31, 0x3B, 0x0A, 0x20, 0x20, 0x77, 0x68, 0x69, 0x6C, 0x65, 0x28, 0x78, 0x3E, 0x30, 0x29, 0x0A, 0x20, 0x20, 0x7B, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x3D, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x2A, 0x78, 0x3B, 0x0A, 0x09, 0x78, 0x2D, 0x2D, 0x3B, 0x0A, 0x20, 0x20, 0x7D, 0x0A, 0x20, 0x20, 0x77, 0x72, 0x69, 0x74, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x3B, 0x0A, 0x7D, };
//uint16_t source1Lines = 11;

//char source2[]= {
//		"/*factorial program*/\n" 
//		"\n" 
//		"main()\n"
//	  "{ read x;\n" 
//		"  factorial=1;\n" 
//		"  while(x>0)\n" 
//	  "  { factorial=factorial*x;\n" 
//		"    x-;\n" /*****ERROR*****/
//		"  }\n" 
//		"  write factorial;\n" 
//		"}\n" 
//};
//uint16_t source2Lines = 11;

//char source3[]={
//		"/*summation program*/\n"
//		"\n"
//		"main()\n"
//		"{ read x;\n"
//		"  sum=1;\n"
//		"  while(x>1)\n"
//		"  { sum=sum+x;\n"
//		"    x--;\n"
//		"  }\n"
//		"  write sum;\n"
//		"}\n"
//};
//uint16_t source3Lines = 11;

//char source4[]={
//	 "/*square program*/\n" 
//   "\n"
//	 "main()\n"
//	 "{ read x;\n"
//	 "  product=0;\n"
//   "  y=x;\n"	
//	 "  while(y>0)\n"
//	 "  { z=x;\n"
//	 "    while (z>0)\n"
//	 "    { product++;\n"
//	 "      z--;\n"
//	 "    }\n"
//	 "    y--;\n"
//	 "  }\n"	                      
//	 "  write product;\n"
//	 "}\n"	                      	
//};

//uint16_t source4Lines = 16;

//char source5[]= {
//		"/*factorial program*/\n" //sourceIndex at start of line=0
//		"\n" //sourceIndex at start of line=22 etc.
//		"main()\n" //23
//		"{ read x;\n" //30
//		"  factorial=1;\n" //40
//	  "  if(x>0)\n"	   
//	  "  { while(x>1)\n" //
//		"    { factorial=factorial*x;\n" //
//		"      x--;\n" //
//		"    }\n" //		     
//	  "  }\n"
//		"  write factorial;\n" //
//		"}\n" //, //sourceIndex after last character=		
//};
//uint16_t source5Lines = 13;

//char source6[]= {
//		"/*factorial program*/\n" //sourceIndex at start of line=0
//		"\n" //sourceIndex at start of line=22 etc.
//		"main()\n" //23
//		"{ read x;\n" //30
//		"  factorial=1;\n" //40
//	  "  if(x<0)\n"
//	  "  { factorial=-10;\n" 
//	  "  }\n"
//	  "  else\n" 
//	  "  { while(x>1)\n" //
//		"    { factorial=factorial*x;\n" //
//		"      x--;\n" //
//		"    }\n" //		     
//	  "  }\n"
//		"  write factorial;\n" //
//		"}\n" //, //sourceIndex after last character=		
//};
//uint16_t source6Lines = 16;

//char source7[]= {
//		"/*summation program*/\n"
//		"\n"
//		"incr()\n"
//		"{ return sum+n;\n"
//		"}\n"
//		"\n"
//		"main()\n"
//		"{ sum=0;\n"
//		"  read n;\n"
//		"  while (n>0)\n"
//		"  { sum=incr();\n"
//		"    n--;\n"
//		"  }\n"
//		"  write sum;\n"
//		"}\n"
//};
//uint16_t source7Lines = 15;

//char source8[]={
//		"/*factorial program*/\n" 
//    "\n"
//		"main()\n"
//		"{ read x;\n" 
//		"  fact=1;\n"
//		"  do \n"
//		"  { fact=fact*x;\n"
//		"    x--;\n"
//		"  } while(x>0);\n"	                      
//		"  write fact;\n"
//		"}\n"	                      	
//};

//uint16_t source8Lines = 11;

//char source9[]={
//		"/*factorial program*/\n"
//		"\n"
//		"main()\n"
//		"{ read x;\n"
//		"  fact=1;\n"
//		"  for(i=x;i>1;i--)\n"
//		"  { fact = fact*i;\n"
//		"  }\n"
//		"  write fact;\n"
//		"}\n"
//};

//uint16_t source9Lines = 10;
