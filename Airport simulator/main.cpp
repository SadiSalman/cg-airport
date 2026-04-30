#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>

void drawSky(float c1, float c2, float c3)
{
    glColor3f(c1, c2, c3);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f); // bottom-left
        glVertex2f( 1.0f, -1.0f); // bottom-right
        glVertex2f( 1.0f,  1.0f); // top-right
        glVertex2f(-1.0f,  1.0f); // top-left
    glEnd();
}

void drawGround(float g1, float g2, float g3)
{
    glColor3f(g1, g2, g3);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f); // bottom
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  0.4f); // up to 60% of frame
        glVertex2f(-1.0f,  0.4f);
    glEnd();
}

void drawRunwayLights()
{
    // Runway edge lights (blue points along both sides)
    glPointSize(6.0f);
    glColor3f(0.4f, 0.7f, 1.0f); // bluish lights
    glBegin(GL_POINTS);
        for(float x=-0.95f; x<=0.95f; x+=0.15f) {
            // bottom edge lights
            glVertex2f(x, -0.79f);
            // top edge lights
            glVertex2f(x, -0.11f);
        }
    glEnd();
    glPointSize(1.0f); // reset
}

void drawRunway() {
    // Runway base (dark gray, thicker vertically, end-to-end)
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.8f);  // bottom-left
        glVertex2f( 1.0f, -0.8f);  // bottom-right
        glVertex2f( 1.0f, -0.1f);  // top-right
        glVertex2f(-1.0f, -0.1f);  // top-left
    glEnd();

    // Side lines (white, thicker)
    glLineWidth(4.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex2f(-1.0f, -0.78f);
        glVertex2f( 1.0f, -0.78f);

        glVertex2f(-1.0f, -0.12f);
        glVertex2f( 1.0f, -0.12f);
    glEnd();
    glLineWidth(1.0f); // reset

    // Center dashed stripes (lighter yellow, thinner height, longer width)
    glColor3f(1.0f, 1.0f, 0.7f);
    for(float x=-0.98f; x<=0.98f; x+=0.3f) { // spaced a bit wider
        glBegin(GL_QUADS);
            glVertex2f(x, -0.46f);       // thinner vertically
            glVertex2f(x+0.15f, -0.46f); // longer horizontally
            glVertex2f(x+0.15f, -0.44f); // thinner height
            glVertex2f(x, -0.44f);
        glEnd();
    }

    // Optional: runway lights if you want them
    drawRunwayLights();
}


void drawTaxiway() {
    // Taxiway base (lighter gray, above runway)
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -0.05f);  // bottom-left
        glVertex2f( 1.0f, -0.05f);  // bottom-right
        glVertex2f( 1.0f,  0.15f);  // top-right
        glVertex2f(-1.0f,  0.15f);  // top-left
    glEnd();

    // Taxiway sidelines (white, very thin)
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex2f(-1.0f, -0.04f);
        glVertex2f( 1.0f, -0.04f);

        glVertex2f(-1.0f,  0.14f);
        glVertex2f( 1.0f,  0.14f);
    glEnd();
    glLineWidth(1.0f); // reset

    // Taxiway center line (very thin yellow dashed)
    glColor3f(1.0f, 1.0f, 0.6f); // lighter yellow
    for(float x=-0.9f; x<=0.9f; x+=0.25f) {
        glBegin(GL_QUADS);
            glVertex2f(x, 0.045f);
            glVertex2f(x+0.08f, 0.045f); // shorter width
            glVertex2f(x+0.08f, 0.055f); // thinner height
            glVertex2f(x, 0.055f);
        glEnd();
    }
}

