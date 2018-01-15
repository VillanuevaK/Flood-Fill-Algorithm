/*this version differs from the previous versions because the entire maze is updated as appropriate with the walls.
the floodfill function is changed for this -- the next version will include two versions of a maze, one that just keeps track
of the walls (this is abstracted representation of the actual maze) and another that will be the "mind" of the mouse*/

#include <iostream>
#include <cmath>
#include <string>
#include <array>
#include <iomanip>
#include "nodeStack.h"
using namespace std;

nodeStackPtr head;
const int rowAndColSize = 16;//Global Constant Variable for the dimensions of the maze
const int cellDataSize = 3;/*The data that each cell holds [Value of cell, North Wall (is there a wall here), 
                        West Wall (is there a wall here)]*/
const int OUT_OF_RANGE_VALUE = 3000;

//Print array
void printArray(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize]);
//Gives starting vals to each cell
void giveValueToEachCell(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize]);
//Finds distance to center of given row or column NOTE: Will cause error if dimensions of maze are odd
int minDistToCenter(int rowOrCol, int center1, int center2);
//Updates cell value to be 1+ min value
void updateCell(int (*arr)[rowAndColSize][cellDataSize], int locationToUpdate[2], int valToChangeTo);
//Updates cells with respect to the actual distance to the center given walls
void floodfill(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize], int currentPoint[2], int (*oldWall)[rowAndColSize][2]);
//Finds the value of the lowest surrounding cell AND gives the location of lowest surrounding cell through call by reference variable
int findLowestSurroundingValue(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize], int *currentPoint);
//checks if that cell is in range
bool inRange(int* cell);
//pushes all open (in range and no walls in way) neighbors into stack
void pushOpenNeighbors(int (*arr)[rowAndColSize][cellDataSize], int* cell);
//just moving through the maze
void moveThroughMaze(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize], int (*wallTracker)[rowAndColSize][2]);
//manually adds walls (must change walls in function)
void manuallyAddWalls(int (*arr)[rowAndColSize][cellDataSize]);


int main()
{
    //maze[RowValue][ColumnValue][Cell Value, northWall, westWall]
    int maze[rowAndColSize][rowAndColSize][cellDataSize]={0};
    //oldWall[rowVal][colVal][[northwall][westwall]]
    int oldWall[rowAndColSize][rowAndColSize][2]={0};//keeps track if you've seen wall before
    giveValueToEachCell(rowAndColSize, maze);
    manuallyAddWalls(maze);
    printArray(rowAndColSize, maze);
    moveThroughMaze(rowAndColSize, maze, oldWall);
    cout<<"\n";
    printArray(rowAndColSize, maze);
    cout<<"\nPoints are in the (y,x) order\n"<<endl;
    emptyStack(head);
    return 0; 
}

void printArray(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize])
{
    for(int r = 0; r < sizeOfMatrix; r++)
    {
        for(int i = 0; i < sizeOfMatrix; i++)
        {
            if(arr[r][i][1] || r==0)
                cout<<"+---";
            else
                cout<<"+   ";
            if(i==sizeOfMatrix-1)
                cout<<"+";
        }
        cout<<"\n";
        for(int col = 0; col < sizeOfMatrix; col++)
        {
            if(arr[r][col][2]||col==0)
                cout<<"|"<<setw(2)<<arr[r][col][0]<<" ";
            else
                cout<<" "<<setw(2)<<arr[r][col][0]<<" ";
            if(col==sizeOfMatrix-1)
                cout<<"|";
        }
        cout<<"\n";
    }
    for(int i = 0; i < sizeOfMatrix; i++)
        cout<<"+---";
    cout<<"+";
    return;
}

void giveValueToEachCell(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize])
{
    int center1 = sizeOfMatrix/2;
    int center2 = ((sizeOfMatrix/2) - 1);

    for(int r = 0; r < sizeOfMatrix; r++)
    {
        for(int col = 0; col < sizeOfMatrix; col++)
        {
            arr[r][col][0] = minDistToCenter(r, center1, center2)+minDistToCenter(col, center1, center2);
        }
    }

    return;
}

int minDistToCenter(int rowOrCol, int center1, int center2)
{
    int max1 = abs(rowOrCol - center1);
    int max2 = abs(rowOrCol - center2);
    /*center is essentially a 2x2 matrix, this just determines what value of cell based on how far it is from the closest point in the center. 
    If center becomes a 1x1 point, or if center is not where you want to end, this needs to be modified*/
    if(max1<max2)
            return max1;
    else//(max2<max1)
            return max2;
}

void updateCell(int (*arr)[rowAndColSize][cellDataSize], int locationToUpdate[2], int valToChangeTo)
{
        arr[locationToUpdate[0]][locationToUpdate[1]][0] = valToChangeTo + 1;
        return;
}

