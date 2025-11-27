#pragma once
#include <unordered_map>
#include <iostream>
#include <list>

template <typename NodeName>
class Graph
{
	using ListWeightsType = std::list<std::pair<NodeName, int>>;

public:
	void addToGraph(NodeName start, NodeName end, int weight)
	{
		if (start == NodeName{} || end == NodeName{}) {
			throw "You can't add to graph a default value!";
		}

		ListWeightsType weights{ {end, weight} };
		graph.insert({start, weights});
	}

	void addToGraph(NodeName start)
	{
		ListWeightsType weights{};
		std::pair<NodeName, ListWeightsType> pair{ start, weights };
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


	void deikstra(NodeName start, NodeName end) {
		std::unordered_map<NodeName, int> weights;
		std::unordered_map<NodeName, NodeName> parents;

		for (auto x : graph) { // WARNING: это надо будет оптимизировать
			if (x.first != start) {
				parents.insert({ x.first, NodeName{} });
				weights.insert({ x.first, INT_MAX });
			}
		}

		auto list = graph.at(start);
		for (auto x : list) {
			weights.at(x.first);
		}

	}

private:
	std::unordered_map<NodeName, ListWeightsType> graph;

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

