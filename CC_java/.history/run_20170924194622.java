import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class run {
  public static void main(String args[]) throws FileNotFoundException{
    File inFile = new File(argv[0]);
    File out1 = new out1(argv[1]);
    File out2 = new out2(argv[2]);
    File out3 = new out3(argv[3]);
    ConnectedComponent processor=new ConnectedComponent(inFile, out1, out2, out3);
    processor.loadImage();
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
      newMin = newmax = newLabel = 0;
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
    scan.close();
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