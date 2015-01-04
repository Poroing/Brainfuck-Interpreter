#include <iostream>
#include "interpreter.h"
#include "args.h"

int main(int argc, char* argv[]) {
	BFInterpreter interpreter;
	try {
		handle_args(interpreter, argc, argv);
		interpreter.run();
	}
	catch (std::invalid_argument e) {
		std::cout << e.what();
		std::cout << "The command must be run like this:\n" << argv[0] << "[-h] [-f 'file path' | -c | -b 'code']\n";
	}
	catch (std::runtime_error e) {
		std::cout << e.what();
	}
	return 0;
}