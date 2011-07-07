#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <cerrno>
#include <fstream>
#include <ctime>

using namespace std;

int main(int argc,char *argv[]){
    errno = 0;

    ofstream fout;
    char tmp[100];
    sprintf(tmp, "x3");
    fout.open(tmp);

    double data[500];  // = new double[500];

    double noise[500]; // The error at each point

    int j = 0;

    double c = 1.325;

    double sigma = 0.f;

    try
    {
       srand(time(NULL));

       /* Setup constants */
       const static int q = 15;
       const static double c1 = (1 << q) - 1;
       const static double c2 = ((int)(c1 / 3)) + 1;
       const static double c3 = 1.f / c1;

       /* random number in range 0 - 1 not including 1 */
       double random = 0.f;

       /* the white noise */
       double noise = 0.f;

       for(double i = 0.2; i < 100; i += 0.2){
            random = ((double)rand() / (double)(RAND_MAX + 1));
            noise = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
            sigma += noise;
            data[j] = sin(i) + noise + 2 + c;
            fout << data[j] << endl;
            j++;
        }

    } catch (...){
        if(errno != 0)
            perror("System error: ");
    }

    double fit[500];

    sigma = sigma/j;

    double c_fit = 100, error;
    double d_max = -100, d_min = 100;

    for(int a = 0; a < j+1; a++){
        if(d_max < data[a])
            d_max = data[a];
        if(d_min > data[a])
            d_min = data[a];
    }

    double prob = 0;

    double sum = 0;
    
    c_fit = (d_max + d_min)/2;
    cout << c_fit << endl;

    double prior = 1;

    int m = 0;

    while(prob < 0.95){
        for(double i = 0.2; i < 100; i += 0.2){
           sum += pow(sigma, 1-j)*exp((data[m] - (sin(i) + c_fit))/pow(sigma,2));
           m++;
        }
        m = 0;
        prob = sum * prior;
        cout << prob << endl;
    }
    return 0;
}
