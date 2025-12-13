#pragma once

/*
    # OperationSequenceFinder

    Класс OperationSequenceFinder предоставляет методы решения задач в пространстве состояний.
    Класс принимает на вход начальное и целевое число, а также арифмитические операции, с
    помощью которых класс пытаетя придти из начального в целевое число.

    ## Подробное описание

    Класс основан на объектно-ориентированном подходе, не использует решений вида комбинации трех векторов,
    а основан на дереве, где Node узел дерева. Класс реализует методы поиска в ширину и в глубину (двунаправленный
    поиск находится на этапе реализации).

    В класс передается (setOperations method) вектор пар вида <имя функция>, функция - арифмитическая операция
    которая будет использоваться для поиска. Их число по-сути не ограничено. После чего вызывается функция поиска
    (findSequenceBFS, findSequenceDFS) которая возвращает результат ввиде класса.


    ## Члены класса

    ### macros

    OPERATIONS_SEQUENCE_ENABLED - макрос, если он определен, то у нас будет включен подсчет операций,
    что позволит в выводе результата узнать какии образом мы пришли от начального числа в финальное.

    ### using

    using OperationsNumberType - тип числа возможных операций
    using TreeDepthType - тип глубины дерева
    using ValueType - тип начальное и целевого числа (которое ищем)
    using NodeType - тип узла дерева.

    ### classes

    class Node - реализует узел дерева
    {
        T data - дата
        TreeDepthType depth - глубина node в дереве
        Node* parent - указатель на родителя
        OperationsNumberType operationNumber - номер операции в массиве с помощью которой был поражден этот Node
    }

    class FinderResult - является результатом поиска. Информацию о результате можно узнать используюя метод print

    ### methods

    setOperations - принимает вектор пар, которые имеют вид <имя функции, функция>, с помощью которых будет
    проходить поиск.

    FinderResult findSequenceBFS(ValueType source, ValueType target, TreeDepthType searchDepth = 20) - поиск в ширину
        source - откуда начинаем поиск
        target - что ищем
        searchDepth - максимальная глубина дерева

    FinderResult findSequenceDFS(ValueType source, ValueType target, TreeDepthType searchDepth = 20) - поиск в глубину


    ### Что следует сделать

    1. Заменить значения в контейнерах с NodeType* на NodeType
    2. Реализовать двунаправленный поиск

*/

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
#include <unordered_map>

#include "macros.h"

#define OPERATIONS_SEQUENCE_ENABLED

class OperationSequenceFinder
{
    template<typename T>
    class Node;

public:
    using OperationsNumberType = short;
    using TreeDepthType = unsigned short;


    using ValueType = int;
    using NodeType = Node<ValueType>;
private:

    // NOTE: move to different file ?
    template<typename T>
    class Node {

    public:
        T data;
        TreeDepthType depth;

#ifdef OPERATIONS_SEQUENCE_ENABLED
        Node* parent;
        OperationsNumberType operationNumber;
#endif

        // стоит ли хранить массив операций?
        T getData() const noexcept {
            return data;
        }

#ifdef OPERATIONS_SEQUENCE_ENABLED
        Node(T data, TreeDepthType depth, Node* parent, OperationsNumberType opNumb)
            : data(data)
            , depth(depth)
            , parent(parent)
            , operationNumber(opNumb)

        {};
#else
        Node(T data, TreeDepthType depth)
            : data(data)
            , depth(depth)
        {};
#endif 
    };

public:
    class FinderResult {
        ValueType _source;
        ValueType _target;
        bool _isFind;
        TreeDepthType _depth;
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
        FinderResult(ValueType source, ValueType target, int isFind, NodeType* node, size_t contSize, 
            const std::vector<std::pair<std::string, std::function<int(int)>>>& operations)
            : _source(source)
            , _target(target)
            , _isFind(isFind)
            , _depth(0)
            , _containerSize(contSize)
            // использовано памяти = размер указателя * размер ноды * кол-во
            , _memoryUsed(8 * sizeof(NodeType) * _containerSize)

#ifdef OPERATIONS_SEQUENCE_ENABLED
            , operationSequence("")
            , _operations(operations)
#endif
        {
#ifdef OPERATIONS_SEQUENCE_ENABLED
            if (node) {
                _depth = node->depth;
            }

            if (isFind) {
                std::stack<NodeType*> stack;
                stack.push(node);

                while (stack.top()->parent != nullptr) {
                    NodeType* node = stack.top();
                    stack.push(node->parent);
                }

                while (!stack.empty()) {
                    NodeType* node = stack.top();
                    stack.pop();

                    int opNumb = node->operationNumber;
                    if (opNumb >= 0 && opNumb < _operations.size()) {
                        std::string operation = _operations.at(opNumb).first;
                        operationSequence += operation += " ";
                        operationSequence += " ";
                    }
                }
            }
#endif
        }

