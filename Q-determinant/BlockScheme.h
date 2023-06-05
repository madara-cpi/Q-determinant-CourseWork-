#pragma once
#include <string>
#include <vector>
#include "Dependencies/include/nlohmann/json.hpp"

struct Vertice
{
	unsigned int id = 0;
	unsigned int type = 0;
	std::string content = " "; //?
};

using edges = std::vector<std::pair<std::pair<int, int>, int> >;
using vertices = std::vector<Vertice>;



class SchemeReader
{
public:
	SchemeReader() = default;
	~SchemeReader();
	edges Edges;
	vertices Vertices;
	void SerializeToJson(const std::string& filename, const std::string& qdet);
	void DeserializeFromJson(const std::string& filename);

	void PrintScheme();


};

