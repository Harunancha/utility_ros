/*
TODO: 
*/

#ifndef PROBABILITY_H_
#define PROBABILITY_H_

namespace pd_u
{
    class NormalDistribution1D
    {
    private:
        double mu;
        double sigma;

    public:
        NormalDistribution1D(/* args */){};
        ~NormalDistribution1D(){};
    };
    
    struct Mu2D
    {
        double x;
        double y;
    };

    struct Sigma2D
    {
        double xx;
        double yy;
        double xy;
    };

    class NormalDistribution2D
    {
    private:
        Mu2D mu;
        Sigma2D sigma;

    public:
        NormalDistribution2D(/* args */){};
        ~NormalDistribution2D(){};
    };
}

#endif