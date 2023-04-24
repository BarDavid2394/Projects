#include <stdio.h>
#include <string.h>
#define MAX_GRADE 100

int main(int argc, char *argv[])
{
    FILE *fp;
    int min = 100, grades = 0, N = 1;
    //checks if there is one argument is given as needed and that it's not empty
    if (argc == 1 || !strcmp("-", argv[1]))
    {
        fp = stdin;
    }
    else
    {
        //opening needed the file with reading permission
        fp = fopen(argv[1], "r");
    }
    //extracting all the grades from the input file
    while (fscanf(fp, "%d", &grades) != EOF)
    {
        //checks if the grade is valid. if not, printing an error
        if (grades > MAX_GRADE || grades < 0)
        {
            fprintf(stderr, "Line number %d : Grade not valid\n", N);
            continue;
        }
        N++;
        if (grades < min)
        {
            min = grades;
        }
    }
    //closing the file, printing the min to the stdout file
    fclose(fp);
    fprintf(stdout, "%d\t", min);
    return 0;
}