#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//----------------------------------------------------------------------------------------------------------------------
const double l = 0.5;
const double d = 0.00035;
//----------------------------------------------------------------------------------------------------------------------
const double Tolerance = 1e-10;
const double ns_answer = 1e-7;
const double MinValue = 0.0;
const double MaxValue = 1000.0;
const char data_file[] = "labadata.txt";
const char result_file[] = "labaresult.txt";
//----------------------------------------------------------------------------------------------------------------------
// author. Mulihov Arthur vk.com/0chickenmcnuggets0

//прототипы функций
void calculatelaba();
void readdata();
void write_laba();
int tolerance();
//функции отвечающие за подсчет сопротивление и погрешности при помощи мнк
double calculate_mnk();
double calculate_delta();
double middle_UI();
double middle_sqrI();
double middle_sqrU();
//----------------------------------------------------------------------------------------------------------------------
int main()
{
    printf("Write number of lines in your laba:\n");
    int nlines;
    scanf("%d", &nlines);

    //creating and clearing massive of measures
    double* U = (double*) calloc(nlines, sizeof(double));
    double* I = (double*) calloc(nlines, sizeof(double));
    double* R = (double*) calloc(nlines, sizeof(double));

    if(!U || !I || !R)
    {
        printf("Allocation failure!\n");
        return 1;
    }

    // required variables for calculation of indirect measurements
    double epsilon = 0;
    double Ro = 0;
    double delta_ro = 0;

    // reading measurements from a file
    readdata(I, U, R, &nlines);

    // calculation of mnk
    double mnk = calculate_mnk(I, U, &nlines);
    double delta = calculate_delta(I, U, &nlines);

    // calculation of final data
    calculatelaba(&mnk, &delta, &Ro, &delta_ro, &epsilon);

    // printing final data
    write_laba(R, &Ro, &delta_ro, &epsilon, &nlines, &delta);

    free(U);
    free(I);
    free(R);

    return 0;
}

void readdata(double I[], double U[], double R[], int* number)
{
    FILE* labadata = fopen(data_file , "r");
    if (!labadata) {printf("Can't open labadata.txt\n"); return;}

    int i = 0;
    for(i = 0; i < *number; i++)
    {
        fscanf (labadata, "%lg %lg", &U[i], &I[i]);

        if ((tolerance(&U[i]) == 1) || (tolerance(&I[i]) == 1))
        {
            printf("Please, check your measures");
            return;
        }

        R[i] = U[i] / I[i];
    }
    fclose(labadata);
}

void calculatelaba(double* mnk_ptr, double* delta_ptr, double* Ro, double* delta_ro, double* epsilon) // main calculates
{
    *Ro = (( *mnk_ptr * 3.14 * d * d ) / ( 4 * l )) / ns_answer;
    *delta_ro = (( *delta_ptr * 3.14 * d * d ) / ( 4 * l )) / ns_answer;
    *epsilon = ( *delta_ro / *Ro ) * 100;
    if ((tolerance(Ro) == 1) || (tolerance(delta_ro) == 1) || (tolerance(epsilon) == 1))
    {
        printf("Strange values\n");
        return;
    }
}

void write_laba(double R[], double* Ro, double* delta_ro, double* epsilon, int* number, double* delta)
{
    FILE* labaresult = fopen(result_file , "w");
    if (!labaresult) printf("Can't open labaresult.txr\n");

    for(int i = 0; i <= *number; i++)
    {
        if(i == 0) fprintf(labaresult, "(%lg +-%lg)*10^-7 [Ohm * m] (E = %.1lg ) \n", *Ro, *delta_ro, *epsilon);
        if(i != 0) fprintf(labaresult, "R[%d] = %lgOhm +- %lg\n", i, R[i - 1], *delta);
    }
    fclose(labaresult);
}

//----------------------------------------------------------------------------------------------------------------------
double calculate_mnk(double I[], double U[], int* number)
{
    double middleUI = middle_UI(I, U, number);
    double middlesqrI = middle_sqrI(I, number);

    double R = middleUI / middlesqrI;

    return R;
}

double calculate_delta(double I[], double U[], int* number)
{
    double middlesqrU = middle_sqrU(U, number);
    double middlesqrI = middle_sqrI(I, number);
    double R = calculate_mnk(I, U, number);

    double delta_r = 1 / sqrt(*number);
    delta_r = delta_r * sqrt(middlesqrU / middlesqrI - R * R);


    return delta_r;
}
//----------------------------------------------------------------------------------------------------------------------
double middle_UI(double I[], double U[], int* number)
{
    double sum = 0;
    for(int i = 0; i < *number; i++)
        sum  = sum + (U[i] * I[i]);

    return (sum / *number);
}

double middle_sqrI(double I[], int* number)
{
    double sum = 0;
    for(int i = 0; i < *number; i++)
        sum = sum + (I[i] * I[i]);

    return (sum / *number);
}

double middle_sqrU(double U[], int* number)
{
    double sum = 0;
    for(int i = 0; i < *number; i++)
        sum = sum + (U[i] * U[i]);

    return (sum / *number);
}
//----------------------------------------------------------------------------------------------------------------------

int tolerance(double* a)
{
    if (*a < MinValue - Tolerance || *a > MaxValue - Tolerance) return 1;
    if(fabs(*a - 0.0) < Tolerance)  return 1;
    return 0;
}
