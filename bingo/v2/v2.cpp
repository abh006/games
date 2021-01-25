#include<iostream>
#include<vector>
#include<bits/stdc++.h>
#include<numeric>
#include<map>

using namespace std;

vector<int> numbers(25);
vector<int> hitMatrix(25, 0);
vector<int> axisLevels(12,0);
vector<int> axisWeights(12, 0);
vector<int> cellWeights(25, 0);
vector<int> axisQueue = {10, 11, 2, 7, 5, 4, 6, 3, 9, 0, 8, 1}; // TODO   // ltr diag, v1, h5, v2, h4,... ( v - vertical, h-horizontal, numbering top to bottom , left to right)


void initNumbers();
void printMatrix();
vector<int> getAxes(int);
vector<int> getFriendAxes(int);
int callNumber(int);
void updateAxisLevels(int);
void updateAxisWeights(int);
void updateAxisQueue();
int getNextNumber();
int getIndex(int);
int isBingo();

int main(){
    int num=0;
    
    initNumbers();
    
    cout<<"BINGO - ONE-on-ONE"<<endl;
    num = callNumber(num);
    cout<<"Computer Says: "<<num<<endl;

    do
    {
        cout<<"Your number: ";
        cin>>num;
        num = callNumber(num);
        cout<<"Computer says: "<<num<<endl;

        if(isBingo()==1){
            num = 26;
        }
    } while (num > 0 && num < 26);

    if( num == 26 ){
        cout<<endl<<"Computer Wins !!!"<<endl;
    }else if( num == -1 ){
        cout<<endl<<"Congratulations... You have beat the computer !!!"<<endl;
    }
    
    return 0;
}

void initNumbers(){
    // initialise numbers
    iota(begin(numbers), end(numbers), 1);
    shuffle(numbers.begin(),numbers.end(), default_random_engine(0));
}

void printMatrix(){
    cout<<"Numbers "<<endl;
    for(int i=0; i<25; i++){
        if(i%5==0)
            cout<<endl;
        cout<<numbers[i]<<" ";
    }
}

vector<int> getAxes(int num){
    // fetch row and col of the num
    int index = getIndex(num);
    int row = index/5;
    int col = index%5;

    vector<int> axes = { row+1, col+5};

    int i = 2;
    if( row == col ){
        axes[i] = 10;   // ltr diagonal
        i++;
    }
    if(row == 4-col){
        axes[i] = 11;   // rtl diagonal
        i++;
    }
    
    return axes;
}

int callNumber(int num){
    if(num == 0){
        // game is just starting
        return numbers[16];  // TODO
    }else if( num == -1){
        // opponent has won
        return 26;
    }
    // mark the hit
    int index = getIndex(num);
    hitMatrix[index] = 1;

    // update axis levels and axis weights accordingly
    updateAxisLevels(num);
    updateAxisWeights(num);
    updateAxisQueue();

    num = getNextNumber();

    // update axis levels and axis weights accordingly
    updateAxisLevels(num);
    updateAxisWeights(num);
    updateAxisQueue();
    
    return num;
}

void updateAxisLevels(int num){
    // fetch axes of the number
    vector<int> axes = getAxes(num);
    for(int i=0; i<axes.size(); i++){
        int axisIndex = axes[i];
        axisLevels[axisIndex] += 1;
    }
}

void updateAxisWeights(int num){
    // fetch axes of the number
    vector<int> axes = getAxes(num);
    int length = axes.size();

    // increase weight of all these axes by the length of vector<int> axes
    // because all axes has an extra hit in it so level will be increased by 1
    // so the weight of all the friend axes increases by length times 1 = length
    for( int i=0; i<length; i++){
        int axisIndex = axes[i];
        axisWeights[axisIndex] += length;
    }
}

void updateCellWeight(int num){
    // fetch axes of the number
    vector<int> axes = getAxes(num);
    int length = axes.size();

    // get index of num
    int numIndex = getIndex(num);

    // increase weights of all the cells in all these axes by 1
    // except the cell of num
    for(int i=0; i<length; i++){
        int axisIndex = axes[i];
        for( int j=0; j<5; j++){
            int row, col;
            if(axisIndex == 11){
                row = j;
                col = 4-j;
            }else if(axisIndex == 10){
                row = col = j;
            }else if(axisIndex > 4){
                col = axisIndex-5;
                row = j;
            }else{
                row = axisIndex-5;
                col = j;
            }
            int index = row*5 + col;
            if(index == numIndex)
                continue;
            else
                cellWeights[index] += 1;
        }
    }

}

void updateAxisQueue(){
    // this is not sorting the queue
    // just reordering the numbers in the queue based on the axislevel, axis weight and the current order of the numbers
    map<int, vector<int>> axisMap;
    vector<int> updatedAxisQueue;

    for(int i=0; i<axisQueue.size(); i++){
        int axisIndex = axisQueue[i];
        int axisLevel = axisLevels[axisIndex];

        // insert into map, with key as axisLevel
        axisMap[axisLevel].push_back(axisIndex);
    }

    // sort vectors in the axisMap based on axis weights
    for (auto const& mapEntry : axisMap){
        int axisLevel = mapEntry.first;
        vector<int> axesList = mapEntry.second;
        
        // bubble sort axesList based on axis weight
        for(int i=0; i<axesList.size(); i++){
            for(int j=0; j<axesList.size()-i-1 ; j++){
                // get the axis weights
                int axisIndex1 = axesList[j];
                int axisIndex2 = axesList[j+1];
                int axisWeight1 = axisWeights[axisIndex1];
                int axisWeight2 = axisWeights[axisIndex2];

                // compare and swap 
                if( axisWeight1 < axisWeight2 ){
                    int temp = axesList[j];
                    axesList[j] = axesList[j+1];
                    axesList[j+1] = temp;
                }
            }
        }

        // push the sorted list back to updated axis queue
        updatedAxisQueue.insert( updatedAxisQueue.end(), axesList.begin(), axesList.end());
    }
    // assign updated version of axis queue to the original one
    axisQueue = updatedAxisQueue;
}

int getNextNumber(){
    // get the most weighted cell from the axis in front of axis queue

    int axisIndex = axisQueue[0];

    int numIndex = -1;
    int maxWeight = -1;
    for(int i=0; i<5; i++){
        int row, col;
        if(axisIndex == 11){
            row = i;
            col = 4-i;
        }else if(axisIndex == 10){
            row = col = i;
        }else if(axisIndex > 4){
            row = i;
            col = axisIndex-5;
        }else{
            row = axisIndex-5;
            col = i;
        }
        int index = row*5 + col;
        if( cellWeights[index] > maxWeight ){
            maxWeight = cellWeights[index];
            numIndex = index;
        }
    }
    return numbers[numIndex];
}

int isBingo(){
    int rsum, csum, dsum=0, adsum=0, score=0;
    for(int i=0; i<5; i++){
        rsum = csum = 0;
        for(int j=0; j<5; j++){
            csum += hitMatrix[j*5+i];
            rsum += hitMatrix[i*5+j];
        }
        if(rsum==5)
            score++;
        if(csum==5)
            score++;
        dsum += hitMatrix[i*5+i];
        adsum += hitMatrix[i*5+(4-i)];

    }
    if(dsum==5)
        score++;
    if(adsum==5)
        score++;
    
    if( score>=5)
        return 1;
    else
        return 0;
}

int getIndex(int num){
    vector<int>::iterator it = find(numbers.begin(), numbers.end(), num);
    if( it == numbers.end()){
        cout<<"Could not find index of "<<num<<endl;
        return -1;
    }
    int index = distance(numbers.begin(), it);
    return index;
}