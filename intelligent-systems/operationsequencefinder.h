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
        Node* parent;

        T getData() const noexcept {
            return data;
        }

        Node(T data, Node* parent) {
            this->data = data;
            this->parent = parent;
        };
    };

    using ValueType = int;
    using NodeType = Node<ValueType>;
    using Container = std::deque<NodeType*>; // what bad in poiners?
    
    int CountOperations = 2;

public:
    NodeType* rootPointer;

    OperationSequenceFinder(ValueType source, ValueType target) {
        {
            this->source = source;
            this->target = target;

            NodeType* root = new NodeType(source, nullptr);           
            deq.push_back(root);
            rootPointer = root;
            currentElem = root;
        }
    }

    int findSequence(NodeType* node)
    {
        bool isFind = checkNode(node);

        if (isFind) {
            std::cout << "find" << std::endl;
            return 1;
        }

        for (int i = 0; i < 500000; i++) {
            NodeType* node = deq.at(i);
            if (node) {
                //std::cout << node->data << std::endl;

                isFind = checkNode(node);

                if (isFind) {
                    std::cout << "find" << std::endl;
                    return calcDeep(node);
                }
            }

            createNode(node);
        }
    }

    int calcDeep(NodeType* node) {
        if (!node->parent) {
            return 0;
        }

        int cnt = 0;
        while (node->parent != nullptr) {
            node = node->parent;
            cnt++;
        }

        return cnt;

    }

    void createNode(NodeType* parent)
    {
        if (!parent) {
            return;
        }

        NodeType* left = new NodeType{ parent->data + 3, parent};
        NodeType* right = new NodeType{ parent->data * 2, parent };

        deq.push_back(left);
        deq.push_back(right);
    }

    bool checkNode(const NodeType* node) {
        if (node->data == target) {
            return true;
        }
        else return false;
    }

    bool checkLastTwoNodes()
    {
        auto lastNodes = getLastNodes();
        for (int i = 0; i < lastNodes.size(); i++) {
            auto node = lastNodes.at(i);
            std::cout << "data: " << node->data << std::endl;
            if (lastNodes.at(i)->data == target) {
                std::cout << "find!" << std::endl;
                return true;
            }
        }

        return false;
    }

    std::vector<NodeType*> getLastNodes() {
        std::vector<NodeType*> vec;
       

        if (deq.size() >= 2) {
            for (int i = deq.size() - CountOperations - 1; i < deq.size(); i++) {
                vec.push_back(deq[i]);
            }
        }

        if (deq.size() == 1) {
            vec.push_back(deq[0]);
        }
        return vec;
    }

private:
    Container deq;

    ValueType source;
    ValueType target;

    NodeType* currentElem;
};

