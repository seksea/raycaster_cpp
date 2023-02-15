#include <GL/glut.h>

#include "textures.hpp"
#include "map.hpp"
#include "mapview.hpp"
#include "gamestate.hpp"
#include "player.hpp"

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 480.0, 0.0);
    Textures::initTextures();
    Map::init();
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

	if (!MapView::mapViewOpen)
    {
        Textures::hotbar->drawImage(0, 480);
    }
    else
    {
        MapView::renderRecursively(Map::map);
    }
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("raycaster");
    init();
    glutDisplayFunc(render);
    glutMainLoop();
    return 0;
}