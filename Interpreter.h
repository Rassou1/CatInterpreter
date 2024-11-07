#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>
#include <map>

class Interpreter
{

public:

	Interpreter(std::ostream& out_stream, std::string fileName);
	
	std::vector <std::string> tokens;

	std::vector<std::string> statements;

	std::map<std::string, int> symbolTable;

	std::ostream& outstream;
	
	int position = 0;
	
	const std::string ETX = "\u0003";
	
	std::string Peek();

	std::string PeekP();

	std::string PeekP(int steps);
	
	std::string Peek(int steps);

	bool ReadFile(const std::string& fileName);

	void Consume(const std::string& token);

	std::vector<std::string> Tokenize(const std::string& input);

	void Evaluate(const std::vector<std::string>& tokens);

	void ParseStatement(const std::vector<std::string>& tokens);

	void ParseConfigStatement(const std::vector<std::string>& tokens);

	void ParsePrintStatement(const std::vector<std::string>& tokens);

	void ParseAssignmentStatement(const std::vector<std::string>& tokens);

	int ParseMathExp(const std::vector<std::string>& tokens);

	int ParseSumExp(const std::vector<std::string>& tokens);

	int ParseProductExp(const std::vector<std::string>& tokens);

	int ParsePrimaryExp(const std::vector<std::string>& tokens);

	bool IsInt(const std::string& str);

	bool IsValue(const std::string& str);

};


