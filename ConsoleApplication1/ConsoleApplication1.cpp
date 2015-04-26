#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <iostream>
#include <cstdlib>
#include "medianFilterQS.h"
#include <algorithm>

int windowSize;
int windowLengthMax;
int windowLengthMin;

int colLength;
int rowLength;

int maxValue;

int* bucketArray;

int** mainArray;
int** newArray;

void clearBucket()
{
	for (int i = 0; i < maxValue; i++)
	{
		bucketArray[i] = 0;
	}
}

int getAverageBucket(int c)
{
	int count = 0;
	if (c % 2 == 0)
	{
		int firstIndex = c / 2;
		int secondIndex = firstIndex + 1;

		int tempReturn = 0;
		bool foundFirst = false;

		for (int i = 0; i < maxValue; i++)
		{
			count += bucketArray[i];

			if (count == firstIndex && !foundFirst)
			{
				tempReturn = i;
				foundFirst = true;
			}
			else if (count >= secondIndex)
			{
				if (!foundFirst)
				{
					return i;
				}
				else
				{
					return (i + tempReturn) / 2;
				}
			}
		}
	}
	else
	{
		int index = c / 2;

		for (int i = 0; i < maxValue; i++)
		{
			count += bucketArray[i];

			if (count > index)
			{
				return i;
			}
		}
	}
}

void runBucketSort(){
	for (int i = 0; i < rowLength; i++)
	{
		for (int j = 0; j < colLength; j++)
		{
			int count = 0;

			for (int y = windowLengthMin; y < windowLengthMax; y++)
			{
				for (int x = windowLengthMin; x < windowLengthMax; x++)
				{
					if (i + y >= 0 && j + x >= 0 && i + y < rowLength && j + x < colLength)
					{
						bucketArray[mainArray[i + y][j + x]]++;
						count++;
					}
				}
			}

			newArray[i][j] = getAverageBucket(count);
			clearBucket();
		}
	}
}

std::queue<int> separateByComma(std::string string)
{
	std::queue<int> ret;
	std::string temp = "";

	for (int i = 0; i < string.length(); i++)
	{
		if (string[i] != ' ')
		{
			temp += string[i];
		}
		else
		{
			ret.push(std::stoi(temp));
			temp = "";
		}
	}

	if (!temp.empty())
	{
		ret.push(std::stoi(temp));
	}

	return ret;
}

void writeToFile(std::string fileName, std::string windowSize, std::queue<std::string> comments)
{
	std::ofstream myfile;
	int cLength = comments.size();

	int lastindex = fileName.find_last_of(".");
	fileName = fileName.substr(0, lastindex);

	myfile.open(fileName + "_processed_" + windowSize + ".pgm");
	//Write file type
	myfile << "P2\n";
	//Write comments
	for (int i = 0; i < cLength; i++)
	{
		myfile << comments.front() + "\n";
		comments.pop();
	}
	//Write sizes
	myfile << colLength << " " << rowLength << "\n";
	myfile << maxValue << "\n";
	//Write new values
	for (int i = 0; i < rowLength; i++)
	{
		for (int j = 0; j < colLength; j++)
		{
			myfile << newArray[i][j] << " ";
		}
		myfile << "\n";
	}

	myfile.close();
}

void mainQuickSort(std::string fileLocation,std::string windowSizeString)
{
	openfile(fileLocation);
	QS(windowSize, fileLocation, windowSizeString);
}

void mainBucketSort(std::string fileLocation, std::string windowSizeString){
	std::ifstream infile(fileLocation);

	std::string line;

	bool gotRowsCols = false;

	std::queue<std::string> comments;

	//Setup and initialization
	while (std::getline(infile, line))
	{
		if (line == "P2")
		{
			continue;
		}
		else if (line.at(0) == '#')
		{
			comments.push(line);
			continue;
		}
		else if (!gotRowsCols)
		{
			std::queue<int> vecColsRows = separateByComma(line);
			colLength = vecColsRows.front();
			vecColsRows.pop();
			rowLength = vecColsRows.front();
			vecColsRows.pop();
			gotRowsCols = true;
		}
		else if (gotRowsCols)
		{
			maxValue = std::stoi(line);
			break;
		}
		else
		{
			return;
		}
	}

	//Create Main Array
	mainArray = new int*[rowLength];
	newArray = new int*[rowLength];
	for (int i = 0; i < rowLength; ++i)
	{
		mainArray[i] = new int[colLength];
		newArray[i] = new int[colLength];
	}

	//Create Bucket Array
	bucketArray = new int[maxValue];
	clearBucket();

	//Iteration variables
	int colIter = 0;
	int rowIter = 0;

	while (std::getline(infile, line))
	{
		//Create a queue of integers from the line
		std::queue<int> csvLine = separateByComma(line);

		//Get the length of the queue
		int iterLength = csvLine.size();

		//Iterate through queue and add it to the 2d-array
		for (int i = 0; i < iterLength; i++)
		{
			if (colIter == colLength)
			{
				colIter = 0;
				rowIter++;
			}

			mainArray[rowIter][colIter++] = csvLine.front();
			csvLine.pop();
		}
	}

	//Run Algorithm
	clock_t startTime = clock();
	runBucketSort();

	//Display Time
	printf("Bucket Sort Time : %f seconds\n", ((float) clock() - startTime) / CLOCKS_PER_SEC);

	//Write to file
	writeToFile(fileLocation, windowSizeString, comments);
}

void mainInsertionSort()
{

}

int _tmain(int argc, _TCHAR* argv [])
{
	if (argc >= 3)
	{
		std::string windowSizeString(argv[1]);
		windowSize = std::stoi(windowSizeString);
		windowLengthMax = (windowSize / 2) + 1;
		windowLengthMin = -(windowSize / 2);
		
		std::string fileLocation(argv[2]);

		if (argc >= 4)
		{
			std::string algorithmString(argv[3]);
			if (algorithmString.find("insertion") != -1)
			{
				//mainInsertionSort();
			}
			else if (algorithmString.find("quicksort") != -1)
			{
				mainQuickSort(fileLocation, windowSizeString);
			}
			else
			{
				mainBucketSort(fileLocation, windowSizeString);
			}
		}
		else
		{
			mainBucketSort(fileLocation, windowSizeString);
		}
	}

	return 0;

}