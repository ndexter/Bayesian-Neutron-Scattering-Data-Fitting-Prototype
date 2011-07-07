#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Curve.h"
#include "Gaussian.h"
#include "Lorentzian.h"
#include "Polynomial.h"
extern "C" 
{
    #include <gsl/gsl_math.h>
    #include <gsl/gsl_vector.h>
    #include <gsl/gsl_multimin.h>
}

using namespace std;

// Prototype declarations
double obj_f(const gsl_vector *x, void *params);

double obj_df(const gsl_vector *x, void *params);

bool getModelConfig(string line);

double getFunctionParam(string line, string val);

double getData(string line, double *d);

template < typename T > T **Allocate2DArray( int nRows, int nCols);

template < typename T > void Free2DArray(T** Array);

vector<Curve*> curves;
vector<double**> curveparams;
vector<double> ranges;
vector<double> space;
vector<double> intensity;
vector<double> errordown;
vector<double> errorup;

gsl_vector *x_orig;

double evalprev = 0.0;
double evalcur = 100.0;
double constraint = 0.1;

size_t iter = 0;
int* modelConfig = NULL;
int* paraminfo = NULL;
string optimizer;

ofstream fout ("out.txt");
ofstream final ("final.txt");

bool flag = false;
int itcount = 0;

int curveskip = 0;
int paramskip = 0;

