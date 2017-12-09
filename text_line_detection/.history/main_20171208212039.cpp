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
        printf("BOX: %u %u %u %u %u ", type, minRow, minCol, maxRow, maxCol);
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


    void computePP(boxNode *&box)
    { //compuyte VPP and HPP combined
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
        printPP(HPP, VPP);
    };

    void clearArrays(){
        memset(HPP, 0 , sizeof(int)* numRows);
        memset(HPPbin, 0 , sizeof(int)* numRows);
        memset(VPP, 0 , sizeof(int)* numCols);
        memset(VPP, 0 , sizeof(int)* numCols);
    };

    void printPP(int *Harr, int *Varr)
    {
        printPPHorizontal(Harr);
        printPPVertical(Varr);
    };

    void printPPHorizontal(int *Harr){
        cout << "\nHOR: ";
        for (int i = 0; i < numRows; i++)
        {
            cout << Harr[i] << " ";
        };
    };
    void printPPVertical(int *Varr){
        cout << "\nVER: ";
        for (int j = 0; j < numCols; j++)
        {
            cout << Varr[j] << " ";
        }
        cout << "\n";
    };

    void thresholding()
    {
        thresholdHorizontal();
        thresholdVertical();
        printPP(HPPbin, VPPbin);
    }

    void thresholdHorizontal(){
        for (int i = 0; i < numRows; i++)
        {
            if (HPP[i] < threshold)
                HPPbin[i] = 0;
            else
                HPPbin[i] = 1;
        }
        printPPHorizontal(HPPbin);
    }

    void thresholdVertical(){
        for (int i = 0; i < numCols; i++)
        {
            if (VPP[i] < threshold)
                VPPbin[i] = 0;
            else
                VPPbin[i] = 1;
        }
        printPPVertical(VPPbin);
    }

    int findReadingDir()
    {
        int HPPboxCt = countBoxesHorizontal();
        int VPPboxCt = countBoxesVertical();
        printf("\nBOXCOUNT H: %u V: %u", HPPboxCt, VPPboxCt);
        if (HPPboxCt > VPPboxCt)
            return 1; // horinzontal
        else
            return 0; //vertical
    }

    int countBoxesHorizontal()
    {
        int index = 0;
        int boxCount = 0;
        int counterZero = 0;
        int counterOne = 0;
        do
        {
            counterZero = 0;
            counterOne = 0;
            while (HPPbin[index] <= 0){
                index++;
                counterZero++;

            };
            if(index < numRows){
            while (HPPbin[index] > 0){
                index++;
                counterOne++;
            }
            }
            if(counterZero > 0 && counterOne > 0) boxCount++;
        } while (index < numRows);
        return boxCount;
    }

    int countBoxesVertical()
    {
        int index = 0;
        int boxCount = 0;
        int counterZero = 0;
        int counterOne = 0;
        do
        {
            counterZero = 0;
            counterOne = 0;
            while (VPPbin[index] <= 0){
                index++;
                counterZero++;

            };
            if(index < numCols){
            while (VPPbin[index] > 0){
                index++;
                counterOne++;
            }
            }
            if(counterZero > 0 && counterOne > 0) boxCount++;
        } while (index < numCols);
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

    boxNode *findImgBox(int **&imgAry, const int numRows, const int numCols)
    {
        bool firstTime = true;
        for (int i = 0; i < numRows; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                if (imgAry[i][j] > 0)
                {
                    if(firstTime == true){
                        firstTime = false;
                        imgBox = new boxNode(1,i,j,i,j);
                    };
                    compare(imgBox, i, j);
                };
            };
        };
        imgBox->printbox();
        return imgBox;
    };

    //update the bouding box of the image if new max for row and col are found
    void compare(boxNode *& imgBox, const int row, const int col)
    {
        if (row < imgBox->minRow){
            imgBox->minRow = row;
        };
        if (row > imgBox->maxRow){
            imgBox->maxRow = row;
        };
        if (col < imgBox->minCol){
            imgBox->minCol = col;
        };
        if (col > imgBox->maxCol){
            imgBox->maxCol = col;
        };
    };

    boxList findLineBoxes(int *PP, const int PPSize)
    {
        int minRow, maxRow = 0;
        boxList boxHead;
        int index = 0;
        while (index < PPSize)
        {

            while (PP[index] <= 0)
                index++;
            minRow = index; // found first starting point : row
            if(index< PPSize){
            while (PP[index] > 0)
                index++;
            maxRow = index - 1; // found the last starting point :row

            boxNode *newBox = new boxNode(2, minRow, imgBox->minCol, maxRow, imgBox->maxCol);
            boxHead.insertLast(newBox);
            };
        }
        return boxHead;
    };

    void findWordBoxes(int *PP, const int PPSize){
        
    };
};
int main(int argc, char *argv[])
{
    //intializations
    imagePP textImage(argv[1], argv[2], argv[3]);
    BBox box;
    textImage.loadImage();

    //find text image bouding box
    box.findImgBox(textImage.imageAry, textImage.numRows, textImage.numCols); //image box

    // compute HPP and VPP
    textImage.computePP(box.imgBox);

    cout << "After threshold";
    //thresholding HPP and VPP with user input
    textImage.thresholding();

    //determine reading dir
    string Readingdir;
    if (textImage.findReadingDir() == 1)
        Readingdir = "horizontal";
    else
        Readingdir = "vertical";
    cout << "\nReading DIR:" << Readingdir << "\n";
    cout << "\nGoing through each line";
    //find text-line bouding boxes
    if (Readingdir == "horizontal")
    {
        BBox::boxList list = box.findLineBoxes(textImage.HPPbin, textImage.numRows);
        list.printList();  

        boxNode* walker = list.listHead;
        while(walker != 0 ){
            textImage.clearArrays();
            walker->printbox();
            textImage.computePP(walker);
            textImage.thresholdHorizontal();
            walker = walker->nextBox;
        }
        free(walker);
    }
    else
    {
        BBox::boxList list = box.findLineBoxes(textImage.VPPbin, textImage.numCols);
        list.printList();
        textImage.clearArrays();

    };
};