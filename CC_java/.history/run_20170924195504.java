import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class run {
  public static void main(String args[]) throws FileNotFoundException{
    File inFile = new File(args[0]);
    File out1 = new File(args[1]);
    File out2 = new File(args[2]);
    File out3 = new File(args[3]);
    ConnectedComponent processor=new ConnectedComponent(inFile, out1, out2, out3);
    processor.loadImage();
    processor.ConnectCC_Pass1();
    processor.ConnectCC_Pass2();
    processor.closeFiles();
  }
}
class ConnectedComponent{
  private class componentProperty {
    int label;
    int numbpixels;
    int minRow;
    int minCol;
    int maxRow;
    int maxCol;
    boolean firstTime=true;
  }
  int numRows;
  int numCols;
  int minVal;
  int maxVal;
  int newMin;
  int newMax;
  int newLabel;
  int[][] zeroFramedAry;
  int[] neighborAry=new int[4];
  int[] EQAry;
  int EQArySize;
  componentProperty[] property;
  Scanner input;
  PrintWriter out1;
  PrintWriter out2;
  PrintWriter out3;
  
  ConnectedComponent(File inFile, File outFile1, File outFile2, File outFile3) {
    try{
      input = new Scanner(inFile);
      out1 = new PrintWriter(outFile1, "UTF-8");
      out2 = new PrintWriter(outFile2, "UTF-8");
      out3 = new PrintWriter(outFile3, "UTF-8");
      numRows = input.nextInt();
      numCols = input.nextInt();
      minVal = input.nextInt();
      maxVal = input.nextInt();
      newMin = newMax = newLabel = 0;
      zeroFramedAry = new int [numRows + 2][numCols + 2];
      EQArySize = (numRows * numCols) / 2;
      EQAry = new int[EQArySize];
      for (int i = 0; i < EQArySize; i++) {
        EQAry[i] = i;
      }
    } catch (IOException e){
      e.printStackTrace();
    }
  }

  void closeFiles() {
    out1.close();
    out2.close();
    out3.close();
    input.close();
  }
  void loadImage() {
    // zero framed
    for (int i = 1; i < numRows + 1; i++) {
      for (int j = 1; j < numCols + 1; j++) {
        zeroFramedAry[i][j] = input.nextInt();
      }
    }
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
    out1.write("\nPass Case 1\n");
    prettyPrint();
    printEQAry();
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
      out1.write("\nEQAry:\n");
      for (int i = 1; i < newLabel+1; i++) {
        out1.write(i +" "+EQAry[i]+"\n");
      }
    }

  void printZeroFramedAry() {
    for (int i = 0; i < numRows + 2; i++) {
      for (int j = 0; j < numCols + 2; j++) {
        if(zeroFramedAry[i][j]<10)out1.write(zeroFramedAry[i][j]+"  ");
        else out1.write(zeroFramedAry[i][j]+" ");
      }
      out1.write("\n");
    }
  }

  void prettyPrint(){
    for (int i = 0; i < numRows + 2; i++) {
      for (int j = 0; j < numCols + 2; j++) {
        if(zeroFramedAry[i][j]>0){
          if(zeroFramedAry[i][j]<10)out1.write(zeroFramedAry[i][j]+"  ");
          else out1.write(zeroFramedAry[i][j]+" ");
        }
        else out1.write(" "+"  ");
      }
      out1.write("\n");
    }
  }
  
    void sop(Object e){
      System.out.print(e);
    }
};