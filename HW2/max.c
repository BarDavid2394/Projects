#include <stdio.h>
#include <string.h>
#define MAX_GRADE 100

int main(int argc, char *argv[])
{
    FILE *fp;
    int max = 0, grades = 0, N = 1;
    //checks if there is one argument  given as needed and that it's not empty
    if (argc == 1 || !strcmp("-", argv[1]))
    {
        fp = stdin;
    }
    else
    {
        //reading the file containing the student's grades
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
        if (grades > max)
        {
            max = grades;
        }
    }
    //closing the file, printing the max to the stdout file
    fclose(fp);
    fprintf(stdout, "%d\t", max);
    return 0;
}