#ifndef MAPAREA_H
#define MAPAREA_H

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <FL/fl_draw.H>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

namespace FLTK_MAP {

struct Vec {
    int x, y;
    Vec(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    Vec(const Vec &rhs) : x(rhs.x), y(rhs.y) {}
    Vec(Vec &&rhs) : x(rhs.x), y(rhs.y) {}
    Vec &operator=(const Vec &rhs) {
        if (this == &rhs) return *this;
        x = rhs.x;
        y = rhs.y;
        return *this;
    }
};

int crossProduct(const Vec &v1, const Vec &v2);

using Point = Vec;

class MapArea : public Fl_Widget {
    int pixelLength;
    int realLength;
    int imageWidth;
    int imageHeight;
    std::vector<Point> points;
    Point currentPoint;
    Fl_Box *areaLabel;
    unsigned long long pixelArea;
    unsigned long long realArea;
    std::string areaString;
    bool mouseInside;
    bool done;

   public:
    MapArea(int X, int Y, int W, int H, int imgW, int imgH) : Fl_Widget(X, Y, W, H), imageWidth(imgW), imageHeight(imgH), pixelArea(0), realArea(0), done(false) {
        areaLabel = new Fl_Box(0, 0, 0, 0);
    }
    ~MapArea() {
        delete areaLabel;
    }
    void highlight(int leftX, int rightX, const std::vector<Point> &ps) const;
    void draw() override;
    int handle(int event) override;
    bool inMap(int px, int py) const {
        return px > x() && px < x() + w() && py > y() && py < y() + h();
    }
    void show() override {
        Fl_Widget::show();
        areaLabel->show();
    }
    void hide() override {
        Fl_Widget::hide();
        areaLabel->hide();
    }
    void resize(int X, int Y, int W, int H) {
        Fl_Widget::resize(X, Y, W, H);
        areaLabel->resize(X + W * 2 / 5, Y - H / 5, W / 5, H / 10);
        areaLabel->labelsize(W / 70);
    }
    void undo() {
        if (!points.empty()) points.pop_back();
    }
    void reset() {
        points.clear();
        pixelArea = realArea = 0;
        done = false;
    }
    bool confirm() {
        if (points.size() >= 3) {
            done = true;
            return true;
        }
        return false;
    }
    void setScale(int pxLen, int realLen) {
        pixelLength = pxLen;
        realLength = realLen;
    }
    void computeArea();
};

}  // namespace FLTK_MAP

#endif