#include <ncnn/net.h>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(){

    ncnn::Net net;
    net.load_param("./ml/models/model.ncnn.param");
    net.load_model("./ml/models/model.ncnn.bin");

    printf("Success\n");
}