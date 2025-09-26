//#define DEBUG_LOG_DISABLED

#include <iostream>
#include <deque>
#include <stack>
#include <chrono>

#include "OperationSequenceFinder.h"


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
	/*Node n4{ 5, nullptr, nullptr };
Node n3{ 4, nullptr, nullptr };
Node n2{ 3, nullptr, nullptr};
Node n1{ 2, &n3, &n4};

Node root{1, &n1, &n2};

BFS(&root);
std::cout << std::endl;

DFS(&root);*/
}


int f1(int a) {
	return a * 2;
}
int f2(int a) {
	return a + 3;
}
int f3(int a) {
	return a * 1000;
}

void OperationSequenceFinderTest() 
{
	OperationSequenceFinder finder{};

	std::vector<std::function<int(int)>> operations;
	operations.push_back(f1);
	operations.push_back(f2);
	operations.push_back(f3);

	finder.setOperations(operations);

	auto start = std::chrono::high_resolution_clock::now();
	//auto res = finder.findSequenceDFS(2, 100, 20);
	//res.print();
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() << " milisec" << std::endl;


	start = std::chrono::high_resolution_clock::now();
	auto res = finder.findSequenceBFS(1, 1000000, 10);
	res.print();
	end = std::chrono::high_resolution_clock::now();
	std::cout << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() << " milisec" << std::endl;

	/*deep = finder.findSequence(2, 55);
	std::cout << "deep: " << deep << std::endl << std::endl;;

	deep = finder.findSequence(2, 100);
	std::cout << "deep: " << deep << std::endl << std::endl;;

	deep = finder.findSequence(1, 97);
	std::cout << "deep: " << deep << std::endl << std::endl;;

	deep = finder.findSequence(2, 1000);
	std::cout << "deep: " << deep << std::endl << std::endl;;

	deep = finder.findSequence(2, 10000001, 32);
	std::cout << "deep: " << deep << std::endl << std::endl;;*/
}

int main() {

	OperationSequenceFinderTest();


	return 0;
}