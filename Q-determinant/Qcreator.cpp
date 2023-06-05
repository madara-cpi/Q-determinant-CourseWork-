#define _CRT_SECURE_NO_WARNINGS

#include "Qcreator.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>



QShaper::QShaper(SchemeReader* _scheme)
{
	this->scheme = _scheme;
}
QShaper::~QShaper()
{
	this->scheme->~SchemeReader();
}

void QShaper::Init()
{
	outVariables.resize(1);
	ReadInputParametrs();
	ReadOutVars();
	IterateVertices();
	
	auto iter = outputQterm.begin();
	std::string out = "";
	while (iter != outputQterm.end())
	{
		std::cout << *(iter) << std::endl;
		out = out + *(iter)+"\n";
		++iter;
	}
	this->scheme->SerializeToJson("output.json", out);
	
	 
	
}
void QShaper::ReadContent()
{
	auto iter = scheme->Vertices.begin();
	while (iter != scheme->Vertices.end())
	{
		switch ((*iter).type)
		{
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		}
	}
}

void QShaper::ReadInputParametrs()
{
	auto iter = scheme->Vertices.begin()+1;
	
	while ((*iter).type == 4)
	{
		bool hook = false;
		for (int i = 0; i < (*iter).content.length(); i++)
		{
			if ((*iter).content[i] == '[')
			{
				hook = true;
				char buf[50];
				size_t n = (*iter).content.copy(buf, i,0);
				buf[n] = '\0';
				inputVariables.push_back(buf);
			}
			if ((*iter).content[i] == ',' || (*iter).content[i] == ']')
			{
				std::cout << "Введите по порядку значения параметров для " << (*iter).content << std::endl;
				int a = 0;
				std::cin >> a;
				inputParametrs[std::string(1,(*iter).content[i - 1])] = a;
			}
		}
		if (!hook)
		{
			std::cout << "Введите значение " << (*iter).content << std::endl;
			int a = 0;
			std::cin >> a;
			inputParametrs[(*iter).content] = a;
		}
		

		++iter;
	}
}
void QShaper::ReadOutVars()
{
	auto iter = scheme->Vertices.begin() + 1;
	while ((*iter).type != 5) ++iter;
	while ((*iter).type != 1)
	{
		auto hook = (*iter).content.find('[');
		if (hook != std::string::npos)
		{
			char varb[50];
			size_t nn = (*iter).content.copy(varb, hook, 0);
			varb[nn] = '\0';
			std::string locv = std::string(1,(*iter).content[hook+1]);
			if ((*iter).content.find(',') != std::string::npos)
			{
				std::string locv2 = std::string(1, (*iter).content[hook + 3]);
				for (int i = 1; i < inputParametrs[locv] + 1; i++)
				{
					for (int j = 1; j < inputParametrs[locv2]+1; j++)
					{
						std::string outv = std::string(varb) + '[' + std::to_string(i) + ',' + std::to_string(j) + ']';
						outVariables.push_back(outv);
						outNames[varb] = varb;
					}
				}
			}
			else
			{
				for (int i = 1; i < inputParametrs[locv]+1; i++)
				{
					std::string outv = std::string(varb) + '[' + std::to_string(i) + ']';
					outVariables.push_back(outv);
					outNames[varb] = varb;
				}
			}
		}
		else
		{
			outVariables.push_back((*iter).content);
			outNames[(*iter).content] = (*iter).content;
		}
		++iter;
	}
}
void QShaper::UpdateVars()
{
	localVariables.clear();
	localHookVariables.clear();
	outputQterm.push_back(*(j_out.end() - 1));
	j_out.clear();
	
}

