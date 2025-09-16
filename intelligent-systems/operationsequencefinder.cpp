#include "OperationSequenceFinder.h"

OperationSequenceFinder::OperationSequenceFinder(ValueType source, ValueType target)
{
	this->source = source;
	this->target = target;

	NodeType root{ source, nullptr };
	deq.push_back(root);
	createNode(&root);
	bool isCheck = checkNode();

	if (isCheck) {
		std::cout << "find" << std::endl;
	}
	else {
		std::cout << "not find" << std::endl;
	}

	
}
