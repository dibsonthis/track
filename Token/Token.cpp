#include "Token.hpp"

std::string Token::get_id_value()
{
    if (type == Type::ID)
    {
        return id_value;
    }
    else
    {
        return std::string();
    }
}

std::string Token::get_str_value()
{
    if (type == Type::STRING)
    {
        return str_value;
    }
    else
    {
        return std::string();
    }
}

int Token::get_int_value()
{
    if (type == Type::INT)
    {
        return int_value;
    }
    else
    {
        return -1;
    }
}

float Token::get_float_value()
{
    if (type == Type::FLOAT)
    {
        return float_value;
    }
    else
    {
        return -1;
    }
}

void Token::print()
{
    std::cout << type_repr(type);

    if (type == Type::ID)
    {
        std::cout << "(" << id_value << ")";
    }
    else if (type == Type::STRING)
    {
        std::cout << "(" << str_value << ")";
    }
    else if (type == Type::INT)
    {
        std::cout << "(" << int_value << ")";
    }
    else if (type == Type::FLOAT)
    {
        std::cout << "(" << float_value << ")";
    }
}

std::string type_repr(Type type)
{
    switch(type)
    {
        case Type::AND:
            return "&";
        case Type::AT:
            return "@";
        case Type::B_SLASH:
            return "\\";
        case Type::CARET:
            return "^";
        case Type::COLON:
            return ":";
        case Type::COMMA:
            return ",";
        case Type::D_ARROW:
            return ">>";
        case Type::D_QUOTE:
            return "/""";
        case Type::DIV:
            return "/";
        case Type::DOLLAR:
            return "$";
        case Type::DOT:
            return ".";
        case Type::DOUBLE_COLON:
            return "::";
        case Type::END_OF_FILE:
            return "END_OF_FILE";
        case Type::EQ:
            return "=";
        case Type::EQ_EQ:
            return "==";
        case Type::ERROR:
            return "ERROR";
        case Type::EXCLAMATION:
            return "!";
        case Type::FLOAT:
            return "FLOAT";
        case Type::GT_EQ:
            return ">=";
        case Type::HASH:
            return "#";
        case Type::ID:
            return "ID";
        case Type::INT:
            return "INT";
        case Type::L_ANGLE:
            return "<";
        case Type::L_BRACE:
            return "{";
        case Type::L_BRACKET:
            return "[";
        case Type::L_PAREN:
            return "(";
        case Type::LT_EQ:
            return "<=";
        case Type::MINUS:
            return "-";
        case Type::MINUS_EQ:
            return "-=";
        case Type::MINUS_MINUS:
            return "--";
        case Type::MUL:
            return "*";
        case Type::NONE:
            return "{x}";
        case Type::NOT_EQ:
            return "!=";
        case Type::OR:
            return "|";
        case Type::PERCENT:
            return "%";
        case Type::PLUS:
            return "+";
        case Type::PLUS_EQ:
            return "+=";
        case Type::PLUS_PLUS:
            return "++";
        case Type::QUESTION:
            return "?";
        case Type::R_ANGLE:
            return ">";
        case Type::R_BRACE:
            return "}";
        case Type::R_BRACKET:
            return "]";
        case Type::R_PAREN:
            return ")";
        case Type::S_QUOTE:
            return "'";
        case Type::SEMICOLON:
            return ";";
        case Type::STRING:
            return "STRING";
        default:
            return "undefined_type_repr";
    }
}