/*
 * Superclass for curves
 */ 
#ifndef Curve_h_
#define Curve_h_
#include <string>
#include <vector>
extern "C"
{
    #include <gsl/gsl_vector.h>
}

using namespace std; 

class Curve
{
    public: 
        
        virtual string toString() = 0;

        virtual string toString(vector<double> p) = 0;

        virtual int type() = 0;

        virtual void f_eval(vector<double> x, vector<double> &F) = 0;

        virtual void f_eval(vector<double> x, vector<double> p, vector<double> &F) = 0;

        virtual int paramNum() = 0;

        virtual bool skip(int i) = 0;

        virtual int ignored() = 0;

        virtual double getParam(int i) = 0;

};

#endif
