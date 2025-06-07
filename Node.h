//
// Created by alexandre on 07/06/25.
//
#ifndef NODE_H
#define NODE_H

using namespace std;

class Node {
public:
    int X;
    int Y;
    bool Walkable;

    int GCost;
    int HCost;

    Node* Parent;

    Node(int x, int y, bool walkable);
    int FCost();
    bool Same(Node *node);
};

#endif //NODE_H
