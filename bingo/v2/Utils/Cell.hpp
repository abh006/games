class Cell{
    private:
        int number, row, col, weight;
    public:
        Cell(int, int, int);
};

Cell::Cell(){
    
}
Cell::Cell(int num, int r, int c ){
    number = num;
    row = r;
    col = c;
    weight = 0;
}