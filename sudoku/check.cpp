#include<iostream>
#include<vector>
#include<bits/stdc++.h>
#define size 81

using namespace std;

void printMatrix(vector<int>);
int** initSquares();
int isValid(vector<int>);

int main(){

    vector<int> matrix(81);
    int x;

    cout<<"Sudoku Matrix: \n";
    for(int i=0; i<size; i++){
        cin>>x;
        matrix.at(i) = x;
    }
    printMatrix(matrix);
    if(isValid(matrix)==0){
        cout<<"Invalid";
    }else{
        cout<<"Valid";
    }
    return 0;
}

void printMatrix(vector<int> m){
    for(int i=0; i<size; i++){
        if(i%27==0)
            cout<<endl;
        if(i%9==0)
            cout<<endl;
        if(i%3==0)
            cout<<"\t";
        cout<<m[i]<<"\t";
    }
    cout<<endl;
}

int** initSquares(){
    int **squares=0;
    squares = new int*[3];
    for(int i=0; i<3; i++){
        squares[i] = new int[10];
        for(int j=0; j<10; j++)
            squares[i][j] = 0;
    }
    return squares;
}


int isValid(vector<int> matrix){
    
    int **squares = initSquares();
    int num, squareCount;

    for(int i=0; i<9; i++){
        vector<int> rowList(10,0), colList(10,0);
        for(int j=0; j<9; j++){
            num = matrix[i*9 + j];
            
            // check if num is repeating in its corresponging square
            squareCount = j/3;
            if( squares[squareCount][num] > 0 ){
                return 0;
            }else{
                squares[squareCount][num] = 1;
            }

            // check if num is repeating in the row
            if(rowList[num]>0){
                return 0;
            }else{
                rowList[num] = 1;
            }
            
            num = matrix[j*9+i];
            // check if num is repeating in the column
            if(colList[num] > 0){
                return 0;
            }else{
                colList[num] = 1;
            }
        }
        if(i%3==2){
            squares = initSquares();
        }
    }
    return 1;
}