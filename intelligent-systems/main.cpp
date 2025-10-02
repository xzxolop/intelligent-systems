

#include <iostream>
#include <deque>
#include <stack>
#include <chrono>
#include <unordered_map>

#include "OperationSequenceFinder.h"
#include "macros.h"


class Node {
public:
	int data;
	Node* left;
	Node* right;

	Node(int data, Node* left, Node* right) {
		this->data = data;
		this->left = left;
		this->right = right;
	}
};

void BFS(Node* root) {
	std::deque<Node*> deq;
	deq.push_back(root);

	for (int i = 0; i < deq.size(); i++) {
		Node* node = deq.front();
		deq.pop_front();

		if (node) {
			std::cout << node->data << std::endl;
			deq.push_back(node->left);
			deq.push_back(node->right);
		}
	}
}

void DFS(Node* root) {
	std::stack<Node*> stack;
	stack.push(root);

	while (!stack.empty()) {
		Node* node = stack.top();
		stack.pop();

		if (node) {
			std::cout << node->data << std::endl;
			stack.push(node->left);
			stack.push(node->right);
		}
	}
}

void binTreeTest() 
{
	Node n4{ 5, nullptr, nullptr };
	Node n3{ 4, nullptr, nullptr };
	Node n2{ 3, nullptr, nullptr};
	Node n1{ 2, &n3, &n4};

	Node root{1, &n1, &n2};

	BFS(&root);
	std::cout << std::endl;

	DFS(&root);
}


int f1(int a) {
	return a * 2;
}
int f2(int a) {
	return a + 3;
}
int f3(int a) {
	return a * 10;
}

int f1_reverse(int a) {
	return a / 2;
}

int f2_reverse(int a) {
	return a - 3;
}

int f3_reverse(int a) {
	return a / 10;
}

void OperationSequenceFinderTest() 
{
//#define DEBUG_LOG_ENABLED


	OperationSequenceFinder finder{};

	// map vs hash Для больших и маленьких значений ?
	std::vector<std::pair<std::string, std::function<int(int)>>> operations;

	operations.push_back({ "*2", f1 });
	operations.push_back({ "+3", f2 });
	//operations.push_back({ "*10", f3 });

	finder.setOperations(operations);

	//FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 1, 10090, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 1, 100, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 2, 55, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 2, 100, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 1, 97, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 2, 1000, 20);

	std::vector<std::pair<std::string, std::function<int(int)>>> reverseOperations;
	reverseOperations.push_back({ ":2", f1_reverse });
	reverseOperations.push_back({ "-3", f2_reverse });
	finder.setOperations(reverseOperations);

	std::cout << std::endl;
	std::cout << "reverse" << std::endl;
	std::cout << std::endl;

	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 100, 1, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 55, 2, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 100, 2, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 97, 1, 20);
	FINDER_TIME_TEST_MCS(finder.findSequenceBFS, 1000, 2, 20);


	finder.findSequenceBidir(100, 1, 20);

}

void bidirectionalFinder() 
{
	OperationSequenceFinder finder{};

	std::vector<std::pair<std::string, std::function<int(int)>>> operations;
	operations.push_back({ "*2", f1 });
	operations.push_back({ "+3", f2 });
	finder.setOperations(operations);

	std::vector<std::pair<std::string, std::function<int(int)>>> reverseOperations;
	reverseOperations.push_back({ ":2", f1_reverse });
	reverseOperations.push_back({ "-3", f2_reverse });
	finder.setReverseOperations(reverseOperations);

	finder.findSequenceBidir(1, 10);

	
}

int main() 
{
	//OperationSequenceFinderTest();
	bidirectionalFinder();


	return 0;
}