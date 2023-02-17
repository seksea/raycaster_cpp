#include <GL/glut.h>

#include "textures.hpp"
#include "map.hpp"
#include "mapview.hpp"
#include "player.hpp"

void init(void)
{
    glClearColor(0, 0, 0, 1.f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 480.0, 0.0);

    Textures::initTextures();
    Map::init();
    localPlayer.m_curSpace = Map::map;
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

	if (!MapView::mapViewOpen)
    {
        Textures::hotbar->drawImage(0, 480);
        localPlayer.renderView();
    }
    else
    {
        MapView::render(Map::map);
        localPlayer.renderView();
    }
    glFlush();
}

void keydown(unsigned char key, int x, int y) {
    if (key == 119) keysPressed.w = true;
    if (key == 97) keysPressed.a = true;
    if (key == 115) keysPressed.s = true;
    if (key == 100) keysPressed.d = true;

    if (key == 27) // esc
        exit(0);

    printf("%i\n", key);
}

void keyup(unsigned char key, int x, int y)
{
    if (key == 119) keysPressed.w = false;
    if (key == 97) keysPressed.a = false;
    if (key == 115) keysPressed.s = false;
    if (key == 100) keysPressed.d = false;
}

void timer(int val)
{
    glutTimerFunc(16, timer, 0);
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("raycaster");
    init();
    glutKeyboardFunc(keydown);
    glutKeyboardUpFunc(keyup);
    glutTimerFunc(0, timer, 0);
    glutDisplayFunc(render);
    glutMainLoop();
    return 0;
}