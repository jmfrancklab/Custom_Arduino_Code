#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// this is *not* arduino code
// I built this with
// gcc write_example.cpp -o test

enum { SIZE = 5 };
int main(void)
{
    double a[SIZE] = {1, 2, 3, 4, 5};
    FILE *f1 = fopen("file.bin", "wb");
    assert(f1);
    size_t r1 = fwrite(a, sizeof a[0], SIZE, f1);
    printf("wrote %zu elements out of %d requested\n", r1,  SIZE);
    fclose(f1);
}



