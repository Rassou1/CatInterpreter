#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>
class Interpreter
{

public:
	std::vector<std::string> tokens;
	int position = 0;
	const std::string ETX = "\u0003";
	
	std::string peek()
	{
		return peek(0);
	}
	// Return token @steps ahead
	std::string peek(int steps)
	{
		if (position + steps >= tokens.size()) return ETX;
		return tokens[position + steps];
	}

	void consume(const std::string& token)
	{
		std::string next_token = peek();
		if (next_token == ETX)
			throw std::runtime_error("Consumed past last token\n");
		if (next_token != token)
			throw std::runtime_error("Could not consume token " + token + "\n");
		++position;
	}

	std::vector<std::string> Tokenize(const std::string& input) {
		std::istringstream stream(input);
		std::string line;

		// Read each line (statement) from the input
		while (std::getline(stream, line)) {
			// Tokenize each line and store the resulting tokens
			tokens.push_back(line);
		}

		return tokens;
	}

	int Parse_MathExp() {
		return Parse_SumExp();
	}

	int Parse_SumExp() {

		int result = Parse_ProductExp();

		std::string next_token = peek();
		while (1) {
			if (next_token == "+") {
				consume("+");
				int rhs = Parse_ProductExp();
				result = result + rhs;
			}
			else if (next_token == "-") {
				consume("-");
				int rhs = Parse_ProductExp();
				result = result + rhs;
			}
			else break;
		}
		next_token = peek();
		return result;
	}

	int Parse_ProductExp() {
		int result = Parse_PrimaryExp();

		std::string next_token = peek();
		while (1) {
			if (next_token == "*") {
				consume("*");
				int rhs = Parse_PrimaryExp();
				result = result * rhs;
			}
			else if (next_token == "/") {
				consume("/");
				int rhs = Parse_PrimaryExp();
				result = result / rhs;
			}
			else break;
		}
		return result;
	}

	int Parse_PrimaryExp() {
		int value;
		std::string input;
		std::string next_token = peek();
		
		if (IsInt(next_token))
		{
			value = std::stoi(next_token);
			consume(next_token);
		}
		// No valid PrimaryExp found, which is an error
		else
			throw std::runtime_error("expected int");
		return value;
	}

	bool IsInt(const std::string& str) {
		// Define the regex pattern for integers 
		std::regex pattern("-?[0-9]+");

		// Use std::regex_match to check if the string matches the pattern
		return std::regex_match(str, pattern);
	}
	
	bool IsValue(const std::string& str) {
		// Define the regex pattern for values
		std::regex pattern("[a-zA-Z][a-zA-Z0-9]*");

		// Use std::regex_match to check if the string matches the pattern
		return std::regex_match(str, pattern);
	}
	
};

