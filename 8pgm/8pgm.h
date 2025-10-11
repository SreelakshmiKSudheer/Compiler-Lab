#include <stdio.h>
#include <stdlib.h>

typedef struct ASTNode
{
	char op;
	struct ASTNode *left;
	struct ASTNode *right;
	int value;
}ASTNode;

ASTNode* makeNode(char op, ASTNode *left, ASTNode *right);
ASTNode* makeLeaf(int value);
void printAst(ASTNode *node);
void preAst(ASTNode *node);

