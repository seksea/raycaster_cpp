#include <GL/glut.h>
#include <filesystem>
#include <chrono>

#include "textures.hpp"
#include "map.hpp"
#include "mapview.hpp"
#include "player.hpp"
#include "textrenderer.hpp"
#include "gamestate.hpp"
#include "sprites.hpp"

void init(void)
{
    glClearColor(0, 0, 0, 1.f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 480.0, 0.0);

    Textures::initTextures();
    TextRenderer::init();
    Map::init();
    localPlayer.m_curSpace = Map::map;
}

void fpsCounter()
{
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto curTime = std::chrono::high_resolution_clock::now();

    GameState::deltaTime = ((float)std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - lastTime).count()) / 1000000;

    TextRenderer::drawNumber(1000 / GameState::deltaTime, Vec2(5, 24));
    lastTime = curTime;
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

	if (!MapView::mapViewOpen)
    {
        // Draw sky and floor
        int offset = (int)(localPlayer.m_lookDir.angle() * 520);
        for (int i = 0; i < 640; i += 4)
        {
            Textures::sky->drawColumn(i, 0, abs(offset + i) % 500, 4, 150, 1.f);
        }

        glColor3f(0, 0.4f, 0);
		glBegin(GL_QUADS);
		glVertex2i(0, 150);
		glVertex2i(640, 150);
		glVertex2i(640, 480);
		glVertex2i(0, 480);
		glEnd();

        glColor3f(1.f, 1.f, 1.f);

		localPlayer.renderView();
        Sprites::renderSprites();
        Textures::pistol->drawImage(295, 360);

        // hotbar and hotbar text
        Textures::hotbar->drawImage(0, 480);
        TextRenderer::drawNumber(100, Vec2(40, 397));
        TextRenderer::drawNumber(22, Vec2(40, 423));

        //TextRenderer::drawNumber(abs(localPlayer.m_pos.m_x), Vec2(5, 50));
        //TextRenderer::drawNumber(abs(localPlayer.m_pos.m_y), Vec2(50, 50));
    }
    else
    {
        MapView::render(Map::map);
    }
    fpsCounter();
    glFlush();
}

void keydown(unsigned char key, int x, int y) {
    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    {
        keysPressed.shift = true;
        if (key + 32 == 'w') keysPressed.w = true;
        if (key + 32 == 'a') keysPressed.a = true;
        if (key + 32 == 's') keysPressed.s = true;
        if (key + 32 == 'd') keysPressed.d = true;

        printf("down %i %i\n", glutGetModifiers(), key+32);
    }
    else {
        keysPressed.shift = false;
        if (key == 'w') keysPressed.w = true;
        if (key == 'a') keysPressed.a = true;
        if (key == 's') keysPressed.s = true;
        if (key == 'd') keysPressed.d = true;

        if (key == 27) // esc = open map view
            MapView::mapViewOpen = !MapView::mapViewOpen;

        printf("down %i %i\n", glutGetModifiers(), key);
    }
#ifdef _DEBUG
#endif
}

void keyup(unsigned char key, int x, int y)
{
    if (glutGetModifiers() == 3)
    {
        keysPressed.shift = true;
        if (key + 32 == 'w') keysPressed.w = false;
        if (key + 32 == 'a') keysPressed.a = false;
        if (key + 32 == 's') keysPressed.s = false;
        if (key + 32 == 'd') keysPressed.d = false;

        printf("up %i %i\n", glutGetModifiers(), key + 32);
    }
    else {
        keysPressed.shift = false;
        if (key == 'w') keysPressed.w = false;
        if (key == 'a') keysPressed.a = false;
        if (key == 's') keysPressed.s = false;
        if (key == 'd') keysPressed.d = false;

        printf("up %i %i\n", glutGetModifiers(), key);
    }
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