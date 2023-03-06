// helloworld.cpp

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv)
{
    int rows = 256;
    int cols = 256;
    int chess_rows = 300;
    int chess_cols = 300;
    
    Mat vertical_gradient(rows,cols,CV_8UC1);
    Mat horizontal_gradient(rows,cols,CV_8UC1);
    Mat big_chessboard(chess_rows,chess_cols,CV_8UC1);
    Mat small_chessboard(chess_rows,chess_cols,CV_8UC1);

    // Draw gradients
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            vertical_gradient.at<unsigned char>(i,j) = i;
            horizontal_gradient.at<unsigned char>(i,j) = j;
        }
    }

    int a = 20;
    int b = 50;

    // Draw chessboards here
    // Small board
    for (int i = 0; i <= chess_rows / a; i++){
        for (int j = 0; j <= chess_cols / a; j++){
            rectangle(small_chessboard, Point(i*a*2, 2*j*a), Point(i*a*2+a, 2*j*a+a), Scalar(255), -1, 4);
            
        }
    }

    for (int i = 0; i <= chess_rows / a; i++){
        for (int j = 0; j <= chess_cols / a; j++){
            rectangle(small_chessboard, Point(i*a*2+a, 2*j*a+a), Point(i*a*2+2*a, 2*j*a+2*a), Scalar(255), -1, 4);
            
        }
    }

    // Big board
    for (int i = 0; i <= chess_rows / b; i++){
        for (int j = 0; j <= chess_cols / b; j++){
            rectangle(big_chessboard, Point(i*b*2, 2*j*b), Point(i*b*2+b, 2*j*b+b), Scalar(255), -1,4);
        }
    }

    for (int i = 0; i <= chess_rows / b; i++){
        for (int j = 0; j <= chess_cols / b; j++){
            rectangle(big_chessboard, Point(i*b*2+b, 2*j*b+b), Point(i*b*2+2*b, 2*j*b+2*b), Scalar(255), -1,4);
        }
    }

    namedWindow("Horizontal"); // Create a named window
    namedWindow("Vertical");
    namedWindow("Big chess");
    namedWindow("Small chess");

    imshow("Horizontal", horizontal_gradient);
    imshow("Vertical", vertical_gradient);
    imshow("Big chess", big_chessboard);
    imshow("Small chess", small_chessboard);

    char waitKeyOutput = waitKey(0); // Wait for key-push to exit
    std::cout << "[INFO] Key pressed: " << waitKeyOutput << "\n";

    return 0;
}