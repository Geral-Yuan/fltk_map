#include "mapWindow.h"
#include <assert.h>

using namespace FLTK_MAP;

int main(int argc, char *argv[]) {
    assert(argc == 3);
    MapWindow window(1200, 800, "fltk map", argv[1], argv[2]);
    window.end();
    window.show();
    return Fl::run();
}