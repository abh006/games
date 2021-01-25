#include "Cell.hpp"

class Axis{
    private:
        int numbers[5];
        int type;   // 1 for horizontal, 2 for vertical, 3 for ltr-diagonal, 4 for rtl-diagonal
        int rowIndex, colIndex;
        int weight, level;

        void initializeAxis();
    public:
        Cell cells[5];
        Axis(int[], int, int);
};

Axis::Axis(){
    
}
Axis::Axis(int nums[], int t, int index){ 
    numbers = nums;
    type = t;
    weight = 0;
    level = 5;

    switch(type){
        case 1:
            rowIndex = index;
            colIndex = -1;
            break;
        case 2:
            rowIndex = -1;
            colIndex = index;
            break;
        case 3:
            rowIndex = -1;
            colIndex = -1;
            break;
        case 4:
            rowIndex = -2;
            colIndex = -2;
    }
    initializeAxis();
}

void Axis::initializeAxis(){

}