int main(int argc, char * argv[]){

    //for(int z = 1; z < argc; z++)
    //    cout << argv[z] << endl;

    ifstream data ("data.txt");
    ifstream model ("model.txt");

    string sdata;
    string smodel;

    string gaussian = "GAUSSIAN";
    string lorentzian = "LORENTZIAN";
    string polynomial = "POLYNOMIAL";

    bool parsed = false;

    int degree = 0;
    int type = 0;

    vector<int> skip;

    stringstream tmp;

    Gaussian *gauss;
    Lorentzian *lor;
    Polynomial *poly;

    if(data.is_open() && model.is_open())
    {
        double **icurve;
        while(!model.eof())
        {
            getline(model, smodel);
            if(smodel.find("NUMCURVES") != string::npos)
            {
                parsed = getModelConfig(smodel);
            } 
            else if(smodel.find("CURVE") != string::npos)
            {
                // CURVE GAUSSIAN
                // A $A0 AL $A0_min AU $A0_max
                // X $x0 AL $x0_min AU $x0_max
                // W $w0 WL $w0_min WU $w0_max
                if(smodel.find(gaussian) != string::npos && !model.eof())
                {
                    type = 2;
                    if(smodel.find("@") != string::npos)
                    {
                        curveskip++;
                        type = 0;
                    }
                    icurve = Allocate2DArray<double> (3, 3);
                    getline(model, smodel);
                    if(smodel.find("@") != string::npos)
                    {
                        paramskip++;
                        skip.push_back(0);
                    }
                    icurve[0][0] = getFunctionParam(smodel, "A ");
                    icurve[0][1] = getFunctionParam(smodel, "AL ");
                    icurve[0][2] = getFunctionParam(smodel, "AU ");
                    ranges.push_back(icurve[0][1]);
                    ranges.push_back(icurve[0][2]);
                    getline(model, smodel);
                    if(smodel.find("@") != string::npos)
                    {
                        paramskip++;
                        skip.push_back(1);
                    }
                    icurve[1][0] = getFunctionParam(smodel, "X ");
                    icurve[1][1] = getFunctionParam(smodel, "XL ");
                    icurve[1][2] = getFunctionParam(smodel, "XU ");
                    ranges.push_back(icurve[1][1]);
                    ranges.push_back(icurve[1][2]);
                    getline(model, smodel);
                    if(smodel.find("@") != string::npos)
                    {
                        paramskip++;
                        skip.push_back(2);
                    }
                    icurve[2][0] = getFunctionParam(smodel, "W ");
                    icurve[2][1] = getFunctionParam(smodel, "WL ");
                    icurve[2][2] = getFunctionParam(smodel, "WU ");
                    ranges.push_back(icurve[2][1]);
                    ranges.push_back(icurve[2][2]);
                    gauss = new Gaussian(icurve[0][0], icurve[0][1], icurve[0][2],
                            icurve[1][0], icurve[1][1], icurve[1][2],
                            icurve[2][0], icurve[2][1], icurve[2][2], type, skip);
                    curves.push_back(gauss);
                    curveparams.push_back(icurve);
                    skip.clear();
                }
                // CURVE LORENTZIAN
                // I $A0 AL $A0_min AU $A0_max
                // X $x0 AL $x0_min AU $x0_max
                // EP $ep0 EPL $ep0_min EPU $ep0_max
                else if(smodel.find(lorentzian) != string::npos && !model.eof())
                {
                    type = 3;
                    if(smodel.find("@") != string::npos)
                    {
                        curveskip++;
                        type = 0;
                    }
                    icurve = Allocate2DArray<double> (3, 3);
                    getline(model, smodel);
                    if(smodel.find("@") != string::npos)
                    {
                        paramskip++;
                        skip.push_back(0);
                    }
                    icurve[0][0] = getFunctionParam(smodel, "I ");
                    icurve[0][1] = getFunctionParam(smodel, "IL ");
                    icurve[0][2] = getFunctionParam(smodel, "IU ");
                    ranges.push_back(icurve[0][1]);
                    ranges.push_back(icurve[0][2]);
                    getline(model, smodel);
                    if(smodel.find("@") != string::npos)
                    {
                        paramskip++;
                        skip.push_back(1);
                    }
                    icurve[1][0] = getFunctionParam(smodel, "X ");
                    icurve[1][1] = getFunctionParam(smodel, "XL ");
                    icurve[1][2] = getFunctionParam(smodel, "XU ");
                    ranges.push_back(icurve[1][1]);
                    ranges.push_back(icurve[1][2]);
                    getline(model, smodel);
                    if(smodel.find("@") != string::npos)
                    {
                        paramskip++;
                        skip.push_back(2);
                    }
                    icurve[2][0] = getFunctionParam(smodel, "EP ");
                    icurve[2][1] = getFunctionParam(smodel, "EPL ");
                    icurve[2][2] = getFunctionParam(smodel, "EPU ");
                    ranges.push_back(icurve[2][1]);
                    ranges.push_back(icurve[2][2]);
                    lor = new Lorentzian(icurve[0][0], icurve[0][1], icurve[0][2],
                            icurve[1][0], icurve[1][1], icurve[1][2],
                            icurve[2][0], icurve[2][1], icurve[2][2], type, skip);
                    curves.push_back(lor);
                    curveparams.push_back(icurve);
                    skip.clear();
                }
                // CURVE POLYNOMIAL $degree
                // P0 $p0[0] P0L $p0_min[0] P0U $p0_max[0]
                // .
                // .
                // .
                // P[degree-1] $p0[degree-1] P[degree-1]L...
                //          ... $p0_min[degree-1] P[degree-1]U $p0_max[degree-1]
                else if(smodel.find(polynomial) != string::npos && !model.eof())
                {
                    type = 1;
                    if(smodel.find("@") != string::npos)
                    {
                        curveskip++;
                        type = 0;
                    }
                    stringstream tmp;
                    tmp << smodel.substr(smodel.find(" ", 
                                smodel.find(polynomial) + polynomial.size()));
                    tmp >> degree;
                    tmp.str(std::string());
                    tmp.clear();
                    degree++;
                    icurve = Allocate2DArray<double> (degree, 3);
                    string p = "";
                    string pmin = "";
                    string pmax = "";
                    for(int i = 0; i < degree; i++)
                    {
                        getline(model, smodel);
                        if(smodel.find("@") != string::npos)
                        {
                            paramskip++;
                            skip.push_back(i);
                        }
                        tmp << "P" << i << " ";
                        p = tmp.str();
                        tmp.str(std::string());
                        tmp.clear();
                        tmp << "P" << i << "L ";
                        pmin = tmp.str();
                        tmp.str(std::string());
                        tmp.clear();
                        tmp << "P" << i << "U ";
                        pmax = tmp.str();
                        tmp.str(std::string());
                        tmp.clear();
                        icurve[i][0] = getFunctionParam(smodel, p);
                        icurve[i][1] = getFunctionParam(smodel, pmin);
                        icurve[i][2] = getFunctionParam(smodel, pmax);
                        ranges.push_back(icurve[i][1]);
                        ranges.push_back(icurve[i][2]);
                    }
                    poly = new Polynomial(icurve, degree, type, skip);
                    curves.push_back(poly);
                    curveparams.push_back(icurve);
                    skip.clear();
                }
            }
        }
        model.close();

        if(curves.size() != modelConfig[0])
        {
            cout << "Expected " << modelConfig[0] << ", found " << curves.size() << endl;
            return 1;
        }

        if(true)
        {
            cout << "NUMCURVES " << modelConfig[0] << " NUMPARAMS " << modelConfig[1] 
                << " OPTIMIZER " << optimizer << endl;
            for (int i = 0; i < curves.size(); i++)
                cout << curves[i]->toString();
        }

        double* d;

        // Read data in the form:
        // spacial_variable intensity error
        while(!data.eof())
        {
            d = new double[4];
            getline(data, sdata);
            getData(sdata, d);
            space.push_back(d[0]);
            intensity.push_back(d[1]);
            errordown.push_back(d[2]);
            errorup.push_back(d[3]);
            //errordown.push_back(0.0);
            //errorup.push_back(2.0);
        }

        delete [] d;

        data.close();

        const gsl_multimin_fminimizer_type *T = 
            gsl_multimin_fminimizer_nmsimplex2;
        gsl_multimin_fminimizer *s = NULL;
        gsl_vector *ss, *x;
        gsl_multimin_function minex_func;


        int status;
        double size;

        paraminfo = new int[modelConfig[0]];

        x = gsl_vector_alloc((int)(modelConfig[1]/3) - paramskip);
        x_orig = gsl_vector_alloc((int)(modelConfig[1]/3) - paramskip);
        ss = gsl_vector_alloc((int)(modelConfig[1]/3) - paramskip);
        gsl_vector_set_all(ss, 1.0);

        // beginning of curve
        unsigned int boc = 0;

        // number of ignored parameters and curves according to curves
        // sanity check to make sure curves are tracking skips
        int igparam = 0;
        int igcurve = 0;
        for(int i = 0; i < curves.size(); i++)
        {
            // store curve parameter start
            paraminfo[i] = boc;
            if(curves[i]->type() == 0)
            {
                igcurve++;
                igparam += curves[i]->ignored();
            }
            else if(curves[i]->paramNum() == 0)
                igparam += curves[i]->ignored();
            else
            {
                for(int j = 0; j < curves[i]->paramNum(); j++)
                {
                    gsl_vector_set(x, boc + j, curves[i]->getParam(j));
                    gsl_vector_set(x_orig, boc + j, curves[i]->getParam(j));
                    if(j+1 == curves[i]->paramNum())
                        boc += curves[i]->paramNum();
                }
                igparam += curves[i]->ignored();
            }
        }

        if(igcurve != curveskip)
            cout << "Expected " << curveskip << 
                " skipped curves, found " << igcurve << endl;
        if(igparam != paramskip)
            cout << "Expected " << paramskip << 
                " skipped parameters, found " << igparam << endl;

        cout << "Size of optimization vector: " << x->size << endl;
        cout << "Members:" << endl;
        for(int i = 0; i < x->size; i++)
            cout << gsl_vector_get(x, i) << endl;

        minex_func.n = (int)(modelConfig[1]/3 - paramskip);
        minex_func.f = &obj_f;
        minex_func.params = (void *)&ranges;

        /*
           for(int i = 0; i < curves.size(); i++)
           cout << curves[i]->paramNum() << endl;
         */

        s = gsl_multimin_fminimizer_alloc (T, (int)(modelConfig[1]/3) - paramskip);
        gsl_multimin_fminimizer_set(s, &minex_func, x, ss);

        vector<double> TESTPVEC;

        fout.precision(16);
        final.precision(16);

        int resetcount = 0;
        int goodsince = 0;
        int getmeoutofhere = 1;

        do
        {
            vector<double> TEST(space.size(), 0.0);

            iter++;
            status = gsl_multimin_fminimizer_iterate(s);

            if (status) 
                break;

            size = gsl_multimin_fminimizer_size (s);
            status = gsl_multimin_test_size (size, 1e-8);

//            if(evalcur > 0.001 && size <= 1e-5)
//                resetcount++;
//            else //if(goodsince >= 10000)
//            {
//                resetcount = 0;
////                getmeoutofhere = 1;
////                constraint = 0.4;
//            } 
////            else
////                goodsince ++;
//
//            if(resetcount > 1000)
//            {
////                if(getmeoutofhere = 10)
////                {
////                    constraint += 0.1;
////                    cout << "well, that didn't really work so..." << 
////                        "setting constraint to: " << constraint << endl;
////                    getmeoutofhere = 1;
////                }
////                else if(constraint > 1)
////                {
//                    getmeoutofhere++;
//                    cout << "oops, got stuck, current eval = " << evalcur 
//                        << " ...setting size to: " << getmeoutofhere << endl;
////                  constraint = 0.4;
////                }
//                gsl_vector_set_all(ss, getmeoutofhere);
//                x = s->x;
//                s = gsl_multimin_fminimizer_alloc(T, (int)(modelConfig[1]/3) - paramskip);
//                gsl_multimin_fminimizer_set(s, &minex_func, x, ss);
//                resetcount = 0;
//            }
            //else if(resetcount == 0)
            //{
            if (status == GSL_SUCCESS || (evalcur < 1e-10 && size < 1e-6) || iter > 100000)
            {
                cout << "converged to minimum at" << endl;

                if(iter < 10)
                    cout << iter << "   ";
                if(iter >= 10 && iter < 100)
                    cout << iter << "  ";
                if(iter >= 100 && iter < 1000)
                    cout << iter << " ";
                if(iter >= 1000)
                    cout << iter << "";

                cout << " ";
                for(int j = 0; j < s->x->size; j++)
                    cout << gsl_vector_get (s->x, j) << " ";
                cout << " size = " << size;
                cout << " current eval = " << evalcur << endl;

                for(int i = 0; i < curves.size(); i++)
                {
                    if(curves[i]->type() != 0 && curves[i]->paramNum() != 0)
                    {
                        for(int j = 0; j < curves[i]->paramNum(); j++)
                            TESTPVEC.push_back(gsl_vector_get(s->x, paraminfo[i]+j));
                        curves[i]->f_eval(space, TESTPVEC, TEST);
                        TESTPVEC.clear();
                    }
                    else 
                        curves[i]->f_eval(space, TEST);
                }

                for(int i = 0; i < TEST.size()-1; i++)
                {
                    fout << space[i] << " " << TEST[i] << " " << abs(TEST[i] - intensity[i]) << endl;
                    final << space[i] << " " << TEST[i] << " " << abs(TEST[i] - intensity[i]) << endl;
                }

                TEST.clear();
                TEST.resize(space.size());				

                cout << "NUMCURVES " << modelConfig[0] << " NUMPARAMS " << modelConfig[1] 
                    << " OPTIMIZER " << optimizer << endl;
                for (int i = 0; i < curves.size(); i++)
                {
                    for(int j = 0; j < curves[i]->paramNum(); j++)
                        TESTPVEC.push_back(gsl_vector_get(s->x, paraminfo[i]+j));
                    cout << curves[i]->toString(TESTPVEC);
                    TESTPVEC.clear();
                }

                break;
            }
            else if(iter <= 100)
            {
                if(iter < 10)
                    cout << iter << "   ";
                if(iter >= 10 && iter < 100)
                    cout << iter << "  ";
                if(iter == 100)
                    cout << iter << " ";

                cout << " ";
                for(int j = 0; j < s->x->size; j++)
                    cout << gsl_vector_get (s->x, j) << " ";
                cout << " size = " << size;
                cout << " current eval = " << evalcur << endl;

                for(int i = 0; i < curves.size(); i++)
                {
                    if(curves[i]->type() != 0 && curves[i]->paramNum() != 0)
                    {
                        for(int j = 0; j < curves[i]->paramNum(); j++)
                            TESTPVEC.push_back(gsl_vector_get(s->x, paraminfo[i]+j));
                        curves[i]->f_eval(space, TESTPVEC, TEST);
                        TESTPVEC.clear();
                    }
                    else 
                        curves[i]->f_eval(space, TEST);
                }

                for(int i = 0; i < TEST.size()-1; i++)
                    fout << space[i] << " " << TEST[i] << " " << abs(TEST[i] - intensity[i]) << endl;

                TEST.clear();
                TEST.resize(space.size());				
            }
            else if(iter > 100 && iter <= 10000 && iter % 100 == 0)
            {
                if(iter >= 100 && iter < 10000)
                    cout << iter << " ";
                if(iter == 10000)
                    cout << iter;

                cout << " ";
                for(int j = 0; j < s->x->size; j++)
                    cout << gsl_vector_get (s->x, j) << " ";
                cout << " size = " << size;
                cout << " current eval = " << evalcur << endl;

                for(int i = 0; i < curves.size(); i++)
                {
                    if(curves[i]->type() != 0 && curves[i]->paramNum() != 0)
                    {
                        for(int j = 0; j < curves[i]->paramNum(); j++)
                            TESTPVEC.push_back(gsl_vector_get(s->x, paraminfo[i]+j));
                        curves[i]->f_eval(space, TESTPVEC, TEST);
                        TESTPVEC.clear();
                    }
                    else 
                        curves[i]->f_eval(space, TEST);
                }

                for(int i = 0; i < TEST.size()-1; i++)
                    fout << space[i] << " " << TEST[i] << " " << abs(TEST[i] - intensity[i]) << endl;

                TEST.clear();
                TEST.resize(space.size());				
            }
            else if(iter > 10000 && iter <= 100000 && iter % 1000 == 0)
            {
                cout << iter << " ";
                for(int j = 0; j < s->x->size; j++)
                    cout << gsl_vector_get (s->x, j) << " ";
                cout << " size = " << size;
                cout << " current eval = " << evalcur << endl;

                for(int i = 0; i < curves.size(); i++)
                {
                    if(curves[i]->type() != 0 && curves[i]->paramNum() != 0)
                    {
                        for(int j = 0; j < curves[i]->paramNum(); j++)
                            TESTPVEC.push_back(gsl_vector_get(s->x, paraminfo[i]+j));
                        curves[i]->f_eval(space, TESTPVEC, TEST);
                        TESTPVEC.clear();
                    }
                    else 
                        curves[i]->f_eval(space, TEST);
                }

                for(int i = 0; i < TEST.size()-1; i++)
                    fout << space[i] << " " << TEST[i] << " " << abs(TEST[i] - intensity[i]) << endl;

                TEST.clear();
                TEST.resize(space.size());				
            }
            else if(iter > 100000 && iter % 10000 == 0)
            {
                if(iter >= 100 && iter < 1000)
                    cout << iter << " ";
                if(iter >= 1000)
                    cout << iter << "";

                cout << " ";
                for(int j = 0; j < s->x->size; j++)
                    cout << gsl_vector_get (s->x, j) << " ";
                cout << " size = " << size;
                cout << " current eval = " << evalcur << endl;

                for(int i = 0; i < curves.size(); i++)
                {
                    if(curves[i]->type() != 0 && curves[i]->paramNum() != 0)
                    {
                        for(int j = 0; j < curves[i]->paramNum(); j++)
                            TESTPVEC.push_back(gsl_vector_get(s->x, paraminfo[i]+j));
                        curves[i]->f_eval(space, TESTPVEC, TEST);
                        TESTPVEC.clear();
                    }
                    else 
                        curves[i]->f_eval(space, TEST);
                }

                for(int i = 0; i < TEST.size()-1; i++)
                    fout << space[i] << " " << TEST[i] << " " << abs(TEST[i] - intensity[i]) << endl;

                TEST.clear();
                TEST.resize(space.size());
            }
            //}

        }
        while (status == GSL_CONTINUE);

        gsl_vector_free(x);
        gsl_vector_free(ss);
        gsl_multimin_fminimizer_free (s);
        for(int i = 0; i < curveparams.size(); i++)
            Free2DArray(curveparams[i]);

    } else cout << "Unable to open data.txt and model.txt" << endl;

    return 0;
}

