#include <stdio.h>
#include <string.h>
#define MAX_GRADE 100

int main(int argc, char *argv[])
{
    FILE *fp;
    int N=0, grades=0, sum = 0, median = 0;
   int hist[MAX_GRADE+1] = {0};
   //checks if there is one argument given as needed and that it's not empty
    if (argc == 1 || !strcmp("-", argv[1]))
    {
        fp = stdin;
    }
    else
    {
        //opening needed the file with reading permission
        fp =fopen(argv[1],"r");
    }
    //extracting all the grades from the input file
    while(fscanf(fp, "%d", &grades) !=EOF )
    {
        N++;
        //checks if the grade is valid. if not, printing an error
        if(grades > MAX_GRADE || grades < 0)
        {
            fprintf(stderr, "Line number %d : Grade not valid\n", N);
            N--;
            continue;
        }
        hist[grades]++;
        }
    //M is valued for half the students.
    int M = (N+1)/2, temp_2=0;
    //we need to check in which we went through half of the students
    while (sum < M)
    {
        sum +=hist[median];
        temp_2 = median;
        median++;
    }
    //if the number of odd, then the median is the "middle" student's grade.
    if (N%2 == 1)
    {
        fprintf(stdout,"%d\t",(temp_2));
        return 0;
    }
    else //if the nubmer is even
    {
        if(sum > 501)
        {
            fprintf(stdout,"%d\t", temp_2);
        }
        else // in case that the last grade inserted was the 500th student.
        {
          while(median<=MAX_GRADE)
          {
              if(hist[median] != 0)
              {
                  fprintf(stdout,"%d\t", median);
                  break;
              }
              median++;
          }
        }
    }
    //closing the file, printing the median to the stdout file
    fclose(fp);
return 0;
}




















