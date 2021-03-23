#pragma once
#include "Box.h"

class BoxNode
{
private:
	int boxType;
	Box* BBox;
	BoxNode* next;

public:
	BoxNode();

	~BoxNode();

	BoxNode(int, Box*);

	int getBoxType();

	void setBoxType(int);

	Box* getBBox();

	void setBBox(Box*);

	BoxNode* getNext();

	void setNext(BoxNode*);
};

