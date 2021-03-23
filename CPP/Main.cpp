#include <iostream>
#include <fstream>
#include <string>

#include "Image.h"

using namespace std;



int main(int argc, char** argv)
{
	
	string imageFileName;
	string thresholdValue;
	string boxedTextLineFileName;
	string debugingFileName;

	if (argc != 5) {
		cout << "Invalid number of arguments." << endl;
		cout << "There should be data file, mask file, averageFilterOutput file, gaussianFilterOutputFile, amd medianFilterOutputFile." << endl;
		exit(1);
	}
	try {

		imageFileName = argv[1];
		thresholdValue = argv[2];
		boxedTextLineFileName = argv[3];
		debugingFileName = argv[4];

		ifstream imageFile;
		int threshold = 0;
		ofstream boxedTextLine;
		ofstream debuging;

		imageFile.open(imageFileName);
		threshold = stoi(thresholdValue);
		boxedTextLine.open(boxedTextLineFileName);
		debuging.open(debugingFileName);

		Image* image = new Image(imageFile, threshold);
		image->loadImage(imageFile);
		image->computeProjectionProfile("horizontal", image->horizontalProjection);
		image->computeProjectionProfile("vertical", image->verticalProjection);
		debuging << "Computation of Horizontal Projection Profile:" << endl;
		image->printProjectionProfile("horizontal", image->horizontalProjection, debuging);
		debuging <<"\nComputation of Vertical Projection Profile:" << endl;
		image->printProjectionProfile("vertical", image->verticalProjection, debuging);

		image->thresholding("horizontal", image->horizontalProjection, image->binaryHorizontalProjection);
		debuging << "\nComputation of Binary Horizontal Projection Profile:" << endl;
		image->printProjectionProfile("horizontal", image->binaryHorizontalProjection, debuging);

		image->thresholding("vertical", image->verticalProjection, image->binaryVerticalProjection);
		debuging << "\nComputation of Binary Vertical Projection Profile:" << endl;
		image->printProjectionProfile("vertical", image->binaryVerticalProjection, debuging);


		image->morphologicalClosing(image->horizontalProjectionMorphology, image->binaryHorizontalProjection, image->getImageRows() + 2);
		image->morphologicalClosing(image->verticalProjectionMorphology, image->binaryVerticalProjection, image->getImageCols() + 2);

		image->determineDirection(debuging);


		if (image->direction.compare("horizontal") == 0) {
			image->findLineBoxes("horizontal", image->horizontalProjection);
		}

		else if (image->direction.compare("vertical") == 0) {
			image->findLineBoxes("vertical", image->verticalProjection);
		}

		image->drawBoxes(image->boxQueue, image->imageArray);
		image->prettyPrint(boxedTextLine);
		image->printBoxQueue(debuging);

		imageFile.close();
		boxedTextLine.close();
		debuging.close();

	}
	catch (exception e) {
		cout << "There is an error: " << e.what();
	}
	
	return 0;
}