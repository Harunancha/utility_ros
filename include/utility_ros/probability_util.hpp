/*
TODO: kalman filter for nd1d and nd2d
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
        double random_sampling();
        void kalman_filtering(double p){}; //TODO: 
    };

    double NormalDistribution1d::random_sampling()
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> score(0.0, 1.0);
        double std_dev = std::sqrt(sigma);
        double p = double(mu + std_dev * std::sqrt(-2. * log(score(mt))) * sin(2 * M_PI * score(mt)));
        return p;
    }
    
    typedef cv::Point2d Mu2d;
    class Sigma2d
    {
    public:
        double xx;
        double yy;
        double xy;

    public:
        Sigma2d(): xx(0.), yy(0.), xy(0.){};
        Sigma2d(double sxx, double syy, double sxy): xx(sxx), yy(syy), xy(sxy){};
        Sigma2d(const Sigma2d& s): xx(s.xx), yy(s.yy), xy(s.xy){};
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
        NormalDistribution2d(Mu2d m, Sigma2d s): mu(m), sigma(s){};
        NormalDistribution2d(std::vector<cv::Point2d> &ps)
        {
            double mu_x, mu_y, sigma_xx, sigma_yy, sigma_xy;
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
        cv::Point2d random_sampling();
        void kalman_filtering(cv::Point2d p); //TODO:
    };

    cv::Point2d NormalDistribution2d::random_sampling()
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

    static double KL_divergence(NormalDistribution1d nd1d1, NormalDistribution1d nd1d2)
    {
        return log(nd1d2.sigma / nd1d1.sigma) + (nd1d1.sigma * nd1d1.sigma + (nd1d1.mu - nd1d2.mu) * (nd1d1.mu - nd1d2.mu) - nd1d2.sigma * nd1d2.sigma) / (2 * nd1d2.sigma * nd1d2.sigma);
    };

    static size_t random_sampling_uniform1i(size_t min, size_t max)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> score(min, max);
        return score(mt);
    };

    double random_sampling_uniform1d(double min, double max)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> score(min, max);
        return score(mt);
    };

    cv::Point2d random_sampling_uniform2d(cv::Point2d min, cv::Point2d max)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> score_x(min.x, max.x);
        std::uniform_real_distribution<double> score_y(min.y, max.y);
        // std::cout << "random 2d: " << cv::Point2d(score_x(mt), score_y(mt)) << std::endl;
        return cv::Point2d(score_x(mt), score_y(mt));
    }

    cv::Point2d add_noise_nd2d(Mu2d mu, Sigma2d sigma)
    {
        NormalDistribution2d nd2d(mu, sigma);
        cv::Point2d p = nd2d.random_sampling();
        // ROS_INFO("random: mu[%2.2lf, %2.2lf], sigma[%2.2lf, %2.2lf, %2.2lf]: sample: (%2.2lf, %2.2lf)", mu.x, mu.y, sigma.xx, sigma.yy, sigma.xy, p.x, p.y);
        return p;
    }

    double add_noise_nd1d(double mu, double sigma)
    {
        NormalDistribution1d nd1d(mu, sigma);
        double p = nd1d.random_sampling();
        // ROS_INFO("random: mu: %2.2lf, sigma: %2.2lf sample: %2.2lf", mu, sigma, p);
        return p;
    }

} // namespace pd_u

#endif