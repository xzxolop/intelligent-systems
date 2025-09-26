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

int main() {
	/*Node n4{ 5, nullptr, nullptr };
	Node n3{ 4, nullptr, nullptr };
	Node n2{ 3, nullptr, nullptr};
	Node n1{ 2, &n3, &n4};

	Node root{1, &n1, &n2};

	BFS(&root);
	std::cout << std::endl;

	DFS(&root);*/

	OperationSequenceFinder finder{};

	auto start = std::chrono::high_resolution_clock::now();
	auto res = finder.findSequenceDFS(2, 100, 20);
	res.print();
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() << " milisec" << std::endl;


	start = std::chrono::high_resolution_clock::now();
	res = finder.findSequenceBFS(2, 10000001, 31);
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

	return 0;
}