void drawAirportComplex(float x, float y, float totalWidth, float totalHeight) {
    // Dimensions
    float leftWidth  = totalWidth * 0.4f;   // smaller building
    float leftHeight = totalHeight * 0.6f;  // 2 floors
    float rightWidth = totalWidth * 0.6f;   // larger building
    float rightHeight= totalHeight;         // 3 floors

    // --- Left Building (2-storied) ---
    glColor3f(0.75f, 0.75f, 0.75f); // base
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + leftWidth, y);
        glVertex2f(x + leftWidth, y + leftHeight);
        glVertex2f(x, y + leftHeight);
    glEnd();

    int leftFloors = 2;
    float lfHeight = leftHeight / leftFloors;
    for(int f=0; f<leftFloors; f++) {
        float fy = y + f * lfHeight;
        // Glass panes
        glColor3f(0.3f, 0.6f, 0.9f);
        glBegin(GL_QUADS);
            glVertex2f(x+0.02f, fy+0.02f);
            glVertex2f(x+leftWidth-0.02f, fy+0.02f);
            glVertex2f(x+leftWidth-0.02f, fy+lfHeight-0.02f);
            glVertex2f(x+0.02f, fy+lfHeight-0.02f);
        glEnd();
        // Separator line
        glColor3f(0.8f, 0.9f, 1.0f);
        glBegin(GL_LINES);
            glVertex2f(x, fy+lfHeight);
            glVertex2f(x+leftWidth, fy+lfHeight);
        glEnd();
    }

    // --- Right Building (3-storied main terminal) ---
    float rx = x + leftWidth + 0.05f; // small gap
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(rx, y);
        glVertex2f(rx + rightWidth, y);
        glVertex2f(rx + rightWidth, y + rightHeight);
        glVertex2f(rx, y + rightHeight);
    glEnd();

    int rightFloors = 3;
    float rfHeight = rightHeight / rightFloors;
    for(int f=0; f<rightFloors; f++) {
        float fy = y + f * rfHeight;
        // Glass panes
        glColor3f(0.3f, 0.6f, 0.9f);
        glBegin(GL_QUADS);
            glVertex2f(rx+0.02f, fy+0.02f);
            glVertex2f(rx+rightWidth-0.02f, fy+0.02f);
            glVertex2f(rx+rightWidth-0.02f, fy+rfHeight-0.02f);
            glVertex2f(rx+0.02f, fy+rfHeight-0.02f);
        glEnd();
        // Separator line
        glColor3f(0.8f, 0.9f, 1.0f);
        glBegin(GL_LINES);
            glVertex2f(rx, fy+rfHeight);
            glVertex2f(rx+rightWidth, fy+rfHeight);
        glEnd();
    }

    // --- Roof (parallelogram effect) ---
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(rx, y+rightHeight);
        glVertex2f(rx+rightWidth, y+rightHeight);
        glVertex2f(rx+rightWidth-0.1f, y+rightHeight+0.08f);
        glVertex2f(rx-0.05f, y+rightHeight+0.08f);
    glEnd();

    // --- Airport Sign (dark navy background, white text) ---
    glColor3f(0.0f, 0.0f, 0.3f); // dark navy
    glBegin(GL_QUADS);
        glVertex2f(rx+rightWidth/4, y+rightHeight+0.08f);
        glVertex2f(rx+3*rightWidth/4, y+rightHeight+0.08f);
        glVertex2f(rx+3*rightWidth/4, y+rightHeight+0.18f);
        glVertex2f(rx+rightWidth/4, y+rightHeight+0.18f);
    glEnd();

    // Text "AIRPORT"
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(rx+rightWidth/3, y+rightHeight+0.11f);
    const char* label = "AIRPORT";
    for(const char* c = label; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky(0.6f, 1.0f, 1.0f); // light blue
    drawGround(0.3f, 0.8f, 0.3f);// greenish
    drawRunway();
    // Taxiway (above runway)
    drawTaxiway();
    drawAirportComplex(-0.5f, 0.2f, 1.0f, 0.4f);


    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Scene 3: Runway Management & Takeoff");

    // Set default background to white (will be overwritten by drawSky)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