double obj_f(const gsl_vector *x, void *params)
{
    // Parameters, Function evaluation
    vector<double> pvec;
    vector<double> F(space.size(), 0.0);
    //double pholder = 0.0;

    for(int i = 0; i < curves.size(); i++)
    {
        if(curves[i]->type() != 0 && curves[i]->paramNum() != 0)
        {
            for(int j = 0; j < curves[i]->paramNum(); j++)
            {
                //pholder = gsl_vector_get(x, paraminfo[i]+j);
                pvec.push_back(gsl_vector_get(x, paraminfo[i]+j));
            }
            // evaluate with just space, space and parameters, or 
            // space, parameters and ranges
            curves[i]->f_eval(space, pvec, F);

            // clear temp evaluation vectors
            pvec.clear();
        } 
        else 
            curves[i]->f_eval(space, F);
    }

    double Chi = 0.0, holder1, C = 0.0, holder2;
    double eval = 0.0;
    
    double R2;

    for(int i = 0; i < F.size()-1; i++)
    {
        R2 = pow((F[i] - intensity[i])/abs((errorup[i]-errordown[i])/2),2);
        if(abs(R2) > 1e-14)
        {
            holder1 = log((1.0-exp(-R2/2))/R2);
        }
        else
            holder1 = 0.0;
        Chi -= holder1;

        // Chi-squared only
//        holder1 = R2;
//        Chi += holder1;
    }
    for(int i = 0; i < x->size; i++)
    {
        holder2 = pow((gsl_vector_get(x,i) - gsl_vector_get(x_orig,i))
                /(gsl_vector_get(x_orig,i)*constraint),2);
        C += holder2;
    }
    eval = 0.5 * (Chi + C);
    evalprev = evalcur;
    evalcur = eval;
    F.clear();
    return eval;
}

