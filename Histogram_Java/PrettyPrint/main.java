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
            prettyPrint.print(inFile, outFile, thresholdValue);
    }
}
class prettyPrint
{    static void print(File inFile,File outFile,int value){
    int numRows,numCols,minVal,maxVal,thresholdValue,pixel;
    try{ 
        thresholdValue = value;
        Scanner input = new Scanner(inFile);
        PrintWriter output = new PrintWriter(outFile+"_PP.txt", "UTF-8");
        numRows = input.nextInt();
        numCols = input.nextInt();
        minVal = input.nextInt();
        maxVal = input.nextInt();
        for(int i=0;i<numRows;i++){
            for(int j=0;j<numCols;j++){
                pixel = input.nextInt();
                if(pixel < thresholdValue) output.write("  ");
                else output.write(pixel+" ");
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
