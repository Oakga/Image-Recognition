#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class imagePP;
class BBox;
struct boxNode;

struct boxNode{
        int type;// 1 for img box, 2 for text-line box, 3 for text-word box, etc.
        int minRow;
        int minCol;
        int maxRow;
        int maxCol;
        boxNode* nextBox;
        boxNode(){
            type= 1; 
            minRow = minCol = maxRow = maxCol = 0;
            nextBox = 0;
        };
        boxNode(int type_,int minRow_,int minCol_,int maxRow_, int maxCol_){
            type = type_;
            minRow = minRow_;
            minCol = minCol_;
            maxRow = maxRow_;
            maxCol = maxCol_;
        };
        void printbox(){
            printf("BOX: %u %u %u %u %u ", type, minRow, minCol, maxRow, maxCol);
        };
    };
class imagePP{
public:
    int numRows,numCols,minVal, maxVal;
    int threshold;
    int **imageAry;
    int *HPP;
    int *HPPbin;
    int *VPP;
    int *VPPbin;
    ifstream imageScan;
    ofstream out;
    imagePP(string binaryImage, string thresholdValue, string output){
        threshold = stoi(thresholdValue);
        imageScan.open(binaryImage);
        out.open(output);
        imageScan >> numRows >> numCols >> minVal >> maxVal;

        imageAry = new int *[numRows];
        for (int i = 0; i < numRows; i++)
        {
            imageAry[i] = new int[numCols];
        }
        HPP = new int [numRows]();
        HPPbin = new int [numCols];
        VPP = new int [numCols]();
        VPPbin = new int [numCols];
    };
    ~imagePP(){
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
    void loadImage(){
        for (int i = 0; i < numRows; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                imageScan >> imageAry[i][j];
            }
        }
    }
    void computePP(boxNode* box){ //compuyte VPP and HPP combined
        for (int i = box->minRow; i < box->maxRow; i++)
        {
            for (int j = box->minCol; j < box->maxCol; j++)
            {
                if(imageAry[i][j]>0){
                    HPP[i]++;
                    VPP[j]++;
                }
            }
        }
        // for (int i = 0; i < numRows; i++)
        // {
        //     for (int j = 0; j < numCols; j++)
        //     {
        //         if(imageAry[i][j]>0){
        //             HPP[i]++;
        //             VPP[j]++;
        //         }
        //     }
        // }
        printPP();
    };
    void printPP(){
        cout << "\nHPP: ";
        for(int i= 0; i< numRows; i++){
            cout<< HPP[i] << " ";
        };
        cout << "\nVPP: ";
        for(int j=0; j< numCols; j++){
            cout<< VPP[j] << " ";
        }
        cout << "\n";
    }
};
class BBox{
    friend class imagePP;
public:
    boxNode imgBox;
    class boxList{
    public:
        boxNode* listHead;
        boxNode* last;
        boxList(){
            boxNode* dummy = new boxNode;
            listHead = last = dummy;
        }
        ~boxList(){
            freeList();
        }
        void insertLast(boxNode* box){
            boxNode* lastBox = last;
            lastBox->nextBox = box;
            last = box;
        }

        void printList(){
            boxNode* spot = listHead->nextBox;
            cout << "\n";
            cout << "ListHead-->";
            while(spot!=0){
                cout << "(" << to_string(spot->type) << ")-->";
                spot = spot->nextBox;
            }
            cout << "NULL\n";
            delete spot;
        }
        
        void freeList(){
            boxNode *spot = listHead;
            while(listHead != 0){
                spot = listHead;
                listHead = listHead ->nextBox;
                free(spot);
            }
        }
    };

    static boxNode* findImgBox(int** imgAry, int numRows, int numCols){
        boxNode* box = new boxNode();
        for(int i= 0 ; i< numRows ; i++){
            for(int j= 0 ; j< numCols; j++){
                if(imgAry[i][j]>0){
                    compare(box, i,j);
                };
            };
        };
        box->printbox();
        return box;
    };

    static void compare(boxNode* box,int row, int col){
        box->type = 1; // img box
        if(row < box->minRow) box->minRow = row;
        if(row > box->maxRow) box->maxRow = row;
        if(col < box->minCol) box->minCol = col;
        if(col > box->maxCol) box->maxCol = col;
    };

    

    void test(){
        boxList L;
        boxNode* test = new boxNode;
        boxNode* testb = new boxNode;
        L.printList();
        L.insertLast(test);
        L.insertLast(testb);
        L.printList();
    }
    

};
int main(int argc, char *argv[]){
    imagePP processor(argv[1], argv[2],argv[3]);
    boxNode* box=new boxNode(1,0,0,42,31);
    processor.loadImage();
    processor.computePP(box);
    // BBox::findImgBox(processor.imageAry,processor.numRows, processor.numCols); //image box
    free(box);
};