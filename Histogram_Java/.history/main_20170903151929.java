import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

public class main{
    public static void main(String args[]) throws FileNotFoundException {
            File inFile = new File(args[0]);
            File outFile = new File(args[1]);

            Scanner in = new Scanner(System.in);
            System.out.println("thresholdValue:");
            int thresholdValue = in.nextInt();
            in.close();

            histogram processor = new histogram(inFile, outFile, thresholdValue);
            processor.computeHistogram();
            processor.computeThreshold();
            prettyPrint image = new prettyPrint(inFile, outFile, thresholdValue);
    }
}
class histogram {
    int[] histogram;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int thresholdValue;
    File inFile;
    File outFile;
    histogram(File inFile, File outFile, int thresholdValue){
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
    void computeHistogram(){
        try{
        int pixel, maxPlus;
        PrintWriter output = new PrintWriter(outFile, "UTF-8");
        Scanner input = new Scanner(inFile);
        for(int i=0;i<4;i++){
            input.nextInt();
        }
        while(input.hasNextInt()){
            pixel = input.nextInt();
            histogram[pixel]++;
        }
        output.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
        for(int i=0;i< maxVal;i++){
            output.write("(" +i+"): "+ histogram[i]);
            if(histogram[i]> 80) maxPlus = 80;
            else maxPlus = histogram[i];
            for(int j = 0; j< maxPlus; j++){
                output.write("+");
            };
            output.write("\n");
        }
        output.write("\n");
        input.close();
        output.close();
    } catch (IOException e){
        e.printStackTrace();
    }
    }

    void computeThreshold(){
        try{
        int pixel = 0;
        PrintWriter output = new PrintWriter("thr_"+thresholdValue, "UTF-8");
        Scanner input = new Scanner(inFile);
        for(int i=0;i<4;i++){
            input.nextInt();
        }
        output.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
        for(int i =0; i<numRows;i++){
            for(int j=0;j<numCols;j++){
                pixel=input.nextInt();
                if(pixel < thresholdValue) output.write("  ");
                else output.write(pixel+" ");
            }
            output.write("\n");
        }
        output.close();
    } catch (IOException e){
        e.printStackTrace();
    }
    }
}
class prettyPrint
{    static void print(File inFile,File outFile,int value){
    try{ 
        int numRows,numCols,minVal,maxVal,thresholdValue;
        thresholdValue = value;
        Scanner input = new Scanner(inFile);
        PrintWriter output = new PrintWriter(outFile+"_PP", "UTF-8");
        numRows = input.nextInt();
        numCols = input.nextInt();
        minVal = input.nextInt();
        maxVal = input.nextInt();
        int pixel;
        for(int i=0;i<numRows;i++){
            for(int j=0;j<numCols;j++){
                pixel = input.nextInt();
                if(pixel < thresholdValue) output.write("  ");
                else output.write(1+" ");
            }
        }
        output.close();
        input.close();
    } catch (IOException e){
        e.printStackTrace();
    }
    }
}
