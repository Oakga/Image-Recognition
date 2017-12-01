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
    Skeleton processor=new Skeleton(inFile, out1, out2, out3);
    processor.closeFiles();
  }
}

class Skeleton{
  int numRows;
  int numCols;
  int minVal;
  int maxVal;
  int newMinVal;
  int newMaxVal;
  int[][] zeroFramedAry;
  int[][] skeletonAry;
  Scanner input;
  PrintWriter out1;
  PrintWriter out2;
  PrintWriter out3;
  
  Skeleton(File inFile, File outFile1, File outFile2, File outFile3) {
    try{
      input = new Scanner(inFile);
      out1 = new PrintWriter(outFile1, "UTF-8");
      out2 = new PrintWriter(outFile2, "UTF-8");
      out3 = new PrintWriter(outFile3, "UTF-8");
      numRows = input.nextInt();
      numCols = input.nextInt();
      minVal = input.nextInt();
      maxVal = input.nextInt();
      newMinVal = maxVal;
      newMaxVal = minVal;
      skeletonAry = new int [numRows + 2][numCols + 2];
      zeroFramedAry = new int [numRows + 2][numCols + 2];
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
  void sop(Object e){
    System.out.print(e);
  }
};