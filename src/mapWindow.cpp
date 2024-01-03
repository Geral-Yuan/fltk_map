#include "mapWindow.h"

namespace FLTK_MAP {

void calibrate_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    window->calibrateScale->hide();
    window->cursor->show();
    window->realPrompt->show();
    window->realInput->show();
    window->pixelInputButton->show();
}

void scaleConfirm_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    window->pixelLength = window->cursor->getPixelLength();
    if (window->pixelLength == -1) window->pixelLength = std::stoi(window->pixelInput->value());
    window->realLength = std::stoi(window->realInput->value());
    window->mapArea->setScale(window->pixelLength, window->realLength);
    window->scale = "Scale: [ " + std::to_string(window->pixelLength) + " px : " + std::to_string(window->realLength) + " m ]";
    window->cursor->hide();
    window->realPrompt->hide();
    window->realInput->hide();
    window->pixelPrompt->hide();
    window->pixelInput->hide();
    window->hideScaleButton();
    window->back2cursor->hide();
    window->scaleLabel->label(window->scale.c_str());
    window->scaleLabel->show();
    window->mapArea->show();
    window->areaComfirm->show();
    window->pointUndo->show();
}

void scaleCancel_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    window->cursor->reset();
    window->hideScaleButton();
    window->showPixelInput();
}

void pixelInput_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    window->hidePixelInput();
    window->cursor->reset();
    window->cursor->hide();
    window->scaleConfirm->show();
    window->back2cursor->show();
    window->pixelPrompt->show();
    window->pixelInput->show();
}

void back2cursor_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    window->scaleConfirm->hide();
    window->back2cursor->hide();
    window->showPixelInput();
    window->cursor->show();
    window->pixelPrompt->hide();
    window->pixelInput->hide();
}

void areaConfirm_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    if (window->mapArea->confirm()) {
        window->areaComfirm->hide();
        window->pointUndo->hide();
    }
}

void pointUndo_callback(Fl_Widget *, void *win) {
    MapWindow *window = (MapWindow *)win;
    window->mapArea->undo();
    window->redraw();
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
        fl_line(x() + click_X[i] * w() / imageWidth, y(), x() + click_X[i] * w() / imageWidth, y() + h());
    if (mouseInside && clickCnt != 2)
        fl_line(x() + cursor_X * w() / imageWidth, y(), x() + cursor_X * w() / imageWidth, y() + h());
    fl_line_style(0);
    if (clickCnt == 2) {
        pixel_dis = abs(click_X[1] - click_X[0]);
        labelContent = std::to_string(pixel_dis) + " pixels";
    } else if (clickCnt == 1) {
        pixel_dis = abs(cursor_X - click_X[0]);
        labelContent = std::to_string(pixel_dis) + " pixels";
    } else
        labelContent = "N/A";
    cursorDisLabel->label(labelContent.c_str());
}

int Cursor::handle(int event) {
    int event_x = Fl::event_x();
    int event_y = Fl::event_y();
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
                cursor_X = (event_x - x()) * imageWidth / w();
                window()->redraw();
                return 1;
            }
            break;
        }
        case FL_PUSH: {
            if (clickCnt < 2 && Fl::event_button() == FL_LEFT_MOUSE && inMap(event_x, event_y)) {
                click_X[clickCnt++] = (event_x - x()) * imageWidth / w();
                if (clickCnt == 2) {
                    ((MapWindow *)window())->hidePixelInput();
                    ((MapWindow *)window())->showScaleButton();
                }
                return 1;
            }
            break;
        }
    }
    return 0;
}

