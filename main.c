#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define uchar unsigned char
#define snod struct nod
#define ALL_SET 0x3fe

//#define normal 1
#define insane 1

#ifdef normal
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
#else 
#ifdef insane
unsigned char original_table[] = {
    8, 0, 0,      0, 0, 0,      0, 0, 0,
    0, 0, 3,      6, 0, 0,      0, 0, 0,
    0, 7, 0,      0, 9, 0,      2, 0, 0,


    0, 5, 0,      0, 0, 7,      0, 0, 0,
    0, 0, 0,      0, 4, 5,      7, 0, 0,
    0, 0, 0,      1, 0, 0,      0, 3, 0,


    0, 0, 1,      0, 0, 0,      0, 6, 8,
    0, 0, 8,      5, 0, 0,      0, 1, 0,
    0, 9, 0,      0, 0, 0,      4, 0, 0
};
#else
unsigned char original_table[] = {
    0, 0, 0,      0, 0, 0,      0, 0, 0,
    0, 0, 0,      0, 0, 0,      0, 0, 0,
    0, 0, 0,      0, 0, 0,      0, 0, 0,


    0, 0, 0,      0, 0, 0,      0, 0, 0,
    0, 0, 0,      0, 6, 0,      0, 0, 0,
    0, 0, 0,      0, 0, 0,      0, 0, 0,


    0, 0, 0,      0, 0, 0,      0, 0, 0,
    0, 0, 0,      0, 0, 0,      0, 0, 0,
    0, 0, 0,      0, 0, 0,      0, 0, 0
};
#endif // insane
#endif // normal


struct nod
{
    unsigned char nr;
    //int nr_available_options;
    unsigned char* available;
};

int is_still_valid(snod* table);
snod* copy_table(snod* table);
void print_unit(snod* u);
void show(snod* table);
int store_new_values(snod* table);
void free_table(snod* table);
int finished_solving(snod* table);
int analize_position(snod* table);

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

void free_table(snod* table)
{
    int i = 0;
    for (i = 0; i < 81; i++)
    {
        if (table[i].available != NULL)
        {
            free(table[i].available);
        }
    }
    free(table);
}


