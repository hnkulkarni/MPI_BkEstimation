/*
 * CWork.h
 *
 *  Created on: May 9, 2012
 *      Author: hrushikesh
 */

#ifndef CWORK_H_
#define CWORK_H_

#include "Includes.h" // Contains All the Includes.

#include "CPPM.h"

class CWork :
public CPPM
{
private:

#ifdef cv
	cv::VideoCapture Camera; //Capture Handle
	cv::Mat Camframe; //Stores the Captured Frame From the Camera.
	cv::Mat CamGrayFrame; //Stores the Gray Frame;
#endif

	stringstream PPMFileName;   //Stores the File Name for the PPM

	vector< vector< int > > StripCatImage; // Concatenated Strips of Images.
	vector<int> StripImage;


	int iPPMRows;
	int iPPMCols;

	vector<int> vToSort;

	int iNumProcesses;
	int rank;

	int *Medians;

	int iNthElement;


	vector<int> imBackground;

public:

	int iTotalImages;

	CWork();
	virtual ~CWork();

	//Captures and saves files in the DirPath
	int CaptureImages(string DirPath ,  int iNumCaptures );

	//Reads files from the DirPath
	int ReadFiles( string DirPath , int iNumReads  );

	//Concatenate Images, i.e cat strip at col iNumIamge.
	int CatImages(vector< vector< int > > & CatImage , vector<int> Strip , int iNumImage );


	//Allocate the Data to Sort
	int AllocateSortVector();

	//Scatter Data
	int ScatterData(int iStartRows, int iEndRows);

	//Find Nth - Element
	int FindNthElement();

	//Gather Nth Elements
	//int GatherNthElement();

	int FindBackground();

	//Broadcast Parameters
	int BroadCastParams();

	//Write the Data;
	int WriteData( string DirPath );

};

#endif /* CWORK_H_ */
