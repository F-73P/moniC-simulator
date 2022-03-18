//includes

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

//assembly prototypes

extern double thirty_two_bit_to_double(uint32_t lowerWord, uint32_t upperWord);
extern uint32_t lower_word_to_32bit(double value);
extern uint32_t upper_word_to_32bit(double value);

//functions

_Bool double_test_less_than_or_equal(uint32_t *stack, uint32_t stackIndex) {
	double double_precision_value1;
	double double_precision_value2;

	double_precision_value1 = thirty_two_bit_to_double(stack[stackIndex],stack[stackIndex+1]);
	double_precision_value2 = thirty_two_bit_to_double(stack[stackIndex+3],stack[stackIndex+4]);

	if (double_precision_value1<=double_precision_value2) return true;
	else return false;
}

_Bool double_test_less_than(uint32_t *stack, uint32_t stackIndex) {
	double double_precision_value1;
	double double_precision_value2;

	double_precision_value1 = thirty_two_bit_to_double(stack[stackIndex],stack[stackIndex+1]);
	double_precision_value2 = thirty_two_bit_to_double(stack[stackIndex+3],stack[stackIndex+4]);

	if (double_precision_value1<double_precision_value2) return true;
	else return false;
}

_Bool double_test_greater_than_or_equal(uint32_t *stack, uint32_t stackIndex) {
	double double_precision_value1;
	double double_precision_value2;

	double_precision_value1 = thirty_two_bit_to_double(stack[stackIndex],stack[stackIndex+1]);
	double_precision_value2 = thirty_two_bit_to_double(stack[stackIndex+3],stack[stackIndex+4]);

	if (double_precision_value1>=double_precision_value2) return true;
	else return false;
}

_Bool double_test_greater_than(uint32_t *stack, uint32_t stackIndex) {
	double double_precision_value1;
	double double_precision_value2;

	double_precision_value1 = thirty_two_bit_to_double(stack[stackIndex],stack[stackIndex+1]);
	double_precision_value2 = thirty_two_bit_to_double(stack[stackIndex+3],stack[stackIndex+4]);

	if (double_precision_value1>double_precision_value2) return true;
	else return false;
}

_Bool double_test_equal(uint32_t *stack, uint32_t stackIndex) {
	double double_precision_value1;
	double double_precision_value2;

	double_precision_value1 = thirty_two_bit_to_double(stack[stackIndex],stack[stackIndex+1]);
	double_precision_value2 = thirty_two_bit_to_double(stack[stackIndex+3],stack[stackIndex+4]);

	if (double_precision_value1==double_precision_value2) return true;
	else return false;
}

_Bool double_test_not_equal(uint32_t *stack, uint32_t stackIndex) {
	double double_precision_value1;
	double double_precision_value2;

	double_precision_value1 = thirty_two_bit_to_double(stack[stackIndex],stack[stackIndex+1]);
	double_precision_value2 = thirty_two_bit_to_double(stack[stackIndex+3],stack[stackIndex+4]);

	if (double_precision_value1!=double_precision_value2) return true;
	else return false;
}

uint32_t push_double(uint32_t *array, uint32_t arrayIndex, double doublePrecisionValue) {
	array[arrayIndex++]=DBL;
	array[arrayIndex++]=lower_word_to_32bit(doublePrecisionValue);
	array[arrayIndex++]=upper_word_to_32bit(doublePrecisionValue);
	return arrayIndex;
}
