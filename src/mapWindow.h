#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <FL/Fl.H>
#include <Fl/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <Fl/Fl_JPEG_Image.H>
#include <Fl/Fl_Input.H>
#include <FL/fl_draw.H>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "mapArea.h"

namespace FLTK_MAP {

class Canvas : public Fl_Widget {
   public:
    Canvas(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H) {}
    void draw() override;
};

class Cursor : public Fl_Widget {
    int imageWidth;
    int imageHeight;
    int clickCnt;
    int click_X[2];
    int cursor_X;
    bool mouseInside;
    int pixel_dis;
    std::string labelContent;
    Fl_Box *cursorDisLabel;

   public:
    Cursor(int X, int Y, int W, int H, int imgW, int imgH) : Fl_Widget(X, Y, W, H), imageWidth(imgW), imageHeight(imgH), clickCnt(0), mouseInside(false), pixel_dis(0), labelContent("N/A") {
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

class MapWindow : public Fl_Double_Window {
    int pixelLength;
    int realLength;
    std::string scale;
    Canvas *canvas;
    Fl_RGB_Image *backgroundImage;
    Fl_Box *background;
    Fl_Button *calibrateScale;
    Cursor *cursor;
    Fl_Box *realPrompt;
    Fl_Input *realInput;
    Fl_Box *pixelPrompt;
    Fl_Input *pixelInput;
    Fl_Button *scaleConfirm;
    Fl_Button *scaleCancel;
    Fl_Box *scaleLabel;
    Fl_Button *pixelInputButton;
    Fl_Button *back2cursor;
    MapArea *mapArea;
    Fl_Button *areaComfirm;
    Fl_Button *pointUndo;
    Fl_Button *areaCancel;
    Fl_Box *badInputPrompt;
    Fl_Box *invalidPolygonPrompt;
    bool badInput;
    bool invalidPolygon;
    bool cursorMode;
    friend void calibrate_callback(Fl_Widget *, void *);
    friend void scaleConfirm_callback(Fl_Widget *, void *);
    friend void scaleCancel_callback(Fl_Widget *, void *);
    friend void pixelInput_callback(Fl_Widget *, void *);
    friend void back2cursor_callback(Fl_Widget *, void *);
    friend void areaConfirm_callback(Fl_Widget *, void *);
    friend void pointUndo_callback(Fl_Widget *, void *);
    friend void areaCancel_callback(Fl_Widget *, void *);
    friend void badInputTimer_callback(void *);
    friend void invalidPolygonTimer_callback(void *);

   public:
    MapWindow(int W, int H, const char *L = 0, const char *testcase = "testcase1", const char *suffix = "png");
    ~MapWindow();
    void resize(int X, int Y, int W, int H);
    void showScaleButton() {
        scaleConfirm->show();
        scaleCancel->show();
    }
    void hideScaleButton() {
        scaleConfirm->hide();
        scaleCancel->hide();
    }
    void showPixelInput() {
        pixelInputButton->show();
    }
    void hidePixelInput() {
        pixelInputButton->hide();
    }
};

}  // namespace FLTK_MAP

#endif