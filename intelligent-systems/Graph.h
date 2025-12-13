#pragma once

#include <unordered_map>
#include <set>

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

		if (!graph.contains(start)) {
			ListWeightsType weights{ {end, weight} };
			graph.insert({ start, weights });
		}
		else {
			graph[start].push_back({ end, weight });
		}
		
	}

	void addToGraph(NodeName start)
	{
		ListWeightsType weights{};
		graph.insert({ start, weights });
	}

	void printGraph() {
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

	int deikstra(NodeName start, NodeName end) {
		
		// Заполнение таблицы весов и родителей перед началом работы.
		for (auto x : graph) { // WARNING: это надо будет оптимизировать
			if (x.first != start) {
				parents.insert({ x.first, NodeName{} });
				weights.insert({ x.first, INT_MAX });
			}
		}

		std::list<std::pair<NodeName, int>> list = graph.at(start);
		for (auto x : list) {
			weights[x.first] = x.second;
		}

		printWeights();
		printParents();

		auto node = findLowestCostNode();
		while (node.first != NodeName{}) {
			int weight = weights.at(node.first);
			auto neighbors = graph.at(node.first);
			for (auto x: neighbors) {
				int newWight = weight + x.second;
				if (weights.at(x.first) > newWight) {
					weights[x.first] = newWight;
					parents[x.first] = node.first;
				}
			}
			visited.insert(node.first);
			
			node = findLowestCostNode();


			//std::cout << node.first << " " << node.second << std::endl;
		}
		if (weights.contains(end)) {
			return weights.at(end);
		}
		else {
			return INT_MAX;
		}

		return node.second;
	}

private:
	std::unordered_map<NodeName, ListWeightsType> graph;
	std::unordered_map<NodeName, int> weights;
	std::unordered_map<NodeName, NodeName> parents;

	std::set<NodeName> visited;

	std::pair<NodeName, int> findLowestCostNode() {
		int minWeight = INT_MAX;
		NodeName minWeightName;

		for (auto x : weights) {
			if (!visited.contains(x.first)) {
				if (x.second < minWeight) {
					minWeight = x.second;
					minWeightName = x.first;
				}
			}
		}

		return { minWeightName, minWeight  };
	}

	void printWeights() {
		for (auto x : weights) {
			std::cout << x.first << " " << x.second << std::endl;
		}
	}

	void printParents() {
		for (auto x : parents) {
			std::cout << x.first << " " << x.second << std::endl;
		}
	}

};

