/*
TODO: 
*/
#ifndef PROBABILITY_DISTRIBUTION_H_
#define PROBABILITY_DISTRIBUTION_H_

// #include <vector>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace pd_u
{
    class NormalDistribution1d
    {
    public:
        double mu;
        double sigma;

    public:
        NormalDistribution1d()
        {
            mu = 0;
            sigma = 0;
        };
        ~NormalDistribution1d(){};
        NormalDistribution1d(double m, double s): mu(m), sigma(s){};
        NormalDistribution1d(std::vector<double> &p)
        {
            double ave = 0.0, var = 0.0;
            for(const auto &x : p){
                ave += x;
                var += x * x;
            }
            ave /= p.size();
            var = var / p.size() - ave * ave;
            mu = ave;
            sigma = var;
        };
        double random_sampling()
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> score(0.0, 1.0);
            double std_dev = std::sqrt(sigma);
            double p = double(mu + std_dev * std::sqrt(-2. * log(score(mt))) * sin(2 * M_PI * score(mt)));
            return p;
        };
    };
    
    class Mu2d : public cv::Point2d 
    {
    public:
        Mu2d(): cv::Point2d(0.,0.){};
        Mu2d(double x, double y): cv::Point2d(x, y){};
        ~Mu2d(){};
    };

    class Sigma2d
    {
    public:
        double xx;
        double yy;
        double xy;

    public:
        Sigma2d(): xx(0.), yy(0.), xy(0.){};
        Sigma2d(double sxx, double syy, double sxy): xx(sxx), yy(syy), xy(sxy){};
        ~Sigma2d(){};
    };

    class NormalDistribution2d
    {
    public:
        Mu2d mu;
        Sigma2d sigma;

    public:
        NormalDistribution2d(){};
        ~NormalDistribution2d(){};
        NormalDistribution2d(Mu2d m, Sigma2d s)
        {
            mu = m;
            sigma = s;
        };
        NormalDistribution2d(std::vector<cv::Point2d> &ps)
        {
            double mu_x, mu_y;
            double sigma_xx, sigma_yy, sigma_xy;
            for (const auto &p : ps)
            {
                mu_x += p.x;
                mu_y += p.y;
                sigma_xx += p.x * p.x;
                sigma_yy += p.y * p.y;
                sigma_xy += p.x * p.y;
            }
            mu_x /= ps.size();
            mu_y /= ps.size();
            sigma_xx = sigma_xx / ps.size() - mu_x * mu_x;
            sigma_yy = sigma_yy / ps.size() - mu_y * mu_y;
            sigma_xy = sigma_xy / ps.size() - mu_x * mu_y;
            mu = Mu2d(mu_x, mu_y);
            sigma = Sigma2d(sigma_xx, sigma_yy, sigma_xy);
        };
        cv::Point2d random_sampling()
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> score(0.0, 1.0);

            double std_dev_x = std::sqrt(sigma.xx);
            double std_dev_y = std::sqrt(sigma.yy);
            double correlation = sigma.xy / (std_dev_x * std_dev_y);
            // ROS_INFO("random: std_x[%lf], std_y[%lf], cor[%lf]", std_dev_x, std_dev_y, correlation);

            cv::Point2d random_point;
            if (sigma.xx != 0.)
                random_point.x = double(mu.x + std_dev_x * std::sqrt(-2. * log(score(mt))) * sin(2 * M_PI * score(mt)));
            else
                random_point.x = mu.x;

            double one_average = mu.y + correlation * std_dev_y / std_dev_x * (random_point.x - mu.x);
            double one_std_dev = std::sqrt(1 - correlation * correlation) * std_dev_y;
            if (sigma.yy != 0.)
                random_point.y = double(one_average + one_std_dev * std::sqrt(-2. * log(score(mt))) * sin(2 * M_PI * score(mt)));
            else
                random_point.y = mu.y;
            //ROS_INFO("random (%lf, %lf)", random_point.x, random_point.y);

            return random_point;
        } 
    };

    static double KL_divergence(pd_u::NormalDistribution1d nd1d1, pd_u::NormalDistribution1d nd1d2)
    {
        return log(nd1d2.sigma / nd1d1.sigma) + (nd1d1.sigma * nd1d1.sigma + (nd1d1.mu - nd1d2.mu) * (nd1d1.mu - nd1d2.mu) - nd1d2.sigma * nd1d2.sigma) / (2 * nd1d2.sigma * nd1d2.sigma);
    };
}

#endif