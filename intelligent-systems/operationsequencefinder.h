#pragma once

#include <deque>
#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <stack>
#include <unordered_set>
#include <type_traits>
#include <functional>
#include <string>

#include "macros.h"

#define OPERATIONS_SEQUENCE_ENABLED

class OperationSequenceFinder
{
    // NOTE: move to different file ?
    template<typename T>
    class Node {
    public:
        T data;
        unsigned short depth;

#ifdef OPERATIONS_SEQUENCE_ENABLED
        Node* prev;
        char operationNumber;
#endif

        // стоит ли хранить массив операций?

        T getData() const noexcept {
            return data;
        }

#ifdef OPERATIONS_SEQUENCE_ENABLED
        Node(T data, unsigned short depth, Node* prev, short opNumb)
            : data(data)
            , depth(depth)
            , prev(prev)
            , operationNumber(opNumb)

        {};
#else
        Node(T data, unsigned short depth)
            : data(data)
            , depth(depth)
        {};
#endif 
    };

    using ValueType = int;
    using NodeType = Node<ValueType>;

public:
    NodeType* rootPointer;

    class FinderResult {
        bool _isFind;
        int _depth;
        size_t _containerSize;
        unsigned long long _memoryUsed;
        std::vector<std::pair<std::string, std::function<int(int)>>> _operations;

#ifdef OPERATIONS_SEQUENCE_ENABLED
        std::string operationSequence;
#endif  
        // char будет экономней ?
        const char* _isFindString() const {
            if (_isFind) {
                return "true";
            }
            return "false";
        }

    public: 

        FinderResult(int isFind, NodeType* node, size_t contSize, 
            const std::vector<std::pair<std::string, std::function<int(int)>>>& operations)
            : _isFind(isFind)
            , _depth(node->depth)
            , _containerSize(contSize)
            // использовано памяти = размер указателя * размер ноды * кол-во
            , _memoryUsed(8 * sizeof(NodeType) * _containerSize)

#ifdef OPERATIONS_SEQUENCE_ENABLED
            , operationSequence("")
            , _operations(operations)
#endif
        {
#ifdef OPERATIONS_SEQUENCE_ENABLED
            if (isFind) {
                std::stack<NodeType*> stack;
                stack.push(node);
                while (!stack.empty()) {
                    NodeType* node = stack.top();
                    stack.pop();

                    int opNumb = node->operationNumber;
                    if (opNumb >= 0 && opNumb < _operations.size()) {
                        std::string operation = _operations.at(opNumb).first;
                        operationSequence += operation += " ";
                        operationSequence += " ";
                    }

                    //rename prev to parent
                    if (node->prev) {
                        stack.push(node->prev);
                    }
                }
            }
#endif
        }

        bool isFind() const {
            return _isFind;
        }

        int getDepth() const {
            return _depth;
        }

        unsigned long long getUsedMemory() const {
            return _memoryUsed;
        }

        void print() const {
            std::cout << "isFind: " << _isFindString() << " depth: " << _depth
                << " containerSize: " << _containerSize << " memoryUsed in byte: " << _memoryUsed 
#ifdef OPERATIONS_SEQUENCE_ENABLED
                << " opSeq: " << operationSequence 
#endif
                << std::endl;
        }
    };

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

    void setOperations(const std::vector<std::pair<std::string, std::function<int(int)>>>& operations)
    {
        _operations = operations;
    };

    // В ширину
    FinderResult findSequenceBFS(ValueType source, ValueType target, unsigned short searchDepth = 20) {
        this->source = source;
        this->target = target;
        this->searchDepth = searchDepth;

#ifdef OPERATIONS_SEQUENCE_ENABLED
        NodeType* root = new NodeType(source, 0, nullptr, -1);
#else
        NodeType* root = new NodeType(source, 0);
#endif
        deq.push_back(root);
        rootPointer = root;
        currentElem = root;

        if (countOperations < 2) {
            treeSize = searchDepth;
        }
        else {
            treeSize = pow(countOperations, searchDepth);
        }

        DEBUG_LOG("source: " << source << " target: " << target << " size: " << treeSize);

        FinderResult res = BFSFinder(rootPointer);
        clear();

        return res;
    }

