#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/*
double* getModelParam(string line, string val);
double getData(string aline, double *d);
int* getNum(string pline, string val);
*/

int main(int argc, char *argv[])
{
    double w = 5.0;
    double *x = NULL;
    double *y = x;
    double *z = new double(2342.0230235);
    delete z; z = NULL;
    bool tw = &w == NULL;
    bool ty = y == NULL;
    bool tx = x == NULL;
    bool tz = z == NULL;
    bool c = z == x;
    cout << w << " " << x << " " << y << " " << z << endl;
    cout << &w << " " << &x << " " << &y << " " << &z << " " << endl;
    cout << tw << " " << tx << " " << ty << " " << tz << " " << c << endl;
    cout << endl;
    y = new double(42.0);
    ty = y == NULL;
    tx = x == NULL;
    tz = z == NULL;
    c = z == x;
    cout << w << " " << x << " " << y << " " << z << endl;
    cout << &w << " " << &x << " " << &y << " " << &z << " " << endl;
    cout << tw << " " << tx << " " << ty << " " << tz << " " << c << endl;
    cout << endl;
    *y = NULL;
    x = new double(NULL);
    z = y;
    ty = y == NULL;
    tx = x == NULL;
    tz = z == NULL;
    c = z == x;
    cout << w << " " << x << " " << y << " " << z << endl;
    cout << w << " " << *x << " " << *y << " " << *z << endl;
    cout << &w << " " << &x << " " << &y << " " << &z << " " << endl;
    cout << tw << " " << tx << " " << ty << " " << tz << " " << c << endl;
    cout << endl << endl << endl;
}
    /*
    string s = "A 23.255 AL 42.2135 AU NULL";
    int pos1 = s.find("A ")+2;
    int pos2 = s.find(" ",pos1);
    int pos3 = s.find("AL ")+3;
    int pos4 = s.find(" ",pos3);
    int pos5 = s.find("AU ");
    //cout << pos1 << " " << pos2 << " " << pos3 << " " << pos4 << " " << pos5 << endl;
    //cout << s << endl;
    //cout << "a " << s.substr(pos1, pos2-pos1) << " al " << s.substr(pos3, pos4-pos3) <<
    //    " au " << s.substr(pos5+3) << endl;
    //cout << getModelParam(s, "AU ") << endl;
    double *d;
    d = new double[3];
    string aline = "0.251235214241 231.215234124 743.2342346134";
    getData(aline, d);
    //cout << aline << endl;
    //cout << d[0] << " " << d[1] << " " << d[2] << endl;
    string pline = "NUMCURVES NULL NUMPARAMS 36";
    cout << "hel1" << endl;
    int *x = getNum(pline, "NUMCURVES ");
    cout << "hel2" << endl;
    int *y = getNum(pline, "NUMPARAMS ");
    cout << "done extracting" << endl;
    cout.flush();
    cout << "x = " ;
    cout << x << endl;
    cout << "y = " ;
    cout << y << endl;
    return 0;
}

int* getNum(string line, string val)
{
    int p1, p2;
    stringstream tmp;
    int *i;
    p1 = line.find(val) + val.size();
    p2 = line.find(" ", p1);
    cout << "substr " << line.substr(p1, p2-p1) << endl;
    tmp << line.substr(p1, p2-p1);
    cout << "substr " << line.substr(p1, p2-p1) << endl;
    if(tmp.str().compare("NULL") == 0)
    {
        cout << "substr " << line.substr(p1, p2-p1) << endl;
        i = NULL;
        cout << "substr " << i << endl;
    }
    else
    {
        cout << "substr " << line.substr(p1, p2-p1) << endl;
        tmp >> *i;
        cout << "substr " << line.substr(p1, p2-p1) << endl;
    }
    return i;
}

double* getModelParam(string line, string val)
{
    int p1, p2;
    stringstream tmp;
    double *d;
    p1 = line.find(val) + val.size();
    p2 = line.find(" ", p1);
    tmp << line.substr(p1, p2-p1);
    if(tmp.str().compare("NULL") == 0)
        d = NULL;
    else
        tmp >> *d;
    return d;
}

double getData(string line, double *d)
{
    int p1, p2;
    stringstream tmp;
    p1 = line.find(" ");
    p2 = line.find(" ", p1+1);
    tmp << line.substr(0, p1);
    tmp >> d[0];
    tmp.str(std::string());
    tmp.clear();
    tmp << line.substr(p1+1, p2-p1);
    tmp >> d[1];
    tmp.str(std::string());
    tmp.clear();
    tmp << line.substr(p2+1);
    tmp >> d[2];
    return *d;
}
*/
