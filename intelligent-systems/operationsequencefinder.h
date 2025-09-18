#pragma once

#include <deque>
#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <stack>

class OperationSequenceFinder
{
    // NOTE: move to different file ?
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
    
    unsigned short countOperations = 2;
    unsigned short searchDepth = 0;
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

    // В ширину
    int findSequenceBFS(ValueType source, ValueType target, unsigned short searchDepth = 20) {
        this->source = source;
        this->target = target;
        this->searchDepth = searchDepth;

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

        int deepth = BFSFinder(rootPointer);
        clear();

        std::cout << "deq size: " << deq.size() << std::endl;
        return deepth;
    }

    int BFSFinder(NodeType* node)
    {
        bool isFind = checkNode(node);

        if (isFind) {
            return 0;
        }

        while(!deq.empty()) {
            NodeType* node = deq.front();
            deq.pop_front();
            if (node) {
                isFind = checkNode(node);

                if (isFind) {
                    return node->depth;
                }

                currentDepth.insert(node->depth + 1);
                if (setSize != currentDepth.size()) {
                    std::cout << "Depth is " << currentDepth.size() << ", deque size: " << deq.size() << "; " << std::endl;
                    setSize = currentDepth.size();
                }

                if (node->depth + 1 < searchDepth) {
                    createNodeDeq(node, node->depth + 1);
                }

                delete node;
            }
            
        }

        std::cout << "not find" << std::endl;
        return -1;
    }

    // В глубину
    int findSequenceDFS(ValueType source, ValueType target, unsigned short searchDepth = 20) {
        this->source = source;
        this->target = target;
        this->searchDepth = searchDepth;

        NodeType* root = new NodeType(source, 0);
        stack.push(root);
        rootPointer = root;
        currentElem = root;

        if (countOperations < 2) {
            treeSize = searchDepth;
        }
        else {
            treeSize = pow(countOperations, searchDepth);
        }

        std::cout << "source: " << source << " target: " << target << " size: " << treeSize << std::endl;

        int deepth = DFSFinder(rootPointer);
        clear();

        return deepth;
    }

    int DFSFinder(NodeType* node) 
    {
        int max = 0;

        bool isFind = checkNode(node);

        if (isFind) {
            return 0;
        }

        while (!stack.empty()) {
            NodeType* node = stack.top();
            stack.pop();

            if (node) {
                /*if (node->data < 10000001 + 1000 && node->data > 10000001 - 1000) {
                    std::cout << node->data << std::endl;
                }*/

                /*if (node->data > max) {
                    max = node->data;
                    std::cout << "max: " << max << " " << node->depth << std::endl;
                }*/

                isFind = checkNode(node);
               //std::cout << "check: " << node->data << std::endl;

                if (isFind) {
                    return node->depth;
                }
                currentDepth.insert(node->depth + 1);
                if (setSize != currentDepth.size()) {
                    //std::cout << "data: " << node->data << " Depth is " << currentDepth.size() << ", stack size: " << stack.size() << "; " << std::endl;
                    setSize = currentDepth.size();
                }

                //std::cout << "n->d + 1:" << node->depth + 1 << searchDepth << std::endl;
                if (node->depth + 1 < searchDepth) {
                    createNodeStack(node, node->depth + 1);
                }
                delete node;
            }
        }

        std::cout << "not find" << std::endl;
        return -1;
    }

    void createNodeDeq(NodeType* parent, unsigned short depth)
    {
        if (!parent) {
            return;
        }

        NodeType* left = new NodeType{ parent->data + 3, depth };
        NodeType* right = new NodeType{ parent->data * 2, depth };

        deq.push_back(left);
        deq.push_back(right);
    }

    void createNodeStack(NodeType* parent, unsigned short depth) {
        if (!parent) {
            return;
        }
        NodeType* left = new NodeType{ parent->data + 3, depth };
        NodeType* right = new NodeType{ parent->data * 2, depth };

        stack.push(left);
        stack.push(right);
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
        

        while (!stack.empty()) {
            NodeType* ptr = stack.top();
            delete ptr;
            stack.pop();
        }

        currentDepth.clear();
        setSize = 0;
    }

private:
    std::deque<NodeType*> deq;
    std::stack<NodeType*> stack;

    ValueType source;
    ValueType target;

    NodeType* currentElem;
};

