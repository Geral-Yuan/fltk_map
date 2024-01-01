#include "mapWindow.hpp"

using namespace FLTK_MAP;

int main() {
    MapWindow window(1200, 800, "fltk map");
    window.end();
    window.show();
    return Fl::run();
}