#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;


int inputarray[1920][1080];		//max size of image supported
int outputArray[1920][1080];	//max size of image supported
std::vector <int> v;
int Partition(vector<int> & data, int low, int high);
int selectedRows;
int currentRow;
int selectedColumns;
int currentColumn;
int counter;
int grayvalue;
int filewidth;
int fileheight;
int median;
int size;
int filltree;
ifstream pgmFI;
string getfilesize[8];
string getHeader;
string intfilter2;
size_t vSize;

void openfile(string a){
	pgmFI.open(a); // opens pgm file
	if (pgmFI.is_open()){      // if pgm file opens
			{
				for (int i = 0; i < 8; i++){        //gets height and width of pgm 
					pgmFI >> getfilesize[i];		//file from line 3.
				}
				getHeader = getfilesize[5];			//file width
				std::istringstream width(getHeader);	//converts from string to int
				width >> filewidth;					//width to use for output file

				getHeader = getfilesize[6];			//file height
				std::istringstream height(getHeader);	//converts from string to int
				height >> fileheight;					//height to use for output file

				getHeader = getfilesize[7];			//RGB value
				std::istringstream rgb(getHeader);	//converts from string to int
				rgb >> grayvalue;					//RGB value to use for output file
			}

	}
	if (pgmFI.is_open()){									//checks if file is open
		for (int i = 0; i < fileheight; i++){
			for (int j = 0; j < filewidth; j++){

				pgmFI >> intfilter2;						// load array;
				std::istringstream intfilter(intfilter2);	// converts string to int
				intfilter >> filltree;						// put into filltree
				inputarray[i][j] = filltree;
			}
		}
	}
}

//quick sort function
void swap(vector<int> & data, int i, int j)
{
	int tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
}

//quick sort function
void QSort(vector<int> & data, int small, int large)
{
	if (small >= large) { return; } //stop when small is >= large
	int val = Partition(data, small, large);
	QSort(data, small, val - 1);	//repeat new partitions
	QSort(data, val + 1, large);	//repeat new partitions
}

//quick sort function
int Partition(vector<int> & data, int small, int large)
{
	int a = small;
	for (int i = a + 1; i <= large; ++i)
	{
		if (data[i] < data[a])
		{
			swap(data, i, a);
			if (i != a + 1)
			{
				swap(data, i, a + 1);
			}
			a = a + 1;
		}
	}
	return a;
}

//main function for quick sort
void QS(int a, string name, string b){
	size = a;
	clock_t begin = clock();

	// locates bounding box around selected pixel
	for (int i = 0; i < fileheight; i++){
		selectedRows = i - (size / 2);

		for (int j = 0; j < filewidth; j++){
			selectedColumns = j - (size / 2);

			for (int k = 0; k < size; k++){
				currentRow = selectedRows + k;

				for (int l = 0; l < size; l++){
					currentColumn = selectedColumns + l;

					if (currentRow >= 0 && currentRow < fileheight && currentColumn >= 0 && currentColumn < filewidth){
						v.push_back(inputarray[currentRow][currentColumn]);
					}
				}
			}
			QSort(v, 0, v.size() - 1); //Quick sort function 
			vSize = v.size();

			// finds median of vector
			if ((vSize % 2) == 1){
				median = v[vSize / 2];
				outputArray[i][j] = median;
			}
			if ((vSize % 2) == 0){
				median = ((v[vSize / 2] + v[(vSize / 2) - 1]) / 2);
				outputArray[i][j] = median;
			}
			v.clear();
		}
	}
	clock_t end = clock();

	if (counter == 0){
		ofstream pgmfileout(name.substr(0, name.size() - 4) + "_processed_" + b + ".pgm"); //create output file

		if (pgmfileout.is_open()){

			// Create Header of file
			pgmfileout << "P2" << endl;
			pgmfileout << "# Created by IrfanView" << endl;
			pgmfileout << filewidth << " " << fileheight << endl;	//put width and height values into new file
			pgmfileout << grayvalue << endl;						//put RGB value into new file

			// Start pixel value input
			for (int i = 0; i < fileheight; i++){
				for (int j = 0; j < filewidth; j++){
					pgmfileout << outputArray[i][j];
					pgmfileout << " ";
					counter++;
					if (counter == filewidth){	// at end of line go go to next
						pgmfileout << endl; // go to next line of pgm
						counter = 0;		// reset counter
					}
				}
			}
		}
		float elapsed = end - begin;
		cout << "QS time: " << elapsed << endl;
	}
}

