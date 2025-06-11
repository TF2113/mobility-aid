#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>  //Used for system()

int main() {

    for (int i = 0; i < 5; i ++){
        const std::string imagePath = "builds/captures/image" + std::to_string(i) + ".jpg";

        // Use libcamera-still command to capture image
        std::string cmd = "libcamera-jpeg -n -t 1 --width 640 --height 480 -o " + imagePath;
        int returnValue = system(cmd.c_str());

        if (returnValue != 0) {
            std::cerr << "Error: libcamera-still failed." << std::endl;
            return 1;
        }

        std::cout << "Image successfully captured." << std::endl;
    }

    return 0;
}
