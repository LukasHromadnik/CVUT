//
//  main.c
//  stromy
//
//  Created by Lukáš Hromadník on 18.12.13.
//  Copyright (c) 2013 Lukáš Hromadník. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int val;
    struct Node * L;
    struct Node * R;
};

void printTree(struct Node * root) {
    if (root == NULL)
        return;
    
    printTree(root->L);
    
    // inorder
    printf("%d ", root->val);
    
    printTree(root->R);
}

struct Node * getNode(int val) {
    struct Node * node = (struct Node *) malloc(sizeof(struct Node));
    node->L = NULL;
    node->R = NULL;
    node->val = val;
    
    return node;
}

void insertNode(struct Node * root, int val) {
    if (root->val > val) {
        if (root->L == NULL)
            root->L = getNode(val);
        else
            insertNode(root->L, val);
    }
    else if (root->val < val) {
        if (root->R == NULL)
            root->L = getNode(val);
        else
            insertNode(root->R, val);
    }
}

int main(int argc, const char * argv[])
{
    struct Node * root = getNode(42);
    insertNode(root, 21);
    insertNode(root, 22);
    insertNode(root, 43);
    
    printTree(root);

}
