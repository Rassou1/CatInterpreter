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

	Interpreter(std::ostream& out_stream);
	
	std::vector <std::string> tokens;

	std::vector<std::string> statements;

	std::map<std::string, int> symbolTable;
	
	int position = 0;
	
	const std::string ETX = "\u0003";
	
	std::string peek();

	std::string peekP();

	std::string peekP(int steps);
	
	std::string peek(int steps);

	bool ReadFile(const std::string& fileName);

	void consume(const std::string& token);

	std::vector<std::string> Tokenize(const std::string& input);

	void evaluate(const std::vector<std::string>& tokens);

	void parse_stmt(const std::vector<std::string>& tokens);

	void parse_configstmt(const std::vector<std::string>& tokens);

	void parse_printstmt(const std::vector<std::string>& tokens);

	void parse_assgstmt(const std::vector<std::string>& tokens);

	

	int Parse_MathExp(const std::vector<std::string>& tokens);

	int Parse_SumExp(const std::vector<std::string>& tokens);

	int Parse_ProductExp(const std::vector<std::string>& tokens);

	int Parse_PrimaryExp(const std::vector<std::string>& tokens);

	bool IsInt(const std::string& str);

	bool IsValue(const std::string& str);

};


