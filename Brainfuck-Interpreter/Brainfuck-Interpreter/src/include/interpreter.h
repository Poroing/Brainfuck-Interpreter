#ifndef BRAINFUCKCONSOLE_SRC_INTERPRETER_H_
#define BRAINFUCKCONSOLE_SRC_INTERPRETER_H_

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <set>
#include <map>

//Flag used to know how to run the interpreter
//Could be use later for state and other
enum class Flag;

class BFInterpreter
{
private:
	std::string m_code;
	std::istream* m_in;
	std::ostream* m_out;

	size_t m_current_action;
	std::stack<size_t> m_loop_stack;
	size_t m_current_cell;
	std::vector<char> m_cell_vector;
	int m_in_loop = 0;
	int m_wait_for_end_loop = 0;

	bool m_running_console = false;
	std::string m_prompt;

	Flag m_flag;

	static const std::string CONSOLE_HELP;

public:

	static const std::set<char> BF_CHAR;

	enum class ConsoleAction { END, CELL, EXIT, CODE, PROMPT, HELP };
	const static std::map<std::string, ConsoleAction> INPUT_TO_ACTION;

	BFInterpreter();
	BFInterpreter(std::string const& code, std::ostream& out, std::istream& in);
	BFInterpreter(std::string const& code, std::ostream& out);
	BFInterpreter(std::string const& code, std::istream& in);
	explicit BFInterpreter(std::string const& code);

	void run();

	void set_console() throw();
	bool console() const throw();

	void set_file() throw();
	bool file() const throw();

	void set_code(std::string const& code) throw();
	bool is_usable_code(std::string const& code) const throw();

	inline static bool is_valid_brainfuck_char(char const& c) throw();
	inline static bool is_valid_console_input(std::string const& s) throw();
	inline bool is_valid_input(std::string const& s) throw();

private:

	//---BRAINFUCK BASIC ACTION---

	void start_loop() throw();
	void end_loop() throw();
	void plus() throw();
	void minus() throw();
	void left() throw();
	void right() throw();
	void input() throw();
	void output() throw();

	void execute_action(char const& action) throw();

	//---HELPING METHODS---

	void run_code_part(int start = 0) throw();
	void run_file(int start = 0) throw();
	void run_console() throw();

	void initialize() throw();

	void read_string(std::string const& code) throw();
	void add_char(char const& new_char) throw();

	//---COMMAND METHOD---

	void process_console_input(std::vector<std::string>& args) throw();

	void command_help() const throw();
	void command_end() throw();
	void command_cell(std::vector<std::string> const& Flag) const;
	void command_code() const throw();
	void command_exit() throw();
	void command_prompt(std::vector<std::string> const& args);

	void read_console_brainfuck(std::string const& input) throw();
};

#endif
