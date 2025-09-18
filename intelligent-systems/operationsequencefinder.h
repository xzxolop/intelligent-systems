#pragma once

#include <deque>
#include <iostream>
#include <vector>
#include <cmath>
#include <set>

class OperationSequenceFinder
{
    // NOTE: לב גûםוסעט ‎עמ ג מעהוכüםûי פאיכ
    template<typename T>
    class Node {
    public:
        T data;
        unsigned short depth;

        T getData() const noexcept {
            return data;
        }

        Node(T data, unsigned short depth) {
            this->data = data;
            this->depth = depth;
        };
    };

    using ValueType = int;
    using NodeType = Node<ValueType>;
    using Container = std::deque<NodeType*>; // what bad in poiners?
    
    unsigned short countOperations = 2;
    unsigned short searchDepth = 20;
    size_t treeSize;

    std::set<unsigned short> currentDepth;
    size_t setSize;

public:
    NodeType* rootPointer;

    OperationSequenceFinder() 
        : currentElem{nullptr}
        , rootPointer{nullptr}
        , source{0}
        , target{0}
        , treeSize{0}
        , setSize{ currentDepth.size() }
    {}

    ~OperationSequenceFinder() {
        clear();
    }

    int findSequence(ValueType source, ValueType target, unsigned short searchDepth = 20) {
        this->source = source;
        this->target = target;

        NodeType* root = new NodeType(source, 0);
        deq.push_back(root);
        rootPointer = root;
        currentElem = root;

        if (countOperations < 2) {
            treeSize = searchDepth;
        }
        else {
            treeSize = pow(countOperations, searchDepth);
        }

        std::cout << "source: " << source << " target: " << target << " size: " << treeSize << std::endl;

        int deepth = BfsFinder(rootPointer);
        clear();

        std::cout << "deq size: " << deq.size() << std::endl;
        return deepth;
    }

    int BfsFinder(NodeType* node)
    {
        bool isFind = checkNode(node);

        if (isFind) {
            return 0;
        }

        //for (int i = 0; i < treeSize; i++) {
        while(!deq.empty()) {
            NodeType* node = deq.front(); //= deq.at(i);
            deq.pop_front();
            if (node) {
                isFind = checkNode(node);

                if (isFind) {
                    return node->depth; //calcDeep(node);
                }

                currentDepth.insert(node->depth + 1);
                if (setSize != currentDepth.size()) {
                    std::cout << "Depth is " << currentDepth.size() << ", deque size: " << deq.size() << "; " << std::endl;
                    setSize = currentDepth.size();
                }

                createNode(node, node->depth + 1);

                delete node;
            }
            
        }
    }

    void createNode(NodeType* parent, unsigned short depth)
    {
        if (!parent) {
            return;
        }

        NodeType* left = new NodeType{ parent->data + 3, depth };
        NodeType* right = new NodeType{ parent->data * 2, depth };

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
            for (int i = deq.size() - countOperations - 1; i < deq.size(); i++) {
                vec.push_back(deq[i]);
            }
        }

        if (deq.size() == 1) {
            vec.push_back(deq[0]);
        }
        return vec;
    }

    void clear() {
        for (NodeType* ptr : deq) {
            delete ptr;
        }
        deq.clear();
        currentDepth.clear();
        setSize = 0;
    }

private:
    Container deq;

    ValueType source;
    ValueType target;

    NodeType* currentElem;
};

