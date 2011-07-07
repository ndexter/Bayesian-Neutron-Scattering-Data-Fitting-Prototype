#include <cerrno>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
    
    errno = 0;

    ofstream fout;
    char tmp[100];
    sprintf(tmp,"out.txt");
    fout.open(tmp);
    
    try
    {
        string line; 
        char holder[100];
        char * pEnd;
        int i = 1, j = 0;
        double *d = new double[1000];
        ifstream inf ("Raw.nxs");

        if(inf.is_open()){
            while(! inf.eof()){
                getline(inf, line);

                string::iterator it;
                d[0] = strtod(holder,&pEnd);
                for(it=line.begin(); it < line.end(); it++){
                    /*if(isdigit(*it) || *it == '.'){
                        holder[j] = *it;
                        j++;
                    }else{*/
                        d[i] = strtod(pEnd, NULL);
                    //    holder[0] = '\0';
                        i++;
                    //    j=0;
                    //}
                }
            }
        }

        for(int k = 0; k < i+1; k++){
            fout << d[k] << endl;
        }

        inf.close();
    } catch(...){
        if(errno != 0)
            perror("System error: ");
    }

    return 0;
}
