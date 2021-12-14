#include <fstream>

#include "Lexer.hpp"

void Lexer::error_and_continue(std::string message)
{
	std::string error_message = "[Lexer] Lexical Error in '" + l_file_path + "' @ (" + std::to_string(line) + ", " + std::to_string(column) + "): " + message;
	errors.push_back(error_message);
}

void Lexer::load_source(std::string file_path)
{
	l_file_path = file_path;
	std::ifstream stream(file_path);
	std::string source_str((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());
	source_code = source_str;
}

char Lexer::peek()
{
	int peek_index = index + 1;
	if (peek_index >= source_code.length())
	{
		return '\0';
	}
	else
	{
		return source_code[peek_index];
	}
}

void Lexer::advance(int steps)
{
	for (int i = 0; i < steps; i++)
	{
		index++;
		column++;
		if (index < source_code.length())
		{
			current_char = source_code[index];
		}
		else
		{
			current_char = '\0';
		}
	}
}

void Lexer::build_number()
{
	Token token(Type::NONE, line, column);

	std::string value;
	int num_dots = 0;

	while (isdigit(current_char) || current_char == '.')
	{
		value += current_char;

		if (current_char == '.')
		{
			num_dots++;
		}

		advance();
	}

	if (num_dots == 0)
	{
		token.type = Type::INT;
		token.int_value = std::stoi(value);
	}
	else if (num_dots == 1)
	{
		token.type = Type::FLOAT;
		token.float_value = std::stof(value);
	}
	else
	{
		token.type = Type::ERROR;
		error_and_continue("Unacceptable number of dots in number token.");
	}

	tokens.push_back(token);
}

void Lexer::build_id()
{
	Token token(Type::ID, line, column);

	std::string name = std::string();

	while (isalpha(current_char) || current_char == '_' || isdigit(current_char))
	{
		name += current_char;
		advance();
	}

	token.id_value = name;

	tokens.push_back(token);
}

void Lexer::build_str()
{
	Token token(Type::STRING, line, column);

	std::string str = std::string();

	advance();

	while (current_char != '"')
	{
		if (current_char == '\0')
		{
			error_and_continue("Warning: Missing end '\"', end of file reached.");
			return;
		}

		if (current_char == '\n')
		{
			line++;
			column = 0;
			advance(); // consume '\n'
		}

		str += current_char;
		advance();
	}

	advance();

	token.str_value = std::string();

	// Parse escape characters

	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '\\' && str[i + 1] == 'n')
		{
			str[i] = '\n';
			(token.str_value).push_back('\n');
			i++;
		}
		else if (str[i] == '\\' && str[i + 1] == 'r')
		{
			str[i] = '\r';
			(token.str_value).push_back('\r');
			i++;
		}
		else if (str[i] == '\\' && str[i + 1] == 't')
		{
			str[i] = '\t';
			(token.str_value).push_back('\t');
			i++;
		}
		else
		{
			(token.str_value).push_back(str[i]);
		}
	}

	tokens.push_back(token);
}

void Lexer::handle_line_comment()
{
	advance();
	advance();

	while (current_char != '\n')
	{
		if (current_char == '\0')
		{
			return;
		}

		advance();
	}

	line++;
	column = 0;

	advance(); // consume '\n'

	return;
}

void Lexer::handle_block_comment()
{
	advance(); // consume '/'
	advance(); // consume '*'

	while (!(current_char == '*' && peek() == '/'))
	{
		if (current_char == '\n')
		{
			line++;
			column = 0;
		}
		if (current_char == '\0')
		{
			error_and_continue("Warning: No end to block comment, end of file reached.");
			return;
		}
		if (current_char == '/' && peek() == '*')
		{
			handle_block_comment();
		}

		advance();
	}

	advance(); // consume '*'
	advance(); // consume '/'

	return;
}

