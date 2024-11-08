#include "Interpreter.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>


Interpreter::Interpreter(std::ostream& outstream, std::string fileName) {
	outstream.out << ReadFile(fileName) << outstream.end;
	
}
std::string Interpreter::peek()
{
	return peek(0);
}

std::string Interpreter::peekP()
{
	return peekP(0);
}

std::string Interpreter::peekP(int steps)
{
	return tokens[position - steps];
}

bool Interpreter::ReadFile(const std::string& fileName)
{
	std::string line;
	std::ifstream myfile(fileName);

	if (myfile.is_open())
	{

		while (std::getline(myfile, line))
		{
			statements.push_back(line);
		}
		myfile.close();
		for (std::string statement : statements) {
			Tokenize(statement);
		}
		return true;
	}
	else {
		std::cout << "Unable to open file";
		return false;
	}
}

std::string Interpreter::peek(int steps)
{
	if (position + steps >= tokens.size()) return ETX;
	return tokens[position + steps];
}

void Interpreter::consume(const std::string& token)
{
	std::string next_token = peek();
	if (next_token == ETX)
		throw std::runtime_error("Consumed past last token\n");
	if (next_token != token)
		throw std::runtime_error("Could not consume token " + token + "\n");
	//std::cout << token << position << std::endl;
	position++;
}

std::vector<std::string> Interpreter::Tokenize(const std::string& input)
{
	std::string token;
	
	for (int i = 0; i < input.size(); i++) {
		char ch = input[i];
	
		if (!isspace(ch)) {
			token += ch;
		}
		else {
			tokens.push_back(token);
			token.clear();
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
	evaluate(tokens);
	return tokens;
}

void Interpreter::evaluate(const std::vector<std::string>& tokens)
{
	parse_stmt(tokens);
}

void Interpreter::parse_stmt(const std::vector<std::string>& tokens)
{
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");
	std::string next_token = peek();

	if (next_token == "config")
	{
		consume(next_token);
		parse_configstmt(tokens);
	}
	else if (next_token == "print")
	{
		consume(next_token);
		parse_printstmt(tokens);
	}
	else if (std::regex_match(next_token, variable))
	{

		symbolTable[next_token] = 0;
		consume(next_token);
		parse_assgstmt(tokens);

	}
	else if (next_token == ETX)
	{
		return;
	}
	else
	{
		std::cout << "error bitch: " << std::endl;
		throw std::runtime_error("error");

	}
}

void Interpreter::parse_configstmt(const std::vector<std::string>& tokens)
{
	std::string next_token = peek();
	consume(next_token);

	if (next_token == "dec")
	{
	}
	else if (next_token == "hex")
	{
	}
	else if (next_token == "bin")
	{
	}
}

int Interpreter::parse_printstmt(const std::vector<std::string>& tokens)
{
	int result = 0;
	std::string next_token = peek();
	std::regex Int("-?[0-9]+");
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");

	if (std::regex_match(next_token, Int) || next_token == "(")
	{
		result = Parse_MathExp(tokens);
		
	}
	else if (std::regex_match(next_token, variable)) 
	{
		result = symbolTable[next_token];
		consume(next_token);
	}
	else
	{
		std::cout << "error bitch: " << std::endl;
		throw std::runtime_error("Error at token: " + next_token);
	}

	//make this go to the printstream
	return result;
}

void Interpreter::parse_assgstmt(const std::vector<std::string>& tokens)
{
	int result = 0;
	std::string next_token = peek();
	std::string variableToken = peekP();

	if (next_token == ETX)
	{
		return;
	}
	if (next_token != "=")
	{
		std::cout << "error bitch: " << std::endl;
		throw std::runtime_error("error");
	}
	consume(next_token);
	result = Parse_MathExp(tokens);
	symbolTable[variableToken] = result;
}

int Interpreter::Parse_MathExp(const std::vector<std::string>& tokens) {
	return Parse_SumExp(tokens);
}

int Interpreter::Parse_SumExp(const std::vector<std::string>& tokens) {

	int result = Parse_ProductExp(tokens);

	std::string next_token = peek();
	while (1) {
		if (next_token == "+") {
			consume("+");
			int rhs = Parse_ProductExp(tokens);
			result = result + rhs;
		}
		else if (next_token == "-") {
			consume("-");
			int rhs = Parse_ProductExp(tokens);
			result = result - rhs;
		}
		else break;
		next_token = peek();
	}
	return result;
}

int Interpreter::Parse_ProductExp(const std::vector<std::string>& tokens) {
	int result = Parse_PrimaryExp(tokens);

	std::string next_token = peek();
	while (1) {
		if (next_token == "*") {
			consume("*");
			int rhs = Parse_PrimaryExp(tokens);
			result = result * rhs;
		}
		else if (next_token == "/") {
			consume("/");
			int rhs = Parse_PrimaryExp(tokens);
			result = result / rhs;
		}
		else break;
		next_token = peek();
	}
	return result;
}

int Interpreter::Parse_PrimaryExp(const std::vector<std::string>& tokens) {
	int result = 0;
	std::string next_token = peek();
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");
	std::regex Int("-?[0-9]+");

	if (next_token == "(") {
		consume(next_token);
		result = Parse_MathExp(tokens);
		next_token = peek();

		if (next_token != ")") {
			std::cout << "Fel: Saknad avslutande parentes." << std::endl;
			throw std::runtime_error("Fel vid token: " + next_token);
		}
		consume(next_token);
	}
	else if (std::regex_match(next_token, variable)) {
		result = symbolTable[next_token];
		consume(next_token);
	}
	else if (std::regex_match(next_token, Int)) {
		result = std::stoi(next_token);
		consume(next_token);
	}
	else {
		std::cout << "Fel: Oväntad token: " << next_token << std::endl;
		throw std::runtime_error("Fel vid token: " + next_token);
	}
	return result;
}
//int Interpreter::Parse_PrimaryExp(const std::vector<std::string>& tokens) {
//	int result;
//	std::string next_token = peek();
//
//	if (next_token == "(") {
//		consume(next_token);
//		result = Parse_MathExp(tokens);
//		next_token = peek();
//
//		if (next_token != ")") {
//			std::cout << "Fel: Saknad avslutande parentes." << std::endl;
//			throw std::runtime_error("Fel vid token: " + next_token);
//		}
//		consume(next_token);
//	}
//	else if (IsInt(next_token))
//	{
//		result = std::stoi(next_token);
//		consume(next_token);
//	}
////	 No valid PrimaryExp found, which is an error
//	else
//		throw std::runtime_error("expected int");
//	return result;
//}

bool Interpreter::IsInt(const std::string& str) {
	// Define the regex pattern for integers 
	std::regex pattern("-?[0-9]+");

	// Use std::regex_match to check if the string matches the pattern
	return std::regex_match(str, pattern);
}

bool Interpreter::IsValue(const std::string& str) {
	// Define the regex pattern for values
	std::regex pattern("[a-zA-Z][a-zA-Z0-9]*");

	// Use std::regex_match to check if the string matches the pattern
	return std::regex_match(str, pattern);
}



