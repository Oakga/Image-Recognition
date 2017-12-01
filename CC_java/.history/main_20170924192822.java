import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class main {
  public static void main(String args[]) throws FileNotFoundException{
    File inFile = new File(argv[0]);
    File out1 = new out1(argv[1]);
    File out2 = new out2(argv[2]);
    File out3 = new out3(argv[3]);
    ConnectedComponent processor=new ConnectedComponent(inFile, out1, out2, out3);
    processor.loadImage();
    processor.ConnectCC_Pass1();
    processor.ConnectCC_Pass2();
    processor.manageEQAry();
    processor.ConnectCC_Pass3();
    processor.printGreyScale();
  }
}
class ConnectedComponent{
public:
  int numRows;
  int numCols;
  int minVal;
  int maxVal;
  int newMin;
  int newMax;
  int newLabel;
  int **zeroFramedAry;
  int neighborAry[4];
  int *EQAry;
  int EQArySize;
  struct componentProperty {
    int label;
    int numbpixels;
    int minRow;
    int minCol;
    int maxRow;
    int maxCol;
    bool firstTime=true;
  } * Property;
  ifstream scan;
  ofstream out1;
  ofstream out2;
  ofstream out3;
  
  ConnectedComponent(string inFile, string outFile1, string outFile2, string outFile3) {
    scan.open(inFile);
    out1.open(outFile1);
    out2.open(outFile2);sefsefsdfsajik;ufuiashfuisaghfjdsaghfgdjhksg
    out3.open(outFile3);
    scan >> numRows >> numCols >> minVal >> maxVal;
    newMin = newMax = newLabel = 0;
    
    zeroFramedAry = new int *[numRows + 2];
    for (int i = 0; i < numRows + 2; i++) {
      zeroFramedAry[i] = new int[numCols + 2];
    }
    
    EQArySize = (numRows * numCols) / 2;
    EQAry = new int[EQArySize];
    for (int i = 0; i < EQArySize; i++) {
      EQAry[i] = i;
    }
  }
  ~ConnectedComponent() {
    for (int i = 0; i < numRows + 2; i++) {
      delete[] zeroFramedAry[i];
    }
    
    delete[] zeroFramedAry;
    delete[] EQAry;
    out1.close();
    out2.close();
    out3.close();
    scan.close();
  }
  void loadImage() {
    // zero framed
    for (int i = 1; i < numRows + 1; i++) {
      for (int j = 1; j < numCols + 1; j++) {
        scan >> zeroFramedAry[i][j];
      }
    }
  }
  
  void manageEQAry(){
    out1<< "\nManage EQAry";
    int trueLabel = 0;
    int index = 1;
    while(index<=newLabel){
      if(EQAry[index]==index){
        trueLabel++;
        EQAry[index] = trueLabel;
      }
      else EQAry[index] = EQAry[EQAry[index]];
      index++;
    }
    newMin = newMax = trueLabel;
    printEQAry();
  }
  void ConnectCC_Pass1() {
    int pixel = 0, min = 0, pass = 1, passCase = 0;
    for (int row = 1; row < numRows + 1; row++) {
      for (int col = 1; col < numCols + 1; col++) {
        pixel = zeroFramedAry[row][col];
        if (pixel > 0) {
          loadNeighbors(row, col);
          passCase = checkCasesForPass1();
          if (passCase == 1) {
            newLabel++;
            zeroFramedAry[row][col] = newLabel;
          } else if (passCase == 2) {
            if (neighborAry[0] != 0)
            zeroFramedAry[row][col] = neighborAry[0];
            else
            zeroFramedAry[row][col] = neighborAry[1];
          } else if (passCase == 3) {
            min = getMin(pass,pixel);
            zeroFramedAry[row][col] = min;
            updateEQAry(pass,min);
          }
        }
      }
    }
    out1 << "\nPass Case 1\n";
    prettyPrint();
    printEQAry();
  }
  
  void ConnectCC_Pass2(){
    int pixel = 0, min = 0, pass = 2, passCase = 0;
    int count =0;
    for (int row = numRows; row > 1; row--) {
      for (int col = numCols; col > 1; col--) {
        pixel = zeroFramedAry[row][col];
        if (pixel > 0) {
          loadNeighbors(row, col);
          min = getMin(pass,pixel);
          count++;
          zeroFramedAry[row][col]= min;
          updateEQAry(pass,min);
        }
      }
    }
    out1 << "\nPass Case 2\n";
    prettyPrint();
    printEQAry();
  }
  
