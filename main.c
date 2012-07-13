#include <stdio.h>
#include <malloc.h>

#define uchar unsigned char
#define snod struct nod

unsigned char original_table[] = {
    3, 1, 2,      0, 9, 5,      0, 7, 6,
    5, 0, 9,      1, 0, 7,      0, 8, 2,
    4, 0, 7,      2, 6, 3,      5, 0, 0,


    9, 0, 0,      7, 0, 0,      2, 4, 0,
    0, 2, 8,      0, 1, 0,      0, 9, 3,
    0, 3, 0,      9, 8, 2,      0, 5, 7,


    0, 4, 5,      6, 0, 0,      0, 3, 1,
    1, 7, 0,      3, 5, 8,      9, 0, 4,
    8, 0, 3,      4, 2, 0,      7, 0, 5
};


struct nod
{
    unsigned char nr;
    unsigned char* available;
};

void print_unit(snod* u)
{
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        printf(" %d", u[i].nr);
    }
    printf("\n");
}

void show(snod* table)
{
    int i;
    for (i = 0; i < 81; i++)
    {
        if ((i % 9) == 0)
        {
            if ((i % 3) == 0 && i != 0)
            {
                printf("|");
            }
            printf("\n");
            if (i % 27 == 0)
            {
                printf("+------+------+------+");
                printf("\n");
            }
        }

        if ((i % 3) == 0)
        {
            printf("|");
        }
        if (table[i].nr)
        {
            printf("%2d", table[i].nr);
        }
        else
        {
            printf("  ");
        }
    }
    printf("|\n+------+------+------+\n");
}

snod get_line_element(snod* table, int nr_line, int line_element)
{
   int index =  9 * nr_line + line_element;
   return table[index];
}

snod* get_line(struct nod* table, int line)
{
    int i = 0;
    snod* rez = (snod*) malloc(sizeof(snod) * 9);
    for (i = 0; i < 9; i++)
    {
        rez[i] = get_line_element(table, line, i);
    }
    return rez;
}

snod get_column_element(snod* table, int nr_col, int col_element)
{
    int index = 9 * col_element + nr_col;
    return table[index];
}


snod* get_col(snod* table, int col)
{
    int i = 0;
    snod* rez = (snod*) malloc(sizeof(snod) * 9);
    for (i = 0; i < 9; i++)
    {
        rez[i] = get_column_element(table, col, i);
    }
    return rez;
}

/*
 0 1 2
 3 4 5
 6 7 8

 0 line 0 col 0
 1 line 0 col 3
 2 line 0 col 6

 3 line 3 col 0
 4 line 3 col 3
 5 line 3 col 6

 6 line 6 col 0
 7 line 6 col 3
 8 line 6 col 6
*/
void print_cell(snod* table, int cel)
{
    int i = 0;
    int j = 0;
    int start_line = 3 * (cel / 3);
    int end_line = start_line + 3;
    int start_col = (cel % 3) * 3;
    int end_col = start_col + 3;

    for (i = start_line; i < end_line; i++)
    {
        for (j = start_col; j < end_col; j++)
        {
            printf(" %d", get_line_element(table, i, j).nr);
        }
        printf("\n");
    }
}

snod* get_cell(snod* table, int cel)
{
    int i = 0;
    int j = 0;
    int start_line = 3 * (cel / 3);
    int end_line = start_line + 3;
    int start_col = (cel % 3) * 3;
    int end_col = start_col + 3;
    snod* rez = (snod*) malloc(sizeof(snod) * 9);
    int q = 0;

    for (i = start_line; i < end_line; i++)
    {
        for (j = start_col; j < end_col; j++, q++)
        {
            rez[q] = get_line_element(table, i, j);
        }
    }
    return rez;
}

/*
 [0 - 3) 1
 [9 -12) 1
 [18-21) 1

 [3 - 6) 2
 [12-15) 2
 [21-24) 2

 [6 - 9) 3
 [15-18) 3
 [24-27) 3

 */

int get_cell_number(int pos_in_array)
{
    int line = pos_in_array / 9; 
    int col = pos_in_array % 9;
    int cel = 3 * (line % 3) + (col / 3) ;
    //printf("pos = %d, line = %d, col = %d, cel = %d\n", pos_in_array, line, col, cel);
    return cel;
}

void build_table(uchar table[], snod* dest)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < 81; i++)
    {
        dest[i].nr = table[i];
        dest[i].available = NULL;

        if (dest[i].nr == 0)
        {
            dest[i].available = (uchar*) malloc(sizeof(uchar) * 9);
            for (j = 0; j < 9; j++)
            {
                dest[i].available[j] = j + 1;
            }
        }
    }
}


void print_available_options(snod x)
{
    if (x.nr != 0)
    {
        printf("Node %d available None!\n", x.nr);
    }
    else
    {
        int i = 0;
        for (i = 0; i < 9; i++)
        {
            printf(" %d", x.available[i]);
        }
        printf("\n");
    }
}

void set_intern_available_cell_options(snod* cel, int exclude_value)
{
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        if (cel[i].nr == 0)
        {
            if (cel[i].available == NULL)
            {
                printf("nr == 0 iar available e null !!!!!!!!!1\n");
            }

            cel[i].available[exclude_value-1] = 0;
        }
    }
}

void set_available_options(snod* table, int cel_nr)
{
    int i = 0;
    snod* cell = NULL;

    cell = get_cell(table, cel_nr);
    for (i = 0; i < 9; i++)
    {
        if (cell[i].nr != 0)
        {
            set_intern_available_cell_options(cell, cell[i].nr);
        }
    }
}

int main()
{
    snod* table;
    snod* cel;
    int i = 0;

    table = (snod*) malloc(sizeof(snod) * 81);
    build_table(original_table, table);
    show(table);

    set_available_options(table, 0);
    cel = get_cell(table, 0);
    for (i = 0; i < 9; i++)
    {
        print_available_options(cel[i]);
    }

    printf("\n\n");
    free(table);
    return 0;
}
