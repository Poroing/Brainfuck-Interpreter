#include "interpreter.h"

#include "utils.h"
#include "bfexception.h"
#include <algorithm>
#include <numeric>
#include <sstream>

//Flag used to know how to run the interpreter
//Could be use later for state and other
enum class Flag {
	empty = 0x00,
	file = 0x01,
	console = 0x02,
};

inline Flag operator|(Flag const& a, Flag const& b) throw() {
	return Flag(static_cast<int>(a) | static_cast<int>(b));
}

inline bool operator& (Flag const& a, Flag const& b) throw() {
	return (static_cast<int>(a)& static_cast<int>(b)) != 0;
}

inline Flag operator^(Flag const& a, Flag const& b) throw() {
	return Flag(static_cast<int>(a) ^ static_cast<int>(b));
}



	//Create the Interpreter from a string
BFInterpreter::BFInterpreter(std::string const& code, std::ostream& out, std::istream& in) :
		m_prompt(":::"),
		m_flag(Flag::empty) {
		m_in = &in;
		m_out = &out;
		read_string(code);
	};

BFInterpreter::BFInterpreter(std::string const& code, std::ostream& out) :
	BFInterpreter(code, out, std::cin) {};

BFInterpreter::BFInterpreter(std::string const& code, std::istream& in) :
	BFInterpreter(code, std::cout, in) {};

BFInterpreter::BFInterpreter(std::string const& code) :
	BFInterpreter(code, std::cout, std::cin) {};

BFInterpreter::BFInterpreter() :
	BFInterpreter(std::string(), std::cout, std::cin) {};

void BFInterpreter::run() {
	if (m_flag == Flag::empty) throw std::runtime_error("Interpreter flag is not set\n");
	if (m_flag & Flag::file) run_file();
	else if (m_flag & Flag::console) run_console();
}

void BFInterpreter::set_console() throw() {
	if (m_flag & Flag::file)
		m_flag = m_flag ^ Flag::file;
	m_flag = m_flag | Flag::console;
}

bool BFInterpreter::console() const throw() {
	return m_flag & Flag::console;
}

void BFInterpreter::set_file() throw() {
	if (m_flag & Flag::console)
		m_flag = m_flag ^ Flag::console;
	m_flag = m_flag | Flag::file;
}

bool BFInterpreter::file() const throw() {
	return m_flag & Flag::file;
}

void BFInterpreter::set_code(std::string const& code) {
	m_code = code;
}

bool BFInterpreter::is_usable_code(std::string const& code) const throw() {
	int end_loop(0);
	for (char c : code) {
		if (c == ']') ++end_loop;
		else if (c == '[') --end_loop;
		if (end_loop > m_in_loop) return false;
	}
	return true;
}

//---BRAINFUCK BASIC ACTION---

void BFInterpreter::start_loop() {
	++m_in_loop;
	if (m_cell_vector[m_current_cell] != 0) {
		m_loop_stack.push(m_current_action); //Save the place where the loop start			
	}
	else ++m_wait_for_end_loop;
}

void BFInterpreter::end_loop() {
	if (m_cell_vector[m_current_cell] == 0) {
		m_loop_stack.pop();
		--m_in_loop;
	}
	else m_current_action = m_loop_stack.top();
}

void BFInterpreter::plus() {
	//Increment the pointed cell's value
	++m_cell_vector[m_current_cell];
}

void BFInterpreter::minus() {
	//Decrement the pointed cell's value
	--m_cell_vector[m_current_cell];
}

void BFInterpreter::left() {
	//Move the pointer to the left
	if (m_current_cell != 0) --m_current_cell;
}

void BFInterpreter::right() {
	//Move the pointer to the right
	if (++m_current_cell == m_cell_vector.size()) m_cell_vector.push_back(0);
}

void BFInterpreter::input() {
	//Ask the user to enter an input
	m_in->get(m_cell_vector[m_current_cell]);
}

void BFInterpreter::output() {
	//Prompt the pointed cell value
	m_out->put(m_cell_vector[m_current_cell]);
	if (m_running_console) *m_out << std::endl;
}

void BFInterpreter::execute_action(char const& action) {
	//Execute the action if the program don't have to end a loop
	if (m_wait_for_end_loop == 0) {
		switch (action) {
		case '<':
			left();
			break;
		case '>':
			right();
			break;
		case '+':
			plus();
			break;
		case '-':
			minus();
			break;
		case '[':
			start_loop();
			break;
		case ']':
			end_loop();
			break;
		case '.':
			output();
			break;
		case ',':
			input();
			break;
		}
	}
	else {
		switch (action) {
		case '[':
			//The code enter a new loop it must wait for another end
			++m_wait_for_end_loop;
			break;
		case ']':
			//The code ended a loop it don't have to end one of the loop encountered
			--m_wait_for_end_loop;
			break;
		}
	}
}

//---HELPING METHODS---

void BFInterpreter::run_code_part(int start) {
for (m_current_action = start; m_current_action != m_code.size(); ++m_current_action)
	execute_action(m_code[m_current_action]);
};

void BFInterpreter::run_file(int start) {
initialize();
run_code_part(start);
}

void BFInterpreter::run_console() {
	m_running_console = true;
	initialize();
	m_current_action = 0;

	std::string user_input;
	std::vector<std::string> Flag_vector;

	while (m_running_console) {
		*m_out << m_prompt;
		std::getline(*m_in, user_input);

		if (std::all_of(user_input.begin(), user_input.end(), &is_valid_brainfuck_char)) {
			read_console_brainfuck(user_input);
			continue;
		}

		Flag_vector.clear();
		split(Flag_vector, user_input);
		if (is_valid_console_input(Flag_vector.front())) {
			process_console_input(Flag_vector);
			continue;
		}
		*m_out << '\'' << user_input << '\'' << "is not a valid bf_console input\n";
	}
}

