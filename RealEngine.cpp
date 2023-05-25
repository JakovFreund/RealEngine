#include <GLFW/glfw3.h>

#include <array>
#include <vector>

// 3DGraphics

class Color {
  public:
    unsigned char R;
    unsigned char G;
    unsigned char B;

    Color(unsigned char red, unsigned char green, unsigned char blue) : R(red), G(green), B(blue){};
    Color(const Color& other) : R(other.R), G(other.G), B(other.B){};
    Color() : R(0x00), G(0x00), B(0x00){};

    friend std::ostream& operator<<(std::ostream& out, Color const& color) {
        out << "<" << +color.R << "," << +color.G << "," << +color.B << ">";
        return out;
    }
};

class RealWindow { // move some stuff to private
  public:
    const int window_width, window_height;
    int window_fps;
    GLFWwindow* glfw_window;
    unsigned char* raw_color_data = new unsigned char[window_width * window_height * 3];
    std::vector<std::vector<Color>> data;
    const Color RED = Color(0x80, 0x00, 0x00);
    const Color BLUE = Color(0x00, 0x00, 0x80);
    const Color YELLOW = Color(0xF0, 0xF0, 0x00);

    RealWindow(int window_width, int window_height, int window_window_fps)
        : window_width(window_width), window_height(window_height), window_fps(window_fps) {
        glfwInit();
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfw_window = glfwCreateWindow(window_width, window_height, "title", NULL, NULL);
        glfwMakeContextCurrent(glfw_window);
        data = std::vector<std::vector<Color>>(window_height, std::vector<Color>(window_width, YELLOW));
    }
    void drawBackground() {
        for (int y = 0; y < window_height; ++y) {
            for (int x = 0; x < window_width; ++x) {
                data[y][x] = BLUE;
            }
        }
    }
    bool windowLoop() { return (!glfwWindowShouldClose(glfw_window)); }
    void clear() {
        // 2D Mode
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window_width, window_height, 0, 0, 1);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor(0.0f, 0.0f, 0.0f, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void render() {
        for (int y = 0; y < window_height; ++y) {
            for (int x = 0; x < window_width; ++x) {
                raw_color_data[y * window_width * 3 + x * 3] = data[y][x].R;
                raw_color_data[y * window_width * 3 + x * 3 + 1] = data[y][x].G;
                raw_color_data[y * window_width * 3 + x * 3 + 2] = data[y][x].B;
            }
        }
        glDrawPixels(window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, raw_color_data);
        glfwSwapBuffers(glfw_window);
        glfwSwapInterval(1);
        glfwPollEvents();
    }
    void close() {
        glfwDestroyWindow(glfw_window);
        glfwTerminate();
    }
};

unsigned char* LoadImageFromFile(const char* filePath) {
    long lCurPos, lEndPos, fileSize;
    unsigned char* fileBuf;
    FILE* file = NULL;

    // opening file in "rb" also checks if the file exists and/or can be opened for reading correctly
    if ((file = fopen(filePath, "rb")) == NULL)
        printf("Could not open specified file\n");

    // filesize for allocating memory
    lCurPos = ftell(file);
    fseek(file, 0, 2);
    lEndPos = ftell(file);
    fseek(file, lCurPos, 0);
    fileSize = lEndPos;

    fileBuf = new unsigned char[fileSize];
    fread(fileBuf, fileSize, 1, file);

    fclose(file);
    return fileBuf;
}

// g++ -g -o main.exe main.cpp -lglfw3 -lopengl32 -lgdi32 || exit