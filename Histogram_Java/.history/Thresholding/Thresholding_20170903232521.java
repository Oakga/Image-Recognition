import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

class main{
public static void main(String args[]) throws FileNotFoundException {
    File inFile = new File(args[0]);
    File outFile = new File(args[1]);

    Scanner in = new Scanner(System.in);
    System.out.println("thresholdValue:");
    int thresholdValue = in.nextInt();
    in.close();

    Thresholding processor = new Thresholding(inFile, outFile, thresholdValue);
    processor.computeThreshold();
}
}
class Thresholding{
    int[] histogram;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int thresholdValue;
    File inFile;
    File outFile;
    Thresholding(File inFile, File outFile, int thresholdValue){
        try{
            this.inFile = inFile;
            this.outFile = outFile;
            this.thresholdValue = thresholdValue;
            Scanner input = new Scanner(inFile);
            numRows = input.nextInt();
            numCols = input.nextInt();
            minVal = input.nextInt();
            maxVal = input.nextInt();
            histogram = new int[maxVal+1];
            input.close();
        } catch (IOException e){
            e.printStackTrace();
        }
    }
    void computeThreshold(){
        int pixel = 0;
        try{
        PrintWriter output = new PrintWriter("thr_"+thresholdValue, "UTF-8");
        Scanner input = new Scanner(inFile);
        for(int i=0;i<4;i++){
            input.nextInt();
        }
        output.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
        for(int i =0; i<numRows;i++){
            for(int j=0;j<numCols;j++){
                pixel=input.nextInt();
                if(pixel < thresholdValue) output.write(0+" ");
                else output.write(1+" ");
            }
            output.write("\n");
        }
        output.close();
        input.close();
    } catch (IOException e){
        e.printStackTrace();
    }
    }

}