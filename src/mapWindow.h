#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <Fl/Fl_Input.H>
#include <FL/fl_draw.H>
#include <algorithm>
#include <cmath>
#include <iostream>

namespace FLTK_MAP {

class Canvas : public Fl_Widget {
   public:
    Canvas(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H) {}
    void draw() override;
};

class Cursor : public Fl_Widget {
    int clickCnt;
    double click_X[2];
    double cursor_X;
    int pixel_dis;
    std::string labelContent;
    Fl_Box *cursorDisLabel;

   public:
    Cursor(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H), clickCnt(0) {
        cursorDisLabel = new Fl_Box(0, 0, 0, 0);
    }
    ~Cursor() {
        delete cursorDisLabel;
    }
    void draw() override;
    void resize(int X, int Y, int W, int H) override {
        Fl_Widget::resize(X, Y, W, H);
        cursorDisLabel->resize(X + W * 2 / 5, Y - H / 5, W / 5, H / 20);
        cursorDisLabel->labelsize(W / 60);
    }
    void hide() override {
        Fl_Widget::hide();
        cursorDisLabel->hide();
    }
    void show() override {
        Fl_Widget::show();
        cursorDisLabel->show();
    }
    int handle(int event) override;
    bool inMap(int px, int py) const {
        return px > x() && px < x() + w() && py > y() && py < y() + h();
    }
    int getPixelLength() const {
        return pixel_dis;
    }
    void reset() {
        clickCnt = 0;
        pixel_dis = 0;
        labelContent = "N/A";
    }
};

class MapWindow : public Fl_Window {
    int pixelLength;
    int realLength;
    std::string scale;
    Canvas *canvas;
    Fl_PNG_Image *backgroundImage;
    Fl_Box *background;
    Fl_Button *calibrateScale;
    Cursor *cursor;
    Fl_Box *prompt;
    Fl_Input *scaleInput;
    Fl_Button *scaleConfirm;
    Fl_Button *scaleCancel;
    Fl_Box *scaleLabel;
    friend void calibrate_callback(Fl_Widget *, void *);
    friend void scaleConfirm_callback(Fl_Widget *, void *);
    friend void scaleCancel_callback(Fl_Widget *, void *);

   public:
    MapWindow(int W, int H, const char *L = 0);
    ~MapWindow();
    void resize(int X, int Y, int W, int H);
    int getPixelWidth() const { return backgroundImage->w(); }
    int getPixelHeight() const { return backgroundImage->h(); }
    void showScaleButton() {
        scaleConfirm->show();
        scaleCancel->show();
    }
    void hideScaleButton() {
        scaleConfirm->hide();
        scaleCancel->hide();
    }
};

}  // namespace FLTK_MAP

#endif