void QShaper::BinaryAdd()
{
	auto binum = log_nums.end() - 1;
	if (*binum == 0) *binum = 1;
	else
	{
		while (*binum != 0)
		{
			if (binum == log_nums.begin())
			{
				if (*binum == 1) return;
				else break;
			}
			binum--;
		}
		*binum = 1;
		binum++;
		while (binum < log_nums.end())
		{
			*binum = 0;
			binum++;
		}
				
	}
}
void QShaper::IterateVertices()
{
	auto iter = scheme->Vertices.begin() + 1;
	std::vector<std::string> localLogicalQterms;
	while ((*iter).type != 1)
	{
			
		if ((*iter).type == 5)
		{
			std::string qterm = "";
			if (!localLogicalQterms.empty())
			{
				auto jiter = localLogicalQterms.begin();

				qterm = *jiter;
				while (jiter != localLogicalQterms.end()-1)
				{
					if (localLogicalQterms.capacity() <= 1)
					{
						break;
					}
					++jiter;
					qterm = std::string("{\"op\":\"&\"") + ',' + std::string("\"fO\":") + '"' + qterm +'"' + ',' + ' ' + std::string("\"sO\": ") + '"' + *jiter  + '"' + '}';
					
				}

				localLogicalQterms.clear();
				bool endcicle = false;
				log_count = 0;
				int index = 0;
				auto first_one = std::find(log_nums.begin(), log_nums.end(), 1);
				if (first_one == log_nums.end())
				{
					*(first_one - 1) = 1;
					endcicle = true;

				}
				else if (first_one == log_nums.end() - 1 && log_nums.capacity()!=1)
				{
					*(first_one) = 0;
					*(first_one - 1) = 1; 
					endcicle = true;
				}
				else if (log_nums.capacity() == 1)
				{
					UpdateVars();
					break;
				}
			
				if (!endcicle) {
					auto log_iter = first_one;

					while (*log_iter != 0)
					{
						if (log_iter == log_nums.end()) break;
						++log_iter;
						if (log_iter == log_nums.end()) break;
					}
					

					if (log_iter == log_nums.end())
					{
						if (first_one == log_nums.begin() && *first_one == 1)
						{
							*(j_out.end() - 1) = (*iter).content + '=' + qterm + ';' + *(j_out.end() - 1);
							UpdateVars();
							break;
						}
						*(first_one - 1) = 1;
					
						while (first_one != log_nums.end())
						{
							(*first_one) = 0;
							++first_one;
						}
					}
					else
					{
						BinaryAdd();
						
					}
				}
				*(j_out.end() - 1) = (*iter).content + '=' + qterm + ';' + *(j_out.end() - 1);
				UpdateVars();
				iter = scheme->Vertices.begin() + 1;

				/*
				for (index; index < log_nums.capacity(); index++)
				{
					if (log_nums[index] == 1) break;
				}
			    if (index >0) curr_log = index-1;
				if (log_nums[curr_log] == 1) curr_log -= 1;

				*(j_out.end() - 1) = (*iter).content + '=' + qterm + ';' + *(j_out.end() - 1); 
				
				UpdateVars();

				if (std::find(log_nums.begin(), log_nums.end(), 0) != log_nums.end())
				{
					iter = scheme->Vertices.begin() + 1;
				}
				else break;
				log_nums[curr_log] = 1;*/
			}
			else
			{
				*(j_out.end() - 1) = (*iter).content + '=' + qterm + ';' + *(j_out.end() - 1);
				
				UpdateVars();
				break;
			}
			
					
		}
		

		while ((*iter).type == 4)
		{
			if ((*iter).type == 5)
			{
				
				break;
			}
			++iter;
		}
		while ((*iter).type != 3)
		{
			if ((*iter).type == 5)
			{
				
				break;
			}


			bool hook = false;
			
			for (int i = 0; i < (*iter).content.length(); i++)
			{
				if ((*iter).content[i] == '(') hook = true;
			}
			int i = 0;
			while ((*iter).content[i] != '=')
			{
				i++;
			}
			char first[50];
			size_t n = (*iter).content.copy(first, i, 0);
			first[n] = '\0';

			char after_equal[50];
			size_t nl = (*iter).content.copy(after_equal, (*iter).content.length(), i + 1);
			after_equal[nl] = '\0';

			int j = i + 1;
			bool isOp = false;
			while (i != (*iter).content.length())
			{
				if (ops.find(std::string(1, (*iter).content[i])) != ops.end())
				{
					isOp = true;
					break;
				}
				++i;
			}
			char second[50];
			size_t nn = (*iter).content.copy(second, i - j, j);
			second[nn] = '\0';

			switch (hook)
			{
			case true:
			{
				bool firstHook = false;
				int k = 0;
				for (k; k < n; k++)
				{
					if (first[k] == '(')
					{
						firstHook = true;
						break;
					}
				}

				char firstName[50];
				size_t nk = std::string(first).copy(firstName, k, 0);
				firstName[nk] = '\0';
				std::string outv = " ";
				if (NameOfHookVar(first) != first) outv = std::string(firstName) + '(' + std::to_string(localVariables[NameOfLocalVar(first)]) + ')';
				else outv = std::string(firstName);


				bool isCom = false;
				for (int j = 0; j < n; j++)
				{
					if (first[j] == ',')
					{
						isCom = true;
						break;
					}
				}
				if (isCom)
				{

				}
				else
				{

					bool isDigit = true;
					for (int k = 0; k < nn; k++)
					{
						if (!std::isdigit(second[k])) isDigit = false;
					}
					std::string s_second = " ";

					if (isDigit) localVariables[outv] = std::atoi(second);
					else
					{

						if (localHookVariables.find(second) == localHookVariables.end()) s_second = NameOfHookVar(second) + '(' + std::to_string(localVariables[NameOfLocalVar(second)]) + ')';
						else s_second = NameOfHookVar(second) + '(' + NameOfLocalVar(second) + ')';

						if (localVariables.find(NameOfHookVar(second)) != localVariables.end())
						{
							localVariables[outv] = localVariables[second];

						}
						else if (localHookVariables.find(s_second) != localHookVariables.end())
						{
							if (outNames.find(firstName) == outNames.end()) localHookVariables[outv] = localHookVariables[s_second];
							else j_out.push_back(localHookVariables[s_second]);
						}
						else
						{
							if (!isOp)
							{
								std::string fO = " ";
								//if (std::find(inputVariables.begin(), inputVariables.end(), ))
								if (localHookVariables[outv] == "") fO = "0";
								else fO = localHookVariables[outv];
								if (outNames.find(firstName) == outNames.end()) localHookVariables[outv] =  MakeQterm("+", fO, s_second);
								else if (std::find(inputVariables.begin(), inputVariables.end(), NameOfHookVar(second)) != inputVariables.end())
								{
									localHookVariables[outv] = s_second;
									j_out.push_back(localHookVariables[outv]);
								}
								else j_out.push_back(s_second);

							}
						}

					}
					if (!isOp) break;
					else
					{
						char third[50];
						nn = (*iter).content.copy(third, (*iter).content.length() - i, i + 1);
						third[nn] = '\0';
						std::string outth = std::string(NameOfHookVar(third)) + '(' + std::to_string(localVariables[NameOfLocalVar(third)]) + ')';
						std::string fO = " ";
						if (localHookVariables[outv] != "") fO = localHookVariables[outv];
						else if (localVariables.find(outv) != localVariables.end()) fO = localVariables[outv];
						else fO = s_second;

						if (outNames.find(firstName) == outNames.end())
						{
							if (localHookVariables[s_second] != "")
							{
								if (localHookVariables[outth] != "") localHookVariables[outv] = MakeQterm(NameOfOp((*iter).content), localHookVariables[s_second], localHookVariables[outth]);
								else localHookVariables[outv] = MakeQterm(NameOfOp((*iter).content), localHookVariables[s_second], outth);
								
							}
							else localHookVariables[outv] = MakeQterm(NameOfOp((*iter).content), s_second, outth);
							//std::cout << localHookVariables[outv] << '\n' <<'\n'<<'\n';

						}

						else
						{
							localHookVariables[outv] = MakeQterm(NameOfOp((*iter).content), fO, outth);
							j_out.push_back(MakeQterm(NameOfOp((*iter).content), fO, outth));
						}
					}
				}

				break;
			}
			case false:

				if (outNames.find(first) != outNames.end())
				{
					char third[50];
					nn = (*iter).content.copy(third, (*iter).content.length() - i, i + 1);
					third[nn] = '\0';
					localHookVariables[first] = MakeQterm(NameOfOp((*iter).content), localHookVariables[second], localHookVariables[third]);
					j_out.push_back(localHookVariables[first]);
					
				}
				else
				{
					bool isDigit = true;
					for (int k = 0; k < nn; k++)
					{
						if (!std::isdigit(second[k])) isDigit = false;
					}
					auto ouiter = std::find(inputVariables.begin(), inputVariables.end(), second);
					if (isDigit) localVariables[first] = std::atoi(second); //если справа от операции присваивания число, присваиваем его во внутренние переменные
					else if (ouiter == inputVariables.end() && localHookVariables[first] == "") localVariables[first] = localVariables[second];
					else  localHookVariables[first] = second; //если другая внутренняя переменная, сохраняем в строковом виде

					if (i == (*iter).content.length())
					{
						
						break;
					}
					else
					{ //вычисление второй переменной после знака присваивания
						nn = (*iter).content.copy(second, (*iter).content.length() - j - 2, j + 2);
						second[nn] = '\0';
						isDigit = true;
						for (int k = 0; k < nn; k++)
						{
							if (!std::isdigit(second[k])) isDigit = false;
						}
						int secOp;
						if (isDigit) secOp = std::atoi(second);
					    if (localHookVariables[first] == "")
						{
							if (!isDigit) secOp = localVariables[second];
							localVariables[first] = CalcOp(std::string(1, (*iter).content[i]), localVariables[first], secOp);//если переменной присваивается число, калькулируем 
						}
						else
						{
							localHookVariables[first] = MakeQterm(NameOfOp((*iter).content), localHookVariables[first], second); //если переменная входная, формируем Q-терм
						}
					}
					
				}
				break;
			}
			int type = 2;
			auto edge_iter = std::find_if(scheme->Edges.begin(), scheme->Edges.end(), [iter, type](std::pair<std::pair<int, int>, int> e) {return (e.first.first == (*iter).id && e.second == type); });
			iter = scheme->Vertices.begin() + (*edge_iter).first.second - 1;
		}
		if ((*iter).type == 3)
		{
			bool hook = false;
			std::string op = " ";
			std::string desop = "";
			for (int i = 0; i < (*iter).content.length(); i++)
			{
				if ((*iter).content[i] == '(') hook = true;
			}
			int i = 0;
			while (true)
			{
				i++;
				if ((*iter).content[i] == '=' || (*iter).content[i] == '>' || (*iter).content[i] == '<')
				{
					op = (*iter).content[i];
					break;

				}
			}

			char first[50];
			size_t n = (*iter).content.copy(first, i, 0);
			first[n] = '\0';
			if ((*iter).content[i] == '>' || (*iter).content[i] == '<')
			{
				if ((*iter).content[i + 1] == '=')
				{
					op = op + (*iter).content[i + 1];
					i++;
				}
			}

			char second[50];
			size_t nn = (*iter).content.copy(second, (*iter).content.length() - i, i + 1);
			second[nn] = '\0';

			if (!hook && std::find(inputVariables.begin(), inputVariables.end(), first)==inputVariables.end())
			{
				int f = 0, s = 0;
				f = localVariables[first];

				bool isDigit = true;
				for (int k = 0; k < nn; k++)
				{
					if (!std::isdigit(second[k])) isDigit = false;
				}
				if (isDigit) s = std::atoi(second);
				else if (inputParametrs.find(second) == inputParametrs.end()) s = localVariables[second];
				else s = inputParametrs[second];

				int type = 0;

				if (op == "<=")
				{
					desop = ">";
					if (f <= s) type = 1;
					else type = 0;
				}
				else if (op == ">=")
				{
					desop = "<";
					if (f >= s) type = 1;
					else type = 0;
				}
				else if (op == ">")
				{
					desop = "<=" ;
					if (f > s) type = 1;
					else type = 0;
				}
				else if (op == "<")
				{
					desop = ">=";
					if (f < s) type = 1;
					else type = 0;
				}
				else if (op == "==")
				{
					desop = "!=";
					if (f == s) type = 1;
					else type = 0;
				}
				else if (op == "!=")
				{
					desop = "==";
					if (f != s) type = 1;
					else type = 0;
				}

				auto edge_iter = std::find_if(scheme->Edges.begin(), scheme->Edges.end(), [iter, type](std::pair<std::pair<int, int>, int> e) {return (e.first.first == (*iter).id && e.second == type); });
				iter = scheme->Vertices.begin() + (*edge_iter).first.second - 1;
			}
			else
			{
				if (op == "<=")
				{
					desop = ">";
					
				}
				else if (op == ">=")
				{
					desop = "<";
					
				}
				else if (op == ">")
				{
					desop = "<=";
					
				}
				else if (op == "<")
				{
					desop = ">=";
					
				}
				else if (op == "==")
				{
					desop = "!=";
				
				}
				else if (op == "!=")
				{
					desop = "==";
					
				}
				std::string fO = " ";
				std::string sO = " ";
				hook = false;
				int i_hook = 0;
				for (int i = 0; i < n; i++)
				{
					if (first[i] == '(')
					{
						hook = true;
						i_hook = i;
					}
				}
				if (hook)
				{
					std::string name_first = NameOfHookVar(std::string(first));
					std::string name_loc = NameOfLocalVar(std::string(first));
					std::string full_first = name_first + '(' + std::to_string(localVariables[name_loc]) + ')';
					
					auto founded = std::find(inputVariables.begin(), inputVariables.end(), name_first);
					if (founded != inputVariables.end())
					{
						fO = *founded;
					}
					else if (localHookVariables[full_first] != "")
					{
						fO = localHookVariables[full_first];
					}
				}
				else
				{
					std::string name_first = NameOfHookVar(std::string(first));
					auto founded = std::find(inputVariables.begin(), inputVariables.end(), name_first);
					if (founded != inputVariables.end())
					{
						fO = *founded;
					}
					else if (localHookVariables[name_first] != "")
					{
						fO = localHookVariables[name_first];
					}
				}
				hook = false;
				for (int i = 0; i < n; i++)
				{
					if (second[i] == '(')
					{
						hook = true;
						i_hook = i;
					}
				}
				if (hook)
				{
					std::string name_first = NameOfHookVar(std::string(second));
					std::string name_loc = NameOfLocalVar(std::string(second));
					std::string full_first = name_first + '(' + std::to_string(localVariables[name_loc]) + ')';

					auto founded = std::find(inputVariables.begin(), inputVariables.end(), name_first);
					if (founded != inputVariables.end())
					{
						sO = full_first;
					}
					else if (localHookVariables[full_first] != "")
					{
						sO = localHookVariables[full_first];
					}
				}
				else
				{
					std::string name_first = NameOfHookVar(std::string(second));
					auto founded = std::find(inputVariables.begin(), inputVariables.end(), name_first);
					if (founded != inputVariables.end())
					{
						sO = *founded;
					}
					else if (localHookVariables[name_first] != "")
					{
						sO = localHookVariables[name_first];
					}
					else
					{
						sO = name_first;
					}
				}
			  

				if (log_nums.capacity() < log_count + 1) log_nums.push_back(0);
				int type = 0;
				if (log_nums[log_count] == 1)
				{
					type = 1;
					
				}
				else op = desop;
				auto edge_iter = std::find_if(scheme->Edges.begin(), scheme->Edges.end(), [iter, type](std::pair<std::pair<int, int>, int> e) {return (e.first.first == (*iter).id && e.second == type); });
				iter = scheme->Vertices.begin() + (*edge_iter).first.second - 1;

				std::string qterm = std::string("{\"op\":") + '"' + op + '"' + ',' + "\"fO\": " + '"' + fO + '"' + ',' + ' ' + "\"sO\": " + '"' + sO + '"' + '}';
				localLogicalQterms.push_back(qterm);
				log_count++;
			}

		}
	}
}

