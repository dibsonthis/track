#pragma once

#include <iostream>
#include <string>
#include <memory>

enum class Type
{
    NONE, ERROR, END_OF_FILE, 
    ID, INT, FLOAT, STRING, D_ARROW, SEMICOLON, COLON, COMMA, DOT, B_SLASH,
    L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_BRACKET, R_BRACKET, L_ANGLE, R_ANGLE,
    EQ_EQ, NOT_EQ, LT_EQ, GT_EQ, PLUS_EQ, MINUS_EQ,
    PLUS_PLUS, MINUS_MINUS, DOUBLE_COLON,
    S_QUOTE, D_QUOTE,
    EQ, PLUS, MINUS, MUL, DIV,
    EXCLAMATION, QUESTION, AT, HASH, DOLLAR, CARET, PERCENT,
    AND, OR
};

class Token
{
    public:

    Type type = Type::NONE;
    int line = 0;
    int col = 0;
    std::string id_value;
    std::string str_value;
    int int_value = 0;
    float float_value;

    bool is_op = false;
    bool is_post_op = false;

    Token() {}
    Token(Type type) : type(type) {}
    Token(Type type, int line, int col) : type(type), line(line), col(col) {}

    std::string get_id_value();
    std::string get_str_value();
    int get_int_value();
    float get_float_value();

    void print();
};

std::string type_repr(Type type);