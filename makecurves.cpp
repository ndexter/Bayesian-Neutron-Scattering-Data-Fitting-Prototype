#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cerrno>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

int main(int argc, char * argv[]){

    ofstream fout;
    char tmp[100];
    sprintf(tmp, "data.txt");
    fout.open(tmp);

    // Seed the randomness..
    //srand(time(NULL));
    // Eat up that first number
    //rand();

    // ...with M Gaussians and N Lorentzians...
    //int M = 1 + (int)(((double)rand()/(RAND_MAX))*10);
    //int N = 1 + (int)(((double)rand()/(RAND_MAX))*10);

    int M = 2;
    int N = 2;

    // Domain size
    double dom = 100.0, dx = 0.2;

    // Ranges for variables, and PI
    const double pi = 3.14159265;
    /*double A_max = 0.5, x1_min = dom*0.15, x1_max = dom*0.85, w_max = 5;
    double I_max = 0.5, x2_min = dom*0.15, x2_max = dom*0.85, ep_max = 10;*/
    //double b_min = 0.0001, b_max = 0.001;

    // Parameters for Gaussian
    double A0[M];
    double x10[M];
    double w0[M];

    
    // Parameters for Lorentzian
    double I0[N];
    double x20[N];
    double ep0[N];

    // Background
    //double b0[2];
    

    /*
    // Generate random values for M Gaussians
    for(int k = 0; k < M; k++){
        A0[k] = ((double)rand()/(RAND_MAX))*A_max;
        x10[k] = ((double)rand()/(RAND_MAX))*(x1_max - x1_min) + x1_min;      
        w0[k] = ((double)rand()/(RAND_MAX))*w_max;
    }

    // Generate random values for N Lorentzians
    for(int k = 0; k < N; k++){
        I0[k] = ((double)rand()/(RAND_MAX))*I_max;
        x20[k] = ((double)rand()/(RAND_MAX))*(x2_max - x2_min) + x2_min;      
        ep0[k] = ((double)rand()/(RAND_MAX))*ep_max;
    }
    */

    // Gaussian 1
    A0[0] =  0.323522462348967;
    x10[0] = 23.45213246892734;
    w0[0] =  2.257345283457123;
    // Lorentzian 1
    I0[0] =  0.267434121896798;
    x20[0] = 44.26121893257192;
    ep0[0] = 7.277423496230948;
    // Gaussian 2
    A0[1] =  0.226324289351729;
    x10[1] = 57.23313847109324;
    w0[1] =  4.262323581723940;
    // Lorentzian 2
    I0[1] =  0.473433817502357;
    x20[1] = 83.21113243867102;
    ep0[1] = 9.166472893476024;

    //b0[0] = ((double)rand()/(RAND_MAX))*(b_max - b_min) + b_min;
    //b0[1] = ((double)rand()/(RAND_MAX))*(b_max - b_min) + b_min;

    //cout.setf(ios::fixed,ios::floatfield);
    //cout.precision(13);
    cout << setprecision(16) << fixed;
    cout << "M = " << M << endl;
    cout << "A0 = {\t\t\tx10 = {\t\t\tw0 = {\n";
    for(int d = 0; d < M; d++){ 
        cout << " " << A0[d] << "\t " << x10[d] << "\t " <<  w0[d] << endl;
    }
    cout << " }\t\t\t }\t\t\t }" << endl;

    
    cout << "N = " << N << endl;
    cout << "I0 = {\t\t\tx20 = {\t\t\tep0 = {\n";
    for(int d = 0; d < N; d++){ 
        cout << " " << I0[d] << "\t " << x20[d] << "\t " <<  ep0[d] << endl;
    }
    cout << " }\t\t\t }\t\t\t }" << endl;
    

    int i = 0;

    double data[(int)(dom/dx)];

    for(double x = 0.0; x <= dom + dx; x+= dx){
        data[i] = (double)0.0;
        // Gaussian peaks
        for(int k = 0; k < M; k++){
            data[i] += (double)A0[k]/(w0[k]*sqrt(2*pi))*exp(-1.0*(x-x10[k])*(x-x10[k])/
                    (2*w0[k]*w0[k]));
        }
        
        // Lorentzian peaks
        for(int k = 0; k < N; k++){
            data[i] += (double)I0[k]*ep0[k]*ep0[k]/(double)((x-x20[k])*(x-x20[k]) + ep0[k]*ep0[k]);
        }
        // Background 
        //data[i] += b0[0]*x + b0[1];
        
        fout << x << " " << data[i] << endl;
        i++;
    }
    
    return 0;
}
