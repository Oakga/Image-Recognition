import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class main{
    public static void main(String args[]) throws FileNotFoundException {
            File inFile = new File(args[0]);
            File outFile = new File(args[1]);
    }
}
class histogram {
    int[] histogram;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int thresholdValue;
    Scanner input;
    PrintWriter output;

    histogram(File inFile, File outFile, int thresholdValue){
        try{
            output = new PrintWriter(outFile, "UTF-8");
            input = new Scanner(inFile);
            this.thresholdValue = thresholdValue;
            numRows = input.nextInt();
            numCols = input.nextInt();
            minVal = input.nextInt();
            maxVal = input.nextInt();
            histogram = new int[maxVal+1];
        } catch (IOException e){
            e.printStackTrace();
        }
    }
    void computeHistogram(){
        int pixel, maxPlus;
        output.write(numRows+" "+numCols+" "+minVal+" "+maxVal);
    }
}