void Lexer::tokenize()
{
	while (current_char != '\0')
	{
		if (current_char == '\n')
		{
			column = 0;
			line++;
			advance();
		}
		else if (current_char == ' ' || current_char == '\t')
		{
			advance();
		}
		else if (isalpha(current_char) || current_char == '_')
		{
			build_id();
		}
		else if (isdigit(current_char))
		{
			build_number();
		}
		else if (current_char == '/' && peek() == '/')
		{
			handle_line_comment();
		}
		else if (current_char == '/' && peek() == '*')
		{
			handle_block_comment();
		}
		else if (current_char == '"')
		{
			build_str();
		}
		else if (current_char == '=' && peek() == '=')
		{
			Token token(Type::EQ_EQ, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume =
			advance(); // consume =
		}
		else if (current_char == '!' && peek() == '=')
		{
			Token token(Type::NOT_EQ, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume !
			advance(); // consume =
		}
		else if (current_char == '<' && peek() == '=')
		{
			Token token(Type::LT_EQ, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume <
			advance(); // consume =
		}
		else if (current_char == '>' && peek() == '=')
		{
			Token token(Type::GT_EQ, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume >
			advance(); // consume =
		}
		else if (current_char == '+' && peek() == '=')
		{
			Token token(Type::PLUS_EQ, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume +
			advance(); // consume =
		}
		else if (current_char == '-' && peek() == '=')
		{
			Token token(Type::MINUS_EQ, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume -
			advance(); // consume =
		}
		else if (current_char == '+' && peek() == '+')
		{
			Token token(Type::PLUS_PLUS, line, column);
			token.is_post_op = true;
			tokens.push_back(token);
			advance(); // consume +
			advance(); // consume +
		}
		else if (current_char == '-' && peek() == '-')
		{
			Token token(Type::MINUS_MINUS, line, column);
			token.is_post_op = true;
			tokens.push_back(token);
			advance(); // consume -
			advance(); // consume -
		}
		else if (current_char == ':' && peek() == ':')
		{
			Token token(Type::DOUBLE_COLON, line, column);
			tokens.push_back(token);
			advance(); // consume :
			advance(); // consume :
		}
		else if (current_char == '>' && peek() == '>')
		{
			Token token(Type::D_ARROW, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume >
			advance(); // consume >
		}
		else if (current_char == '=')
		{
			Token token(Type::EQ, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '(')
		{
			Token token(Type::L_PAREN, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == ')')
		{
			Token token(Type::R_PAREN, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '{')
		{
			Token token(Type::L_BRACE, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '}')
		{
			Token token(Type::R_BRACE, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '[')
		{
			Token token(Type::L_BRACKET, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == ']')
		{
			Token token(Type::R_BRACKET, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '<')
		{
			Token token(Type::L_ANGLE, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '>')
		{
			Token token(Type::R_ANGLE, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '.')
		{
			Token token(Type::DOT, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '\\')
		{
			Token token(Type::B_SLASH, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '\'')
		{
			Token token(Type::S_QUOTE, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '!')
		{
			Token token(Type::EXCLAMATION, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '@')
		{
			Token token(Type::AT, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '#')
		{
			Token token(Type::HASH, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '$')
		{
			Token token(Type::DOLLAR, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '^')
		{
			Token token(Type::CARET, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '&')
		{
			Token token(Type::AND, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '|')
		{
			Token token(Type::OR, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '?')
		{
			Token token(Type::QUESTION, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '%')
		{
			Token token(Type::PERCENT, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '"')
		{
			Token token(Type::D_QUOTE, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '-')
		{
			Token token(Type::MINUS, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '+')
		{
			Token token(Type::PLUS, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '/')
		{
			Token token(Type::DIV, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == '*')
		{
			Token token(Type::MUL, line, column);
			token.is_op = true;
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == ',')
		{
			Token token(Type::COMMA, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == ':')
		{
			Token token(Type::COLON, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else if (current_char == ';')
		{
			Token token(Type::SEMICOLON, line, column);
			tokens.push_back(token);
			advance(); // consume symbol
		}
		else
		{
			error_and_continue("Unexpected token '" + std::string(1, current_char) + "'.");
			Token token(Type::ERROR, line, column);
			tokens.push_back(token);
			advance();
		}
	}

	Token token(Type::END_OF_FILE, line, column);
	tokens.push_back(token);

	//check_for_errors();
}

void Lexer::print_tokens()
{
	std::cout << "[ ";
	for (int i = 0; i < tokens.size(); i++)
	{
		tokens[i].print();
		if (i != tokens.size()-1)
		{
			std::cout << ", ";
		}
	}
	std::cout << " ]";
}