# fltk map

This project is the first project of SJTU 2023 Fall Course CS2309 Problem Solving and Practice.

## How to run

fltk-version >= 1.3.4

Linux is recommended

1. run cmd `make map` under the root directory to compile the project.

2. execute the program by `./main [option1] [option2]`, where `[option1]` is the file name of the image under `./assets` without the format suffix while `[option2]` is the format suffix of that image file. For example, enter `./main testcase1 png` to run the program with the image `./assets/testcase1.png`.

## How to use

First, you need to set the scale of the map, $L1:L2$, which means $L1$ pixels on the map image represents $L2$ meters in real. The scale can be set in either of following two ways: (click the corresponding button to switch the way to set)

- Caribrate the scale of the map by placing two cursor at two ends of the scale segment bottom left corner and enter the real distance it represents.

- Enter the pixel length and real distance manually if you have known the scale of this map image.

Confirm by clicking the "Confirm" button after you set the scale in either way.

Then, click points on the map image to select vertices of the polygon to draw and compute the area. You can click "Uodo" button to undo the click of last point.

If you confirm the choices with less than 3 points or leaving edges intersected with each other, the program reject the confirm and prompt the exception.

After you confirm with valid polygon, you can cancel your choice by button and do points selection again.
