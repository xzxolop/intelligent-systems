#pragma once
#include <unordered_map>
#include <iostream>
#include <list>

template <typename NodeName>
class Graph
{
	using ListWeightsType = std::list<std::pair<NodeName, int>>;
public:
	void addToGraph(NodeName from, NodeName to, int weight) 
	{
		auto from_pointer = graph.find(from);
		if (from_pointer == graph.end()) {
			ListWeightsType weights{ {to, weight} };
			std::pair<NodeName, ListWeightsType> pair{ from, weights };
			graph.insert(pair);
		}
		else {
			from_pointer->second.push_back(std::pair<NodeName, int>{to, weight});
		}
	}

	void print() {
		for (auto x : graph) {
			auto weights = x.second;
			std::cout << x.first << ": ";

			for (auto w : weights) {
				std::cout << w.first << " " << w.second << ", ";
			}

			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

private:
	std::unordered_map<NodeName, ListWeightsType> graph;
};

