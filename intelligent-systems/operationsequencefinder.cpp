#include "OperationSequenceFinder.h"

OperationSequenceFinder::OperationSequenceFinder(ValueType source, ValueType target)
{
	this->source = source;
	this->target = target;

	NodeType root{ source, -1 };
	deq.push_back(root);
	rootPointer = &deq[0];
	createNode(&deq[0]);

	bool isCheck = checkNode();

	if (isCheck) {
		std::cout << "find" << std::endl;
	}
	else {
		std::cout << "not find" << std::endl;
	}

	
}
