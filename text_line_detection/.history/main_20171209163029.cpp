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
    void printbox()
    {
        printf("\nBOX: %u %u %u %u %u ", type, minRow, minCol, maxRow, maxCol);
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

    void computePP(boxNode &box)
    { //compuyte VPP and HPP combined
        for (int i = box.minRow; i < box.maxRow; i++)
        {
            for (int j = box.minCol; j < box.maxCol; j++)
            {
                if (imageAry[i][j] > 0)
                {
                    HPP[i]++;
                    VPP[j]++;
                }
            }
        }

        //printing
        printPP(HPP, numRows, "HOR");
        printPP(VPP, numCols, "VER");
    };

    void clearArrays()
    {
        memset(HPP, 0, sizeof(int) * numRows);
        memset(HPPbin, 0, sizeof(int) * numRows);
        memset(VPP, 0, sizeof(int) * numCols);
        memset(VPP, 0, sizeof(int) * numCols);
    };

    void printPP(int *arr, int size, string name)
    {
        cout << "\n" << name << ":";
        for (int j = 0; j < size; j++)
        {
            cout << arr[j] << " ";
        }
        cout << "\n";
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
            arrName = "HOR";
        }
        else
        {
            arr = VPP;
            arrBin = VPPbin;
            arrName = "VER";
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
        // int VPPboxCt = countBoxes(numCols);
        int VPPboxCt = 5;
        printf("\nBOXCOUNT H: %u V: %u", HPPboxCt, VPPboxCt);
        if (HPPboxCt > VPPboxCt)
            return 1; // horinzontal
        else
            return 0; //vertical
    }

    int countBoxes(const int size)
    {
        //check which array we want to count i.e VPPbin or HPPbin
        int *arrBin;
        string arrName;
        if (size == numRows)
        {
            arrBin = HPPbin;
            arrName = "HOR";
        }
        else
        {
            arrBin = VPPbin;
            arrName = "VER";
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
    // boxNode *imgBox;
    // BBox()
    // {
    //     imgBox = NULL;
    // }
    // ~BBox()
    // {
    //     free(imgBox);
    // }
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

        void printList()
        {
            boxNode *spot = listHead->nextBox;
            cout << "\n";
            cout << "ListHead-->";
            while (spot != 0)
            {
                cout << "(";
                spot->printbox();
                cout << ") -->";
                spot = spot->nextBox;
            }
            cout << "NULL\n";
            delete spot;
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

    static boxNode findImgBox(int **&imgAry, const int numRows, const int numCols)
    {
        bool firstTime = true;
        boxNode box;
        for (int i = 0; i < numRows; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                if (imgAry[i][j] > 0)
                {
                    // intialized the imgBox class object if first time
                    if (firstTime == true)
                    {
                        firstTime = false;
                        box.minRow = box.maxRow = i;
                        box.minCol = box.maxCol = j;
                    };
                    //otherwise update the object
                    compare(box, i, j);
                };
            };
        };
        box.printbox();
        return box;
    };

    //update the bouding box of the image if new max for row and col are found
    static void compare(boxNode &box, const int row, const int col)
    {
        if (row < box.minRow)
            box.minRow = row;
        if (row > box.maxRow)
            box.maxRow = row;
        if (col < box.minCol)
            box.minCol = col;
        if (col > box.maxCol)
            box.maxCol = col;
    };

    /* for example: if reading dirOfPP is horizontal, we need HPP for line boxes
    column is consistant, row will be changing for each line
    and dirOfPP in this case will be dirOfPP of PP and thus, dirOfPP is 1
    in summary:
    if PP is  VPPbin , dirOfPP is 0 horizontal 
    if PP is  HPPbin, dirOfPP is 1 vertical
    */
    boxList findLineBoxes(boxNode &imgBox, const int dirOfPP, int *PP, const int PPSize)
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
                    boxNode *newBox = new boxNode(2, minRow, imgBox.minCol, maxRow, imgBox.maxCol);
                    boxHead.insertLast(newBox);
                }
                else { 
                    maxCol = index;
                    //row is consistant
                    boxNode *newBox = new boxNode(2, imgBox.minRow, minCol, imgBox.maxRow, maxCol);
                    boxHead.insertLast(newBox);
                }
            };
        }

        boxHead.printList();
        return boxHead;
    };
};
int main(int argc, char *argv[])
{
    //intializations
    string dirInEng;
    int readingDir;
    imagePP textImage(argv[1], argv[2], argv[3]);
    textImage.loadImage();

    //find text image bouding box
    boxNode box = BBox::findImgBox(textImage.imageAry, textImage.numRows, textImage.numCols); //image box

    // compute HPP and VPP
    textImage.computePP(box);

    cout << "After threshold";
    //thresholding HPP and VPP with user input
    textImage.thresholding(textImage.numRows);
    textImage.thresholding(textImage.numCols);

    //determine reading dirOfPP
    readingDir = textImage.findReadingDir();
    if (readingDir == 1) dirInEng = "horizontal";
    else dirInEng = "vertical";
    cout << "\nReading DIR:" << dirInEng << "\n";
    //find text-line bouding boxes
    if (readingDir == 1)
    {
        BBox::boxList lineList = BBox::findLineBoxes(box, 1, textImage.VPPbin, textImage.numRows);










        
//         BBox::boxList lineList = box.findLineBoxesHorizontal(textImage.HPPbin, textImage.numRows);
//         lineList.printList();
//         BBox::boxList wordList;
//         boxNode *lineWalker = lineList.listHead->nextBox;
//         while (lineWalker != 0)
//         {
//             textImage.clearArrays();
//             lineWalker->printbox();
//             textImage.computePP(lineWalker);
//             textImage.thresholdVertical();
//             lineWalker = lineWalker->nextBox;

//             cout << endl;
//         }
//         free(lineWalker);
//     }
//     else
//     {
//         BBox::boxList lineList = box.findLineBoxesVertical(textImage.VPPbin, textImage.numCols);
//         lineList.printList();
//         textImage.clearArrays();
//         boxNode *lineWalker = lineList.listHead->nextBox;
//         while (lineWalker != 0)
//         {
//             textImage.clearArrays();
//             lineWalker->printbox();
//             textImage.computePP(lineWalker);
//             textImage.thresholdHorizontal();
//             lineWalker = lineWalker->nextBox;
//             cout << endl;
//         }
//         free(lineWalker);
//     };
// };
};