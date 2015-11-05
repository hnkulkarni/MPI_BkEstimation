/*
 * CPPM.h
 *
 *  Created on: May 9, 2012
 *      Author: hrushikesh
 *
 *      Reads and Writes PPM Files from and into cv::Mat
 */

#ifndef CPPM_H_
#define CPPM_H_

#include "Includes.h"

class CPPM
{

private:
	ofstream ToPPMFile;
	ifstream FromPPMFile;

public:
	CPPM();

#ifdef CV

	//Write a PPM File
	int WritePPMFile( string PPMFileName, cv::Mat Image );

#endif

	//Read a PPM File
	int ReadPPMFile(  string PPMFileName, vector<int>& ImageStrip );

	int GetSampleImageParams(string PPMFileName , int  *iRows, int *iCols );

	int WriteStrip( string PPMFileName , vector<int> imBackgrnd , int iNumRows, int iNumCols );

	virtual ~CPPM();
};

#endif /* CPPM_H_ */
