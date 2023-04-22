#include <stdio.h>
#include <string.h>
#define MAX_GRADE 100

int main(int argc, char *argv[])
{
    FILE *fp;
   double grades = 0, sum = 0;
   int N = 0;
    //checks if there is one argument given as needed and that it's not empty
    if (argc == 1 || !strcmp("-", argv[1]))
    {
        fp = stdin;
    } else
    {
        //opening needed the file with reading permission
        fp = fopen(argv[1], "r");
    }
    //extracting all the grades from the input file
    while (fscanf(fp, "%lf", &grades) != EOF)
    {
        //checks if the grade is valid. if not, printing an error
        if (grades > MAX_GRADE || grades < 0)
        {
            fprintf(stderr, "Line number %d : Grade not valid\n", N);
            N--;
            continue;
        }
        N++;
        sum += grades;
    }
    //calculating the average
    double average = (sum / N);
    //closing the file, printing the average to the stdout file
    fclose(fp);
    fprintf(stdout, "%.2lf\t", average);
    return 0;
}