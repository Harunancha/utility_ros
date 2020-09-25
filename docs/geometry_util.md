# geometry_util (geo_u::)  

幾何計算ユーティリティライブラリ

## description

- class Pose2d : public cv::Point2d \
2次元位置姿勢

- class Transform2d \
2次元座標変換

    * void calc_mat()   \
    座標変換行列計算

    * cv::Point2d tf_point_to_o(cv::Point2d p)  \
    ポーズ座標系の点を原点座標系に変換
    
    * cv::Point2d tf_point_to_p(cv::Point2d p) \
    原点座標系の点をポーズ座標系に変換

    * Pose2d tf_pose2d_to_o(Pose2d p) \
    ポーズ座標系の位置・姿勢を原点座標系に変換

    * Pose2d tf_pose2d_to_p(Pose2d p) \
    原点座標系の位置・姿勢をポーズ座標系に変換

- static double Distance2d(cv::Point2d x1, cv::Point2d x2) \
    2次元距離計算

- static double square_Distance3d(cv::Point3d x1, cv::Point3d x2) \
    3次元距離の二乗（二乗誤差計算に利用）

- static double Distance3d(cv::Point3d x1, cv::Point3d x2) \
    3次元距離計算

- static int RectArea(cv::Rect &rect) \
    長方形面積計算

- 