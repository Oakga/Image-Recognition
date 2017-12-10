#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

class imagePP;
class BBox;
struct boxNode;

struct boxNode
{
    int type; // 1 for img box, 2 for text-line box, 3 for text-word box, etc.
    int minRow;
    int minCol;
    int maxRow;
    int maxCol;
    boxNode *nextBox;
    boxNode()
    {
        type = 1;
        minRow = minCol = maxRow = maxCol = 0;
        nextBox = 0;
    };
    boxNode(int type_, int minRow_, int minCol_, int maxRow_, int maxCol_)
    {
        type = type_;
        minRow = minRow_;
        minCol = minCol_;
        maxRow = maxRow_;
        maxCol = maxCol_;
    };

    string boxDescription()
    {
        return to_string(minRow) +" "+ to_string(minCol) +" "+ to_string(maxRow) + " "+ to_string(maxCol);
    };
};
class imagePP
{
  public:
    int numRows, numCols, minVal, maxVal;
    int threshold;
    int **imageAry;
    int *HPP;
    int *HPPbin;
    int *VPP;
    int *VPPbin;
    ifstream imageScan;
    ofstream out;
    imagePP(string binaryImage, string thresholdValue, string output)
    {
        threshold = stoi(thresholdValue);
        imageScan.open(binaryImage);
        out.open(output);
        out << "Threshold Value: "<< thresholdValue << endl; 
        imageScan >> numRows >> numCols >> minVal >> maxVal;

        imageAry = new int *[numRows];
        for (int i = 0; i < numRows; i++)
        {
            imageAry[i] = new int[numCols];
        }
        HPP = new int[numRows]();
        HPPbin = new int[numRows]();
        VPP = new int[numCols]();
        VPPbin = new int[numCols]();
    };
    ~imagePP()
    {
        imageScan.close();
        out.close();
        for (int i = 0; i < numRows; i++)
        {
            delete[] imageAry[i];
        }
        delete[] imageAry;
        delete[] HPP;
        delete[] HPPbin;
        delete[] VPP;
        delete[] VPPbin;
    }

    void printBox(boxNode* box, string num)
    {
        out << "num" << endl;
        out << box->boxDescription() << endl;
    };
    void loadImage()
    {
        for (int i = 0; i < numRows; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                imageScan >> imageAry[i][j];
            }
        }
    }

    void computePP(boxNode *&box)
    { 
        for (int i = box->minRow; i < box->maxRow; i++)
        {
            for (int j = box->minCol; j < box->maxCol; j++)
            {
                if (imageAry[i][j] > 0)
                {
                        HPP[i]++;
                        VPP[j]++;
                }
            }
        }

        //printing
        printPP(HPP, numRows, "Horizontal Projection Profile");
        printPP(VPP, numCols, "Vertical Projection Profile");
    };

    void printPP(int *arr, int size, string name)
    {
        out << "\n" << name << "\n";
        for (int j = 0; j < size; j++)
        {
            out << arr[j] << " ";
        }
        out << "\n";
    };

    void thresholding(const int size)
    {
        //check which array we want to threshold i.e VPP or HPP
        int *arr;
        int *arrBin;
        string arrName;
        if (size == numRows)
        {
            arr = HPP;
            arrBin = HPPbin;
            arrName = "Binary Horizontal Projection Profile";
        }
        else
        {
            arr = VPP;
            arrBin = VPPbin;
            arrName = "Binary Vertical Projection Profile";
        };

        //thresholding
        for (int i = 0; i < size; i++)
        {
            if (arr[i] < threshold)
                arrBin[i] = 0;
            else
                arrBin[i] = 1;
        }

        //printing
        printPP(arrBin, size, arrName);
    }

    int findReadingDir()
    {
        int HPPboxCt = countBoxes(numRows);
        int VPPboxCt = countBoxes(numCols);
        if (HPPboxCt > VPPboxCt){
            out << "\nReading Direction: HORIZONTAL"<< endl;
            return 1; // horinzontal
        }
        else {
            out << "\nReading Direction: VERTICAL"<< endl;
            return 0; //vertical
        }
    }

    int countBoxes(const int size)
    {
        //check which array we want to count i.e VPPbin or HPPbin
        int *arrBin;
        string arrName;
        if (size == numRows)
        {
            arrBin = HPPbin;
        }
        else
        {
            arrBin = VPPbin;
        };

        //counting
        int index, boxCount, counterZero, counterOne;
        index = boxCount = 0;
        do
        {
            counterZero = counterOne = 0;
            while (arrBin[index] <= 0)
            {
                index++;
                counterZero++;
            };
            //make sure no overflow
            if (index < size)
            {
                while (arrBin[index] > 0)
                {
                    index++;
                    counterOne++;
                }
            }
            // this ensure that each boxes are regex [ 0 0* 1 1* ]
            if (counterZero > 0 && counterOne > 0) boxCount++;
        } while (index < size);
        return boxCount;
    }
};
class BBox
{
    friend class imagePP;

  public:
    boxNode *imgBox;
    BBox()
    {
        imgBox = NULL;
    }
    ~BBox()
    {
        free(imgBox);
    }
    class boxList
    {
      public:
        boxNode *listHead;
        boxNode *last;
        boxList()
        {
            boxNode *dummy = new boxNode;
            listHead = last = dummy;
        }
        ~boxList()
        {
            freeList();
        }
        void insertLast(boxNode *box)
        {
            boxNode *lastBox = last;
            lastBox->nextBox = box;
            last = box;
        }

