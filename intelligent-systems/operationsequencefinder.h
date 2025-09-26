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

#include "macros.h"

class OperationSequenceFinder
{
    // NOTE: move to different file ?
    template<typename T>
    class Node {
    public:
        T data;
        unsigned short depth;

        // стоит ли хранить массив операций?

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
    std::unordered_set<ValueType> visitedValues;
    size_t setSize;

    //using Container = std::deque<NodeType*>;
    //Container cont;

    // NOTE: на макросах скорость будет чуть больше чем на if. Мб выход из ситуации constexpr if.
//#define IS_DEQUE(Container) std::is_same_v<Container, std::deque<NodeType*>>
//#define IS_STACK(Container) std::is_same_v<Container, std::stack<NodeType*>>

public:
    NodeType* rootPointer;

    class FinderResult {
        bool _isFind;
        int _depth;
        size_t _containerSize;
        unsigned long long _memoryUsed;

        // char будет экономней ?
        const char* _isFindString() const {
            if (_isFind) {
                return "true";
            }
            return "false";
        }

    public: 
        FinderResult(int isFind, int depth, size_t contSize) 
            : _isFind(isFind)
            , _depth(depth)
            , _containerSize(contSize)
            // размер указателя * размер ноды * кол-во
            , _memoryUsed(8 * sizeof(NodeType) * _containerSize)
        {}

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
                << " containerSize: " << _containerSize << " memoryUsed in byte: " << _memoryUsed <<  std::endl;
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

    void setOperations(const std::vector<std::function<int(int)>>& operations) 
    {
        _operations = operations;
    };

    // В ширину
    FinderResult findSequenceBFS(ValueType source, ValueType target, unsigned short searchDepth = 20) {
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

        DEBUG_LOG("source: " << source << " target: " << target << " size: " << treeSize);

        FinderResult res = BFSFinder(rootPointer);
        clear();

        return res;
    }

    FinderResult BFSFinder(NodeType* node)
    {
        bool isFind = checkNode(node);

        if (isFind) {
            return FinderResult{ true, node->depth, deq.size()};
        }

        while(!deq.empty()) {
            NodeType* node = deq.front();
            deq.pop_front();
            if (node) {
                isFind = checkNode(node);

                if (isFind) {
                    return FinderResult{ true, node->depth, deq.size() };
                }

                currentDepth.insert(node->depth + 1);
                if (setSize != currentDepth.size()) {
                    DEBUG_LOG("Depth is " << currentDepth.size() << ", deque size: " << deq.size() << "; ");
                    setSize = currentDepth.size();
                }

                if (node->depth + 1 < searchDepth) {
                    createNodeDeq(node, node->depth + 1);
                }

                delete node;
            }
        }

        return FinderResult{ false, node->depth, deq.size() };;
    }

    // В глубину
    FinderResult findSequenceDFS(ValueType source, ValueType target, unsigned short searchDepth = 20) {
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
            FinderResult{ true, node->depth, stack.size() };
        }

        while (!stack.empty()) {
            NodeType* node = stack.top();
            stack.pop();

            if (node) {
                isFind = checkNode(node);

                if (isFind) {
                    return FinderResult{ true, node->depth, stack.size() };
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

        return FinderResult{ false, node->depth, stack.size() };
    }

    void createNodeDeq(NodeType* parent, unsigned short depth)
    {
        if (!parent) {
            return;
        }

        for (auto x : _operations) {

            createNodeIfNotVisitedDeq(x(parent->data), depth);
        }
    }

    void createNodeStack(NodeType* parent, unsigned short depth) {
        if (!parent) {
            return;
        }

        ValueType newValue = parent->data + 3;
        createNodeIfNotVisitedStack(newValue, depth);

        newValue = parent->data * 2;
        createNodeIfNotVisitedStack(newValue, depth);
    }

    // NOTE: добавить флаг в каком контейнере мы работаем и куда добавлять эл-ты.
    void createNodeIfNotVisitedDeq(const ValueType value, const unsigned short depth) {
        if (visitedValues.find(value) == visitedValues.end()) {
            visitedValues.insert(value);
            NodeType* node = new NodeType{ value, depth };
            deq.push_back(node);
        }
    }

    void createNodeIfNotVisitedStack(const ValueType value, const unsigned short depth) {
        if (visitedValues.find(value) == visitedValues.end()) {
            visitedValues.insert(value);
            DEBUG_LOG(value);
            NodeType* node = new NodeType{ value, depth };
            stack.push(node);
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
    std::vector<std::function<int(int)>> _operations;

    std::deque<NodeType*> deq;
    std::stack<NodeType*> stack;

    ValueType source;
    ValueType target;

    NodeType* currentElem;
};

