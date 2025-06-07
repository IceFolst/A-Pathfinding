//
// Created by alexandre on 07/06/25.
//

#include "Node.h"

Node::Node(int X, int Y, bool walkable)
{
    this->X = X;
    this->Y = Y;
    this->Walkable = walkable;
    this->HCost = 0;
    this->GCost = 10;
    Parent = nullptr;
}

int Node::FCost()
{
    return GCost + HCost;
}


bool Node::Same(Node* node)
{
    if (node == nullptr)
    {
        return false;
    }
    return X == node->X && Y == node->Y;
}
