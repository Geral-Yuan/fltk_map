#include "mapArea.h"

namespace FLTK_MAP {

int crossProduct(const Vec &v1, const Vec &v2) {
    return v1.x * v2.y - v2.x * v1.y;
}

std::string toScientificString(unsigned long long value) {
    if (value < 10) return std::to_string(value) + ".00000";
    unsigned long long x = 1;
    int digitCnt = 0;
    std::string s;
    while (value >= x) {
        ++digitCnt;
        x *= 10;
    }
    x /= 10;
    s = std::to_string(value / x) + ".";
    if (digitCnt <= 6) {
        s += std::to_string(value).substr(1, digitCnt) + std::string(6 - digitCnt, '0');
    } else {
        s += std::to_string((value / (x / (int)1e6) + 5) / 10).substr(1, digitCnt);
    }
    s += "*10^" + std::to_string(digitCnt - 1);
    return s;
}

void MapArea::highlight(int leftX, int rightX, const std::vector<Point> &ps) const {
    int X = x(), Y = y(), W = w(), H = h();
    int sz = ps.size();
    int dx = 6;
    fl_color(FL_YELLOW);
    fl_line_style(FL_SOLID, 2);
    for (double lineX = leftX + 0.05; lineX <= rightX; lineX += dx) {
        std::vector<double> intersectY;
        double lx = (lineX - X) * imageWidth / W;
        for (int i = 0; i < sz; ++i) {
            if ((ps[i].x < lx) != (ps[(i + 1) % sz].x < lx))
                intersectY.push_back((double)(ps[(i + 1) % sz].y - ps[i].y) / (ps[(i + 1) % sz].x - ps[i].x) * (lx - ps[i].x) + ps[i].y);
        }
        int intersectCnt = intersectY.size();
        assert(intersectCnt % 2 == 0);
        std::sort(intersectY.begin(), intersectY.end());
        for (int i = 0; i < intersectCnt; i += 2)
            fl_line(lineX, Y + intersectY[i] * H / imageHeight, lineX, Y + intersectY[i + 1] * H / imageHeight);
    }
    fl_line_style(0);
}

void MapArea::draw() {
    std::vector<Point> ps(points);
    if (!done && mouseInside) ps.push_back(currentPoint);
    int sz = ps.size();
    if (sz >= 2) {
        int X = x(), Y = y(), W = w(), H = h();
        int leftmost = X + W, rightmost = X;
        fl_color(FL_CYAN);
        fl_line_style(FL_SOLID, 1 + H / 300);
        fl_begin_loop();
        for (int i = 0; i < sz; ++i) {
            int px = X + ps[i].x * W / imageWidth;
            int py = Y + ps[i].y * H / imageHeight;
            fl_vertex(px, py);
            leftmost = std::min(leftmost, px);
            rightmost = std::max(rightmost, px);
        }
        fl_end_loop();
        fl_line_style(0);
        if (sz >= 3) highlight(leftmost, rightmost, ps);
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
        if (done || !mouseInside)
            area += crossProduct(points[sz - 1], points[0]);
        else
            area += crossProduct(points[sz - 1], currentPoint) + crossProduct(currentPoint, points[0]);
        pixelArea = abs(area) / 2;
        realArea = pixelArea * realLength * realLength / pixelLength / pixelLength;
    }
    areaString = "pixel area: " + toScientificString(pixelArea) + " px²\nreal area: " + toScientificString(realArea) + " m²";
}

}  // namespace FLTK_MAP