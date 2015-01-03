#include "args.h"

#include <fstream>
#include "interpreter.h"

//Enum class use for bitflag to identify which args has been used
enum class Args {
	F = 0x01,
	C = 0x02,
	B = 0x04,
	H = 0x08
};

Args operator| (Args const& a, Args const& b) throw() {
	return Args(static_cast<int>(a) | static_cast<int>(b));
}

int operator& (Args const& a, Args const& b) throw() {
	return static_cast<int>(a)& static_cast<int>(b);
}

const std::string HELP = {
	"BFInterpreter [-h] [-f <file path> | -c | -b <code>]\n"\
	"\n"\
	"-h\tDisplay this help message\n"\
	"\n"\
	"-f <file path> read brainfuck from a file\n"\
	"-c run the interpreter in console mode\n"\
	"-b <code> run the as braifuck code the string pass as code\n"
};

void load_file(std::string& code, char* const& path) {
	std::ifstream f(path);
	if (f.fail()) throw std::invalid_argument("Invalid path or an error have occured when oppening the file");
	char read_char;
	while (f.get(read_char))
		if (BFInterpreter::is_valid_brainfuck_char(read_char)) code += read_char;
}

void handle_args(BFInterpreter& interpreter, int argc, char* argv[]) {
	std::string code;
	//Set the args flag to no args entered
	Args args_set(static_cast<Args>(0));
	//If no argument are used the Interpreter is put in console mode
	if (argc == 1) interpreter.set_console();
	//Iteration over the argument
	for (int i(1); i != argc; ++i) {
		if (*argv[i] == '-') {
			try {
				switch (*(argv[i] + 1)) {
				case 'h':
					std::cout << HELP;
					args_set = args_set | Args::H;
					break;
				case 'f':
					if (args_set & Args::C) throw std::invalid_argument("'-f' and '-c' are unusable together\n");
					if (args_set & Args::B) throw std::invalid_argument("'-f' and '-b' are unsunabe together\n");
					if (argc == ++i) throw std::invalid_argument("missing filename after '-f'\n");
					args_set = args_set | Args::F;
					interpreter.set_file();
					load_file(code, argv[i]);
					interpreter.set_code(code);
					break;
				case 'c':
					if (args_set & Args::F) throw std::invalid_argument("'-f' and '-c' are unusable together\n");
					if (args_set & Args::B) throw std::invalid_argument("'-c' and '-b' are unusable together\n");
					args_set = args_set | Args::C;
					interpreter.set_console();
					break;
				case 'b':
					if (args_set & Args::F) throw std::invalid_argument("'-b' and '-f' are not usable together\n");
					if (args_set & Args::B) throw std::invalid_argument("'-c' and '-b' are unusable together\n");
					if (argc == ++i) throw std::invalid_argument("missing filename after '-b'\n");
					args_set = args_set | Args::B;
					interpreter.set_file();
					interpreter.set_code(argv[i]);
					break;
				}
				//If the '-h' arg has been used we don't need to check for other args
				if (args_set & Args::H) break;
			}
			catch (std::invalid_argument e) {
				throw e;
			}
		}
		else {
			throw std::invalid_argument("Argument unknown\n");
		}
	}
}