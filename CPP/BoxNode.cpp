#include "BoxNode.h"

BoxNode::BoxNode()
{
	this->boxType = -1;
	this->BBox = nullptr;
	this->next = nullptr;
}

BoxNode::BoxNode(int boxType, Box* BBox) {
	this->boxType = boxType;
	this->BBox = BBox;
	this->next = nullptr;
}

BoxNode::~BoxNode() {
	delete this->BBox;
	delete this->next;
}

int BoxNode::getBoxType()
{
	return this->boxType;
}

void BoxNode::setBoxType(int boxType)
{
	this->boxType = boxType;
}

Box* BoxNode::getBBox()
{
	return this->BBox;
}

void BoxNode::setBBox(Box* BBox)
{
	this->BBox = BBox;
}

BoxNode* BoxNode::getNext()
{
	return this->next;
}

void BoxNode::setNext(BoxNode* next)
{
	this->next = next;
}
