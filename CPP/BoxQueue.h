#pragma once
#include "BoxNode.h"

class BoxQueue
{
private:
	BoxNode* queueFront;
	BoxNode* queueBack;


public:
	BoxQueue();

	~BoxQueue();

	BoxQueue(BoxNode*);

	void insert(BoxNode*);

	BoxNode* getQueueFront();

	void setQueueFront(BoxNode*);

	BoxNode* getQueueBack();

    void setQueueBack(BoxNode*);

};

