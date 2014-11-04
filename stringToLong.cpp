//John W
//10-27-2014
//COMPANY Coding Exercise

#include <string>
#include <assert.h>
#include <iostream>
using namespace std;

/*
stringToLong
Parameter: string formatted base 10 number
Returns: long int number equivalent to parameter
	special case: empty string returns 0
Notes: Ignores non-integer characters.
	e.g. param "12a457.6" returns 124576
		 param "-------" returns 0
		 
	Vulnerable to integer overflow.
	e.g. param "999999999999" returns -727379969
	
	O(n) worst-case time complexity,
	where n is the length of str argument.
*/
long int stringToLong(string str) {
	long int result = 0;
	int sign;
	string numbers = "0123456789";		//valid characters for base 10 numbers

	if (str[0] == '-') {				//if first character is negative sign
		sign = -1;						//sign int is negative
	}
	else {
		sign = 1;						//otherwise, sign int is positive
	}
	
	for (unsigned int i = 0; i < str.length(); ++i) {
		if (numbers.find(str[i]) != string::npos) { //ignores non-numbers
			result *= 10;
			result += str[i] - '0';					// - '0' casts char to int
		}
	}
	return sign * result;
}

int main() {
	assert(stringToLong("-12") == -12);
	assert(stringToLong("0") == 0);
	assert(stringToLong("") == 0);
	assert(stringToLong("-------") == 0);
	assert(stringToLong("12") == 12);
	assert(stringToLong("999999999999") == -727379969);
	cout << "All tests ok.";
}