MapWindow::MapWindow(int W, int H, const char *L, const char *testcase, const char *suffix) : Fl_Window(W, H, L) {
    canvas = new Canvas(0, 0, W, H);
    if (std::string(suffix)=="png")
        backgroundImage = new Fl_PNG_Image(("./assets/" + std::string(testcase) + "." + std::string(suffix)).c_str());
    else if (std::string(suffix)=="jpeg")
        backgroundImage = new Fl_JPEG_Image(("./assets/" + std::string(testcase) + "." + std::string(suffix)).c_str());
    background = new Fl_Box(0, 0, W, H);
    background->image(backgroundImage->copy(W, H));
    calibrateScale = new Fl_Button(0, 0, W, H, "calibrate scale");
    cursor = new Cursor(0, 0, W, H, backgroundImage->w(), backgroundImage->h());
    cursor->hide();
    realPrompt = new Fl_Box(0, 0, W, H, "real distance in meter:");
    realPrompt->hide();
    realInput = new Fl_Input(0, 0, W, H);
    realInput->value("0");
    realInput->hide();
    pixelPrompt = new Fl_Box(0, 0, W, H, "pixel distance in px:");
    pixelPrompt->hide();
    pixelInput = new Fl_Input(0, 0, W, H);
    pixelInput->value("0");
    pixelInput->hide();
    scaleConfirm = new Fl_Button(0, 0, W, H, "Confirm");
    scaleConfirm->hide();
    scaleCancel = new Fl_Button(0, 0, W, H, "Cancel");
    scaleCancel->hide();
    scaleLabel = new Fl_Box(0, 0, W, H);
    scaleLabel->hide();
    pixelInputButton = new Fl_Button(0, 0, W, H, "input pixel distance manually");
    pixelInputButton->hide();
    back2cursor = new Fl_Button(0, 0, W, H, "Back");
    back2cursor->hide();
    mapArea = new MapArea(0, 0, W, H, backgroundImage->w(), backgroundImage->h());
    mapArea->hide();
    areaComfirm = new Fl_Button(0, 0, W, H, "Confirm");
    areaComfirm->hide();
    pointUndo = new Fl_Button(0, 0, W, H, "Undo");
    pointUndo->hide();
    calibrateScale->callback(calibrate_callback, this);
    scaleConfirm->callback(scaleConfirm_callback, this);
    scaleCancel->callback(scaleCancel_callback, this);
    pixelInputButton->callback(pixelInput_callback, this);
    back2cursor->callback(back2cursor_callback, this);
    areaComfirm->callback(areaConfirm_callback, this);
    pointUndo->callback(pointUndo_callback, this);
}

MapWindow::~MapWindow() {
    delete canvas;
    delete backgroundImage;
    delete background->image();
    delete background;
    delete calibrateScale;
    delete cursor;
    delete realPrompt;
    delete realInput;
    delete pixelPrompt;
    delete pixelInput;
    delete scaleConfirm;
    delete scaleCancel;
    delete scaleLabel;
    delete pixelInputButton;
    delete back2cursor;
    delete mapArea;
    delete areaComfirm;
    delete pointUndo;
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
    calibrateScale->labelsize(canvas_W / 50);
    cursor->resize(canvas_X, canvas_Y + white_H, background_W, background_H);
    realPrompt->resize(W / 2 - canvas_W / 8, canvas_Y + white_H / 4, canvas_W * 3 / 16, white_H / 4);
    realPrompt->labelsize(canvas_W / 70);
    realInput->resize(W / 2 + canvas_W / 16, canvas_Y + white_H / 4, canvas_W / 16, white_H / 4);
    realInput->textsize(canvas_W / 70);
    pixelPrompt->resize(W / 2 - canvas_W / 8, canvas_Y, canvas_W * 3 / 16, white_H / 4);
    pixelPrompt->labelsize(canvas_W / 70);
    pixelInput->resize(W / 2 + canvas_W / 16, canvas_Y, canvas_W / 16, white_H / 4);
    pixelInput->textsize(canvas_W / 70);
    scaleConfirm->resize(W / 2, canvas_Y + white_H / 2, canvas_W / 10, white_H / 4);
    scaleConfirm->labelsize(canvas_W / 70);
    scaleCancel->resize(W / 2 - canvas_W / 10, canvas_Y + white_H / 2, canvas_W / 10, white_H / 4);
    scaleCancel->labelsize(canvas_W / 70);
    scaleLabel->resize(canvas_X, canvas_Y + white_H / 4, canvas_W / 5, white_H / 4);
    scaleLabel->labelsize(canvas_W / 70);
    pixelInputButton->resize(W / 2 - canvas_W / 8, canvas_Y + white_H / 2, canvas_W / 4, white_H / 4);
    pixelInputButton->labelsize(canvas_W / 70);
    back2cursor->resize(W / 2 - canvas_W / 10, canvas_Y + white_H / 2, canvas_W / 10, white_H / 4);
    back2cursor->labelsize(canvas_W / 70);
    mapArea->resize(canvas_X, canvas_Y + white_H, background_W, background_H);
    areaComfirm->resize(W / 2, canvas_Y + white_H / 2, canvas_W / 10, white_H / 4);
    areaComfirm->labelsize(canvas_W / 70);
    pointUndo->resize(W / 2 - canvas_W / 10, canvas_Y + white_H / 2, canvas_W / 10, white_H / 4);
    pointUndo->labelsize(canvas_W / 70);
}

}  // namespace FLTK_MAP