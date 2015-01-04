#ifndef BRAINFUCKCONSOLE_SRC_UTILS_H_
#define BRAINFUCKCONSOLE_SRC_UTILS_H_

#include <vector>
#include <string>

void split(std::vector<std::string>& v, std::string const& s, char del = ' ');

template<typename T>
ostream& operator<< (ostream& os, std::vector<T> const& v) {
	auto it = v.begin();
	os << '[' << *it;
	++it;
	for (; it != v.end(); ++it) {
		os << ',' << *it;
	}
	os << ']';
};

/*template<typename T, typename U>
void print_vector(ostream& os, std::vector<T> const& v, std::function<U(T)> foo) {
	auto it = v.begin();
	os << '[' << foo(*it);
	++it;
	for (; it != v.end(); ++it) {
		os << ',' << foo(*it);
	}
	os << ']'
};*/

#endif
