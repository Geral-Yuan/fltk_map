#include "mapArea.h"

namespace FLTK_MAP {

int crossProduct(const Vec &v1, const Vec &v2) {
    return v1.x * v2.y - v2.x * v1.y;
}

void MapArea::draw() {
    int sz = points.size();
    if (sz) {
        int X = x(), Y = y(), W = w(), H = h();
        fl_color(FL_CYAN);
        fl_line_style(FL_SOLID, 1 + H / 300);
        fl_begin_loop();
        for (int i = 0; i < sz; ++i)
            fl_vertex(X + points[i].x * W / imageWidth, Y + points[i].y * H / imageHeight);
        if (!done && mouseInside)
            fl_vertex(X + currentPoint.x * W / imageWidth, Y + currentPoint.y * H / imageHeight);
        fl_end_loop();
    }
    computeArea();
    areaLabel->label(areaString.c_str());
}

int MapArea::handle(int event) {
    int event_x = Fl::event_x();
    int event_y = Fl::event_y();
    if (done) return 0;
    int X = x(), Y = y(), W = w(), H = h();
    switch (event) {
        case FL_ENTER: {
            mouseInside = true;
            return 1;
        }
        case FL_LEAVE: {
            mouseInside = false;
            window()->redraw();
            return 1;
        }
        case FL_MOVE: {
            if (mouseInside) {
                currentPoint = Point((event_x - X) * imageWidth / W, (event_y - Y) * imageHeight / H);
                window()->redraw();
                return 1;
            }
            break;
        }
        case FL_PUSH: {
            if (Fl::event_button() == FL_LEFT_MOUSE) {
                points.push_back(Point((event_x - X) * imageWidth / W, (event_y - Y) * imageHeight / H));
                return 1;
            }
            break;
        }
    }
    return 0;
}

void MapArea::computeArea() {
    int sz = points.size();
    if (sz < 2)
        pixelArea = realArea = 0;
    else {
        int area = 0;
        for (int i = 0; i < sz - 1; ++i)
            area += crossProduct(points[i], points[i + 1]);
        if (done)
            area += crossProduct(points[sz - 1], points[0]);
        else
            area += crossProduct(points[sz - 1], currentPoint) + crossProduct(currentPoint, points[0]);
        pixelArea = abs(area) / 2;
        realArea = pixelArea * realLength * realLength / pixelLength / pixelLength;
    }
    areaString = "pixel area: " + std::to_string(pixelArea) + " px²\nreal area: " + std::to_string(realArea)+" m²";
}

}  // namespace FLTK_MAP