void print_available_options(snod x, int nr)
{
    if (x.nr == 0)
    {
        int i = 0;
        int col = nr % 9;
        int line = nr / 9;
        //printf("line %d col %d nr %d cel %d nr_avail %d >", line, col, nr, get_cell_number(nr), x.nr_available_options);
        printf("line %d col %d nr %2d cel %d >", line, col, nr, get_cell_number(nr));
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

/* Returns 1 if it made changes int the available options */
int set_unit_available_options(snod** unit, int exclude_value)
{
    int i = 0;
    int made_changes = 0;
    for (i = 0; i < 9; i++)
    {
        if (unit[i]->nr == 0)
        {
            if (unit[i]->available == NULL)
            {
                printf("nr == 0 and available == NULL!!! BUG!!!\n");
            }

            if (unit[i]->available[exclude_value-1] != 0)
            {
                unit[i]->available[exclude_value-1] = 0;
                made_changes = 1;
            }
        }
    }
    return made_changes;
}

/* Returns 1 if it made changes at the available options level */
int set_available_options(snod** unit)
{
    int i = 0;
    int changes = 0;
    for ( i = 0; i < 9; i++ )
    {
        if (unit[i]->nr != 0)
        {
            changes |= set_unit_available_options(unit, unit[i]->nr);
        }
    }
    return changes;
}

/* Returns 1 if it made changes over the available options */
int analize_position(snod* table)
{
    snod** cel;
    snod** line;
    snod** col;
    int i = 0;
    int changes = 0;

    for (i = 0; i < 9; i++)
    {
        cel = get_cell(table, i);
        changes |= set_available_options(cel);
        free(cel);
    }

    for (i = 0; i < 9; i++)
    {
        line = get_line(table, i);
        changes |= set_available_options(line);
        free(line);
    }

    for (i = 0; i < 9; i++)
    {
        col = get_col(table, i);
        changes |= set_available_options(col);
        free(col);
    }
    return changes;
}

/* Returns 1 if it permanently stored a new value */
int store_new_values(snod* table)
{
    int i = 0;
    int j = 0;
    int stored_new_values = 0;
    for (i = 0; i < 81; i++)
    {
        int non_zero_pos = -1;
        int unique = 1;
        if (table[i].nr == 0)
        {
            /* find if there is only one available option */
            for (j = 0; j < 9 && unique; j++)
            {
                if (table[i].available[j] != 0)
                {
                    if (unique == 1 && non_zero_pos == -1)
                    {
                        non_zero_pos = j;
                    }
                    else
                    {
                        unique = 0;
                    }
                }
            }

            if (unique == 1)
            {
                table[i].nr = table[i].available[non_zero_pos];
                stored_new_values = 1;
            }
        }
    }
    return stored_new_values;
}

int improve_solution(snod* table)
{
    int has_changed = 1;
    while (has_changed == 1)
    {
        has_changed = analize_position(table);
        //printf("improve_solution() has_changed = %d\n", has_changed);
        if (has_changed)
        {
            snod* test_table = copy_table(table);
            int tmp_rez = 0;
            tmp_rez |= store_new_values(test_table);
            tmp_rez |= is_still_valid(test_table);
            free_table(test_table);
            if (tmp_rez)
            {
                store_new_values(table);
                has_changed = 1;
            }
            else
            {
                has_changed = 0;
            }
        }
    }
    return has_changed;
}

int set_bit(int* flag, int pos)
{
    int mask = 1;
    mask <<= pos;
    *flag |= mask;
    return *flag;
}

int check_bit(int flag, int pos)
{
    int mask = 1;
    mask <<= pos;
    flag &= mask;
    return flag;
}

int has_all_elements(snod** unit)
{
    int i = 0;
    int bit_map = 0;
    for (i = 0; i < 9; i++)
    {
        //printf("%2d", unit[i]->nr);
        set_bit(&bit_map, unit[i]->nr);
    }
    //printf("\n");
    if ((bit_map & ALL_SET) == ALL_SET)
    {
        //printf("has_all_elements = 1, bit_map = %X[%d] ALL_SET = %X\n", bit_map, bit_map, ALL_SET);
        return 1;
    }
    return 0;
}

int finished_solving(snod* table)
{
    int i = 0;
    snod** cel;
    snod** line;
    snod** col;
    int is_finished = 1;
    for (i = 0; i < 9; i++)
    {
        cel = get_cell(table, i);
        is_finished &= has_all_elements(cel);
        free(cel);
    }

    for (i = 0; i < 9; i++)
    {
        line = get_line(table, i);
        is_finished &= has_all_elements(line);
        free(line);
    }

    for (i = 0; i < 9; i++)
    {
        col = get_col(table, i);
        is_finished &= has_all_elements(col);
        free(col);
    }

    if (is_finished)
    {
        return 1;
    }
    return 0;
}

int has_duplicate_elements(snod** unit)
{
    int i = 0;
    int bit_map = 0;
    for (i = 0; i < 9; i++)
    {
        if (unit[i]->nr != 0)
        {
            if (check_bit(bit_map, unit[i]->nr))
            {
                return 1;
            }
            else
            {
                set_bit(&bit_map, unit[i]->nr);
            }
        }
    }
    return 0;
}

int is_still_valid(snod* table)
{
    int i = 0;
    snod** cel;
    snod** line;
    snod** col;
    int is_broken = 0;
    for (i = 0; i < 9; i++)
    {
        cel = get_cell(table, i);
        is_broken |= has_duplicate_elements(cel);
        free(cel);
    }
    //printf("1 is_broken > %d\n", is_broken);

    for (i = 0; i < 9; i++)
    {
        line = get_line(table, i);
        is_broken |= has_duplicate_elements(line);
        free(line);
    }
    //printf("2 is_broken > %d\n", is_broken);

    for (i = 0; i < 9; i++)
    {
        col = get_col(table, i);
        is_broken |= has_duplicate_elements(col);
        free(col);
    }
    //printf("2 is_broken > %d\n", is_broken);

    return !is_broken;
}

snod* copy_table(snod* table)
{
    int i = 0;
    int j = 0;
    snod* new_table = NULL;
    new_table = (snod*) malloc(sizeof(snod) * 81);
    for (i = 0; i < 81; i++)
    {
        new_table[i].nr = table[i].nr;
        new_table[i].available = NULL;
        if (new_table[i].nr == 0)
        {
            new_table[i].available = (uchar*)malloc(sizeof(uchar) * 9);
            for (j = 0; j < 9; j++)
            {
                new_table[i].available[j] = table[i].available[j];
            }
        }
    }
    return new_table;
}

int get_position_with_less_options(snod* table)
{ 
    int i = 0;
    int j = 0;
    int best_position = -1;
    int best_value = 0xFFFF;
    for (i = 0; i < 81; i++)
    {
        if (table[i].nr == 0)
        {
            int bit_map = 0;
            for (j = 0; j < 9; j++)
            {
                set_bit(&bit_map, table[i].available[j]);
            }
            if (bit_map < best_value)
            {
                best_value = bit_map;
                best_position = i;
            }
        }
    }
    return best_position;
}

int app(snod* table)
{
    int i = 0;
    static int nr_of_passes = 1;

    improve_solution(table);

    if (!is_still_valid(table))
    {
        return 0;
    }

    if (finished_solving(table))
    {
        printf("Finished in %d passes\n", nr_of_passes);
        show(table);
        exit(0);
        return 0;
    }
    else
    {
        nr_of_passes++;
        int position_for_guess = -1;

        position_for_guess = get_position_with_less_options(table);
        if (position_for_guess != -1)
        {
            for (i = 0; i < 9; i++)
            {
                if (table[position_for_guess].available[i] != 0)
                {
                    snod* new_table = NULL;
                    new_table = copy_table(table);

                    new_table[position_for_guess].nr = new_table[position_for_guess].available[i];
                    if (is_still_valid(new_table))
                    {
                        app(new_table);
                    }
                    free(new_table);
                }
            }
        }
    }

    return 0;
}

int main()
{
    snod* table;

    table = (snod*) malloc(sizeof(snod) * 81);
    build_table(original_table, table);
    show(table);

    app(table);

    show(table);
    free_table(table);
    printf("\n\n");
    return 0;
}
