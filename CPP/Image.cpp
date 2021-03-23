#include "Image.h"

Image::Image(ifstream& input, int threshold)
{
	if (!input.eof()) {
		input >> this->imageRows;
	}
	if (!input.eof()) {
		input >> this->imageCols;
	}
	if (!input.eof()) {
		input >> this->imageMin;
	}
	if (!input.eof()) {
		input >> this->imageMax;
	}

	this->threshold = threshold;
	imageBox = new Box(imageRows, imageCols, 0, 0);
	boxQueue = new BoxQueue(new BoxNode());
	direction = "";

	elementSize = 3;
	structuringElement = new int[elementSize];
	structuringElement[0] = structuringElement[1] = structuringElement[2] = 1;
	origin = 1;

	initializeArrays();
}

Image::~Image()
{
	delete[] horizontalProjection;
	delete[] verticalProjection;
	delete[] binaryHorizontalProjection;
	delete[] binaryVerticalProjection;
	delete[] horizontalProjectionMorphology;
	delete[] verticalProjectionMorphology;
	delete[] structuringElement;

	delete imageBox;
	delete boxQueue;

	for (int i = 0; i < imageRows; i++) {
		delete[] imageArray[i];
	}

	delete[] imageArray;
}

void Image::initializeArrays()
{

	horizontalProjection = new int[imageRows + 2];
	verticalProjection = new int[imageCols + 2];
	binaryHorizontalProjection = new int[imageRows + 2];
	binaryVerticalProjection = new int[imageCols + 2];
	horizontalProjectionMorphology = new int[imageRows + 2];
	verticalProjectionMorphology = new int[imageCols + 2];

	imageArray = new int*[imageRows];

	for (int i = 0; i < (imageRows + 2); i++) {

		horizontalProjection[i] = 0;
		binaryHorizontalProjection[i] = 0;
		horizontalProjectionMorphology[i] = 0;

		if (i < imageRows) {
			imageArray[i] = new int[imageCols];
		}

		for (int j = 0; j < (imageCols + 2); j++) {

			verticalProjection[j] = 0;
			binaryVerticalProjection[j] = 0;
			verticalProjectionMorphology[j] = 0;
			if (i < imageRows && j < imageCols) {
				imageArray[i][j] = 0;
			}
		}
	}
}

void Image::zeroFraming(int** array)
{
	for (int i = 0; i < (imageRows + 2); i++) {
		array[i][0] = 0;
		array[i][imageCols + 1] = 0;
	}

	for (int i = 0; i < (imageCols + 2); i++) {
		array[0][i] = 0;
		array[imageRows + 1][i] = 0;
	}

}

void Image::loadImage(ifstream& input)
{
	while (!input.eof()) {
		for (int i = 0; i < imageRows; i++) {
			for (int j = 0; j < imageCols; j++) {
				input >> imageArray[i][j];
			}
		}
	}

	initializeBox();
}

void Image::initializeBox()
{
	int minRow = imageRows;
	int minCol = imageCols;
	int maxRow = -1;
	int maxCol = -1;

	for (int i = 0; i < imageRows; i++) {
		for (int j = 0; j < imageCols; j++) {
			if (imageArray[i][j] > 0) {
				if (minRow > i) {
					minRow = i;
				}
				if (minCol > j) {
					minCol = j;
				}
				if (maxRow < i) {
					maxRow = i;
				}
				if (maxCol < j) {
					maxCol = j;
				}
			}
		}
	}

	imageBox = new Box(minRow, minCol, maxRow, maxCol);
}


void Image::computeProjectionProfile(string passId, int* projection)
{

	int minRow = -1, minCol = -1, maxRow = -1, maxCol = -1;

	if (passId.compare("horizontal") == 0) {
		minRow = imageBox->getMinRow();
		minCol = 0;
		maxRow = imageBox->getMaxRow() + 1;
		maxCol = imageCols;

	}
	else if (passId.compare("vertical") == 0) {
		minRow = 0;
		minCol = imageBox->getMinCol();
		maxRow = imageRows;
		maxCol = imageBox->getMaxCol() + 1;
	}

	for (int i = minRow; i < maxRow; i++) {
		for (int j = minCol; j < maxCol; j++) {
			if (imageArray[i][j] > 0) {
				int index = -1;

				if (passId.compare("horizontal") == 0) {
					index = i;
				}
				else if (passId.compare("vertical") == 0) {
					index = j;
				}
				projection[index]++;
			}
		}
	}
}

