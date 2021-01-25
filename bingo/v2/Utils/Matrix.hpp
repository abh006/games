#include<vector>
#include "Axis.hpp"

class Matrix{
    private:
        std::vector<int> cutNumbers;
    public:
        Axis axes[12]; 
        // 5 horizontal + 5 vertical + 2 diagonals 
        // indexing in the order horizontal(5)-vertical(5)-ltr diagonal-rtl diagonal

        Matrix();
};

Matrix::Matrix(){
    for(int i=0; i<12; i++){
        axes[i] = Axis();
    }
}