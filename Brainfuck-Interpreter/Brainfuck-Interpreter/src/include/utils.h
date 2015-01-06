#ifndef BRAINFUCKCONSOLE_INCLUDE_UTILS_H_
#define BRAINFUCKCONSOLE_INCLUDE_UTILS_H_

#include <iostream>
#include <functional>
#include <vector>
#include <string>


void split(std::vector<std::string>& v, std::string const& s, char del = ' ');

template<typename T>
std::ostream& operator<< (std::ostream& os, std::vector<T> const& v) {
	auto it = v.begin();
	os << '[' << *it;
	++it;
	for (; it != v.end(); ++it) {
		os << ',' << *it;
	}
	os << ']';
};

template<typename T, typename U>
void print_vector(std::ostream& os, std::vector<T> const& v, std::function<U(T)> const& foo) {
	auto it = v.begin();
	os << '[' << foo(*it);
	++it;
	for (; it != v.end(); ++it) {
		os << ',' << foo(*it);
	}
	os << ']';
};

#endif
