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
    ifstream scan;
    ofstream out;

    Averaging(string inFile, string outFile)
    {
        scan.open(inFile);
        out.open(outFile);
    }

    void loadImage()
    {
        scan >> numRows >> numCols >> minVal >> maxVal;
        int** mirrorFramedAry = new int*[numRows+2];
        int** tempAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; i++){
            mirrorFramedAry[i] = new int[numCols+2];
            tempAry[i] = new int[numCols+2];
        }

        for(int i = 1;i < numRows+1;i++){
            for(int j=1;j< numCols+1;j++){
                scan >> mirrorFramedAry[i][j];
                }
            }
        }
        cout << mirrorFramedAry[1][2];
        for(int i = 1;i < numRows+1;i++){
            for(int j=1;j< numCols+1;j++){
                out<< mirrorFramedAry[i][j] <<" ";
            }
            out<<endl;
        }
    }
    void print(){
        for(int i = 1;i < numRows+1;i++){
            for(int j=1;j< numCols+1;j++){
                out<< mirrorFramedAry[i][j] <<" ";
            }
            out<<endl;
        }
    }
    void mirrorFramed(){
    }
    void deallocation(){
        // for (int i = 0; i < numRows+2; i++) {
        //     delete [] mirrorFramedAry[i];
        // }
        // delete[] mirrorFramedAry;

        // for (int i = 0; i < numRows+2; i++) {
        //     delete [] tempAry[i];
        // }
        // delete [] tempAry;

        scan.close();
        out.close();

    }
};
int main(int argc, char *argv[])
{
    Averaging processor(argv[1], argv[2]);
    processor.loadImage();
    processor.deallocation();
    return 0;
}