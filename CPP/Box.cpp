#include "Box.h"

Box::Box()
{
	this->minRow = 0;
	this->minCol = 0;
	this->maxRow = 0;
	this->maxCol = 0;
}

Box::Box(int minRow, int minCol, int	maxRow, int maxCol) {
	this->minRow = minRow;
	this->minCol = minCol;
	this->maxRow = maxRow;
	this->maxCol = maxCol;
}

Box::~Box()
{
}

int Box::getMinRow()
{
	return this->minRow;
}

void Box::setMinRow(int minRow) {
	this->minRow = minRow;
}

int Box::getMinCol()
{
	return this->minCol;
}

void Box::setMinCol(int minCol) {
	this->minCol = minCol;
}

int Box::getMaxRow()
{
	return this->maxRow;
}

void Box::setMaxRow(int maxRow) {
	this->maxRow = maxRow;
}

int Box::getMaxCol()
{
	return this->maxCol;
}
void Box::setMaxCol(int maxCol) {
	this->maxCol = maxCol;
}
