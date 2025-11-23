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

	void addToGraph(NodeName from)
	{
		ListWeightsType weights{};
		std::pair<NodeName, ListWeightsType> pair{ from, weights };
		graph.insert(pair);
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


	void deikstra(NodeName from, NodeName to) {
		std::unordered_map<NodeName, int> weights;
		std::unordered_map<NodeName, NodeName> parents;

		for (auto x : graph) {
			weights.insert( {x.first, INT_MAX} );
		}

		auto toList = getTo(from);
		auto min = getMin(toList);
		std::cout << min.first << " " << min.second;


	}

private:

	std::unordered_map<NodeName, ListWeightsType> graph;

	ListWeightsType getTo(NodeName node) {
		auto from_ptr = graph.find(node);
		return from_ptr->second;
	}

	std::pair<NodeName, int> getMin(const ListWeightsType& weights) {
		int minWeight = INT_MAX;
		NodeName minWeightName = "";

		for (auto x : weights) {
			if (x.second < minWeight) {
				minWeight = x.second;
				minWeightName = x.first;
			}
		}

		return { minWeightName, minWeight  };
	}
};