void Image::thresholding(string passId, int* array, int* binaryArray)
{
	int maxIndex = -1;

	if (passId.compare("horizontal") == 0) {
		maxIndex = imageRows;
	}
	else if (passId.compare("vertical") == 0) {
		maxIndex = imageCols;
	}

	for (int i = 0; i < maxIndex; i++) {
		if (array[i] >= threshold) {
			binaryArray[i] = 1;
		}
		else {
			binaryArray[i] = 0;
		}
	}
}

void Image::printProjectionProfile(string passId, int* array, ofstream& output)
{
	int size = 0;

	if (passId.compare("horizontal") == 0) {
		size = imageRows + 1;
	}
	else if (passId.compare("vertical") == 0) {
		size = imageCols + 1;
	}
	try {
		for (int i = 1; i < size; i++) {
			output << array[i] << endl;
		}
		output << endl;
	}
	catch (exception e) {
		cout << "There is an error: " << e.what();
	}
}

void Image::morphologicalClosing(int* morphologyArray, int* binaryArray, int size)
{
	int* tempArray = new int[size];

	for (int i = 0; i < size; i++) {
		tempArray[i] = 0;
	}

	for (int i = 0; i < size; i++) {
		tempArray[i] = binaryArray[i];
	}

	for (int i = 1; i < size - 1; i++) {
		if (binaryArray[i] == structuringElement[origin]) {
			int k = i - 1;
			for (int j = 0; j < elementSize; j++) {
				if (structuringElement[j] == 1) {
					tempArray[k + j] = 1;
					morphologyArray[k + j] = 1;
				}
			}
		}
	}

	for (int i = 1; i < size - 1; i++) {
		if (tempArray[i] == 1) {
			bool check = true;
			int k = i - 1;
			for (int j = 0; j < elementSize; j++) {
				if (tempArray[k + j] != structuringElement[j]) {
					check = false;
					break;
				}
			}
			if (!check) {
				morphologyArray[i] = 0;
			}
		}
	}
}

void Image::determineDirection(ofstream& output)
{
	horizontalProjectionRuns = 1;
	verticalProjectionRuns = 1;
	int factor = 3;

	try {
		int val = horizontalProjectionMorphology[0];

		for (int i = 1; i < imageRows + 1; i++) {
			if (val != horizontalProjectionMorphology[i]) {
				val = horizontalProjectionMorphology[i];
				horizontalProjectionRuns++;
			}
		}

		val = verticalProjectionMorphology[0];
		for (int i = 1; i < imageCols + 1; i++) {
			if (val != verticalProjectionMorphology[i]) {
				val = verticalProjectionMorphology[i];
				verticalProjectionRuns++;
			}
		}
		
		if (horizontalProjectionRuns >= factor * verticalProjectionRuns) {
			output << "Reading Direction: horizontal" << endl << endl;
			direction = "horizontal";
		}
		else if (verticalProjectionRuns >= factor * horizontalProjectionRuns) {
			output << "Reading Direction: vertical" << endl << endl;
			direction = "vertical";
		}
		else {
			cout << "Cannot determine the reading direction" << endl;
			exit(1);
		}
	}
	catch (exception e) {
		cout << "There is an error: " << e.what();
	}
}

