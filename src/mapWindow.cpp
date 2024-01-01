#include "mapWindow.h"

namespace FLTK_MAP {

void calibrate_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    window->calibrateScale->hide();
    window->cursor->show();
    window->prompt->show();
    window->scaleInput->show();
}

void scaleConfirm_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    window->pixelLength = window->cursor->getPixelLength();
    window->realLength = std::stoi(window->scaleInput->value());
    window->scale = "Scale: [ " + std::to_string(window->pixelLength) + " px : " + std::to_string(window->realLength) + " m ]";
    window->cursor->hide();
    window->prompt->hide();
    window->scaleInput->hide();
    window->scaleConfirm->hide();
    window->scaleLabel->label(window->scale.c_str());
    window->scaleLabel->show();
}

void Canvas::draw() {
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

void Cursor::draw() {
    int line_width = 1 + h() / 500;
    fl_color(FL_RED);
    fl_line_style(FL_SOLID, line_width);
    for (int i = 0; i < clickCnt; ++i)
        fl_line(x() + click_X[i] * w(), y(), x() + click_X[i] * w(), y() + h());
    if (clickCnt != 2)
        fl_line(x() + cursor_X * w(), y(), x() + cursor_X * w(), y() + h());
    fl_line_style(0);
    int pixelWidth = ((MapWindow *)window())->getPixelWidth();
    if (clickCnt == 2) {
        pixel_dis = fabs(click_X[1] - click_X[0]) * pixelWidth;
        labelContent = std::to_string(pixel_dis) + " pixels";
    } else if (clickCnt == 1) {
        pixel_dis = fabs(cursor_X - click_X[0]) * pixelWidth;
        labelContent = std::to_string(pixel_dis) + " pixels";
    } else
        labelContent = "N/A";
    cursorDisLabel->label(labelContent.c_str());
}

int Cursor::handle(int event) {
    int event_x = Fl::event_x();
    int event_y = Fl::event_y();
    cursor_X = (double)(event_x - x()) / w();
    window()->redraw();
    switch (event) {
        case FL_PUSH: {
            if (clickCnt < 2 && Fl::event_button() == FL_LEFT_MOUSE && inMap(event_x, event_y)) {
                click_X[clickCnt++] = (double)(event_x - x()) / w();
                ((MapWindow *)window())->showScaleConfirm();
                return 1;
            }
            break;
        }
    }
    return 0;
}

MapWindow::MapWindow(int W, int H, const char *L) : Fl_Window(W, H, L) {
    canvas = new Canvas(0, 0, W, H);
    backgroundImage = new Fl_PNG_Image("./assets/testcase1.png");
    background = new Fl_Box(0, 0, W, H);
    background->image(backgroundImage->copy(W, H));
    calibrateScale = new Fl_Button(0, 0, W, H, "calibrate scale");
    cursor = new Cursor(0, 0, W, H);
    cursor->hide();
    prompt = new Fl_Box(0, 0, W, H, "real distance in meter:");
    prompt->hide();
    scaleInput = new Fl_Input(0, 0, W, H);
    scaleInput->value("0");
    scaleInput->hide();
    scaleConfirm = new Fl_Button(0, 0, W, H, "Confirm");
    scaleConfirm->hide();
    scaleLabel = new Fl_Box(0, 0, W, H);
    scaleLabel->hide();
    calibrateScale->callback(calibrate_callback, this);
    scaleConfirm->callback(scaleConfirm_callback, this);
}

MapWindow::~MapWindow() {
    delete canvas;
    delete backgroundImage;
    delete background->image();
    delete background;
    delete calibrateScale;
    delete cursor;
    delete prompt;
    delete scaleInput;
    delete scaleConfirm;
    delete scaleLabel;
}

void MapWindow::resize(int X, int Y, int W, int H) {
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
    int white_H = canvas_H - background_H;
    canvas->resize(canvas_X, canvas_Y, canvas_W, canvas_H);
    delete background->image();
    background->resize(canvas_X, canvas_Y + white_H, background_W, background_H);
    background->image(backgroundImage->copy(background_W, background_H));
    calibrateScale->resize(W / 2 - canvas_W / 10, canvas_Y + white_H / 3, canvas_W / 5, white_H / 3);
    calibrateScale->labelsize(W / 50);
    cursor->resize(canvas_X, canvas_Y + white_H, background_W, background_H);
    prompt->resize(W / 2 - canvas_W / 8, canvas_Y + white_H / 4, canvas_W * 3 / 16, white_H / 4);
    prompt->labelsize(W / 70);
    scaleInput->resize(W / 2 + canvas_W / 16, canvas_Y + white_H / 4, canvas_W / 16, white_H / 4);
    scaleInput->textsize(W / 70);
    scaleConfirm->resize(W / 2 - canvas_W / 20, canvas_Y + white_H / 2, canvas_W / 10, white_H / 4);
    scaleConfirm->labelsize(W / 70);
    scaleLabel->resize(canvas_X, canvas_Y + white_H / 4, canvas_W / 5, white_H / 4);
    scaleLabel->labelsize(W / 70);
}

}  // namespace FLTK_MAP