        bool isFind() const {
            return _isFind;
        }

        TreeDepthType getDepth() const {
            return _depth;
        }

        unsigned long long getUsedMemory() const {
            return _memoryUsed;
        }

        void print() const {
            std::cout << "source: " << _source << " target: " << _target << " isFind: " << _isFindString() << " depth: " << _depth
                << " containerSize: " << _containerSize << " memoryUsed in byte: " << _memoryUsed 
#ifdef OPERATIONS_SEQUENCE_ENABLED
                << " opSeq: " << operationSequence 
#endif
                << std::endl;
        }
    };

    OperationSequenceFinder() 
        : source{0}
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
    }

    void setReverseOperations(const std::vector<std::pair<std::string, std::function<int(int)>>>& reverseOperations)
    {
        _reverseOperations = reverseOperations;
    }

    // TODO: Эта функция создает мешанину. По-хорошему нужно вынести в отдельный класс отнаслед. от этого.
    FinderResult findSequenceBidir(ValueType source, ValueType target, TreeDepthType searchDepth = 20) 
    {

        if (_reverseOperations.size() != _operations.size()) {
            std::cout << "Reverse operations is not assign with forward operations. Please call setReverseOperators method!" << std::endl;
            return FinderResult{ source, target, false, nullptr, deq.size() , _operations };
        }

        this->source = source;
        this->target = target;
        this->searchDepth = searchDepth;

        
#ifdef OPERATIONS_SEQUENCE_ENABLED
        // создаем source
        NodeType* sourceRoot = new NodeType(source, 0, nullptr, -1);
        
        // создаем target
        NodeType* targetRoot = new NodeType(target, 0, nullptr, -1);


#else
        NodeType* root = new NodeType(source, 0);
        NodeType* targetRoot = new NodeType(target, 0);
#endif
        deq.push_back(sourceRoot);
        reverseDeq.push_back(targetRoot);

        if (countOperations < 2) {
            treeSize = searchDepth;
        }
        else {
            treeSize = pow(countOperations, searchDepth);
        }

        DEBUG_LOG("source: " << source << " target: " << target << " max tree size: " << treeSize);

        FinderResult res = bidirFinder(sourceRoot, targetRoot);
        clear();

        return res;

    }

    FinderResult bidirFinder(NodeType* sourceNode, NodeType* targetNode) {


        bool isFind = checkNode(sourceNode);

        if (isFind) {
            return FinderResult{ source, target, true, sourceNode, deq.size() , _operations };
        }

        // and / or
        while (!deq.empty()) {
      
            int sz = deq.size();
            for (int i = 0; i < sz; i++) {
                
                if (checkInReverseVisited(deq.at(i)->data)) {
                    DEBUG_LOG("checkInReverseVisited: " + std::to_string( deq.at(i)->data));
                    auto node = map2[deq.at(i)->data];
                    NodeType* res = mergeTrees(deq.at(i), node);
                    return FinderResult{ source, target, true, res, deq.size() , _operations };
                }
                else {
                    map1.insert({ deq.at(i)->data, deq.at(i)});
                    NodeType* node = deq.front();
                    deq.pop_front();
                    if (node) {
                        isFind = checkNode(node);

                        if (isFind) {
                            return FinderResult{ source, target, true, node, deq.size(), _operations };
                        }

                        currentDepth.insert(node->depth);
                        if (setSize != currentDepth.size()) {
                            DEBUG_LOG("Depth is " << currentDepth.size() - 1 << ", deque size: " << deq.size() << "; ");
                            setSize = currentDepth.size();
                        }

                        if (node->depth + 1 < searchDepth) {
                            createNodeDeq(node);
                        }
                    }
                }
            }

            sz = reverseDeq.size();
            for (int i = 0; i < sz; i++) {
                if (checkInVisited(reverseDeq.at(i)->data)) {
                    DEBUG_LOG("checkInVisited: " + std::to_string(reverseDeq.at(i)->data));
                    auto node = map1[reverseDeq.at(i)->data];
                    NodeType* res = mergeTrees(reverseDeq.at(i), node);
                    return FinderResult{ source, target, true, res, reverseDeq.size() , _operations };
                }
                else {
                    map2.insert({ reverseDeq.at(i)->data, reverseDeq.at(i) });
                    NodeType* node = reverseDeq.front();
                    reverseDeq.pop_front();
                    if (node) {
                        createNodeReverseDeq(node);
                    }
                }
            }

            map1.clear();
            map2.clear();
        }

        return FinderResult{ source, target, false, sourceNode, deq.size(), _operations };;

    }

    // В ширину
    FinderResult findSequenceBFS(ValueType source, ValueType target, TreeDepthType searchDepth = 20) {
        this->source = source;
        this->target = target;
        this->searchDepth = searchDepth;

#ifdef OPERATIONS_SEQUENCE_ENABLED
        NodeType* root = new NodeType(source, 0, nullptr, -1);
#else
        NodeType* root = new NodeType(source, 0);
#endif
        deq.push_back(root);

        if (countOperations < 2) {
            treeSize = searchDepth;
        }
        else {
            treeSize = pow(countOperations, searchDepth);
        }

        DEBUG_LOG("source: " << source << " target: " << target << " max tree size: " << treeSize);

        FinderResult res = BFSFinder(root);
        clear();

        return res;
    }

    void clear() {
        for (NodeType* ptr : deq) {
            delete ptr;
        }
        deq.clear();

        // TODO: вынести в другой класс
        for (NodeType* ptr : reverseDeq) {
            delete ptr;
        }
        reverseDeq.clear();

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

    FinderResult BFSFinder(NodeType* node)
    {
        bool isFind = checkNode(node);

        if (isFind) {
            return FinderResult{ source, target, true, node, deq.size() , _operations };
        }

        while (!deq.empty()) {
            NodeType* node = deq.front();
            deq.pop_front();
            if (node) {
                isFind = checkNode(node);

                if (isFind) {
                    return FinderResult{ source, target, true, node, deq.size(), _operations };
                }

                currentDepth.insert(node->depth);
                if (setSize != currentDepth.size()) {
                    DEBUG_LOG("Depth is " << currentDepth.size() - 1 << ", deque size: " << deq.size() << "; ");
                    setSize = currentDepth.size();
                }

                if (node->depth + 1 < searchDepth) {
                    createNodeDeq(node);
                }

                //delete node;
            }
        }

        return FinderResult{ source, target, false, node, deq.size(), _operations };;
    }

    void createNodeDeq(NodeType* parent)
    {
        if (!parent) {
            return;
        }

        for (OperationsNumberType i = 0; i < _operations.size(); i++) {
            auto func = _operations.at(i).second;
            ValueType value = func(parent->data);
            if (visitedValues.find(value) == visitedValues.end()) {
                visitedValues.insert(value);

                TreeDepthType newDepth = parent->depth + 1;
#ifdef OPERATIONS_SEQUENCE_ENABLED
                NodeType* node = new NodeType{ value, newDepth, parent, i };

#else
                NodeType* node = new NodeType{ value, newDepth };
#endif
                deq.push_back(node);
            }
        }
    }

    void createNodeReverseDeq(NodeType* parent)
    {
        if (!parent) {
            return;
        }

        for (OperationsNumberType i = 0; i < _reverseOperations.size(); i++) {
            auto func = _reverseOperations.at(i).second;
            ValueType value = func(parent->data);
            if (reverseVisitedValues.find(value) == reverseVisitedValues.end()) {
                reverseVisitedValues.insert(value);

                TreeDepthType newDepth = parent->depth + 1;
#ifdef OPERATIONS_SEQUENCE_ENABLED
                NodeType* node = new NodeType{ value, newDepth, parent, i };

#else
                NodeType* node = new NodeType{ value, newDepth };
#endif
                reverseDeq.push_back(node);
            }
        }
    }

    // В глубину
    FinderResult findSequenceDFS(ValueType source, ValueType target, TreeDepthType searchDepth = 20) {
        this->source = source;
        this->target = target;
        this->searchDepth = searchDepth;

#ifdef OPERATIONS_SEQUENCE_ENABLED
        NodeType* root = new NodeType(source, 0, nullptr, -1);
#else
        NodeType* root = new NodeType(source, 0);
#endif
        stack.push(root);

        if (countOperations < 2) {
            treeSize = searchDepth;
        }
        else {
            treeSize = pow(countOperations, searchDepth);
        }

        DEBUG_LOG("source: " << source << " target: " << target << " max tree size: " << treeSize);

        FinderResult res = DFSFinder(root);
        clear();

        return res;
    }

    FinderResult DFSFinder(NodeType* node)
    {
        int max = 0;

        bool isFind = checkNode(node);

        if (isFind) {
            return FinderResult{ source, target, true, node, stack.size() , _operations };
        }

        while (!stack.empty()) {
            NodeType* node = stack.top();
            stack.pop();

            if (node) {
                isFind = checkNode(node);

                if (isFind) {
                    return FinderResult{ source, target, true, node, stack.size(), _operations };
                }
                currentDepth.insert(node->depth);
                if (setSize != currentDepth.size()) {
                    setSize = currentDepth.size();
                }

                if (node->depth + 1 < searchDepth) {
                    createNodeStack(node, node->depth + 1);
                }
                delete node;
            }
        }

        return FinderResult{ source, target, false, node, stack.size(), _operations };
    }

    void createNodeStack(NodeType* parent, TreeDepthType depth) {
        if (!parent) {
            return;
        }

        for (OperationsNumberType i = 0; i < _operations.size(); i++) {

            auto func = _operations.at(i).second;
            ValueType value = func(parent->data);
            if (visitedValues.find(value) == visitedValues.end()) {
                visitedValues.insert(value);
                DEBUG_LOG(value);
                TreeDepthType newDepth = parent->depth + 1;
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

    inline bool checkInVisited(const ValueType value)
    {
        return map1.find(value) != map1.end();
    }

    inline bool checkInReverseVisited(const ValueType value) 
    {
        return map2.find(value) != map2.end();
    }

    NodeType* mergeTrees(NodeType* forwardNode, NodeType* reverseNode)
    {
        // Создаем новый узел, который будет связующим звеном
        // Этот узел будет содержать операцию из обратного дерева
#ifdef OPERATIONS_SEQUENCE_ENABLED
        NodeType* bridgeNode = new NodeType{ forwardNode->data, forwardNode->depth, forwardNode, reverseNode->operationNumber };
#else
        NodeType* bridgeNode = new NodeType{ forwardNode->data, forwardNode->depth };
#endif

        // Теперь связываем обратное дерево через bridgeNode
        if (reverseNode->parent) {
            bridgeNode->parent = reverseNode->parent;
            // Обновляем операцию для bridgeNode на операцию из обратного пути
            bridgeNode->operationNumber = reverseNode->operationNumber;
        }

        return bridgeNode;
    }

    // возвращает указатель на новый root, с которым связывается основное дерево и указатель на новый лист (res), 
    // который будет возвращен как результат.
    std::pair<NodeType*, NodeType*> reverseTree(NodeType* leaf) {
        if (!leaf) return std::pair<NodeType*, NodeType*>{nullptr, nullptr};
        if (!leaf->parent) return std::pair<NodeType*, NodeType*>{nullptr, nullptr};

        NodeType* newRoot = leaf->parent;

        NodeType* res = leaf->parent;
        NodeType* current = leaf->parent;
        NodeType* prev = nullptr;

        while (current) {
            NodeType* next = current->parent;
            current->parent = prev;
            prev = current;
            current = next;
        }

        return std::pair<NodeType*, NodeType*>{newRoot, res};
    }

    ValueType source;
    ValueType target;

    unsigned short countOperations = 2;
    TreeDepthType searchDepth = 0;

    size_t treeSize;
    size_t setSize;
    
    std::set<TreeDepthType> currentDepth;
    std::set<TreeDepthType> reverseCurrentDepth;

    std::unordered_set<ValueType> visitedValues;
    std::unordered_set<ValueType> reverseVisitedValues;

    // Вместо std::unordered_set<ValueType> используйте:
    std::unordered_map<ValueType, NodeType*> map1; // для посещенных значений forward
    std::unordered_map<ValueType, NodeType*> map2; // для посещенных значений reverse

    // TODO: вынести в другой класс
    std::deque<NodeType*> deq;
    std::deque<NodeType*> reverseDeq;

    // Без указателя будет переполнение стека? Нет, т.к. эти данные выделяются в динасической памяти.
    std::stack<NodeType*> stack;

    std::vector<std::pair<std::string, std::function<int(int)>>> _operations;
    std::vector<std::pair<std::string, std::function<int(int)>>> _reverseOperations;

};

