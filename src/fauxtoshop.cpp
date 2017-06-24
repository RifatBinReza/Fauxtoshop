/*File: fauxtoshop.cpp
 * A photoshop-like software to manipulate image
 * Can open, blur and save images
 *
 * ====================================
 * || @author: Rifat Bin Reza        ||
 * || ID: 1511720042                 ||
 * || Email: rifatbinreza@gmail.com  ||
 * || Submitted to: MEK              ||
 * ====================================
*/

#include <iostream>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"

using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;

//Function prototypes
void blurImage(GBufferedImage &img,const Grid<int> &original, int blurAmount);
static bool     openImageFromFilename(GBufferedImage &img, string filename);
static bool 	saveImageToFilename(const GBufferedImage &img, string filename);
static void     getMouseClickLocation(int &row, int &col);

int main() {
    GWindow gw;
    gw.setTitle("Fauxtoshop"); //Title of the software
    gw.setVisible(true);

    while(true){    //This while loop will run the program again and again for the user to use it for multiple images
    GBufferedImage img;
    cout << "Welcome to Fauxtoshop!" << endl;

    while(true){    //This loop will make sure that you enter a valid file name or won't let you proceed
    string fileName=getLine("Enter the file name: ");
    bool fileOpen =openImageFromFilename(img, fileName);
    if(fileOpen){
        cout<<"Please wait while we load the image for you..."<<endl;
        break;
        }
    else
        cout<<"Invalid file name!"<<endl;
    }

    gw.setSize(img.getWidth(), img.getHeight()); //gets the size of the image and makes it to the Gwindow size
    gw.add(&img,0,0); //adds the image to GWindow

    Grid<int> original = img.toGrid(); //converts the image to a grid

    cout<<"Which image filter would you like to apply?"<<endl;
    cout<<"\t1 - Blur"<<endl;
    int choice = getInteger("Enter your choice: ","Please enter a number: ");

    switch(choice){ //for multiple image processing choices. currently we just have one(blur)
    case 1:{ //this will execute for blur
    int blurAmount = getInteger("Enter the degree of Blur[1-100]: ","Please enter an integer between 1-100: ");
    blurImage(img,original,blurAmount);
        break;
        }
    default:
        break;
    }

    string prompt=getLine("Do you want to save the image? [Yes]/[No]"); //ask the user if they want to save the file
    if(prompt=="Yes"){
        while(true){ //this loop will make sure your photo is saved
        string newImageName=getLine("Enter the name for the new image with \".jpg\": ");
        bool saveImage = saveImageToFilename(img,newImageName);
        if(saveImage){
            cout<< "Your photo has been successfully saved!"<<endl;
            break;
        }
        else
            cout<<"Invalid file name!"<<endl;
        }
    }
    else
        gw.clear(); //clears the GWindow for next image
    }
    gw.clear();
    //int row, col;
    //getMouseClickLocation(row, col);
    return 0;
}


/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to open the image file 'filename'.
 *
 * This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
static bool openImageFromFilename(GBufferedImage &img, string filename) {
    try { img.load(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to save the image file to 'filename'.
 *
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
static bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try {
        img.save(filename);
    } catch (...) {
        return false;
    }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Waits for a mouse click in the GWindow and reports click location.
 *
 * When this function returns, row and col are set to the row and
 * column where a mouse click was detected.
 */
static void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED); // bad style to use do-while loop!
    row = me.getY();
    col = me.getX();
}

/* Function blurImage()
 * This function takes the image, a grid of the image and a box size which is named blurAmount
 * This function will take the grid pixel by pixel and average the values of surrounding pixels and set them in the grid again
 * and finally the grid will be put on the image using fromGrid() function.
 */
void blurImage(GBufferedImage &img,const Grid<int> &original,int blurAmount){
    Grid<int> grid = original;

    for(int row=0;row<original.numRows();row++){ //goes through the rows
        for(int column=0;column<original.numCols();column++){ //goes through the columns
            //initializations
            int pixel;
            int red,green,blue;
            int newRed=0;
            int newGreen=0;
            int newBlue=0;
            try{ //using try{} because if the value is out of the bound then it will be ignored
            for(int newRow=row-blurAmount/2;newRow<=row+blurAmount/2;newRow++){ //goes through surrounding pixels of row
                for(int newColumn=column-blurAmount/2;newColumn<=column+blurAmount/2;newColumn++){ //goes through surrounding pixels of column
                    pixel=original[newRow][newColumn]; //setting the new values in pixel
                    GBufferedImage::getRedGreenBlue(pixel,red,green,blue); //getting the RGB values from pixel
                    //summing the new RGB values
                    newRed+=red;
                    newGreen+=green;
                    newBlue+=blue;
                }
            }
            }catch(...){}

            //calculating the average
            newRed/=blurAmount*blurAmount;
            newGreen/=blurAmount*blurAmount;
            newBlue/=blurAmount*blurAmount;

            int averagePixel = GBufferedImage::createRgbPixel(newRed,newGreen,newBlue); //setting the RGB to averagePixel
            grid[row][column]=averagePixel; //setting averagePixel to the grid
        }
    }
    img.fromGrid(grid); //masking the grid to the image

}
