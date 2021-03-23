#include "BoxQueue.h"

BoxQueue::BoxQueue()
{
	this->queueFront = new BoxNode();
	this->queueBack = new BoxNode();
}

BoxQueue::~BoxQueue()
{
	delete this->queueFront;
	delete this->queueBack;
}

BoxQueue::BoxQueue(BoxNode* node)
{
	this->queueFront = node;
	this->queueBack = queueFront;
}

void BoxQueue::insert(BoxNode* newNode)
{
	this->queueBack->setNext(newNode);
	this->queueBack = newNode;
}

BoxNode* BoxQueue::getQueueFront()
{
	return this->queueFront;
}

void BoxQueue::setQueueFront(BoxNode* queueFront)
{
	this->queueFront = queueFront;
}

BoxNode* BoxQueue::getQueueBack()
{
	return this->queueBack;
}

void BoxQueue::setQueueBack(BoxNode* queueBack)
{
	this->queueBack = queueBack;
}