    FinderResult BFSFinder(NodeType* node)
    {
        bool isFind = checkNode(node);

        if (isFind) {
            return FinderResult{ true, node, deq.size() , _operations };
        }

        while(!deq.empty()) {
            NodeType* node = deq.front();
            deq.pop_front();
            if (node) {
                isFind = checkNode(node);

                if (isFind) {
                    return FinderResult{ true, node, deq.size(), _operations };
                }

                currentDepth.insert(node->depth + 1);
                if (setSize != currentDepth.size()) {
                    DEBUG_LOG("Depth is " << currentDepth.size() << ", deque size: " << deq.size() << "; ");
                    setSize = currentDepth.size();
                }

                if (node->depth + 1 < searchDepth) {
                    createNodeDeq(node);
                }

                //delete node;
            }
        }

        return FinderResult{ false, node, deq.size(), _operations };;
    }

    // В глубину
    FinderResult findSequenceDFS(ValueType source, ValueType target, unsigned short searchDepth = 20) {
        this->source = source;
        this->target = target;
        this->searchDepth = searchDepth;

#ifdef OPERATIONS_SEQUENCE_ENABLED
        NodeType* root = new NodeType(source, 0, nullptr, -1);
#else
        NodeType* root = new NodeType(source, 0);
#endif
        stack.push(root);
        rootPointer = root;
        currentElem = root;

        if (countOperations < 2) {
            treeSize = searchDepth;
        }
        else {
            treeSize = pow(countOperations, searchDepth);
        }

        DEBUG_LOG("source: " << source << " target: " << target << " size: " << treeSize);

        FinderResult res = DFSFinder(rootPointer);
        clear();

        return res;
    }

    FinderResult DFSFinder(NodeType* node) 
    {
        int max = 0;

        bool isFind = checkNode(node);

        if (isFind) {
            return FinderResult{ true, node, stack.size() , _operations };
        }

        while (!stack.empty()) {
            NodeType* node = stack.top();
            stack.pop();

            if (node) {
                isFind = checkNode(node);

                if (isFind) {
                    return FinderResult{ true, node, stack.size(), _operations };
                }
                currentDepth.insert(node->depth + 1);
                if (setSize != currentDepth.size()) {  
                    setSize = currentDepth.size();
                }

                if (node->depth + 1 < searchDepth) {
                    createNodeStack(node, node->depth + 1);
                }
                delete node;
            }
        }

        return FinderResult{ false, node, stack.size(), _operations };
    }

    void createNodeDeq(NodeType* parent)
    {
        if (!parent) {
            return;
        }


        for (short i = 0; i < _operations.size(); i++) {
            auto func = _operations.at(i).second;
            ValueType value = func(parent->data);
            if (visitedValues.find(value) == visitedValues.end()) {
                visitedValues.insert(value);

                unsigned short newDepth = parent->depth + 1;
#ifdef OPERATIONS_SEQUENCE_ENABLED
                NodeType* node = new NodeType{ value, newDepth, parent, i};
                
#else
                NodeType* node = new NodeType{ value, newDepth};
#endif
                deq.push_back(node);
            }
        }
    }

    void createNodeStack(NodeType* parent, unsigned short depth) {
        if (!parent) {
            return;
        }

        for (short i = 0; i < _operations.size(); i++) {

            auto func = _operations.at(i).second;
            ValueType value = func(parent->data);
            if (visitedValues.find(value) == visitedValues.end()) {
                visitedValues.insert(value);
                DEBUG_LOG(value);
                unsigned short newDepth = parent->depth + 1;
#ifdef OPERATIONS_SEQUENCE_ENABLED
                NodeType* node = new NodeType{ value, newDepth, parent, i };
#else
                NodeType* node = new NodeType{ value, newDepth };
#endif
                stack.push(node);
            }
        }
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
            if (lastNodes.at(i)->data == target) {
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

        visitedValues.clear();
        currentDepth.clear();
        setSize = 0;
    }

private:
    unsigned short countOperations = 2;
    unsigned short searchDepth = 0;
    size_t treeSize;
    size_t setSize;
    
    std::set<unsigned short> currentDepth;
    std::unordered_set<ValueType> visitedValues;
    std::vector<std::pair<std::string, std::function<int(int)>>> _operations;

    // Без указателя будет переполнение стека?
    std::deque<NodeType*> deq;
    std::stack<NodeType*> stack;

    ValueType source;
    ValueType target;

    NodeType* currentElem;
};

