#ifndef Polynomial_h_
#define Polynomial_h_
#include "Curve.h"
#include <sstream>
#include <cmath>

using namespace std;

class Polynomial:
    public Curve
{
    public:

        Polynomial(double **p_0, int degree, int t, vector<int> skip)
        {
            for(int i = 0; i < degree; i++)
            {
                p0.push_back(p_0[i][0]);
                p0_min.push_back(p_0[i][1]);
                p0_max.push_back(p_0[i][2]);
            }
            skipping = skip;
            ctype = t;
        }

        string toString()
        {
            ostringstream s;
            s.precision(16);
            if(ctype == 0)
                s << "@";
            s << "CURVE POLYNOMIAL " << p0.size() << endl;
            for(int i = 0; i < p0.size(); i++)
            {
                if(skip(i))
                    s << "@";
                s << "P" << i << " " << p0[i]
                  << " P" << i << "L " << p0_min[i]
                  << " P" << i << "U " << p0_max[i] << endl;
            }
            return s.str();
        }

        string toString(vector<double> p)
        {
            ostringstream s;
            int sofar;
            vector<double> pout;
            sofar = 0;
            if(skip(0))
            {
                pout.push_back(p0[0]);
                sofar++;
            }
            else pout.push_back(p[0]);
            for(int j = 1; j < p0.size(); j++)
            {
                if(skip(j))
                {
                    pout.push_back(p0[j]);
                    sofar++;
                }
                else pout.push_back(p[0]);
            }
            s.precision(16);
            if(ctype == 0)
                s << "@";
            s << "CURVE POLYNOMIAL " << pout.size() << endl;
            for(int i = 0; i < pout.size(); i++)
            {
                if(skip(i))
                    s << "@";
                s << "P" << i << " " << pout[i]
                  << " P" << i << "L " << p0_min[i]
                  << " P" << i << "U " << p0_max[i] << endl;
            }
            return s.str();
        }

        // supposed to be 1, if skipping will be 0
        int type()
        {
            return ctype;
        }

        void f_eval(vector<double> x, vector<double> &F)
        {
            for(int i = 0; i < x.size(); i++) 
            {
                F[i] += p0[0];
                for(int j = 1; j < p0.size(); j++)
                    F[i] += p0[j]*pow(x[i],j);
            }
            return;
        }

        void f_eval(vector< double> x, vector< double> p, vector< double> &F)
        {
            int sofar;
            for(int i = 0; i < x.size(); i++)
            {
                sofar = 0;
                if(skip(0))
                {
                    F[i] += p0[0];
                    sofar++;
                }
                else
                    F[i] += p[0];
                for(int j = 1; j < p0.size(); j++)
                {
                    if(skip(j))
                    {
                        F[i] += p0[j]*pow(x[i],j);
                        sofar++;
                    }
                    else
                        F[i] += p[j - sofar]*pow(x[i],j);
                }
            }
            return;
        }

        int paramNum()
        {
            return p0.size() - skipping.size();
        }

        int ignored()
        {
            return skipping.size();
        }

        double getParam(int i)
        {
            do
            {
                if(!skip(i) && i < p0.size())
                    return p0[i];
                else if(i >= p0.size())
                    return -999.99f;
                i++;
            }
            while(true);
        }

        bool skip(int i)
        {
            return find(skipping.begin(), skipping.end(), i) != skipping.end();
        }

        Polynomial operator=(const Polynomial& src)
        {
            if(p0.size() == src.p0.size() &&
               p0_min.size() == src.p0_min.size() &&
               p0_max.size() == src.p0_max.size())
            {
                for(int i = 0; i < p0.size(); i++)
                {
                    p0[i] = src.p0[i];
                    p0_min[i] = src.p0_min[i];
                    p0_max[i] = src.p0_max[i];
                }
            }
        }

        ~Polynomial(){}

    private:

         vector<double> p0;
         vector<double> p0_min;
         vector<double> p0_max;
         vector<int> skipping;

         int ctype;

};

#endif
