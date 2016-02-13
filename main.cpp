#include <stdio.h>
#include <assert.h>

/*
 * TODO
 * - Binary flags to represent the 9 possible options, the 9 possible values stored, and option for uncompleted
 */

typedef unsigned char ubyte;
typedef unsigned short int uword;

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
    printf("SetNotAvail(%d)\n", n);
    Print();
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
    Print();
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
        //printf("opts_left = %d GetNumAvail() = %d\n", opts_left, c2.GetNumAvail());
        assert(c2.GetNumAvail() == opts_left);
    }
    assert(c2.GetVal() == 9);

    return false;
}

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = '%s'\n", i, argv[i]);
    }
    test_Cell();
    return 0;
}
