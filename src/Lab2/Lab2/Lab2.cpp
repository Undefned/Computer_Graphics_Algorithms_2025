#define _USE_MATH_DEFINES
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glu32.lib")

// Идентификатор текстуры
GLuint textureID;
int currentMode = 0; // 0 - passthrough, 1 - UV, 2 - texture

// Загрузка текстуры из массива пикселей
GLuint createTexture() {
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Создаем простую текстуру 64x64
    const int texWidth = 64;
    const int texHeight = 64;
    GLubyte textureData[texWidth * texHeight * 3];

    // Заполняем текстуру паттерном (шахматная доска)
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            int idx = (y * texWidth + x) * 3;
            if ((x / 8 + y / 8) % 2 == 0) {
                // Синий квадрат
                textureData[idx] = 0;
                textureData[idx + 1] = 0;
                textureData[idx + 2] = 255;
            }
            else {
                // Желтый квадрат
                textureData[idx] = 255;
                textureData[idx + 1] = 255;
                textureData[idx + 2] = 0;
            }
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0,
        GL_RGB, GL_UNSIGNED_BYTE, textureData);

    // Настройка фильтрации
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return texID;
}

// 1. Passthrough координаты - преобразование [-1,1] -> [0,1]
void drawPassthroughTriangle() {
    glBegin(GL_TRIANGLES);

    // Вершина 1: левый нижний угол
    glColor3f(0.0f, 0.0f, 0.0f);  // Черный (UV = 0,0)
    glVertex2f(-0.5f, -0.5f);

    // Вершина 2: правый нижний угол
    glColor3f(1.0f, 0.0f, 0.0f);  // Красный (UV = 1,0)
    glVertex2f(0.5f, -0.5f);

    // Вершина 3: верхний центр
    glColor3f(0.5f, 1.0f, 0.0f);  // Зеленый (UV = 0.5,1)
    glVertex2f(0.0f, 0.5f);

    glEnd();
}

// 2. Визуализация UV координат
void drawUVCoordinates() {
    glBegin(GL_TRIANGLES);

    // Сделаем цвета более контрастными, чтобы было видно разницу!

    // Вершина 1: (U=0, V=0)
    glColor3f(0.0f, 0.0f, 1.0f);  // Синий!
    glVertex2f(-0.5f, -0.5f);

    // Вершина 2: (U=1, V=0)
    glColor3f(1.0f, 1.0f, 0.0f);  // Желтый!
    glVertex2f(0.5f, -0.5f);

    // Вершина 3: (U=0.5, V=1)
    glColor3f(1.0f, 0.0f, 1.0f);  // Фиолетовый!
    glVertex2f(0.0f, 0.5f);

    glEnd();
}

// 3. Отображение текстуры
void drawTexturedTriangle() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_TRIANGLES);

    // Вершина 1: левый нижний угол
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);

    // Вершина 2: правый нижний угол
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);

    // Вершина 3: верхний центр
    glTexCoord2f(0.5f, 1.0f);
    glVertex2f(0.0f, 0.5f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Выводим текст с текущим режимом
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.95f, 0.9f);

    const char* modeText = "";  // Инициализируем по умолчанию

    switch (currentMode) {
    case 0: modeText = "Mode 1: Passthrough coordinates"; break;
    case 1: modeText = "Mode 2: UV coordinates visualization"; break;
    case 2: modeText = "Mode 3: Textured triangle"; break;
    default: modeText = "Unknown mode"; break;  // Добавляем default case
    }

    for (const char* c = modeText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    // Рисуем координатные оси для наглядности
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    // Рисуем в зависимости от текущего режима
    switch (currentMode) {
    case 0:
        drawPassthroughTriangle();
        break;
    case 1:
        drawUVCoordinates();
        break;
    case 2:
        drawTexturedTriangle();
        break;
    }

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    // Создаем текстуру
    textureID = createTexture();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:  // ESC - выход
        exit(0);
        break;
    case '1':
        currentMode = 0;
        glutPostRedisplay();
        break;
    case '2':
        currentMode = 1;
        glutPostRedisplay();
        break;
    case '3':
        currentMode = 2;
        glutPostRedisplay();
        break;
    case ' ':
        currentMode = (currentMode + 1) % 3;
        glutPostRedisplay();
        break;
    }
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab 2: Texture Mapping - Passthrough, UV, Texture");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    printf("Controls:\n");
    printf("  1 - Passthrough coordinates\n");
    printf("  2 - UV coordinates visualization\n");
    printf("  3 - Textured triangle\n");
    printf("  SPACE - Next mode\n");
    printf("  ESC - Exit\n");

    glutMainLoop();

    return 0;
}