void Image::findLineBoxes(string direction, int* morphologyArray)
{
	
	int minRow = -1;
	int minCol = -1;
	int maxRow = -1;
	int maxCol = -1;
	if (direction.compare("horizontal") == 0) {
		minRow = imageBox->getMinRow();
		minCol = imageBox->getMinCol();
		maxRow = imageBox->getMaxRow();
		maxCol = imageBox->getMaxCol();
	}
	else if (direction.compare("vertical") == 0) {
		minRow = imageBox->getMinCol();
		minCol = imageBox->getMinRow();
		maxRow = imageBox->getMaxCol();
		maxCol = imageBox->getMaxRow();
	}

	BoxNode* newNode = nullptr;
	Box* box = nullptr;

	for (int i = minRow; i <= maxRow; ++i) {
		if (morphologyArray[i] > 0) {
			box = new Box(imageRows, imageCols, 0, 0);
			newNode = new BoxNode(3, box);
		
			while (morphologyArray[i] > 0) {
				for (int j = minCol; j <= maxCol; j++) {
					if (direction.compare("horizontal") == 0) {
						if (imageArray[i][j] > 0) {

							if (box->getMinRow() > i) {
								box->setMinRow(i);
							}
							if (box->getMinCol() > j) {
								box->setMinCol(j);
							}
							if (box->getMaxRow() < i) {
								box->setMaxRow(i);
							}
							if (box->getMaxCol() < j) {
								box->setMaxCol(j);
							}
						}
					}
					else if (direction.compare("vertical") == 0) {
						if (imageArray[j][i] > 0) {
							if (box->getMinRow() > j) {
								box->setMinRow(j);
							}
							if (box->getMinCol() > i) {
								box->setMinCol(i);
							}
							if (box->getMaxRow() < j) {
								box->setMaxRow(j);
							}
							if (box->getMaxCol() < i) {
								box->setMaxCol(i);
							}
						}
					}
				}
				++i;
			}
			boxQueue->insert(newNode);
		}
	}
}

void Image::printBoxQueue(ofstream& output)
{
	try {
		output << "Queue sequence:" << endl;
		BoxNode* current = boxQueue->getQueueFront()->getNext();
		while (current != nullptr) {
			output << current->getBoxType() << endl;
			output << current->getBBox()->getMinRow() << " " << current->getBBox()->getMinCol() << " ";
			output << current->getBBox()->getMaxRow() << " " << current->getBBox()->getMaxCol() << endl;
			current = current->getNext();
		}
		output << endl;
	}
	catch (exception e) {
		cout << "There is an error: " << e.what();
	}
}

void Image::drawBoxes(BoxQueue* boxQueue, int** imageArray)
{
	BoxNode* current = boxQueue->getQueueFront()->getNext();
	int minRow, minCol, maxRow, maxCol;

	while (current != nullptr) {

		minRow = current->getBBox()->getMinRow();
		minCol = current->getBBox()->getMinCol();
		maxRow = current->getBBox()->getMaxRow();
		maxCol = current->getBBox()->getMaxCol();

		for (int i = minRow; i <= maxRow; i++) {
			imageArray[i][minCol] = 1;
			imageArray[i][maxCol] = 1;
		}
		for (int i = minCol; i <= maxCol; i++) {
			imageArray[minRow][i] = 1;
			imageArray[maxRow][i] = 1;
		}
		current = current->getNext();
	}
	
}

void Image::prettyPrint(ofstream& output)
{
	try {
		for (int i = 0; i < imageRows; i++) {
			for (int j = 0; j < imageCols; j++) {
				if (imageArray[i][j] > 0) {
					output << imageArray[i][j] << " ";
				}
				else {
					output << "  ";
				}
			}
			output << endl;
		}
		output << endl;
	}
	catch (exception e) {
		cout << "There is an error: " << e.what() << endl;
	}
}

int Image::getImageRows()
{
	return this->imageRows;
}

void Image::setImageRows(int imageRows)
{
	this->imageRows = imageRows;
}

int Image::getImageCols()
{
	return this->imageCols;
}

void Image::setImageCols(int imageCols)
{
	this->imageCols = imageCols;
}

int Image::getImageMin()
{
	return this->imageMin;
}

void Image::setImageMin(int imageMin)
{
	this->imageMin = imageMin;
}

int Image::getImageMax()
{
	return this->imageMax;
}

void Image::setImageMax(int imageMax)
{
	this->imageMax = imageMax;
}
