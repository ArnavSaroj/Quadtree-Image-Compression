#include <iostream>

using namespace std;

struct colorValues
{
    float Red;
    float Green;
    float Blue;
};

struct Pos
{
    int x;
    int y;
};

struct Node
{

    Pos position;

    int width;
    int height;

    colorValues Color;

    bool isLeaf = false;

    int depth = 0; // maxdepth=10;

    Node *topRight = nullptr;
    Node *topLeft = nullptr;
    Node *bottomLeft = nullptr;
    Node *bottomRight = nullptr;
};

Node *buildingTree(int x, int y, int width, int height,float threshold,unsigned char* img,int img_width);

void renderTree(Node *startingNode, unsigned char *output_buffer, int img_width);

void deleteTree(Node *currentNode);

