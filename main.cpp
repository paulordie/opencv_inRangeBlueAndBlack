#include <iostream>
#include <dirent.h>
#include <stdio.h>


#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

void houghLine(Mat img){
    Mat dst, cdst, cdstP;
    if (img.empty()){
        std::cout << "ERROR" << std::endl;
    }

    Canny(img, dst, 50, 300, 3);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();
    vector<Vec2f> lines;
    HoughLines(dst, lines, 1, CV_PI/180, 300, 0, 0);

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }
    imshow("Source", img);
    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
}

bool checkIsBlue(std::string img_file, cv::Rect _rect){

    cv::Mat _img = cv::imread(img_file);

    imshow("image", _img);

    Mat img = _img(_rect);
    houghLine(img);
    cv::Mat hsv;

    cv::Mat bw;

    //cv::inRange(img, cv::Scalar(110,10,50), cv::Scalar(190,255,255), bw);
    cv::inRange(img, cv::Scalar(102,49,16), cv::Scalar(255,255,255), bw);

    imshow("blue", bw);
    waitKey();

    bool isBlue = false;
    int qtd_blue = 0;

    int perc = ((bw.rows*bw.cols) * 0.3);
    //    int perc = ((bw.rows*bw.cols) * 0.3);

    std::cout << "Blue Perc: " << perc << std::endl;

    for (int i=0 ; i< (bw.rows) ; i++)
    {
        for (int j=0; j < (bw.cols) ; j++){
            if (bw.at<uchar>(i,j) == 255 ){
                qtd_blue++;
                if (qtd_blue > perc)
                    break;
            }
        }

        if (qtd_blue > perc){
            isBlue = true;
            break;
        }
    }

    std::cout << "Qty Blue: " << qtd_blue << " logical: " << isBlue <<  std::endl;

    return isBlue;
}

bool checkIsBlack(std::string img_file, cv::Rect _rect){

    cv::Mat _img = cv::imread(img_file);
    imshow("image", _img);

    Mat img = _img(_rect);
    houghLine(img);
    cv::Mat hsv;

    cv::Mat bw;

    //cv::inRange(img, cv::Scalar(0,0,0), cv::Scalar(10,30,100), bw);
    cv::inRange(img, cv::Scalar(0,0,0), cv::Scalar(180,255,255), bw);
//    cv::inRange(img, cv::Scalar(60,63,67), cv::Scalar(180,255,255), bw);

    imshow("back", bw);
    waitKey();

    bool isBlack = false;
    int qtd_blacks = 0;

       int perc = ((bw.rows*bw.cols) * 0.6);
    //    int perc = ((bw.rows*bw.cols) * 0.3);

    std::cout << "Black Perc: " << perc << std::endl;

    for (int i=0 ; i< (bw.rows) ; i++)
    {
        for (int j=0; j < (bw.cols) ; j++){
            if (bw.at<uchar>(i,j) == 255 ){
                qtd_blacks++;
                if (qtd_blacks > perc)
                    break;
            }
        }

        if (qtd_blacks > perc){
            isBlack = true;
            break;
        }
    }

    std::cout << "Qty Blacks: " << qtd_blacks << " logical: " << isBlack << std::endl;

    return isBlack;
}

int main() {

    cv::Rect roiDut(45,210,220,100);
    cv::Rect roiBench(365,195,220,100);


    string path = "/home/pcorrea/Documents/projetos/TPV-OrionPax/inRangeBlueAndBlack/imgs/";

    DIR *d;
    struct dirent *dir;
    d = opendir("/home/pcorrea/Documents/projetos/TPV-OrionPax/inRangeBlueAndBlack/imgs/"); //diretório atual;
    if (d != NULL)
    {
        while ((dir = readdir(d)) != NULL)  //lsita todos os arquivos do diretorio
        {
            printf("Name of image is %s\n", dir->d_name);

            std::string nameImage = dir->d_name;
            std::string name = {'.'};
            std::string name1 = {".."};

            if (nameImage == name || nameImage == name1){
                std::cout << "Does not valid image" << std::endl;
            } else {

                string imgName = path + nameImage;
                std::cout << "ROIDUT" << std::endl;
                checkIsBlue(imgName,roiDut);
                checkIsBlack(imgName,roiDut);
                std::cout << "ROIBENCH" << std::endl;

                checkIsBlue(imgName,roiBench);
                checkIsBlack(imgName,roiBench);


                std::cout << "\n" << std::endl;

            }

            FILE * pFile;
            pFile = fopen (dir->d_name,"r"); //abre o arquivo
            if (pFile!=NULL)
            {
                // insira aqui o codigo que manipula o arquivo
                fclose (pFile);
            }
        }
        closedir(d);
    }

    return 0;
}