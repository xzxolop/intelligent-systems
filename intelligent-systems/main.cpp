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

	while (!deq.empty()) {
		for (int i = 0; i < deq.size(); i++) {
			Node* node = deq.front();
			deq.pop_front();

			if (node) {
				std::cout << node->data << std::endl;

			}
			deq.push_back(node->left);
			deq.push_back(node->right);

		}
	}

}

int main() {
	/*Node n2{ 3, nullptr, nullptr};
	Node n1{ 2, nullptr, nullptr};

	Node root{1, &n1, &n2};

	BFS(&root);*/

	OperationSequenceFinder finder{ 2, 100 };
	finder.findSequence(finder.rootPointer);

	return 0;
}