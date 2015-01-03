#include "utils.h"

//Split function to separate args 
//taken from the console taken from the article 'http://www.cplusplus.com/articles/2wA0RXSz/' 
//with a little modification

void split(std::vector<std::string>& v, std::string const& s, char del) {
	std::string buff;
	for (char c : s) {
		if (c != del) buff += c;
		else if (!buff.empty()) {
			v.push_back(buff);
			buff = std::string();
		}
	}
	if (!buff.empty()) v.push_back(buff);
}