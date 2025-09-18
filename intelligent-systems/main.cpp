#include <iostream>
#include <deque>

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

	//while (!deq.empty()) {
		for (int i = 0; i < deq.size(); i++) {
			Node* node = deq.front();
			deq.pop_front();

			if (node) {
				std::cout << node->data << std::endl;
				deq.push_back(node->left);
				deq.push_back(node->right);
			}
		}
	//}

}

int main() {
	/*Node n4{ 5, nullptr, nullptr };
	Node n3{ 4, nullptr, nullptr };
	Node n2{ 3, nullptr, nullptr};
	Node n1{ 2, &n3, &n4};

	Node root{1, &n1, &n2};

	BFS(&root);*/

	OperationSequenceFinder finder{};
	int deep = finder.findSequence(1, 100);
	std::cout << "deep: " << deep << std::endl;

	deep = finder.findSequence(2, 55);
	std::cout << "deep: " << deep << std::endl;

	deep = finder.findSequence(2, 100);
	std::cout << "deep: " << deep << std::endl;

	deep = finder.findSequence(1, 97);
	std::cout << "deep: " << deep << std::endl;

	deep = finder.findSequence(2, 1000);
	std::cout << "deep: " << deep << std::endl;

	deep = finder.findSequence(2, 10000001, 32);
	std::cout << "deep: " << deep << std::endl;

	return 0;
}