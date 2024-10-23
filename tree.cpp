#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>

int Stamp(){
    auto now = std::chrono::system_clock::now();
    std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(now);
    return (int) unix_timestamp;
}

double Tree(double S, double K, double r, double q, double v, double t, int steps, int optype)
{
    double dt = t / (double) steps;

    int row = 4*steps + 2, column = steps + 1;
    
    double ** tree = new double*[row];
    for(int i = 0; i < row; ++i){
        tree[i] = new double[column];
    }

    int center = row / 2 - 1;

    double up = exp(v*sqrt(2.0*dt));
    double dn = 1.0/up;
    double m = 1.0;

    double A = exp((r - q)*dt/2.0);
    double B = exp(-v*sqrt(dt/2.0));
    double C = exp(v*sqrt(dt/2.0));

    double pu = pow((A - B)/(C - B), 2);
    double pd = pow((C - A)/(C - B), 2);
    double pm = 1.0 - (pu + pd);

    for(int j = 0; j < column; ++j){
        tree[center][j] = S;
        for(int i = 1; i < column - j; ++i){
            tree[center - 2*i][i + j] = tree[center - 2*(i-1)][i - 1 + j]*up;
            tree[center][i + j] = tree[center][i - 1 + j]*m;
            tree[center + 2*i][i + j] = tree[center + 2*(i-1)][i - 1 + j]*dn;
        }
    }

    for(int i = 0; i < row; ++i){
        if(i % 2 != 0){
            if(optype == 0){
                tree[i][column - 1] = fmax(tree[i-1][column-1] - K, 0.0);
            } else {
                tree[i][column - 1] = fmax(K - tree[i-1][column-1], 0.0);
            }
        }
    }

    int inc = 2;
    for(int j = 2; j < column + 1; ++j){
        for(int i = inc; i < row - inc; ++i){
            if(i % 2 != 0){
                A = tree[i-2][column-j+1];
                B = tree[i][column-j+1];
                C = tree[i+2][column-j+1];
                double cash = pu*A + pm*B + pd*C;
                cash = exp(-r*dt)*cash;
                if(optype == 0){
                    tree[i][column-j] = fmax(tree[i-1][column-j] - K, cash);
                } else {
                    tree[i][column-j] = fmax(K - tree[i-1][column-j], cash);
                }
            }
        }
        inc += 2;
    }

    return tree[center + 1][0];
}

int main()
{
    double S = 100;
    double K = 90;
    double r = 0.05;
    double q = 0.01;
    double v = 0.25;
    double t = 30.0/365.0;
    int steps = 10000;
    int optype = 1;

    int t0 = Stamp();
    double optionPrice = Tree(S, K, r, q, v, t, steps, optype);
    int t1 = Stamp();

    printf("C++ took %d seconds to run\n", t1 - t0);
    printf("C++ option price is %f\n", optionPrice);
    printf("C++ took %d steps\n", steps);
    return 0;
}