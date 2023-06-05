#pragma once
#include "BlockScheme.h"
#include <string>
#include <map>
#include <set>

class QShaper
{
public:
	QShaper(SchemeReader* _scheme);
	~QShaper();
	void IretateScheme();
	void Init();

private:
	SchemeReader *scheme;
	void ReadContent();
	void ReadInputParametrs();
	void IterateVertices();
	int log_count = 0;
	std::map<std::string,int> inputParametrs;
	std::vector<std::string> inputVariables;
	std::map<std::string, int> localVariables;
	std::map<std::string, std::string> localHookVariables;
	std::vector<std::string> outVariables;
	std::map<std::string, std::string> outNames;
	std::vector<std::vector<char>> derContents;
	std::vector<std::string> j_out;
	std::vector<std::vector<std::string>> logicalQterms;
	std::vector<int> log_nums;
	int curr_log = 0;
	std::vector<std::string> outputQterm;
	std::set<std::string> ops{"+", "-", "/", "*" , "?"};

	int CalcOp(std::string op, const int first,const int second);
	void ReadHook(std::string str);
	void ReadOutVars();
	std::string MakeQterm(std::string op, std::string first, std::string second);
	std::string NameOfHookVar(std::string var);
	std::string NameOfLocalVar(std::string var);
	std::string NameOfOp(std::string var);
	void ClearVars();
	void UpdateVars();
	void BinaryAdd();

};

