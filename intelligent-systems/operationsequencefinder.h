#pragma once

#include <deque>
#include <iostream>
#include <vector>

class OperationSequenceFinder
{
    // NOTE: לב גûםוסעט ‎עמ ג מעהוכüםûי פאיכ
    template<typename T>
    class Node {
    public:
        T data;
        Node* prev;

        Node(T data, Node* prev) {
            this->data = data;
            this->prev = prev;
        };
    };

    using ValueType = int;
    using NodeType = Node<ValueType>;
    using Container = std::deque<NodeType>;
    
    int CountOperations = 2;

public:
    OperationSequenceFinder(ValueType source, ValueType target);

    int findSequence()
    {
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

    void createNode(NodeType* parent)
    {
        if (!parent) {
            return;
        }

        NodeType left{ parent->data + 3, parent };
        NodeType right{ parent->data * 2, parent };

        deq.push_back(left);
        deq.push_back(right);
    }

    bool checkNode()
    {
        auto lastNodes = getLastNodes();
        for (int i = 0; i < lastNodes.size(); i++) {
            std::cout << lastNodes.at(i).data << std::endl;
            if (lastNodes.at(i).data == target) {
                std::cout << "find!" << std::endl;
                return true;
            }
        }

        std::cout << "target node is not find" << std::endl;

        return false;
    }

    std::vector<NodeType> getLastNodes() {
        std::vector<NodeType> vec;
        for (int i = deq.size() - CountOperations - 1; i < deq.size(); i++) {
            vec.push_back(deq[i]);
        }
        return vec;
    }

    /*NodeType getLast() {
       return deq.back();
    }

    NodeType getPreLast() {
        return deq[deq.size() - 2];
    }*/



private:
    Container deq;

    ValueType source;
    ValueType target;
};

