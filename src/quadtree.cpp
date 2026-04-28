#include <iostream>
#include "quadtree.h"

using namespace std;

// pair<long long, float> avg_and_mse_calculation(colorValues arr[], int size)
//{

// long long sum=0;
// long long sumSquare=0;
// for(int i=0;i<size;i++)
// sum=sum+arr[i];
//}

Node *buildingTree(int x, int y, int width, int height, float threshold, unsigned char *img, int img_width)
{

    Node *newNode = new Node();
    newNode->position.x = x;
    newNode->position.y = y;
    newNode->width = width;
    newNode->height = height;

    long long avgRed = 0, avgBlue = 0, avgGreen = 0;
    long long avgSquareRed = 0, avgSquareBlue = 0, avgSquareGreen = 0;

    int area = width * height;

    // pixel color combination
    // red,calculation for avg color
    // pass the stlibimage buffer here for calc
    for (int i = y; i < y + height; i++)
    // i is the row
    {
        for (int j = x; j < x + width; j++)
        {
            // j is the col
            int index = (i * img_width + j) * 3;

            avgRed = avgRed + img[index];
            avgGreen = avgGreen + img[index + 1];
            avgBlue = avgBlue + img[index + 2];

            avgSquareRed = avgSquareRed + img[index] * img[index];
            avgSquareBlue = avgSquareBlue + img[index + 2] * img[index + 2];
            avgSquareGreen = avgSquareGreen + img[index + 1] * img[index + 1];
        }
    }
    // 1. Calculate Mean of Squares

    avgRed = avgRed / area;
    avgBlue = avgBlue / area;
    avgGreen = avgGreen / area;

    float meanSquareRed = (float)avgSquareRed / area;
    float meanSquareGreen = (float)avgSquareGreen / area;
    float meanSquareBlue = (float)avgSquareBlue / area;

    newNode->Color.Red = avgRed;
    newNode->Color.Green = avgGreen;
    newNode->Color.Blue = avgBlue;

    // 2. Variance = Mean of Squares - (Square of Means)
    float varRed = meanSquareRed - (float)(avgRed * avgRed);
    float varGreen = meanSquareGreen - (float)(avgGreen * avgGreen);
    float varBlue = meanSquareBlue - (float)(avgBlue * avgBlue);

    // 3. Final Variance (Average of the three channels)
    float variance = (varRed + varGreen + varBlue) / 3.0f;

    if (variance < threshold || newNode->depth == 10 || width == 1 || height == 1)
    {
        newNode->isLeaf = true;
        return newNode;
    }


 
    int leftW = width >> 2;
    int rightW = width - leftW;

    int topH = height >>2;
    int botH = height - topH;

    newNode->topRight = buildingTree(x + leftW, y, rightW, topH, threshold, img, img_width);
    newNode->topLeft = buildingTree(x, y, leftW, topH, threshold, img, img_width);
    newNode->bottomRight = buildingTree(x + leftW, y + topH, rightW, botH, threshold, img, img_width);
    newNode->bottomLeft = buildingTree(x, y + topH, leftW, botH, threshold, img, img_width);

    return newNode;
}

void renderTree(Node *startingNode, unsigned char *output_buffer, int img_width)
{

    if (startingNode->isLeaf == true)
    {
        for (int i = startingNode->position.y; i < startingNode->position.y + startingNode->height; i++)
        // i is the row
        {
            for (int j = startingNode->position.x; j < startingNode->position.x + startingNode->width; j++)
            {
                // j is the col
                int index = (i * img_width + j) * 3;

                output_buffer[index] = (unsigned char)startingNode->Color.Red;
                output_buffer[index + 1] = (unsigned char)startingNode->Color.Green;
                output_buffer[index + 2] = (unsigned char)startingNode->Color.Blue;
            }
        }
        return;
    }
    else
    {
        renderTree(startingNode->topLeft, output_buffer, img_width);
        renderTree(startingNode->topRight, output_buffer, img_width);
        renderTree(startingNode->bottomLeft, output_buffer, img_width);
        renderTree(startingNode->bottomRight, output_buffer, img_width);
    }
    return;
}

/// cleanup function
void deleteTree(Node *currentNode)
{

    if (currentNode == nullptr)
        return;

    deleteTree(currentNode->topLeft);
    deleteTree(currentNode->topRight);
    deleteTree(currentNode->bottomLeft);
    deleteTree(currentNode->bottomRight);

    delete currentNode;
}
