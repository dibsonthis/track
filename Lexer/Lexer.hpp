#include <vector>

#include "../Token/Token.hpp"

class Lexer
{
	public:

	std::string l_file_path = "<stdin>";
	std::string source_code;
	int line = 0;
	int column = 0;

	char current_char = '\0';
	int index = 0;
	std::vector<Token> tokens;

	// Error Handling
	std::vector<std::string> errors;
	void error_and_continue(std::string message);

	void load_source(std::string file_path);

	char peek();
	void advance(int steps = 1);
	void build_number();
	void build_id();
	void build_str();

	// Comment Handling
	void handle_line_comment();
	void handle_block_comment();

	void tokenize();

	void print_tokens();

	Lexer() {}
	Lexer(std::string file_path)
	{
		load_source(file_path);
		current_char = source_code[0];
	}
};