int QShaper::CalcOp(std::string op, const int first, const int second)
{
	if (op == "+") return first + second;
	else if (op == "-") return first - second;
	else if (op == "*") return first * second;
	else if (op == "/") return first / second;
}

void QShaper::ReadHook(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '(')
		{
			char varb[50];
			size_t nn = str.copy(varb, i, 0);
			varb[nn] = '\0';
			
		}
	}
}


std::string QShaper::MakeQterm(std::string op, std::string first, std::string second)
{
	std::string out =  std::string("{\"op\": ")+ '"' + op + '"' +  ',' + "\"fO\": " + '"' + first + '"' + ',' + ' '+ "\"sO\": " + '"' + second + '"' + '}';
	return out;
}


std::string QShaper::NameOfHookVar(std::string var)
{
	int i = 0;
	for ( i ; i < var.length(); i++)
	{
		if (var[i] == '(') break;
	}
	if (i == var.length()) return var;
	char varb[50];
	size_t nn = var.copy(varb, i, 0);
	varb[nn] = '\0';

	return std::string(varb);
}

std::string QShaper::NameOfLocalVar(std::string var)
{
	int i = 0;
	int k = 0;
	while (k != var.length())
	{
		if (var[i] != '(') i++;
		if (var[k] != ')') k++;
		else break;
	}

	char varb[50];
	size_t nn = var.copy(varb, k-2, i+1);
	varb[nn] = '\0';
	return std::string(varb);
}

std::string QShaper::NameOfOp(std::string var)
{
	int i = 0;
	while (true)
	{
		i++;
		if (var[i] == '+' || var[i] == '-' || var[i] == '*' || var[i] == '/') break;
	}
	char varb[50];
	size_t nn = var.copy(varb, 1, i);
	varb[nn] = '\0';
	return std::string(varb);
}

void QShaper::IretateScheme()
{
	
}
void QShaper::ClearVars()
{
	 inputParametrs;
	 inputVariables;
	 localVariables;
	 localHookVariables;
     outVariables;
	 outNames;
	 derContents;
	 j_out;
	 logicalQterms;
}