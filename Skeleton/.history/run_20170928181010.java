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
    processor.loadImage();
    processor.firstPassDistance();
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
    printZeroFramedAry();
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
  
  void firstPassDistance() {
    int pixel = 0, min = 0,pass = 1;
    int[] neighborAry= new int[4];    
    for (int row = 1; row < numRows + 1; row++) {
      for (int col = 1; col < numCols + 1; col++) {
        pixel = zeroFramedAry[row][col];
        if(pixel>0){
          loadNeighbors(neighborAry,row,col,pass);
          min = getMin(neighborAry);
          sop(row+" "+col+" "+min+"\n");
          zeroFramedAry[row][col] = min+1;
        }
      }
    }
    out1.write("\nPass Case 1\n");
    prettyPrint();
  }
  
  void secondPassDistance(){
    int pixel = 0, min = 0, pass = 2;
    int[] neighborAry= new int[4];
    for (int row = numRows; row > 1; row--) {
      for (int col = numCols; col > 1; col--) {
        pixel = zeroFramedAry[row][col];
        if (pixel > 0) {
          loadNeighbors(neighborAry,row,col,pass);
          min = getMin(pass,pixel);
  }
  
  void loadNeighbors(int[] neighborAry,int row, int column,int pass) {
    //top 4 neighbors
    if(pass==1){
      neighborAry[0] = zeroFramedAry[row - 1][column - 1];
      neighborAry[1] = zeroFramedAry[row - 1][column];
      neighborAry[2] = zeroFramedAry[row - 1][column + 1];
      neighborAry[3] = zeroFramedAry[row][column - 1];
    }
    else {
      //bottom 4 neighbors
      neighborAry[0] = zeroFramedAry[row][column + 1];
      neighborAry[1] = zeroFramedAry[row + 1][column - 1];
      neighborAry[2] = zeroFramedAry[row + 1][column];
      neighborAry[3] = zeroFramedAry[row + 1][column + 1];
    }
  }
  
  int getMin(int[] array){
    int n=array.length;
    int min=0;
    int swappedValue;
    //selection sort
    for(int i=0;i<n-1;i++){
      min=i;
      for(int j=i+1;j<n;j++){
        if(array[j]<array[min])min=j;
      }
      if(min!=i){
        swappedValue=array[i];
        array[i]=array[min];
        array[min]=swappedValue;
      }
    }
    return array[0];
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
  
  void printNeighborAry(int[] array) {
    out1.write("\nNeighorAry:\n");
    for (int i = 0; i < array.length; i++) {
      out1.write(i +" "+array[i]+"\n");
    }
  }

  void sop(Object e){
    System.out.print(e);
  }
};