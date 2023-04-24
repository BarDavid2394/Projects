#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_GRADE 100

static FILE *f;
static int nbins = 10;
//checks if the arguments inserted correctly, and reading from the input file.
void parse_arg(int argc, char **argv)
{
    f = stdin;
    for (int i = 1; i < argc; ++i)
    {
        //checks if the arguments that are given are not empty
        if (!strcmp(argv[i], "-"))
        {
            f = stdin;

        }
        else if (!strcmp(argv[i], "-n_bins"))
        {
            nbins = ((i < argc - 1)) ? atoi(argv[i + 1]) : 10;
            i++;
        }
        else
        {
            //opening the needed file with reading permission
            f = fopen(argv[i], "r");
        }
    }
}
//checks that all the grades are valid and insert the grades to the right bin
void operate(int *bins)
{
    int grade, retval, N=1;
    double pace;
    while (1)
    {
        //extracting all the grades from the input file
        retval = fscanf(f, "%d", &grade);
        N++;
        if(retval == EOF)
        {
            break; /* EOF, no more grades to extract */
        }
        else if (retval != 1)
        {
            fprintf(stderr, "Error: not a number\n");
            continue;
        }
        if(grade > MAX_GRADE || grade < 0)
        {
            fprintf(stderr, "Line number %d : Grade not valid\n", N-1);
            continue;
        }
        //Find bin
        int n = grade / (100 / nbins);
        if(n == 10) // if the score is 100 exactly
        {
            n=9;
        }
        bins[n]++;
    }
    //Printint the bins
    pace = 100.0 / nbins;
    for(int i=0; i<nbins; i++)
    {
        printf("%.0lf-%.0lf\t%d\n",
               i * pace,
               (i< nbins -1) ? ((i+1)* pace-1) :100,
               bins[i]);
    }
}
int main(int argc,char **argv)
{
    parse_arg(argc,argv);
    //checking if the file is in the directory
    if(!f)
    {
        fprintf(stderr,"File not found: \"%s\"\n",argv[1]);
        return 1;
    }
    //allocating and initiallizing the array for the bins
    int *bins = calloc(nbins,sizeof(int));
    operate(bins);
    free(bins);
}