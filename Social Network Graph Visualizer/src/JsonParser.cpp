#include "JsonParser.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include "GraphicsEngine.h"
#include "Actors/Graph.h"

using json = nlohmann::json;

void ParseJson(GraphicsEngine* graphics, const char* jsonPath)
{
	std::ifstream file;
	file.open(jsonPath, std::ios::in);

	json j;

	try
	{
		j = json::parse(file);
	}
	catch (json::parse_error& ex)
	{
		std::cerr << "Error (JsonParser): Parse error at byte " << ex.byte << "\n";
	}

	Graph* graph = graphics->GetGraph();

	// Retrieve Data and create Users
	for (auto& user : j.find("users").value())
	{
		graph->InsertNode(user["id"], user["name"]);
	}

	// Retrieve Data and create Relations
	for (auto& relation : j.find("relations").value())
	{
		graph->AddRelation(relation[0], relation[1]);
	}
}