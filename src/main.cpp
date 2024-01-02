#include "mapWindow.h"
#include <assert.h>

using namespace FLTK_MAP;

int main(int argc, char *argv[]) {
    assert(argc == 2);
    MapWindow window(1200, 800, "fltk map", argv[1]);
    window.end();
    window.show();
    return Fl::run();
}