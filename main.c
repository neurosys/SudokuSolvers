#include <stdio.h>
#include <malloc.h>

#define uchar unsigned char

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

void print_unit(uchar* u)
{
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        printf(" %d", u[i]);
    }
    printf("\n");
}

void show(struct nod* table)
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

uchar get_line_element(struct nod* table, int nr_line, int line_element)
{
   int index =  9 * nr_line + line_element;
   return table[index].nr;
}

uchar* get_line(struct nod* table, int line)
{
    int i = 0;
    unsigned char* rez = (unsigned char*) malloc(sizeof(unsigned char) * 9);
    for (i = 0; i < 9; i++)
    {
        rez[i] = get_line_element(table, line, i);
    }
    return rez;
}

uchar get_column_element(struct nod* table, int nr_col, int col_element)
{
    int index = 9 * col_element + nr_col;
    return table[index].nr;
}


uchar* get_col(struct nod* table, int col)
{
    int i = 0;
    uchar* rez = (uchar*) malloc(sizeof(uchar) * 9);
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
void print_cell(struct nod* table, int cel)
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
            printf(" %d", get_line_element(table, i, j));
        }
        printf("\n");
    }
}

uchar* get_cell(struct nod* table, int cel)
{
    int i = 0;
    int j = 0;
    int start_line = 3 * (cel / 3);
    int end_line = start_line + 3;
    int start_col = (cel % 3) * 3;
    int end_col = start_col + 3;
    uchar* rez = (uchar*) malloc(sizeof(uchar) * 9);
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

void build_table(unsigned char table[], struct nod* dest)
{
    int i = 0;
    for (i = 0; i < 81; i++)
    {
        dest[i].nr = table[i];
        dest[i].available = NULL;
    }
}

void set_available_options(struct nod* table, int cel)
{
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        uchar* cell;
        cell = get_cell(table, i);

        free(cell);
    }
}

int main()
{
    struct nod* table;
    uchar* line = NULL;
    uchar* col = NULL;
    int i = 0;
    table = (struct nod*) malloc(sizeof(struct nod) * 81);
    build_table(original_table, table);
    show(table);
    col = get_cell(table, 1);
    print_unit(col);
    printf("\n\n");
    free(table);
    return 0;
}
