#include "Interpreter.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <bitset>


Interpreter::Interpreter(std::ostream& outstream, std::string fileName) : outstream(outstream) {
	ReadFile(fileName);
}
std::string Interpreter::Peek()
{
	return Peek(0);
}

std::string Interpreter::Peek(int steps)
{
	if (position + steps >= tokens.size()) return ETX;
	return tokens[position + steps];
}

std::string Interpreter::PeekP()
{
	return PeekP(1);
}

std::string Interpreter::PeekP(int steps)
{
	return tokens[position - steps];
}

enum IntBase {
	dec,
	hex,
	bin
};

IntBase IB = dec;

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
		outstream<< "Unable to open file";
		return false;
	}
}

void Interpreter::Consume(const std::string& token)
{
	std::string nextToken = Peek();
	if (nextToken == ETX)
		throw std::runtime_error("Consumed past last token\n");
	if (nextToken != token)
		throw std::runtime_error("Could not Consume token " + token + "\n");
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
	Evaluate(tokens);
	return tokens;
}

void Interpreter::Evaluate(const std::vector<std::string>& tokens)
{
	ParseStatement(tokens);
}

void Interpreter::ParseStatement(const std::vector<std::string>& tokens)
{
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");
	std::string nextToken = Peek();

	if (nextToken == "Config")
	{
		Consume(nextToken);
		ParseConfigStatement(tokens);
	}
	else if (nextToken == "Print")
	{
		Consume(nextToken);
		ParsePrintStatement(tokens);
	}
	else if (std::regex_match(nextToken, variable))
	{

		symbolTable[nextToken] = 0;
		Consume(nextToken);
		ParseAssignmentStatement(tokens);

	}
	else if (nextToken == ETX)
	{
		return;
	}
	else
	{
		outstream<< "Error encountered in method ParseStatement" << std::endl;
		throw std::runtime_error("error");
	}
}

void Interpreter::ParseConfigStatement(const std::vector<std::string>& tokens)
{
	std::string nextToken = Peek();
	Consume(nextToken);

	if (nextToken == "dec")
	{
		IB = dec;
		outstream << "Numbers are now in Base10" << std::endl;
	}
	else if (nextToken == "hex")
	{
		IB = hex;
		outstream << "Numbers are now in Base6" << std::endl;
	}
	else if (nextToken == "bin")
	{
		IB = bin;
		outstream << "Numbers are now in Base2" << std::endl;
	}
}

void Interpreter::ParsePrintStatement(const std::vector<std::string>& tokens)
{
	int result = 0;
	std::string nextToken = Peek();
	std::regex Int("-?[0-9]+");
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");

	if (std::regex_match(nextToken, Int) || nextToken == "(")
	{
		result = ParseMathExp(tokens);
		
	}
	else if (std::regex_match(nextToken, variable)) 
	{
		result = symbolTable[nextToken];
		Consume(nextToken);
	}
	else
	{
		outstream<< "Error encountered in method ParsePrintStatement" << std::endl;
		throw std::runtime_error("Error at token: " + nextToken);
	}

	switch(IB)  {
	case IntBase::dec:
		outstream << std::showbase << std::dec << result << std::endl;
		break;
	case IntBase::hex:
		outstream << std::showbase << std::hex << result << std::endl;
		break;
	case IntBase::bin:
		outstream << std::bitset<32>(result) << std::endl;
	}
	

	
}

void Interpreter::ParseAssignmentStatement(const std::vector<std::string>& tokens)
{
	int result = 0;
	std::string nextToken = Peek();
	std::string variableToken = PeekP();

	if (nextToken == ETX)
	{
		return;
	}
	if (nextToken != "=")
	{
		outstream<< "Error encountered in method ParseAssignmentStatement" << std::endl;
		throw std::runtime_error("error");
	}
	Consume(nextToken);
	result = ParseMathExp(tokens);
	symbolTable[variableToken] = result;
}

int Interpreter::ParseMathExp(const std::vector<std::string>& tokens) {
	return ParseSumExp(tokens);
}

int Interpreter::ParseSumExp(const std::vector<std::string>& tokens) {

	int result = ParseProductExp(tokens);

	std::string nextToken = Peek();
	while (1) {
		if (nextToken == "+") {
			Consume("+");
			int rhs = ParseProductExp(tokens);
			result = result + rhs;
		}
		else if (nextToken == "-") {
			Consume("-");
			int rhs = ParseProductExp(tokens);
			result = result - rhs;
		}
		else break;
		nextToken = Peek();
	}
	return result;
}

int Interpreter::ParseProductExp(const std::vector<std::string>& tokens) {
	int result = ParsePrimaryExp(tokens);

	std::string nextToken = Peek();
	while (1) {
		if (nextToken == "*") {
			Consume("*");
			int rhs = ParsePrimaryExp(tokens);
			result = result * rhs;
		}
		else if (nextToken == "/") {
			Consume("/");
			int rhs = ParsePrimaryExp(tokens);
			result = result / rhs;
		}
		else break;
		nextToken = Peek();
	}
	return result;
}

int Interpreter::ParsePrimaryExp(const std::vector<std::string>& tokens) {
	int result = 0;
	std::string nextToken = Peek();
	std::regex variable("[a-zA-Z][a-zA-Z0-9]*");
	std::regex Int("-?[0-9]+");

	if (nextToken == "(") {
		Consume(nextToken);
		result = ParseMathExp(tokens);
		nextToken = Peek();

		if (nextToken != ")") {
			outstream<< "Fel: Saknad avslutande parentes." << std::endl;
			throw std::runtime_error("Fel vid token: " + nextToken);
		}
		Consume(nextToken);
	}
	else if (std::regex_match(nextToken, variable)) {
		result = symbolTable[nextToken];
		Consume(nextToken);
	}
	else if (std::regex_match(nextToken, Int)) {
		result = std::stoi(nextToken);
		Consume(nextToken);
	}
	else {
		outstream<< "Fel: Oväntad token: " << nextToken << std::endl;
		throw std::runtime_error("Fel vid token: " + nextToken);
	}
	return result;
}

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






