#include <cmath>
#include "image/tgaimage.h"

//https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // 通过两次换元推导出来的判断逻辑
    // dx = abs(x1-x0)
    // dy = adb(y1-y0)
    // k = dy/dx
    // 
    // d(0) = 0, d(i+1)=d(i)+k
    // x(i+1) = x(i)+1;
    // d(i) < 0.5  => y(i+1)=y(i)
    // d(i) >= 0.5 => y(i+1)=y(i)+1  d(i+1)=d(i)-1
    // 
    // 令e(i)=d(i)-0.5 换元去掉小数0.5
    // e(0)=-0.5, e(i+1)=e(i)+k
    // e(i) < 0  => y(i+1)=y(i)
    // e(i) >= 0 => y(i+1)=y(i)+1  e(i+1)=e(i)-1
    //
    // 令f(i)=2dx*e(i)
    // f(0)=-dx, f(i+1)=f(i)+2dy
    // f(i) < 0  => y(i+1)=y(i)
    // f(i) >= 0 => y(i+1)=y(i)+1  f(i+1)=f(i)-2dx
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, TGAColor(255, 1));
        }
        else {
            image.set(x, y, TGAColor(255, 1));
        }
        error2 += derror2;

        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    for (int i = 0; i < 1000000; i++) {
        //line(13, 20, 80, 40, image, white);
        line(20, 13, 40, 80, image, red);
        line(80, 40, 13, 20, image, red);
    }
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}