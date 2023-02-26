#include <GL/glut.h>
#include <filesystem>
#include <chrono>

#include "textures.hpp"
#include "map.hpp"
#include "mapview.hpp"
#include "player.hpp"
#include "textrenderer.hpp"

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

    auto timeBetweenFrames = std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - lastTime).count();
    if (timeBetweenFrames == 0) timeBetweenFrames = 1;

    TextRenderer::drawNumber(1000000000 / timeBetweenFrames, Vec2(5, 24));
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
    if (key == 119) keysPressed.w = true;
    if (key == 97) keysPressed.a = true;
    if (key == 115) keysPressed.s = true;
    if (key == 100) keysPressed.d = true;

    if (key == 27) // esc = open map view
        MapView::mapViewOpen = !MapView::mapViewOpen;
#ifdef _DEBUG
    printf("%i\n", key);
#endif
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
    printf("%s\n\n\n", std::filesystem::current_path().string().c_str());
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