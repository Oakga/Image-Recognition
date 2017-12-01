import java.util.Scanner;

public class histogram {
    int histogram;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int thresholdValue;
    Scanner input;
    PrintWriter out1;
    File inFile, outFile;

    histogram(File inFile, File outFile, int thresholdValue){
        this.inFile = inFile;
        this.outFile = outFile;
    }

    public static void main(String args[]) throws FileNotFoundException {
        File inFile = new File(args[0]);
        File outFile = new File(args[1]);
    }
}
