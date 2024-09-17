#pragma once

#include <cmath>
#include <vector>
#include <algorithm>

class Node {
public:
    int x;
    int y;
    float hCost;
    float gCost;
    float fCost;
    std::vector<Node*> nextNodes;
    Node* previousNode = nullptr;
    bool isBlocked = false;
};

struct MapNode {
    int x;
    int y;
};

class Map {
public:
    Map(int width, int height) {
        this->width = width;
        this->height = height;
        CreateMap();
    }
    std::vector<Node*> map;
    int width = 25;
    int height = 25;

    Node* GetNode(int x, int y) {
        if (x > width - 1 || x < 0 || y > height - 1 || y < 0) {
            return NULL;
        }
        return map[x * height + y];
    }

private:
    void CreateMap() {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                Node* node = new Node();
                node->x = i;
                node->y = j;
                map.push_back(node);
            }
        }
        for (int i = 0; i < map.size(); i++) {
            Node* node = map[i];
            int x = node->x;
            int y = node->y;
            Node* previous = GetNode(x + 1, y);
            if (previous != nullptr) {
                node->nextNodes.push_back(previous);
            }
            previous = nullptr;
            previous = GetNode(x, y + 1);
            if (previous != nullptr) {
                node->nextNodes.push_back(previous);
            }
            previous = nullptr;
            previous = GetNode(x - 1, y);
            if (previous != nullptr) {
                node->nextNodes.push_back(previous);
            }
            previous = nullptr;
            previous = GetNode(x, y - 1);
            if (previous != nullptr) {
                node->nextNodes.push_back(previous);
            }
        }
    }
};

float calculate_distance(const Node* to, const Node* from) {
    return pow((to->x - from->x), 2) + pow((to->y - from->y), 2);
}

std::vector<Node*> GetRoute(Node* startNode, Node* endNode) {
    std::vector<Node*> path;
    std::vector<Node*> queue;

    startNode->gCost = 0;
    startNode->previousNode = nullptr;
    queue.push_back(startNode);

    while (true) {
        int currentSmallestCost = 0;
        for (int i = 0; i < queue.size(); i++) {
            if (queue[currentSmallestCost]->fCost > queue[i]->fCost) {
                currentSmallestCost = i;
            }
        }
        Node* nextNode = nullptr;
        if (queue.size() <= currentSmallestCost) {
            break;
        }
        nextNode = queue[currentSmallestCost];

        path.push_back(nextNode);


        if (nextNode->x == endNode->x && nextNode->y == endNode->y) {
            break;
        }

        queue.erase(queue.begin() + currentSmallestCost);
        if (!nextNode->nextNodes.empty()) {
            for (int i = 0; i < nextNode->nextNodes.size(); i++) {
                Node* pushBack = nextNode->nextNodes[i];
                bool addNode = true;
                for (int j = 0; j < queue.size(); j++)
                {
                    if (queue[j]->x == pushBack->x && queue[j]->y == pushBack->y) {
                        addNode = false;
                        break;
                    }
                }
                for (int j = 0; j < path.size(); j++)
                {
                    if (path[j]->x == pushBack->x && path[j]->y == pushBack->y) {
                        addNode = false;
                        break;
                    }
                }
                if (pushBack->isBlocked) {
                    continue;
                }
                if (addNode) {
                    pushBack->previousNode = nextNode;
                    pushBack->gCost = nextNode->gCost + 10;
                    pushBack->hCost = calculate_distance(endNode, pushBack);
                    pushBack->fCost = pushBack->gCost + pushBack->hCost;
                    queue.push_back(pushBack);
                }
            }
        }
    }

    std::vector<Node*> finalPath;

    if (path.size() > 0) {
        Node* node = path.back();
        while (node != nullptr) {
            finalPath.push_back(node);
            node = node->previousNode;
        }
        std::ranges::reverse(finalPath.begin(), finalPath.end());
    }
    return finalPath;
}
