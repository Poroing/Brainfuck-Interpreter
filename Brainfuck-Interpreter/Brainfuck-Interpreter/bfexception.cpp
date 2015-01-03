#include "bfexception.h"

#include <sstream>

invalid_argument_number::invalid_argument_number(std::string const& command, std::string const& r_Flag, int const& n_arg) {
	std::stringstream ss;
	ss << "'" << command << "' command need " << r_Flag << " argument, " << n_arg << " arguments pass\n";
	e_what = ss.str().c_str();
};

const char* invalid_argument_number::what() const throw() { return e_what; };

cell_out_of_range::cell_out_of_range(int const& cell) {
	std::stringstream ss;
	ss << "Cell " << cell << " is out of bounds\n";
	e_what = std::string(ss.str()).c_str();
};

const char* cell_out_of_range::what() const throw() { return e_what; };