void moveThroughMaze(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize], int (*oldWall)[rowAndColSize][2])
{
    int currentPoint[2] = {0,0};
    //cout<<"Current point: ("<<currentPoint[0]<<","<<currentPoint[1]<<")"<<endl;
    while(currentPoint[0]<rowAndColSize)
    {   
        while(currentPoint[1]<rowAndColSize)
        {        //is there a wall here and is it a new wall
            if( (arr[currentPoint[0]][currentPoint[1]][1] && oldWall[currentPoint[0]][currentPoint[1]][0]==0) || 
                (arr[currentPoint[0]][currentPoint[1]][2] && oldWall[currentPoint[0]][currentPoint[1]][1]==0) || 
                (arr[currentPoint[0]+1][currentPoint[1]][1] && oldWall[currentPoint[0]+1][currentPoint[1]][0]==0) || 
                (arr[currentPoint[0]][currentPoint[1]+1][2] && oldWall[currentPoint[0]][currentPoint[1]+1][1]==0) )//checks if there are any new walls in the premises
            {   
                floodfill(sizeOfMatrix, arr, currentPoint, oldWall);
            }
            //printArray(sizeOfMatrix, arr);
            //cout<<endl<<"Current point: ("<<currentPoint[0]<<","<<currentPoint[1]<<")"<<endl;
            currentPoint[1]++;
        }
        currentPoint[0]++;
        currentPoint[1] = 0;
    }
}

void floodfill(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize], int currentCell[2], int (*oldWall)[rowAndColSize][2])
{
    emptyStack(head);
    int currentPoint[2];
    currentPoint[0] = currentCell[0];
    currentPoint[1] = currentCell[1];
    int lowestSurroundingValue = 0;
    int cellNorth[] = {currentPoint[0]-1, currentPoint[1]};
    int cellEast[] = {currentPoint[0], currentPoint[1]+1};
    int cellSouth[] = {currentPoint[0]+1, currentPoint[1]};
    int cellWest[] = {currentPoint[0], currentPoint[1]-1}; 

    //cout<<endl<<"   Initial walls have been seen, we're in floodfill"<<endl;
    push1(head, currentPoint);
    if(arr[currentPoint[0]][currentPoint[1]][1] && inRange(cellNorth) )//checks if there is a wall to the north and the cell above is in range
    {
        push1(head, cellNorth);
        oldWall[currentPoint[0]][currentPoint[1]][0] = 1;
    }
    if(arr[currentPoint[0]][currentPoint[1]+1][2] && inRange(cellEast) )//checks if there is a wall to the east and the cell east is in range
    {
        push1(head, cellEast);
        oldWall[currentPoint[0]][currentPoint[1]+1][1] = 1;
    }
    if(arr[currentPoint[0]+1][currentPoint[1]][1] && inRange(cellSouth) )//checks if there is a wall to the south and the cell below is in range
    {
        push1(head, cellSouth);   
        oldWall[currentPoint[0]+1][currentPoint[1]][0] = 1;
    }
    if(arr[currentPoint[0]][currentPoint[1]][2] && inRange(cellWest) )//checks if there is a wall to the west and the cell west is in range
    {
        push1(head, cellWest);
        oldWall[currentPoint[0]][currentPoint[1]][1] = 1;
    }
    while(!isLLempty(head))
    {
        pop1(head, currentPoint);//takes what it popped and makes it current point
        //cout<<"     Popped: "<<currentPoint[0]<<","<<currentPoint[1]<<endl;
        lowestSurroundingValue = findLowestSurroundingValue(sizeOfMatrix, arr, currentPoint);
        if(lowestSurroundingValue >= arr[currentPoint[0]][currentPoint[1]][0] && arr[currentPoint[0]][currentPoint[1]][0] != 0 )
        {
            //cout<<"         no lower neighbor found"<<endl;
            updateCell(arr, currentPoint, lowestSurroundingValue);//Matrix update if it can't find a lower value surrouding it
            pushOpenNeighbors(arr, currentPoint);
        }
    }  
    //cout<<" Leaving floodfill"<<endl;
}

