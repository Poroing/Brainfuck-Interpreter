#ifndef BRAINFUCKCONSOLE_SRC_BFEXCEPTION_H_
#define BRAINFUCKCONSOLE_SRC_BFEXCEPTION_H_

#include <exception>
#include <string>

struct invalid_argument_number : std::exception {
	const char *e_what;

	invalid_argument_number(std::string const& command, std::string const& r_Flag, int const& n_arg);

	const char* what() const throw();
};

struct cell_out_of_range : std::exception {
	const char* e_what;

	cell_out_of_range(int const& cell);

	const char* what() const throw();
};

#endif