  void ConnectCC_Pass3(){
    int pixel = 0, label = 0;
    Property = new componentProperty[newLabel+1];
    for (int row = 1; row < numRows + 1; row++) {
      for (int col = 1; col < numCols + 1; col++) {
        pixel = zeroFramedAry[row][col];
        if(pixel>0){
          if(pixel < newMin ) newMin = pixel;
          label = EQAry[pixel];
          zeroFramedAry[row][col] = label;
          if(Property[label].firstTime==true){
            Property[label].minRow= Property[label].maxRow = row;
            Property[label].minCol = Property[label].maxCol = col;
            Property[label].label = label;
            Property[label].firstTime= false;
          }
          else{
            if(row<Property[label].minRow)Property[label].minRow =row;
            if(row>Property[label].maxRow)Property[label].maxRow = row;
            if(col<Property[label].minCol)Property[label].minCol = col;
            if(col>Property[label].maxCol)Property[label].maxCol = col;
          }
          Property[label].numbpixels++;
        }
      }
    }
    out1 << "\nPass Case 3\n";
    prettyPrint();
    printEQAry();
    printCCProperty();
  }
  
  void updateEQAry(int pass,int min){
    int alpha, beta;
    if (pass == 1) {
      alpha = neighborAry[0];
      beta = neighborAry[1];
    } else {
      alpha = neighborAry[2];
      beta = neighborAry[3];
    }
    if(alpha != 0 && alpha != min) EQAry[alpha] = min;
    if(beta != 0 && beta != min) EQAry[beta] = min;
  }
  
  int getMin(int pass,int pixel) {
    int min, alpha, beta;
    if (pass == 1) {
      alpha = neighborAry[0];
      beta = neighborAry[1];
    } else {
      alpha = neighborAry[2];
      beta = neighborAry[3];
    }
    
    if (alpha == 0) min= beta;
    else if (beta == 0) min= alpha;
    else if (alpha < beta)
    min= alpha;
    else
    min= beta;
    
    if(pass == 2) {
      if( min==0 || min>pixel) min=pixel;
    }
    return min;
  }
  int checkCasesForPass1() {
    if (neighborAry[0] == 0 && neighborAry[1] == 0)
    return 1;
    else if (neighborAry[0] != 0 && neighborAry[1] != 0 &&
      neighborAry[0] != neighborAry[1]) {
        return 3;
      } else 
      return 2;
    }
    
    void loadNeighbors(int row, int column) {
      neighborAry[0] = zeroFramedAry[row - 1][column];
      neighborAry[1] = zeroFramedAry[row][column - 1];
      neighborAry[2] = zeroFramedAry[row][column + 1];
      neighborAry[3] = zeroFramedAry[row + 1][column];
    }
    
    void printEQAry() {
      out1 << "\nEQAry:\n";
      for (int i = 1; i < newLabel+1; i++) {
        out1<< i << " "<< EQAry[i] <<endl;
      }
    }
    void printCCProperty(){
      out3<< "Image Headers"<<endl;
      out3<< numRows << " "<< numCols << " "<< newMin << " " << newMax <<endl;      
      out3<< "Connected Components Properties:\n";
      for(int i=1;i<newMax+1;i++){
        out3 << "Number: "<< i <<endl;
        out3<< "Label: " << Property[i].label << endl;
        out3<< "numbpixels: "<< Property[i].numbpixels << endl;
        out3<< "BoundingBox: "<<endl;
        out3<< "minRow: "<< Property[i].minRow << endl;
        out3<< "minCol: "<< Property[i].minCol << endl;
        out3<< "maxRow: "<< Property[i].maxRow << endl;
        out3<< "maxCol: "<< Property[i].maxCol << endl;
        out3<<endl;
      }
      out3<< endl;
    }
    void printZeroFramedAry() {
      for (int i = 0; i < numRows + 2; i++) {
        for (int j = 0; j < numCols + 2; j++) {
          if(zeroFramedAry[i][j]<10)out1 << zeroFramedAry[i][j] << "  ";
          else out1 << zeroFramedAry[i][j] << " ";
        }
        out1 << endl;
      }
    }
    void printGreyScale(){
      out2<< numRows << " "<< numCols << " "<< 0 << " " << newMax <<endl;
      for (int i = 0; i < numRows + 2; i++) {
        for (int j = 0; j < numCols + 2; j++) {
          if(zeroFramedAry[i][j]>0){
            if(zeroFramedAry[i][j]<10)out2 << zeroFramedAry[i][j] << "  ";
            else out2 << zeroFramedAry[i][j] << " ";
          }
          else out2 << "0" << "  ";
        }
        out2 << endl;
      }
    }
    void prettyPrint(){
      for (int i = 0; i < numRows + 2; i++) {
        for (int j = 0; j < numCols + 2; j++) {
          if(zeroFramedAry[i][j]>0){
            if(zeroFramedAry[i][j]<10)out1 << zeroFramedAry[i][j] << "  ";
            else out1 << zeroFramedAry[i][j] << " ";
          }
          else out1 << " " << "  ";
        }
        out1 << endl;
      }
    }
};