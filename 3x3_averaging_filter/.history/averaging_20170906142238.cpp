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
        scan >> numRows >> numCols >> minVal >> maxVal;
        mirrorFramedAry = new int*[numRows+2];
        tempAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; i++){
            mirrorFramedAry[i] = new int[numCols+2];
            tempAry[i] = new int[numCols+2];
        }
    }
    ~Averaging(){
        for (int i = 0; i < numRows+2; i++) {
            delete [] mirrorFramedAry[i];
            delete [] tempAry[i];
        }
        delete[] mirrorFramedAry;
        delete [] tempAry;
        out.close();
        scan.close();
    }
    void loadImage()
    {
        for(int i = 1;i < numRows+1;i++){
            for(int j=1;j< numCols+1;j++){
                scan >> mirrorFramedAry[i][j];
                }
        }
        print();
    }
    void mirrorFramed(){
        //columns first
        for(int i=0;i<numRows;i++){
            mirrorframedAry[i][0]=mirrorFramedAry[i][1];//left
            mirrorframedAry[i][numCols+1]=mirrorFramedAry[i][numCols];//right
        }

        //rows
        for(int i=0;i<nnumCols;i++){
            mirrorframedAry[0][i]=mirrorFramedAry[1][i];//top
            mirrorframedAry[numRows+1][i]=mirrorFramedAry[numRows][i];//bottom
        }
    }
private:
    void print(){
        for(int i = 1;i < numRows+1;i++){
            for(int j=1;j< numCols+1;j++){
                out << mirrorFramedAry[i][j]<<" ";
                }
                out<< endl;
        }
    }

};
int main(int argc, char *argv[])
{
    Averaging processor(argv[1], argv[2]);
    processor.loadImage();
    return 0;
}