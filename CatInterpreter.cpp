#include "Interpreter.h"
#include <iostream>


int main()
	{
		Interpreter interpreter(std::cout);
		interpreter.ReadFile("test.txt");
		return 0;
	}

    



