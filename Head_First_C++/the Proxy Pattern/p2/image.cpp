#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "stdint.h"

using namespace std;

struct Image
{
    virtual void draw() = 0;
};

struct Bitmap: Image {
    Bitmap(const string &filename) {
        cout << "Loading image from " << filename << endl;
    }

    void draw() override { cout << "Drawing image " << endl;}
};

struct LazyBitmap: Image {
    LazyBitmap(const string filename): filename(filename) {}
    ~LazyBitmap() { delete bmp;}

    void draw() override {
        if (!bmp) bmp = new Bitmap(filename);
        bmp->draw();
    }

private:
    Bitmap *bmp{nullptr};
    string filename;
};

void draw_image(Image& img) {
    cout << "About to draw the image" << endl;
    img.draw();
    cout << "Done drawing the image" << endl;
}

int main() {
    LazyBitmap img{"pokemon.png"};
    draw_image(img);
    return 0;
}