double obj_df(const gsl_vector *x, void *params)
{
    double eval = 0.0;
    return eval;
}

bool getModelConfig(string line)
{
    modelConfig = new int[2];

    int p1, p2;
    stringstream tmp;
    double holder;

    p1 = line.find("NUMCURVES ") + 10;
    p2 = line.find(" ", p1);
    tmp << line.substr(p1, p2-p1);

    if(tmp.str().compare("NULL") != 0)
        tmp >> modelConfig[0];
    else
        return false;

    tmp.str(std::string());
    tmp.clear();

    p1 = line.find("NUMPARAMS ") + 10;
    p2 = line.find(" ", p1);
    tmp << line.substr(p1, p2-p1);

    if(tmp.str().compare("NULL") != 0)
        tmp >> modelConfig[1];
    else
        return false;

    tmp.str(std::string());
    tmp.clear();

    p1 = line.find("OPTIMIZER ") + 10;
    p2 = line.find(" ", p1);
    tmp << line.substr(p1, p2-p1);

    if(tmp.str().compare("NULL") != 0)
        optimizer = tmp.str();
    else
        return false;

    return true;
}

double getFunctionParam(string line, string val)
{
    int p1, p2;
    string holder;
    stringstream tmp;
    double d;
    if(line.find("@") != string::npos)
        holder = line.substr(line.find("@"));
    else 
        holder = line;
    p1 = holder.find(val) + val.size();
    p2 = holder.find(" ", p1);
    tmp << holder.substr(p1, p2-p1);
    if(tmp.str().compare("NULL") != 0)
        tmp >> d;
    return d;
}

double getData(string line, double *d)
{
    int p1, p2, p3;
    stringstream tmp;
    p1 = line.find(" ");
    p2 = line.find(" ", p1+1);
    p3 = line.find(" ", p2+1);
    tmp << line.substr(0, p1);
    tmp >> d[0];
    tmp.str(std::string());
    tmp.clear();
    tmp << line.substr(p1+1, p2-p1);
    tmp >> d[1];
    tmp.str(std::string());
    tmp.clear();
    tmp << line.substr(p2+1, p3-p2);
    tmp >> d[2];
    tmp.str(std::string());
    tmp.clear();
    tmp << line.substr(p3+1);
    tmp >> d[3];
    return *d;
}

template < typename T > T **Allocate2DArray( int nRows, int nCols)
{
    //(step 1) allocate memory for array of elements of column
    T **ppi = new T*[nRows];

    //(step 2) allocate memory for array of elements of each row
    T *curPtr = new T [nRows * nCols];

    // Now point the pointers in the right place
    for( int i = 0; i < nRows; ++i)
    {
        *(ppi + i) = curPtr;
        curPtr += nCols;
    }
    return ppi;
}

template < typename T > void Free2DArray(T** Array)
{
    delete [] *Array;
    delete [] Array;
}