int findLowestSurroundingValue(int sizeOfMatrix, int (*arr)[rowAndColSize][cellDataSize], int *currentPoint)
{
    int lowestVal = 2000;//Compares the four surrounding values to this value first
    //Did this to avoid an out of range error - since these are invalid, it will not bother going in these directions unless they are in range
    int upValue= OUT_OF_RANGE_VALUE;
    int rightValue = OUT_OF_RANGE_VALUE;
    int downValue = OUT_OF_RANGE_VALUE;
    int leftValue = OUT_OF_RANGE_VALUE;

    //These if statements check if up,down,left,right are in range and if a wall is in the way
    //If they are out of range, or there is a wall in the way, the code won't update in that direction since 
    //  they will not be changed to the actual value of the positions, intstead they will keep the invalid values.
    if(currentPoint[0]-1 > -1 && arr[currentPoint[0]][currentPoint[1]][1]==0)//check that upwards is not out of range and that upwards there isnt a wall
        upValue= arr[currentPoint[0]-1][currentPoint[1]][0];
    if(currentPoint[1] < sizeOfMatrix-1 && arr[currentPoint[0]][currentPoint[1]+1][2]==0)
        rightValue = arr[currentPoint[0]][currentPoint[1]+1][0];
    if(currentPoint[0] < sizeOfMatrix-1 && arr[currentPoint[0]+1][currentPoint[1]][1]==0)
        downValue = arr[currentPoint[0]+1][currentPoint[1]][0];
    if(currentPoint[1]-1 > -1 && arr[currentPoint[0]][currentPoint[1]][2]==0)
        leftValue = arr[currentPoint[0]][currentPoint[1]-1][0];

    if(lowestVal>upValue)
    {
        lowestVal = upValue;
    }
    if(lowestVal>rightValue)
    {
        lowestVal = rightValue;
    }
    if(lowestVal>downValue)
    {
        lowestVal = downValue;
    }
    if(lowestVal>leftValue)
    {
        lowestVal = leftValue;
    }

    return lowestVal;
}

bool inRange(int* cell)
{
    if(cell[0]>=0 && cell[0] < rowAndColSize && cell[1] >= 0 && cell[1] < rowAndColSize)
        return true;
    return false;
}

void pushOpenNeighbors(int (*arr)[rowAndColSize][cellDataSize], int* cell)
{
    int cellNorth[] = {cell[0]-1, cell[1]};
    int cellEast[] = {cell[0], cell[1]+1};
    int cellSouth[] = {cell[0]+1, cell[1]};
    int cellWest[] = {cell[0], cell[1]-1};

    if(inRange(cellNorth) && (arr[cell[0]][cell[1]][1]==0) )//if the cell is in range and there are no walls in the way
    {
        push1(head, cellNorth);
    }   
    if(inRange(cellEast) && (arr[cell[0]][cell[1]+1][2]==0) )
    {
        push1(head, cellEast);
    }   
    if(inRange(cellSouth) && (arr[cell[0]+1][cell[1]][1]==0) )
    {
        push1(head, cellSouth);
    }   
    if(inRange(cellWest) && (arr[cell[0]][cell[1]][2]==0) )
    {
        push1(head, cellWest);
    }   
}

void manuallyAddWalls(int (*arr)[rowAndColSize][cellDataSize])
{
    //in third spot, 1 is up, 2 left
    arr[0][1][2] = 1;
    arr[1][1][2] = 1;
    arr[2][1][2] = 1;
    arr[3][1][2] = 1;
    arr[4][1][2] = 1;

    arr[3][3][2] = 1;
    arr[4][3][2] = 1;

    arr[3][3][1] = 1;
    arr[3][5][2] = 1;
    arr[4][5][2] = 1;
    arr[5][3][1] = 1;
    arr[5][4][1] = 1;

    arr[7][7][1] = 1;
    arr[7][6][1] = 1;
    arr[7][5][1] = 1;
    arr[7][4][1] = 1;
    arr[7][3][1] = 1;

    arr[14][5][2] = 1;
    arr[1][13][2] = 1;
    arr[13][1][2] = 1;
    arr[3][12][2] = 1;
    arr[11][1][2] = 1;

    arr[10][3][2] = 1;
    arr[10][3][2] = 1;

    arr[10][10][1] = 1;
    arr[10][9][2] = 1;
    arr[10][8][2] = 1;
    arr[10][7][1] = 1;
    arr[10][6][1] = 1;

    arr[12][7][1] = 1;
    arr[2][12][1] = 1;
    arr[8][12][1] = 1;
    arr[4][12][1] = 1;
    arr[5][12][1] = 1;

    arr[15][15][1] = 1;
    arr[14][14][1] = 1;
    arr[13][13][1] = 1;
    arr[12][12][1] = 1;
    arr[11][11][1] = 1;
    arr[10][10][1] = 1;
    arr[10][9][1] = 1;

    arr[14][15][2] = 1;
    arr[13][14][2] = 1;
    arr[12][13][2] = 1;
    arr[11][12][2] = 1;
    arr[10][11][2] = 1;
    arr[9][10][2] = 1;

    arr[13][12][1] = 1;
    arr[13][11][1] = 1;
    arr[13][10][1] = 1;
    arr[13][9][1] = 1;
    arr[13][8][1] = 1;
    arr[13][7][1] = 1;
    return;
}

//Notes:
//In actual implementation, mouse needs to continue to move straight if no obstacle/greater val in the way
//mouse might be more efficient if it seeks to move cells it's gone to the least
//In the searching stage, the mouse should look to go to as many places on the map as possible for flood fill to accurately depict the walls
