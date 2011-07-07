#ifndef Lorentzian_h_
#define Lorentzian_h_
#include "Curve.h"
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

class Lorentzian:
    public Curve
{
    public:

        /**
         * Inferred ranges and optimal values
         * some values may be null on construction
         * due to unknown upper or lower bounds or
         * optimal parameter values.
         */
        Lorentzian( double I,  double I_max,  double I_min, 
                double x,  double x_max,  double x_min, 
                double ep,  double ep_max,  double ep_min, int t, vector<int> skip)
        {
            I0 = I;
            I0_max = I_max;
            I0_min = I_min;
            x0 = x;
            x0_max = x_max;
            x0_min = x_min;
            ep0 = ep;
            ep0_max = ep_max;
            ep0_min = ep_min;
            ctype = t;
            skipping = skip;
        }

        string toString()
        {
            ostringstream s;
            s.precision(16);
            if(ctype == 0)
                s << "@";
            s << "CURVE LORENTZIAN" << endl;
            if(find(skipping.begin(), skipping.end(), 0) != skipping.end())
                s << "@";
            s << "I " << I0 << " IL " << I0_min << " IU " << I0_max << endl;
            if(find(skipping.begin(), skipping.end(), 1) != skipping.end())
                s << "@";
            s << "X " << x0 << " XL " << x0_min << " XU " << x0_max << endl;
            if(find(skipping.begin(), skipping.end(), 2) != skipping.end())
                s << "@";
            s << "EP " << ep0 << " EPL " << ep0_min << " EPU " << ep0_max << endl;
            return s.str();
        }

        string toString(vector<double> p)
        {
            ostringstream s;
            double p0, p1, p2;
            int sofar = 0;
            if(skip(0))
            {
                p0 = I0;
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
                p2 = ep0;
                sofar++;
            }
            else
                p2 = p[2 - sofar];
            s.precision(16);
            if(ctype == 0)
                s << "@";
            s << "CURVE LORENTZIAN" << endl;
            if(find(skipping.begin(), skipping.end(), 0) != skipping.end())
                s << "@";
            s << "I " << p0 << " IL " << I0_min << " IU " << I0_max << endl;
            if(find(skipping.begin(), skipping.end(), 1) != skipping.end())
                s << "@";
            s << "X " << p1 << " XL " << x0_min << " XU " << x0_max << endl;
            if(find(skipping.begin(), skipping.end(), 2) != skipping.end())
                s << "@";
            s << "EP " << p2 << " EPL " << ep0_min << " EPU " << ep0_max << endl;
            return s.str();
        }

        // supposed to be 3, if skipping will be 0
        int type()
        {
            return ctype;
        }

        void f_eval(vector< double> x, vector< double> &F)
        {
            for(int i = 0; i < x.size(); i++)
            {
                F[i] = F[i] + I0*ep0*ep0/((x[i]-x0)*(x[i]-x0) + ep0*ep0);
            }
            return;
        }

        void f_eval(vector< double> x, vector< double> p, vector< double> &F)
        {
            double p0, p1, p2;
            int sofar = 0;
            if(skip(0))
            {
                p0 = I0;
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
                p2 = ep0;
                sofar++;
            }
            else
                p2 = p[2 - sofar];
            for(int i = 0; i < x.size(); i++)
            {
                F[i] = F[i] + p0*pow(p2,2)/(pow(x[i]-p1,2) + pow(p2,2));
            }
            return;
        }

        double getParam(int i)
        {
            switch(i)
            {
                case 0: if(!skip(i))
                            return I0;
                        else
                            i++;
                case 1: if(!skip(i))
                            return x0;
                        else
                            i++;
                case 2: if(!skip(i))
                            return ep0;
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

        Lorentzian operator=(const Lorentzian& src)
        {
            I0 = src.I0;
            I0_min = src.I0_min; I0_max = src.I0_max;
            x0 = src.x0;
            x0_min = src.x0_min; x0_max = src.x0_max;
            ep0 = src.ep0;
            ep0_min = src.ep0_min; ep0_max = src.ep0_max;
        }

        ~Lorentzian(){}

    private:

        double I0, I0_max, I0_min;
        double x0, x0_max, x0_min;
        double ep0, ep0_max, ep0_min;
        vector<int> skipping;

        int ctype;

};

#endif
