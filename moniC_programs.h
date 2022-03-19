//includes

#include <stdint.h>
#include "moniC_definitions.h"

//enter program source code. 
//uncomment one of the examples below or enter your own. 

/*example program 1*/

//char testSource[]= {
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

//programs can also be entered by typing them in an editor and converting to hex 
//using the online tool https://tomeko.net/online_tools/ascii.php?lang=en 
//e.g. for the program above:

//char testSource[]={0x2F, 0x2A, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x2A, 0x2F, 0x0A, 0x0A, 0x6D, 0x61, 0x69, 0x6E, 0x28, 0x29, 0x0A, 0x7B, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x78, 0x3B, 0x0A, 0x20, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x3D, 0x31, 0x3B, 0x0A, 0x20, 0x20, 0x77, 0x68, 0x69, 0x6C, 0x65, 0x28, 0x78, 0x3E, 0x30, 0x29, 0x0A, 0x20, 0x20, 0x7B, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x3D, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x2A, 0x78, 0x3B, 0x0A, 0x09, 0x78, 0x2D, 0x2D, 0x3B, 0x0A, 0x20, 0x20, 0x7D, 0x0A, 0x20, 0x20, 0x77, 0x72, 0x69, 0x74, 0x65, 0x20, 0x66, 0x61, 0x63, 0x74, 0x6F, 0x72, 0x69, 0x61, 0x6C, 0x3B, 0x0A, 0x7D, 0x00};

/*example program 2*/

//char testSource[]= {
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

/*example program 3*/

//char testSource[]={
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

/*example program 4*/

//char testSource[]={
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

/*example program 5*/

//char testSource[]= {
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

/*example program 6*/

//char testSource[]= {
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

/*example program 7*/

//char testSource[]= {
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

/*example program 8*/

//char testSource[]={
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

/*example program 9*/

//char testSource[]={
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

/*example program 10*/

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

/*example program 11*/

//char testSource[]= {
//		"/*add the values on the stack*/\n" 
//    "\n"
//		"main()\n"
//		"{ read x;\n" 
//	  "  read y;\n"    
//		"  write x+y;\n"	  
//		"}\n"	     
//};

/*example program 12*/ 
//bisection method to find a root in [a,b] with tolerance tol for the function x-e^(-0.5x^2). 
//Enter a,b,tol on the stack
char testSource[]={	0x2F, 0x2A, 0x62, 0x69, 0x73, 0x65, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x6D, 0x65, 0x74, 0x68, 0x6F, 0x64, 0x2A, 0x2F, 0x0A, 0x0A, 0x66, 0x75, 0x6E, 0x63, 0x28, 0x29, 0x0A, 0x7B, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x28, 0x78, 0x2D, 0x65, 0x78, 0x70, 0x28, 0x2D, 0x30, 0x2E, 0x35, 0x2A, 0x78, 0x2A, 0x78, 0x29, 0x29, 0x3B, 0x0A, 0x7D, 0x0A, 0x0A, 0x6D, 0x61, 0x69, 0x6E, 0x28, 0x29, 0x0A, 0x7B, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x74, 0x6F, 0x6C, 0x3B, 0x0A, 0x20, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x62, 0x3B, 0x0A, 0x20, 0x20, 0x72, 0x65, 0x61, 0x64, 0x20, 0x61, 0x3B, 0x0A, 0x20, 0x20, 0x78, 0x3D, 0x61, 0x3B, 0x0A, 0x20, 0x20, 0x66, 0x61, 0x3D, 0x66, 0x75, 0x6E, 0x63, 0x28, 0x29, 0x3B, 0x0A, 0x20, 0x20, 0x78, 0x3D, 0x62, 0x3B, 0x0A, 0x20, 0x20, 0x66, 0x62, 0x3D, 0x66, 0x75, 0x6E, 0x63, 0x28, 0x29, 0x3B, 0x0A, 0x20, 0x20, 0x6D, 0x3D, 0x28, 0x61, 0x2B, 0x62, 0x29, 0x2F, 0x32, 0x3B, 0x0A, 0x20, 0x20, 0x77, 0x68, 0x69, 0x6C, 0x65, 0x28, 0x62, 0x2D, 0x6D, 0x3E, 0x3D, 0x74, 0x6F, 0x6C, 0x29, 0x0A, 0x20, 0x20, 0x7B, 0x20, 0x78, 0x3D, 0x6D, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6D, 0x3D, 0x66, 0x75, 0x6E, 0x63, 0x28, 0x29, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x69, 0x66, 0x28, 0x66, 0x6D, 0x2A, 0x66, 0x62, 0x3C, 0x30, 0x29, 0x0A, 0x09, 0x7B, 0x20, 0x61, 0x3D, 0x6D, 0x3B, 0x0A, 0x09, 0x20, 0x20, 0x66, 0x61, 0x3D, 0x66, 0x6D, 0x3B, 0x0A, 0x09, 0x7D, 0x0A, 0x09, 0x65, 0x6C, 0x73, 0x65, 0x0A, 0x09, 0x7B, 0x20, 0x62, 0x3D, 0x6D, 0x3B, 0x0A, 0x09, 0x20, 0x20, 0x66, 0x62, 0x3D, 0x66, 0x6D, 0x3B, 0x0A, 0x09, 0x7D, 0x0A, 0x09, 0x6D, 0x3D, 0x28, 0x61, 0x2B, 0x62, 0x29, 0x2F, 0x32, 0x3B, 0x0A, 0x20, 0x20, 0x7D, 0x0A, 0x20, 0x20, 0x77, 0x72, 0x69, 0x74, 0x65, 0x20, 0x6D, 0x3B, 0x0A, 0x7D, 0x00};

