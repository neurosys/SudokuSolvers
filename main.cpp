#include <stdio.h>
#include <assert.h>

typedef unsigned char ubyte;
typedef unsigned short int uword;

// TODO test whether shifting the bit to create a mask is much slower
// than having an static const array with the association between the
// digits and the masks so that we won't compute them each time
class Cell
{
private:
    ubyte row;
    ubyte col;
    ubyte val;
    uword opts;
    ubyte numopts;

public:
    Cell(ubyte row, ubyte col, ubyte val);
    Cell(Cell& cell);
    Cell& operator=(Cell c);
    ~Cell();

    ubyte GetRow() { return this->row; }
    ubyte GetCol() { return this->col; }

    // Return the value of the cell zero means no value is set
    ubyte GetVal() { return this->val; }

    // Return the number of available options for the current cell
    // If the cell has a value, no options are available, therefore
    // the returned value is zero
    ubyte GetNumAvail();

    // Sets n as an unavailable option for this cell
    // if the cell has only one option left, it will
    // set that option as it's value, ignoring the next
    // attepmpts to set it as unavailable
    void SetNotAvail(ubyte n);

    // Dump all the internal values
    void Print();

private:
    // Check the number of available options, if only one is left
    // that one is set as the value for the current cell
    void UpdateVal();
};

Cell::Cell(ubyte row, ubyte col, ubyte val)
{
    this->row = row;
    this->col = col;
    this->val = val;

    // Value consisting of 9 positions of 1
    opts = 0x01FF;
    numopts = 9;
}

Cell::Cell(Cell& c)
{
    this->row = c.row;
    this->col = c.col;
    this->val = c.val; 
    this->opts = c.opts;
    this->numopts = c.numopts;
}

Cell::~Cell()
{
    // ???
}

Cell& Cell::operator=(Cell c)
{
    this->row = c.row;
    this->col = c.col;
    this->val = c.val; 
    this->opts = c.opts;
    this->numopts = c.numopts;

    return *this;
}

void Cell::Print()
{
    char vals[] = "123456789";
    uword bit = 0x1;
    for (int i = 0; i < 9; i++, bit <<= 1)
    {
        if (!(bit & opts))
        {
            vals[i] = ' ';
        }
    }
    // row col opts
    printf("[r%d c%d] = %d (numopts %d) (opts = %s)\n",
            row, col, val, numopts, vals);
}

ubyte Cell::GetNumAvail()
{
    if (val != 0)
    {
        return 0;
    }
    return numopts;
}

void Cell::UpdateVal()
{
    if (numopts != 1)
    {
        return;
    }

    // There is only one value available for this cell
    // find which one is and set it as the current value
    uword bit = 0x1;
    for (int i = 0; i < 9 && val == 0; i++, bit <<= 1)
    {
        if (bit & opts)
        {
            val = i + 1;
        }
    }
}

void Cell::SetNotAvail(ubyte n)
{
    if (n > 9 || n == 0)
    {
        return;
    }

    uword bit = 0x1;
    bit <<= n-1;

    // Check to see if the value has already been ruled out
    if (!(opts & bit))
    {
        return;
    }

    numopts--;
    opts ^= bit;

    UpdateVal();
}

/*
class Map
{
    Cell* cells[9];
public:
    Map(Cell* cells[9]);
    Map(Map& grp);
    ~Map();
    Map& operator=(Map grp);
};
*/

class GridParser
{
public:
    GridParser();
    
    bool GeneratePuzzleGrid(char* file_name);
    bool ReadPuzzleFromCSV(char* file_name);
};

GridParser::GridParser()
{
    // Do nothing for the moment
}

bool GridParser::GeneratePuzzleGrid(char* fname)
{
    if (fname == NULL || *fname == '\0')
    {
        return false;
    }

    FILE* f = fopen(fname, "w+t");
    if (f == NULL)
    {
        return false;
    }

    const char top[] = "+---+---+---+---+---+---+---+---+---+\n";
    const char row[] = "|   |   |   |   |   |   |   |   |   |\n";

    for (int i = 0; i < 9; i++)
    {
        fprintf(f, top);
        fprintf(f, row);
    }
    fprintf(f, top);
    fflush(f);
    fclose(f);
    return true;
}

bool ReadPuzzleFromCSV(char* fname)
{
    if (fname == NULL || *fname == '\0')
    {
        return false;
    }
    FILE* f = fopen(fname, "r+t");

    if (f == NULL)
    {
        return false;
    }

    //
    return true;
}

bool test_Cell()
{
    ubyte row = 1;
    ubyte col = 2;
    ubyte val = 9;
    ubyte val2 = 0;
    Cell c(row, col, val);
    Cell c2(row, col, val2);
    assert(c.GetRow() == row);
    assert(c.GetCol() == col);

    assert(c.GetVal() == val);
    assert(c2.GetVal() == val2);

    assert(c.GetNumAvail() == 0);
    assert(c2.GetNumAvail() == 9);

    // On c (which has a value of 9, SetNotAvail()
    // has no effect
    c.SetNotAvail(1);
    assert(c.GetNumAvail() == 0);

    // When left with only one option, the
    // object sets its value by itself
    int opts_left = 0;
    for (int i = 0; i < 9; i++)
    {
        opts_left = 9 - (i + 1);
        opts_left = (opts_left == 1) ? 0 : opts_left;

        c2.SetNotAvail(i + 1);
        assert(c2.GetNumAvail() == opts_left);
    }
    assert(c2.GetVal() == 9);

    return true;
}

/*
bool test_Map()
{
    Cell* cells[9] = 
    {
        c1(1, 0, 1),
        c2(2, 0, 2),
        c3(3, 0, 3),
        c4(4, 0, 4),
        c5(5, 0, 5),
        c6(6, 0, 6),
        c7(7, 0, 7),
        c8(8, 0, 8),
        c9(9, 0, 0)
    }
    CellGroup cg = Column(cells);

    assert(cg.UpdateCells());
    assert(c9.GetVal() == 9);


    return true;
}
*/

bool test_GridParser()
{
    GridParser gp;
    char template_file_name[] = "Sudoku_template.txt";
    assert(gp.GeneratePuzzleGrid(template_file_name));
    assert(gp.ReadPuzzleFromCSV());
    //gp.ReadPuzzle(template_file_name);
    return true;
}

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = '%s'\n", i, argv[i]);
    }
    test_Cell();
    //test_Line();
    test_GridParser();
    return 0;
}
