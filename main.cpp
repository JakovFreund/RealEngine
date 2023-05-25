#include <windows.h>

#include <ctime>
#include <iostream>

#include "RealEngine.cpp"

using namespace std;

const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 600;
int FPS = 100;

clock_t start_timer;
double time_duration;
int frame = 0;


int main() {
    int xpos = 0;
    RealWindow window = RealWindow(WINDOW_WIDTH, WINDOW_HEIGHT, FPS);

    while (window.windowLoop()) {
        start_timer = clock(); // move to class

        window.clear();

        window.drawBackground();

        time_duration = (clock() - start_timer) / (double)CLOCKS_PER_SEC;
        Sleep(int(1000 / FPS) - time_duration);
        frame += 1;
        if (frame >= FPS)
            frame = 0;

        window.render();
    }

    window.close();
    return 0;
}

// g++ -g -o main.exe main.cpp -lglfw3 -lopengl32 -lgdi32 || exit
