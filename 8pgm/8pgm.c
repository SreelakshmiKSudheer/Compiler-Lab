#include "8pgm.h"

ASTNode* makeNode(char op, ASTNode *left, ASTNode *right)
{
	ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
	node->op = op;
	node->left = left;
	node->right = right;
	node->value = 0;

	return node;
}

ASTNode* makeLeaf(int value) 
{ 
        ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
        node->op = 0;
        node->left = NULL;
        node->right = NULL;
        node->value = value;

        return node;
}

void printAst(ASTNode *node)
{
	if(node->left)
		printAst(node->left);
	if(node->right)
		printAst(node->right);
	if(node->op)
		printf("%c ", node->op);
	else
		printf("%d ", node->value);
}

void preAst(ASTNode *node)
{
	if(node->op)
                printf("Node: %c \n", node->op);
        else
                printf("Leaf: %d \n", node->value);
        if(node->left)
                preAst(node->left);
        if(node->right)
                preAst(node->right);
}

