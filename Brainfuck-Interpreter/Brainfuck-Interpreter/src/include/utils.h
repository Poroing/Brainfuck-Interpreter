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
	for (; it != v.end(); ++it)
		os << ',' << *it;
	os << ']';
};

template<typename T, class TypeReturningFunc>
void print_vector(std::ostream& os, 
				  std::vector<T> const& v, 
				  TypeReturningFunc const& foo) {
	auto it = v.begin();
	os << '[' << foo(*it);
	++it;
	for (; it != v.end(); ++it)
		os << ',' << foo(*it);
	os << ']';
};

template<class InputIterator, class TypeReturningFunc>
void print_array(std::ostream& os, 
				 InputIterator first, InputIterator const& last, 
				 TypeReturningFunc const& foo) {
	os << '[' << foo(*first);
	++first;
	for (;first != last; ++first)
		os << ',' << foo(*first);
	os << ']';
}

#endif
