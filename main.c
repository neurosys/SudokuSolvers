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

snod* get_line_element(snod* table, int nr_line, int line_element)
{
   int index =  9 * nr_line + line_element;
   return &table[index];
}

snod** get_line(struct nod* table, int line)
{
    int i = 0;
    snod** rez = (snod**) malloc(sizeof(snod*) * 9);
    for (i = 0; i < 9; i++)
    {
        rez[i] = get_line_element(table, line, i);
    }
    return rez;
}

snod* get_column_element(snod* table, int nr_col, int col_element)
{
    int index = 9 * col_element + nr_col;
    return &table[index];
}


snod** get_col(snod* table, int col)
{
    int i = 0;
    snod** rez = (snod**) malloc(sizeof(snod*) * 9);
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
            printf(" %d", get_line_element(table, i, j)->nr);
        }
        printf("\n");
    }
}

snod** get_cell(snod* table, int cel)
{
    int i = 0;
    int j = 0;
    int start_line = 3 * (cel / 3);
    int end_line = start_line + 3;
    int start_col = (cel % 3) * 3;
    int end_col = start_col + 3;
    snod** rez = (snod**) malloc(sizeof(snod*) * 9);
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
0:0[00] 0:1[01] 0:2[02]     0:3[03] 0:4[04] 0:5[05]     0:6[06] 0:7[07] 0:8[08]
1:0[09] 1:1[10] 1:2[11]     1:3[12] 1:4[13] 1:5[14]     1:6[15] 1:7[16] 1:8[17] 
2:0[18] 2:1[19] 2:2[20]     2:3[21] 2:4[22] 2:5[23]     2:6[24] 2:7[25] 2:8[26] 

3:0[27] 3:1[28] 3:2[29]     3:3[30] 3:4[31] 3:5[32]     3:6[33] 3:7[34] 3:8[35]
4:0[36] 4:1[37] 4:2[38]     4:3[39] 4:4[40] 4:5[41]     4:6[42] 4:7[43] 4:8[44] 
5:0[45] 5:1[46] 5:2[47]     5:3[48] 5:4[49] 5:5[50]     5:6[51] 5:7[52] 5:8[53] 

6:0[54] 6:1[55] 6:2[56]     6:3[57] 6:4[58] 6:5[59]     6:6[60] 6:7[61] 6:8[62]
7:0[63] 7:1[64] 7:2[65]     7:3[66] 7:4[67] 7:5[68]     7:6[69] 7:7[70] 7:8[71] 
8:0[72] 8:1[73] 8:2[74]     8:3[75] 8:4[76] 8:5[77]     8:6[78] 8:7[79] 8:8[80] 
*/

int get_cell_number(int pos_in_array)
{
    int line = pos_in_array / 9; 
    int col = pos_in_array % 9;
    int cel = 3 * (line / 3) + col / 3;
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


void print_available_options(snod x, int nr)
{
    if (x.nr == 0)
    {
        int i = 0;
        int col = nr % 9;
        int line = nr / 9;
        printf("line %d col %d nr %d cel %d >", line, col, nr, get_cell_number(nr));
        for (i = 0; i < 9; i++)
        {
            if (x.available[i] != 0)
            {
                printf(" %d", x.available[i]);
            }
        }
        printf("\n");
    }
}

void set_unit_available_options(snod** unit, int exclude_value)
{
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        if (unit[i]->nr == 0)
        {
            if (unit[i]->available == NULL)
            {
                printf("nr == 0 iar available e null !!!!!!!!!1\n");
            }

            unit[i]->available[exclude_value-1] = 0;
        }
    }
}

void set_available_options(snod** unit)
{
    int i = 0;
    for ( i = 0; i < 9; i++ )
    {
        if (unit[i]->nr != 0)
        {
            set_unit_available_options(unit, unit[i]->nr);
        }
    }
}

int analize_position(snod* table)
{
    snod** cel;
    snod** line;
    snod** col;
    int i = 0;

    for (i = 0; i < 9; i++)
    {
        cel = get_cell(table, i);
        set_available_options(cel);
        free(cel);
    }

    for (i = 0; i < 9; i++)
    {
        line = get_line(table, i);
        set_available_options(line);
        free(line);
    }

    for (i = 0; i < 9; i++)
    {
        col = get_col(table, i);
        set_available_options(col);
        free(col);
    }
    return -1;
}

int main()
{
    snod* table;
    int i = 0;

    table = (snod*) malloc(sizeof(snod) * 81);
    build_table(original_table, table);
    show(table);

    analize_position(table);

    for (i = 0; i < 81; i++)
    {
        print_available_options(table[i], i);
    }

    printf("\n\n");
    free(table);
    return 0;
}
