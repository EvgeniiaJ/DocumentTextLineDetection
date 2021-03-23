#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "BoxQueue.h"
class Image
{
private:
	int imageRows;
	int imageCols;
	int imageMin;
	int imageMax;

public:
	int threshold;
	int horizontalProjectionRuns;
	int verticalProjectionRuns;

	int* horizontalProjection;
	int* verticalProjection;
	int* binaryHorizontalProjection;
	int* binaryVerticalProjection;
	int* horizontalProjectionMorphology;
	int* verticalProjectionMorphology;
	int** imageArray;

	Box* imageBox;
	BoxQueue* boxQueue;
	string direction;

	int* structuringElement;
	int elementSize;
	int origin;

public:

	Image(ifstream&, int);

	~Image();

	void initializeArrays();

	void zeroFraming(int**);

	void loadImage(ifstream&);

	void initializeBox();

	void computeProjectionProfile(string, int*);

	void thresholding(string, int*, int*);

	void printProjectionProfile(string, int*, ofstream&);

	void morphologicalClosing(int*, int*, int);

	void determineDirection(ofstream&);

	void findLineBoxes(string, int*);

	void printBoxQueue(ofstream&);

	void drawBoxes(BoxQueue*, int**);

	void prettyPrint(ofstream&);

	int getImageRows();

	void setImageRows(int);

	int getImageCols();

	void setImageCols(int);

	int getImageMin();

	void setImageMin(int);

	int getImageMax();

	void setImageMax(int);
};