        void freeList()
        {
            boxNode *spot = listHead;
            while (listHead != 0)
            {
                spot = listHead;
                listHead = listHead->nextBox;
                free(spot);
            }
        }
        static void printList(boxList *box)
        {
            boxNode *spot = box->listHead->nextBox;
            cout << "\n";
            cout << "ListHead-->";
            while (spot != 0)
            {
                cout << "(" << to_string(spot->type) << ")-->";
                spot = spot->nextBox;
            }
            cout << "NULL\n";
            delete spot;
        }
    };

    boxNode* findImgBox(imagePP &img)
    {   
        bool firstTime = true;
        for (int i = 0; i < img.numRows; i++)
        {
            for (int j = 0; j < img.numCols; j++)
            {
                if (img.imageAry[i][j] > 0)
                {
                    // intialized the imgBox class object if first time
                    if (firstTime == true)
                    {
                        firstTime = false;
                        imgBox = new boxNode();
                        imgBox->minRow = imgBox->maxRow = i;
                        imgBox->minCol = imgBox->maxCol = j;
                    };
                    //otherwise update the object
                    compare(imgBox, i, j);
                };
            };
        };
        img.printBox(imgBox);
        return imgBox;
    };

    //update the bouding box of the image if new max for row and col are found
    void compare(boxNode *&box, const int row, const int col)
    {
        if (row < box->minRow)
            box->minRow = row;
        if (row > box->maxRow)
            box->maxRow = row;
        if (col < box->minCol)
            box->minCol = col;
        if (col > box->maxCol)
            box->maxCol = col;
    };

    static boxList findLineBoxes(imagePP &img, boxNode *&imgBox, const int dirOfPP, int *PP, const int PPSize)
    {
        int minCol, maxCol = 0;
        int minRow, maxRow = 0;
        boxList boxHead;
        int index = 0;
        while (index < PPSize)
        {
            //skip all 0s
            while (PP[index] <= 0)
                index++;
            
            //found starting point
            if( dirOfPP == 1) minRow = index;  //given PP is HPPbin and dirOfPP 1 is horizontal
            else minCol = index;

            //avoid overflow
            if (index < PPSize)
            {
                //skip all 1s
                while (PP[index] > 0)
                    index++;
                
                // found the last point of the boundary: bounding box is now complete
                if( dirOfPP == 1) { 
                    maxRow = index - 1; 
                    // col is consistant
                    boxNode *newLineBox = new boxNode(2, minRow, imgBox->minCol, maxRow, imgBox->maxCol);
                    img.printBox(newLineBox);
                    boxHead.insertLast(newLineBox);
                }
                else { 
                    maxCol = index - 1;
                    //row is consistant
                    boxNode *newLineBox = new boxNode(2, imgBox->minRow, minCol, imgBox->maxRow, maxCol);
                    img.printBox(newLineBox);
                    boxHead.insertLast(newLineBox);
                }
            };
        }

        return boxHead;
    };

    static void findWordBoxes(boxList lineList, boxNode *&lineBox, const int dirOfPP, int *PP, const int PPSize)
    {
        int minCol, maxCol = 0;
        int minRow, maxRow = 0;
        boxList boxHead;
        int index = 0;
        while (index < PPSize)
        {
            //skip all 0s
            while (PP[index] <= 0)
                index++;
            
            //found starting point
            if( dirOfPP == 1) minRow = index;  //given PP is HPPbin and dirOfPP 1 is horizontal
            else minCol = index;

            //avoid overflow
            if (index < PPSize)
            {
                //skip all 1s
                while (PP[index] > 0)
                    index++;
                
                // found the last point of the boundary: bounding box is now complete
                if( dirOfPP == 1) { 
                    maxRow = index - 1; 
                    // col is consistant
                    boxNode *newBox = new boxNode(3, minRow, lineBox->minCol, maxRow, lineBox->maxCol);
                    boxHead.insertLast(newBox);
                }
                else { 
                    maxCol = index;
                    //row is consistant
                    boxNode *newBox = new boxNode(3, lineBox->minRow, minCol, lineBox->maxRow, maxCol);
                    boxHead.insertLast(newBox);
                }
            };
        }
    };
};
int main(int argc, char *argv[])
{
    //intializations
    string dirInEng;
    int readingDir;
    BBox boxProcessor;
    imagePP textImage(argv[1], argv[2], argv[3]);
    textImage.loadImage();

    //find text image bouding box
    boxProcessor.findImgBox(textImage); //image box

    // compute HPP and VPP
    textImage.computePP(boxProcessor.imgBox);

    //thresholding HPP and VPP with user input
    textImage.thresholding(textImage.numRows);
    textImage.thresholding(textImage.numCols);

    //determine reading dirOfPP
    readingDir = textImage.findReadingDir();
    
    //print out the bouding box
    textImage.printBox(boxProcessor.imgBox, "1");
    if (readingDir == 0)
    {
        BBox::boxList lineList = BBox::findLineBoxes(textImage, boxProcessor.imgBox, 0, textImage.VPPbin, textImage.numCols);
    }
    else {
        BBox::boxList lineList = BBox::findLineBoxes(textImage, boxProcessor.imgBox, 1, textImage.HPPbin, textImage.numRows);
    }
};    