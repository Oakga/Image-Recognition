#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Averaging
{
public:
    int **mirrorFramedAry;
    int **tempAry;
    int neighborAry[9];
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMin;
    int newMax;
    string inFile;
    string outFile;

    Averaging(string inFile, string outFile)
    {
        this->inFile = inFile;
        this->outFile = outFile;
    }

    void computeHistogram()
    {
        scan.open(inFile);
        out.open(outFile);
        scan >> numRows >> numCols >> minVal >> maxVal;
        histogram = new int[maxVal + 1]();
        int pixel, maxPlus;
        while (scan >> pixel)
        {
            histogram[pixel]++;
        }
        out << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;

        for (int i = 0; i < maxVal; i++)
        {
            out << "(" << i << "):"<< " " << histogram[i];
            if (histogram[i] > 80)
                maxPlus = 80;
            else
                maxPlus = histogram[i];
            for (int j = 0; j < maxPlus; j++)
            {
                out << "+";
            }
            out << endl;
        }
        out << endl;
        scan.close();
        out.close();
    }
};
int main(int argc, char *argv[])
{
    Averaging processor(argv[1], argv[2]);
    processor.computeHistogram();
    return 0;
}