void BFInterpreter::initialize() {
	//Set all value to their default state
	m_current_cell = 0;
	m_cell_vector = std::vector<char>(1, 0);
	m_loop_stack = std::stack<size_t>();
}

void BFInterpreter::read_string(std::string const& code) throw() {
	for (char read_char : code)
		add_char(read_char);
}

void BFInterpreter::add_char(char const& new_char) throw() {
	if (is_valid_brainfuck_char(new_char)) m_code.push_back(new_char);
}

//---COMMAND METHOD---

void BFInterpreter::process_console_input(std::vector<std::string>& args) throw() {
	std::string command = *args.begin();
	args.erase(args.begin());
	try {
		switch (INPUT_TO_ACTION.at(command)) {
		case ConsoleAction::HELP:
			//Show the help message
			command_help();
			break;
		case ConsoleAction::END:
			//Reset all velue to start a new brainfuck session
			command_end();
			break;
		case ConsoleAction::CELL:
			//Prompt the pointed cell and his value
			command_cell(args);
			break;
		case ConsoleAction::CODE:
			//Prompt the addition of all the brainfuck action
			command_code();
			break;
		case ConsoleAction::PROMPT:
			//Change the prompt string
			command_prompt(args);
			break;
		case ConsoleAction::EXIT:
			command_exit();
			//End the console session
		}
	}
	catch (invalid_argument_number e) {
		*m_out << e.what();
	}
	catch (cell_out_of_range e) {
		*m_out << e.what();
	}
	catch (std::invalid_argument e) {
		*m_out << e.what();
	}
}

void BFInterpreter::command_help() const throw() {
	*m_out << CONSOLE_HELP;
}

void BFInterpreter::command_end() throw() {
	*m_out << "Ending script, reset all value.\n";
	m_code = std::string();
	initialize();
	m_current_action = 0;
}

void BFInterpreter::command_cell(std::vector<std::string> const& Flag) const {
	if (Flag.size() > 1) throw invalid_argument_number("cell", "1 or 0", Flag.size());
	unsigned int cell = m_current_cell;
	if (!Flag.empty() && !(std::stringstream(Flag[0]) >> cell)) throw std::invalid_argument("'cell' take an integer as argument\n");
	if (cell >= m_cell_vector.size()) throw cell_out_of_range(cell);
	*m_out << "Cell: " << cell << " with value: " << static_cast<int>(m_cell_vector[cell]) << std::endl;
}

void BFInterpreter::command_code() const throw() {
	if (m_code.empty()) *m_out << "There is no code yet.\n";
	else *m_out << m_code << std::endl;
}

void BFInterpreter::command_exit() throw() {
	*m_out << "Quiting brainfuck console\n";
	m_running_console = false;
}

void BFInterpreter::command_prompt(std::vector<std::string> const& args) {
	if (args.empty()) throw invalid_argument_number("prompt", "at least 1", 0);
	m_prompt = std::accumulate(args.begin(), args.end(), std::string());
}

void BFInterpreter::read_console_brainfuck(std::string const& input) {
	//Verify is the bracket are correct and run code entered in console
	if (is_usable_code(input)) {
		read_string(input);
		run_code_part(m_current_action);
	}
	else *m_out << "'[' and ']' are not balance, try use 'code' command to see previous code\n";
}

//All valid brainfuck character
const std::set<char> BFInterpreter::BF_CHAR = { '>', '<', '+', '-', ',', '.', '[', ']' };
//All shells command
const std::map<std::string, BFInterpreter::ConsoleAction> BFInterpreter::INPUT_TO_ACTION = { { "end", BFInterpreter::ConsoleAction::END },
																							 { "exit", BFInterpreter::ConsoleAction::EXIT },
																					 		 { "cell", BFInterpreter::ConsoleAction::CELL },
																							 { "code", BFInterpreter::ConsoleAction::CODE },
																							 { "prompt", BFInterpreter::ConsoleAction::PROMPT },
																							 { "help", BFInterpreter::ConsoleAction::HELP } };
const std::string BFInterpreter::CONSOLE_HELP = {
	"You are in the console mode the different thing you can do are:\n"\
	"\n"\
	"run brainfuck command separatly or on the same line like so:\n"\
	":::+++\n"\
	":::[>+++++<\n"\
	":::-]++>.\n"\
	"They can be ran all in the order, since you don't write more ending brackets than opening brackets\n"\
	"\n"\
	"There is also 6 command that you can run:\n"\
	"'end' : end the brainfuck script started and start a new one\n"\
	"'cell [<index>]' : show the index of the cell and his value in decimal, index is the current pointed cell\n"\
	"'code' : show the entire code that the interpreter have runned in the current script\n"\
	"'prompt <new_prompt>' : change the prompt, is just for fun or for your eyes ;) \n"\
	"'help' : show this message\n"\
	"\n"\
	"I wish you are having fun with this little piece of software\n"
};



inline bool BFInterpreter::is_valid_brainfuck_char(char const& c) throw() {
	return BFInterpreter::BF_CHAR.find(c) != BFInterpreter::BF_CHAR.end();
}

inline bool BFInterpreter::is_valid_console_input(std::string const& s) throw() {
	return BFInterpreter::INPUT_TO_ACTION.find(s) != BFInterpreter::INPUT_TO_ACTION.end();
}

inline bool BFInterpreter::is_valid_input(std::string const& s) throw() {
	return is_valid_console_input(s)
		|| std::all_of(s.begin(), s.end(), &is_valid_brainfuck_char);
}