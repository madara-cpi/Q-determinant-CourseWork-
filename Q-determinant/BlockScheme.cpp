#include "BlockScheme.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>


SchemeReader::~SchemeReader()
{
	this->Vertices.clear();
	this->Edges.clear();
}

void SchemeReader::SerializeToJson(const std::string& filename, const std::string& qdet)
{
	std::fstream file;
	file.open(filename);
	if(file.is_open())
	{
		file << qdet;
	}
	else
	{
	std::cout << "Не удалось открыть файл";
	return;
	}

}
void SchemeReader::DeserializeFromJson(const std::string& filename)
{
	std::fstream file;
	file.open(filename);
	nlohmann::json jobj;
	if (file.is_open())
	{
		file >> jobj;
	}
	else
	{
		std::cout << "Не удалось открыть файл";
		return;
	}
	for (int i = 0; i < jobj.at("Vertices").size(); i++)
	{
	
		Vertice v{ jobj["Vertices"][i]["Id"], jobj["Vertices"][i]["Type"], jobj["Vertices"][i]["Content"] };
		Vertices.push_back(v);
	}

	for (int i = 0; i < jobj.at("Edges").size(); i++)
	{
		std::pair<int, int> p1(jobj["Edges"][i]["From"], jobj["Edges"][i]["To"]);
		std::pair<std::pair<int, int>, int> p2(p1, jobj["Edges"][i]["Type"]);
		Edges.push_back(p2);
	
	}
	jobj.clear();
	file.close();	
}

void SchemeReader::PrintScheme()
{
	
	auto iter1 = Vertices.begin();
	while (iter1 != Vertices.end())
	{
		Vertice v = *iter1;
		std::cout << "Id: " << v.id << " Type: " << v.type << " Content: " << v.content << std::endl;
		++iter1;
	}


    auto iter2 = Edges.begin();
	while (iter2 != Edges.end())
	{

		int a = 0, b = 0, w = 1;
		std::pair<std::pair<int, int>, int> p = *iter2;
		std::pair<int, int> p1 = p.first;
		a = p1.first; b = p1.second; w = p.second;
		std::cout <<"From: "<< a << " To: " << b << " ";
		std::cout << "Type: "<< w;
		std::cout << std::endl;
		++iter2;

	}
}
