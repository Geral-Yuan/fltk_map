#ifndef MAPWINDOW_HPP
#define MAPWINDOW_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <algorithm>
#include <iostream>

namespace FLTK_MAP {

class Canvas : public Fl_Widget {
   public:
    Canvas(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H) {}

    void draw() override {
        int X = x(), Y = y(), W = w(), H = h();
        int line_width = 1 + H / 500;
        fl_color(FL_WHITE);
        fl_rectf(X, Y, W, H);
        fl_line_style(FL_SOLID, line_width);
        fl_color(FL_BLACK);
        fl_rect(X - line_width, Y - line_width, W + line_width * 2, H + line_width * 2);
        fl_line(X - line_width, Y + H / 6 - line_width, X + W + line_width, Y + H / 6 - line_width);
        fl_line_style(0);
    }
};

class MapWindow : public Fl_Window {
    Canvas *canvas;
    Fl_PNG_Image *backgroundImage;
    Fl_Box *background;

   public:
    MapWindow(int W, int H, const char *L = 0) : Fl_Window(W, H, L) {
        canvas = new Canvas(0, 0, W, H);
        backgroundImage = new Fl_PNG_Image("./assets/testcase1.png");
        background = new Fl_Box(0, 0, W, H);
        background->image(backgroundImage->copy(W, H));
    }

    ~MapWindow() {
        delete canvas;
        delete backgroundImage;
        delete background->image();
        delete background;
    }

    void resize(int X, int Y, int W, int H) override {
        Fl_Window::resize(X, Y, W, H);
        int imageWidthPixels = backgroundImage->w();
        int imageHeightPixels = backgroundImage->h();
        double pixelSize = std::min((double)W / imageWidthPixels, (double)H / (imageHeightPixels * 1.2));
        int canvas_W = imageWidthPixels * pixelSize;
        int canvas_H = imageHeightPixels * 1.2 * pixelSize;
        int background_W = canvas_W;
        int background_H = imageHeightPixels * pixelSize;
        int canvas_X = (W - canvas_W) / 2;
        int canvas_Y = (H - canvas_H) / 2;
        canvas->resize(canvas_X, canvas_Y, canvas_W, canvas_H);
        delete background->image();
        background->resize(canvas_X, canvas_Y + canvas_H - background_H, background_W, background_H);
        background->image(backgroundImage->copy(background_W, background_H));
    }
};

}  // namespace FLTK_MAP

#endif