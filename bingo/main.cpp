#include<iostream>
#include<vector>
#include<numeric>
#include<bits/stdc++.h>
#include<math.h>

#define HIT_NUMBER -1

using namespace std;

vector<int> updateMatrix(vector<int>, int);
vector<int> getHitMatrix(vector<int>);
vector<int> getResultantWeightMatrix(vector<int>);
int callNumber(vector<int>);
int getNeighbourSum(vector<int>, int, int);
int getMaxWeightedCornerIndex(vector<int>);
bool isBingo(vector<int>);
void printMatrices(vector<int>, vector<int>, int[]);

int main(){
    vector<int> numbers(25), hitMatrix(25);
    int  origNumbers[25];
    int noOfPlayers;

    iota(begin(numbers), end(numbers), 1);
    shuffle(numbers.begin(),numbers.end(), default_random_engine(0));
    
    copy(numbers.begin(), numbers.end(), origNumbers);

    cout<<"No of players (max: 3) including Computer : ";
    cin>>noOfPlayers;

    int num = -1, turn = 0;
    while(num != 0){    
        turn=0;
        while(turn<noOfPlayers){
            printMatrices(hitMatrix, numbers, origNumbers);
            if( turn==noOfPlayers-1){
                num = numbers[callNumber(hitMatrix)];
                cout<<"Computer says: "<<num<<endl;
            }else{
                cout<<"Player "<<turn+1<<": ";
                cin>>num;
            }
            if( num == 0 ){
                cout<<"Did you win? (0/1)"<<endl;
                cin>>num;
                if( num == 1 ){
                    cout<<"Player "<<turn+1<<" BINGO"<<endl;
                    num=0;
                    break;
                }
            }

            numbers = updateMatrix(numbers, num);
            hitMatrix = getHitMatrix(numbers);
            if( isBingo(hitMatrix)){
                cout<<"Bingo"<<endl;
                num= 0;
                break;
            }
            turn++;
        }       
    }

    printMatrices(hitMatrix, numbers, origNumbers);

    return 0;
}

void printMatrices(vector<int> hitMatrix, vector<int> numbers, int origNumbers[]){
    cout<<"Hit Matrix"<<endl;
    for(int i=0; i<25; i++){
        if(i%5==0)
            cout<<endl;
        cout<<hitMatrix[i]<<" ";
    }
    cout<<"Updated Matrix"<<endl;
    for(int i=0; i<25; i++){
        if(i%5==0)
            cout<<endl;
        cout<<numbers[i]<<" ";
    }
    cout<<"Original Matrix"<<endl;
    for(int i=0; i<25; i++){
        if(i%5==0)
            cout<<endl;
        cout<<origNumbers[i]<<" ";
    }
}

vector<int> updateMatrix(vector<int> numbers, int num){
    vector<int>::iterator it = find(numbers.begin(), numbers.end(), num);
    if( it == numbers.end()){
        cout<<"Invalid number"<<endl;
        return vector<int>();
    }
    int index = distance(numbers.begin(), it);
    numbers[index] = HIT_NUMBER;
    return numbers;
}

vector<int> getHitMatrix(vector<int> numbers){
    vector<int> hitMatrix(25);
    for(int i=0; i<25; i++){
        if( numbers[i] == HIT_NUMBER)
            hitMatrix[i] = 1;
        else
            hitMatrix[i] = 0;
    }
    return hitMatrix;
}

vector<int> getResultantCellWeightMatrix(vector<int> hitMatrix){
    vector<int> rowHit(5), colHit(5);
    
    iota(begin(rowHit), end(rowHit), 0);
    iota(begin(colHit), end(colHit), 0);
    
    vector<int> weightMatrix(25);
    
    int colIndex, rowIndex;
    for(int i=0; i<25; i++){
        if( hitMatrix[i] == 1){
            colIndex = i%5;
            rowIndex =  i/5;
            colHit[colIndex] += 1;
            rowHit[rowIndex] += 1;
        }
    }

    // calculate weightMatrix
    int row=0, col=0, invertedHit;
    for(int i=0; i<25; i++){
        row = i/5;
        col = i%5;
        invertedHit = 1 - hitMatrix[i];
        weightMatrix[i] = (rowHit[row] + colHit[col])*invertedHit;
    }
    return weightMatrix;
}

bool isBingo(vector<int> hitMatrix){
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
        return true;
    else
        return false;
}

int callNumber(vector<int> hitMatrix){
    vector<int> weightMatrix = getResultantCellWeightMatrix(hitMatrix);
    int max = *max_element(weightMatrix.begin(), weightMatrix.end());
    
    int neighbourSum, maxNeighbourSum=0, index, cornerDistance, minCornerDistance=26;
    for(int i=0; i<25; i++){
        if(weightMatrix[i] != max)
            continue;

        neighbourSum = getNeighbourSum(weightMatrix, i/5, i%5);
        if(neighbourSum>maxNeighbourSum){
            maxNeighbourSum = neighbourSum;
            index = i;
        }else if(neighbourSum == maxNeighbourSum){
            cornerDistance = abs(getMaxWeightedCornerIndex(weightMatrix) - i);
            if(cornerDistance<minCornerDistance){
                minCornerDistance = cornerDistance;
                maxNeighbourSum = neighbourSum;
                index = i;
            }
        }
    }
    return index;
}

int getNeighbourSum(vector<int> weightMatrix, int row, int col){
    int col_start = col-1, col_end = col+1;
    int row_start = row-1, row_end = row+1;

    if(col_start<0)
        col_start = 0;
    if(col_end>4)
        col_end = 4;
    if(row_start<0)
        row_start = 0;
    if(row_end>4)
        row_end = 4;
    
    int num, sum;
    for(int i=row_start; i<=row_end; i++){
        for(int j=col_start; j<=col_end; j++){
            if(i==row && j==col){
                continue;
            }
            num = weightMatrix[i*5+j];
            if(num==0)
                num = 26;
            if(i==j){
                num+=1;
            }
            sum += num;
            
        }
    }
    return sum;
}

int getMaxWeightedCornerIndex(vector<int> weightMatrix){
    vector<int> corners = { weightMatrix[0],
                            weightMatrix[4], 
                            weightMatrix[20],
                            weightMatrix[24]};
    int indices[] = {0,4,20,24};
    int index = max_element(corners.begin(), corners.end())-corners.end();
    return indices[index];
}