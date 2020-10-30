# probability_util (pd_u::)  

確率分布ユーティリティライブラリ

## description

- class NormalDistribution1d \
    1次元正規分布

    * double random_sampling() \
    ランダムサンプリング

- class Mu2d : public cv::Point2d \
    2次元正規分布平均

- class Sigma2d \
    2次元正規分布共分散

- class NormalDistribution2d \
    2次元正規分布

    * double random_sampling() \
    ランダムサンプリング

‐ static double KL_divergence(NormalDistribution1d nd1d1, NormalDistribution1d nd1d2) \
    1次元KL情報量計算

- static size_t random_sampling_uniform1i(size_t min, size_t max) \
    一様分布から整数ランダムサンプリング

- double random_sampling_uniform1d(double min, double max) \
    一様分布から浮動小数点ランダムサンプリング

- cv::Point2d random_sampling_uniform2d(cv::Point2d min, cv::Point2d max) \
    一様分布から2次元点ランダムサンプリング

- cv::Point2d add_noise_nd2d(Mu2d mu, Sigma2d sigma) \
    2次元正規分布ノイズ付加

- double add_noise_nd1d(double mu, double sigma) \
    1次元正規分布ノイズ付加