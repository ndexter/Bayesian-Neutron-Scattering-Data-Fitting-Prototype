#ifndef Gaussian_h_
#define Gaussian_h_
#include "Curve.h"
#include <sstream>
#include <cmath>

using namespace std;

const  double pi = 3.14159265358979;

class Gaussian:
    public Curve
{
    public:

        /**
          * Inferred ranges and optimal values
          * some values may be null on construction
          * due to unknown upper or lower bounds or
          * optimal parameter values.
          */
        Gaussian( double A,  double A_min,  double A_max, 
                 double x,  double x_min,  double x_max, 
                 double w,  double w_min,  double w_max, int t,
                 vector<int> skip)
        {
            A0 = A;
            A0_max = A_max;
            A0_min = A_min;
            x0 = x;
            x0_max = x_max;
            x0_min = x_min;
            w0 = w;
            w0_max = w_max;
            w0_min = w_min;
            ctype = t;
            skipping = skip;
        }

        string toString()
        {
            ostringstream s;
            s.precision(16);
            if(ctype == 0)
                s << "@";
            s << "CURVE GAUSSIAN" << endl;
            if(find(skipping.begin(), skipping.end(), 0) != skipping.end())
                s << "@";
            s << "A " << A0 << " AL " << A0_min << " AU " << A0_max << endl;
            if(find(skipping.begin(), skipping.end(), 1) != skipping.end())
                s << "@";
            s << "X " << x0 << " XL " << x0_min << " XU " << x0_max << endl;
            if(find(skipping.begin(), skipping.end(), 2) != skipping.end())
                s << "@";
            s << "W " << w0 << " WL " << w0_min << " WU " << w0_max << endl;
            return s.str();
        }

        string toString(vector<double> p)
        {
            ostringstream s;
            double p0, p1, p2;
            int sofar = 0;
            if(skip(0))
            {
                p0 = A0;
                sofar++;
            }
            else
                p0 = p[0];
            if(skip(1))
            {
                p1 = x0;
                sofar++;
            }
            else
                p1 = p[1 - sofar];
            if(skip(2))
            {
                p2 = w0;
                sofar++;
            }
            else
                p2 = p[2 - sofar];
            s.precision(16);
            if(ctype == 0)
                s << "@";
            s << "CURVE GAUSSIAN" << endl;
            if(find(skipping.begin(), skipping.end(), 0) != skipping.end())
                s << "@";
            s << "A " << p0 << " AL " << A0_min << " AU " << A0_max << endl;
            if(find(skipping.begin(), skipping.end(), 1) != skipping.end())
                s << "@";
            s << "X " << p1 << " XL " << x0_min << " XU " << x0_max << endl;
            if(find(skipping.begin(), skipping.end(), 2) != skipping.end())
                s << "@";
            s << "W " << p2 << " WL " << w0_min << " WU " << w0_max << endl;
            return s.str();
        }

        // supposed to be 2, if skipping will be 0
        int type()
        {
            return ctype;
        }

        void f_eval(vector< double> x, vector< double> &F)
        {
            for(int i = 0; i < x.size(); i++)
            {
                F[i] = F[i] + A0/(w0*sqrt(2*pi))*exp(-1.0*pow(x[i]-x0,2)/(2*pow(w0,2)));
            }
            return;
        }

        void f_eval(vector< double> x, vector< double> p, vector< double> &F)
        {
            double p0, p1, p2;
            int sofar = 0;
            if(skip(0))
            {
                p0 = A0;
                sofar++;
            }
            else
                p0 = p[0 - sofar];
            if(skip(1))
            {
                p1 = x0;
                sofar++;
            }
            else
                p1 = p[1 - sofar];
            if(skip(2))
            {
                p2 = w0;
                sofar++;
            }
            else
                p2 = p[2 - sofar];
            for(int i = 0; i < x.size(); i++)
            {
                F[i] = F[i] + p0/(p2*sqrt(2*pi))*exp(-1.0*pow(x[i]-p1,2)/(2*pow(p2,2)));
            }
            return;
        }

        double getParam(int i)
        {
            switch(i)
            {
                case 0: if(!skip(i))
                            return A0;
                        else
                            i++;
                case 1: if(!skip(i))
                            return x0;
                        else
                            i++;
                case 2: if(!skip(i))
                            return w0;
                        else
                            i++;
                default: return -999.999f;
            }
        }

        bool skip(int i)
        {
            return find(skipping.begin(), skipping.end(), i) != skipping.end();
        }

        int paramNum()
        {
            return 3 - skipping.size();
        }

        int ignored()
        {
            return skipping.size();
        }

        Gaussian operator=(const Gaussian& src)
        {
            A0 = src.A0;
            A0_min = src.A0_min; A0_max = src.A0_max;
            x0 = src.x0;
            x0_min = src.x0_min; x0_max = src.x0_max;
            w0 = src.w0;
            w0_min = src.w0_min; w0_max = src.w0_max;
        }

        ~Gaussian(){}

    private:

         double A0, A0_max, A0_min;
         double x0, x0_max, x0_min;
         double w0, w0_max, w0_min;

         vector<int> skipping;

         int ctype;

};

#endif
