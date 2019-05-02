#include "view.h"
#include <geologic-core/utils/customToolbox.h>
#include <iostream>

using namespace std;

View::~View() {
    //cout << "View::~View()" << endl;
}

void View::setWindowSize(int width, int height) {
    customToolbox::start("View::setWindowSize");
    screen_width = width;
    screen_height = height;
    resized = true;
    resize();
    customToolbox::end("View::setWindowSize");
}
