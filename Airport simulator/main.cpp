#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>

//--------------------Sadi---------------------------------
const float PI = 3.14159f;

float windmillRotation = 0.0f;
float mainPlaneX = -1.2f;
float mainPlaneY = -0.45f;
float mainPlanePitch = 0.0f;
float mainPlaneSpeed = 0.005f;

float plane2x=0.4f;
float plane2y=0.10f;
float planePitch2=0.0f;
float planeSpeed=0.005f;
bool plane2HasReset = false;


int currentScene = 2;
bool isDayMode = true;

void drawSky(float c1, float c2, float c3){
    glColor3f(c1, c2, c3);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();
}
void drawGround(float g1, float g2, float g3){
    glColor3f(g1, g2, g3);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f); // bottom
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f,  0.4f); // up to 60% of frame
        glVertex2f(-1.0f,  0.4f);
    glEnd();
}
void drawRunwayLights(float r, float g, float b){
    // Runway edge lights (blue points along both sides)
    glPointSize(6.0f);
    glColor3f(r, g, b); // bluish lights
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
void drawRunway(float r, float g, float b) {
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

    glColor3f(1.0f, 1.0f, 0.7f);
    for(float x=-0.98f; x<=0.98f; x+=0.3f) {
        glBegin(GL_QUADS);
            glVertex2f(x, -0.46f);
            glVertex2f(x+0.15f, -0.46f); // longer horizontally
            glVertex2f(x+0.15f, -0.44f); // thinner height
            glVertex2f(x, -0.44f);
        glEnd();
    }

    // Optional: runway lights if you want them
    drawRunwayLights(r, g, b);
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
void drawBuildingDoor(float centerX, float y) {
    float doorW = 0.18f;
    float doorH = 0.12f;
    float frameT = 0.017f;

    // 1. Thick White Outer Frame
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(centerX - doorW/2.0f - frameT, y); glVertex2f(centerX + doorW/2.0f + frameT, y);
        glVertex2f(centerX + doorW/2.0f + frameT, y + doorH + frameT); glVertex2f(centerX - doorW/2.0f - frameT, y + doorH + frameT);
    glEnd();

    // --- Outline ---
    glColor3f(0.1f, 0.1f, 0.1f); glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(centerX - doorW/2.0f - frameT, y); glVertex2f(centerX + doorW/2.0f + frameT, y);
        glVertex2f(centerX + doorW/2.0f + frameT, y + doorH + frameT); glVertex2f(centerX - doorW/2.0f - frameT, y + doorH + frameT);
    glEnd();

    // 2. Door Glass
    glColor3f(0.12f, 0.45f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(centerX - doorW/2.0f, y); glVertex2f(centerX + doorW/2.0f, y);
        glVertex2f(centerX + doorW/2.0f, y + doorH); glVertex2f(centerX - doorW/2.0f, y + doorH);
    glEnd();

    // 3. Center Mullion
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex2f(centerX, y); glVertex2f(centerX, y + doorH);
    glEnd();
}
void drawBuilding(float x, float y) {
    float wingW = 0.3f;
    float centerW = 0.65f;
    float storeyH = 0.15f;
    float totalW = wingW * 2 + centerW;

    // 1. Main White Frame
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        // Left Wing
        glVertex2f(x, y); glVertex2f(x + wingW, y);
        glVertex2f(x + wingW, y + storeyH + 0.05f); glVertex2f(x, y + storeyH + 0.05f);
        // Center
        glVertex2f(x + wingW, y); glVertex2f(x + wingW + centerW, y);
        glVertex2f(x + wingW + centerW, y + (storeyH * 2) + 0.12f); glVertex2f(x + wingW, y + (storeyH * 2) + 0.12f);
        // Right Wing
        glVertex2f(x + wingW + centerW, y); glVertex2f(x + totalW, y);
        glVertex2f(x + totalW, y + storeyH + 0.05f); glVertex2f(x + wingW + centerW, y + storeyH + 0.05f);
    glEnd();

    // --- Frame Outline ---
    glColor3f(0.1f, 0.1f, 0.1f); glLineWidth(1.0f);
    glBegin(GL_LINES);
        // Tracing the silhouette
        glVertex2f(x, y); glVertex2f(x, y + storeyH + 0.05f);
        glVertex2f(x, y + storeyH + 0.05f); glVertex2f(x + wingW, y + storeyH + 0.05f);
        glVertex2f(x + wingW, y + storeyH + 0.05f); glVertex2f(x + wingW, y + (storeyH * 2) + 0.12f);
        glVertex2f(x + wingW, y + (storeyH * 2) + 0.12f); glVertex2f(x + wingW + centerW, y + (storeyH * 2) + 0.12f);
        glVertex2f(x + wingW + centerW, y + (storeyH * 2) + 0.12f); glVertex2f(x + wingW + centerW, y + storeyH + 0.05f);
        glVertex2f(x + wingW + centerW, y + storeyH + 0.05f); glVertex2f(x + totalW, y + storeyH + 0.05f);
        glVertex2f(x + totalW, y + storeyH + 0.05f); glVertex2f(x + totalW, y);
    glEnd();

    // 2. Glass Panes Logic with Outlines
    auto drawGrid = [&](float startX, float startY, float width, float height, int cols) {
        glColor3f(0.12f, 0.45f, 0.6f);
        glBegin(GL_QUADS);
            glVertex2f(startX + 0.01f, startY + 0.01f); glVertex2f(startX + width - 0.01f, startY + 0.01f);
            glVertex2f(startX + width - 0.01f, startY + height - 0.01f); glVertex2f(startX + 0.01f, startY + height - 0.01f);
        glEnd();

        // Pane Outline
        glColor3f(0.1f, 0.1f, 0.1f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(startX + 0.01f, startY + 0.01f); glVertex2f(startX + width - 0.01f, startY + 0.01f);
            glVertex2f(startX + width - 0.01f, startY + height - 0.01f); glVertex2f(startX + 0.01f, startY + height - 0.01f);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f); // Mullions
        glBegin(GL_LINES);
            for(int i = 1; i < cols; i++) {
                float mx = startX + (width / cols) * i;
                glVertex2f(mx, startY); glVertex2f(mx, startY + height);
            }
            glVertex2f(startX, startY + height/2.0f); glVertex2f(startX + width, startY + height/2.0f);
        glEnd();
    };

    drawGrid(x + 0.01f, y + 0.03f, wingW - 0.02f, storeyH, 3);
    drawGrid(x + wingW + centerW + 0.01f, y + 0.03f, wingW - 0.02f, storeyH, 3);
    for(int i = 0; i < 2; i++) {
        float currentY = y + 0.03f + (i * (storeyH + 0.03f));
        drawGrid(x + wingW + 0.02f, currentY, centerW - 0.04f, storeyH, 4);
    }

    // 3. The Red Sign with Outlines
    float signY = y + (storeyH * 2) + 0.12f;
    float signXStart = x + wingW + 0.1f;
    float signXEnd = x + wingW + centerW - 0.1f;

    glColor3f(1.0f, 1.0f, 1.0f); // White base
    glBegin(GL_QUADS);
        glVertex2f(signXStart - 0.02f, signY); glVertex2f(signXEnd + 0.02f, signY);
        glVertex2f(signXEnd + 0.02f, signY + 0.12f); glVertex2f(signXStart - 0.02f, signY + 0.12f);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f); // White base outline
    glBegin(GL_LINE_LOOP);
        glVertex2f(signXStart - 0.02f, signY); glVertex2f(signXEnd + 0.02f, signY);
        glVertex2f(signXEnd + 0.02f, signY + 0.12f); glVertex2f(signXStart - 0.02f, signY + 0.12f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // Red area
    glBegin(GL_QUADS);
        glVertex2f(signXStart, signY + 0.02f); glVertex2f(signXEnd, signY + 0.02f);
        glVertex2f(signXEnd, signY + 0.1f); glVertex2f(signXStart, signY + 0.1f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x + wingW + centerW/2.0f - 0.11f, signY + 0.05f);
    const char* label = "AIRPORT 01";
    for (const char* c = label; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    drawBuildingDoor(x + wingW + centerW/2.0f, y);
}
void drawCircle(float cx, float cy, float r, int segs = 70){
    glBegin(GL_POLYGON);
    for (int i = 0; i < segs; i++) {
        float a = 2.0f * 3.14159f * i / segs; // PI constant
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
    }
    glEnd();
}
void drawSun(float cx, float cy, float r) {
    // Sun body (golden tone)
    glColor3f(0.92f, 0.82f, 0.0f); // golden yellow
    drawCircle(cx, cy, r);

    // Rays (slightly lighter golden)
    glColor3f(1.0f, 0.9f, 0.3f);
    glLineWidth(2.0f);
    int rays = 24;               // number of rays
    float rayLen = r * 1.4f;     // shorter rays
    for (int i = 0; i < rays; i++) {
        float angle = 2.0f * 3.14159f * i / rays;
        float x1 = cx + r * cosf(angle);
        float y1 = cy + r * sinf(angle);
        float x2 = cx + rayLen * cosf(angle);
        float y2 = cy + rayLen * sinf(angle);

        glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        glEnd();
    }
}
void drawCloud(float cx, float cy, float scale) {
    glColor3f(0.85f, 0.85f, 0.9f);
    drawCircle(cx, cy, 0.12f * scale);
    drawCircle(cx - 0.10f * scale, cy - 0.02f * scale, 0.10f * scale);
    drawCircle(cx + 0.10f * scale, cy - 0.02f * scale, 0.10f * scale);
    drawCircle(cx, cy + 0.08f * scale, 0.08f * scale);
}
void drawTree(float x, float y) {
    // 1. Trunk (Dark Grey/Brown)
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(x - 0.01f, y);
        glVertex2f(x + 0.01f, y);
        glVertex2f(x + 0.01f, y + 0.06f);
        glVertex2f(x - 0.01f, y + 0.06f);
    glEnd();

    // 2. Foliage (Vibrant Green)
    glColor3f(0.45f, 0.75f, 0.25f);
    float radiusX = 0.035f;
    float radiusY = 0.06f;
    float centerY = y + 0.12f;

    // Drawing a vertical oval using a triangle fan
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, centerY); // Center of the oval
        for(int i = 0; i <= 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(x + cos(angle) * radiusX, centerY + sin(angle) * radiusY);
        }
    glEnd();

    // 3. Thin Outline for the Foliage (Dark Green)
    glColor3f(0.25f, 0.45f, 0.15f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        for(int i = 0; i <= 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            glVertex2f(x + cos(angle) * radiusX, centerY + sin(angle) * radiusY);
        }
    glEnd();
}

void drawMoon(float cx, float cy, float r) {
    // 1. Bright Moon Body
    glColor3f(0.92f, 0.92f, 0.85f);
    drawCircle(cx, cy, r, 70);

    // 2. Crescent Shadow Overlay
    glColor3f(0.05f, 0.05f, 0.15f);
    float shadowOffsetX = r * 0.4f;
    float shadowOffsetY = r * 0.2f;

    drawCircle(cx + shadowOffsetX, cy + shadowOffsetY, r, 70);
}

void drawWindmill(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    // 1. Concrete Base Plate
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
        glVertex2f(-0.06f, 0.0f); glVertex2f(0.06f, 0.0f);
        glVertex2f(0.04f, 0.02f); glVertex2f(-0.04f, 0.02f);
    glEnd();

    // 2. Realistic Shaded Tower
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 10; i++) {
        float t = (float)i / 10.0f;
        float angle = t * 3.14159f;
        float offset = cos(angle) * 0.025f;
        float brightness = 0.75f + (sin(angle) * 0.15f);
        glColor3f(brightness, brightness, brightness + 0.03f);
        glVertex2f(offset, 0.02f);
        glVertex2f(offset * 0.7f, 0.55f);
    }
    glEnd();

    // 3. Streamlined Nacelle
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.05f, 0.55f); glVertex2f(0.03f, 0.55f);
        glVertex2f(0.05f, 0.58f); glVertex2f(0.03f, 0.61f);
        glVertex2f(-0.05f, 0.61f);
    glEnd();

    // 4. Static Blades (Wider and Smaller)
    glTranslatef(0.04f, 0.58f, 0.0f);
    float bladeLength = 0.28f; // Reduced from 0.38f
    float bladeWidth = 0.025f; // Increased from 0.015f

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(i * 120.0f, 0.0f, 0.0f, 1.0f);

        // Tapered Blade Shape
        glColor3f(0.95f, 0.95f, 0.95f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, -bladeWidth);   // Wider Base bottom
            glVertex2f(0.0f, bladeWidth);    // Wider Base top
            glVertex2f(bladeLength, 0.002f);  // Sharp tip top
            glVertex2f(bladeLength, -0.002f); // Sharp tip bottom
        glEnd();

        // Detail Outline for each blade
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(0.0f, -bladeWidth);
            glVertex2f(0.0f, bladeWidth);
            glVertex2f(bladeLength, 0.002f);
            glVertex2f(bladeLength, -0.002f);
        glEnd();
        glPopMatrix();
    }

    glPopMatrix();
}
void drawTower(float x, float y) {
    float baseW = 0.15f;   // narrower base width
    float baseH = 0.02f;   // thinner base height
    float shaftW = 0.12f;
    float ledgeW = 0.20f;
    float cabinW_top = 0.27f;
    float cabinW_bottom = 0.17f;

    glLineWidth(0.5f); // thinner outlines everywhere

    // 1. Pedestal Base
    glColor3f(0.85f, 0.85f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(x - baseW/2.0f, y);
        glVertex2f(x + baseW/2.0f, y);
        glVertex2f(x + baseW/2.0f, y + baseH);
        glVertex2f(x - baseW/2.0f, y + baseH);
    glEnd();

    glColor3f(0.5f, 0.5f, 0.5);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x - baseW/2.0f, y);
        glVertex2f(x + baseW/2.0f, y);
        glVertex2f(x + baseW/2.0f, y + baseH);
        glVertex2f(x - baseW/2.0f, y + baseH);
    glEnd();

    // 2. Shaft Storeys
    float sectionsY[] = {y + baseH, y + 0.12f, y + 0.32f, y + 0.37f, y + 0.52f};
    float sectionsW[] = {baseW/2.2f, shaftW/2.0f, ledgeW/2.0f, shaftW/2.0f};

    for(int i = 0; i < 4; i++) {
        if(i % 2 == 0) glColor3f(0.75f, 0.75f, 0.75f);
        else glColor3f(0.85f, 0.85f, 0.85f);

        glBegin(GL_QUADS);
            glVertex2f(x - sectionsW[i], sectionsY[i]);
            glVertex2f(x + sectionsW[i], sectionsY[i]);
            glVertex2f(x + sectionsW[i], sectionsY[i+1]);
            glVertex2f(x - sectionsW[i], sectionsY[i+1]);
        glEnd();

        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x - sectionsW[i], sectionsY[i]);
            glVertex2f(x + sectionsW[i], sectionsY[i]);
            glVertex2f(x + sectionsW[i], sectionsY[i+1]);
            glVertex2f(x - sectionsW[i], sectionsY[i+1]);
        glEnd();
    }

    // 2.5. Middle Connector (between shaft and cabin)
    float connectorY1 = y + 0.52f;
    float connectorY2 = y + 0.55f;
    float connectorW  = 0.18f;

    glColor3f(0.7f, 0.75f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(x - connectorW/2.0f, connectorY1);
        glVertex2f(x + connectorW/2.0f, connectorY1);
        glVertex2f(x + connectorW/2.0f, connectorY2);
        glVertex2f(x - connectorW/2.0f, connectorY2);
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x - connectorW/2.0f, connectorY1);
        glVertex2f(x + connectorW/2.0f, connectorY1);
        glVertex2f(x + connectorW/2.0f, connectorY2);
        glVertex2f(x - connectorW/2.0f, connectorY2);
    glEnd();

    // 3. Observation Deck
    float cabinY = y + 0.55f;
    float cabinH = 0.18f;

    glColor3f(0.12f, 0.45f, 0.78f);
    glBegin(GL_QUADS);
        glVertex2f(x - cabinW_bottom/2.0f, cabinY);
        glVertex2f(x + cabinW_bottom/2.0f, cabinY);
        glVertex2f(x + cabinW_top/2.0f, cabinY + cabinH);
        glVertex2f(x - cabinW_top/2.0f, cabinY + cabinH);
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x - cabinW_bottom/2.0f, cabinY);
        glVertex2f(x + cabinW_bottom/2.0f, cabinY);
        glVertex2f(x + cabinW_top/2.0f, cabinY + cabinH);
        glVertex2f(x - cabinW_top/2.0f, cabinY + cabinH);
    glEnd();

    // 4. Grid Lines
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
        for(float i = -3; i <= 3; i++) {
            glVertex2f(x + (i * 0.03f), cabinY);
            glVertex2f(x + (i * 0.045f), cabinY + cabinH);
        }
    glEnd();

    // 5. Roof Cap
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_POLYGON);
        glVertex2f(x - 0.14f, cabinY + cabinH);
        glVertex2f(x + 0.14f, cabinY + cabinH);
        glVertex2f(x + 0.08f, cabinY + cabinH + 0.06f);
        glVertex2f(x - 0.08f, cabinY + cabinH + 0.06f);
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x - 0.14f, cabinY + cabinH);
        glVertex2f(x + 0.14f, cabinY + cabinH);
        glVertex2f(x + 0.08f, cabinY + cabinH + 0.06f);
        glVertex2f(x - 0.08f, cabinY + cabinH + 0.06f);
    glEnd();

    // 6. Antenna
    glBegin(GL_LINES);
        glVertex2f(x, cabinY + cabinH + 0.06f);
        glVertex2f(x, cabinY + cabinH + 0.14f);
    glEnd();
}

void drawFin(float x, float y, float scale, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y-.015, 0.0f);
    glScalef(scale, scale, 1.0f);

    // 1. FILL: Solid Color
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex2f(0.0f, 0.0f);          // Bottom front (Joint)
        glVertex2f(-0.25f, -0.05f);      // Bottom back (Connects to tail tip)
        glVertex2f(-0.35f, 0.23f);       // Top back corner
        glVertex2f(-0.12f, 0.23f);       // Top front (Flat top edge)
    glEnd();

    // 2. OUTLINE: Structural Definition
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-0.25f, -0.05f);
        glVertex2f(-0.35f, 0.23f);
        glVertex2f(-0.12f, 0.23f);
    glEnd();

    glPopMatrix();
}

void drawPlaneWheel(float strutX, float strutTopY, float strutLength, float wheelRadius) {
    float strutBottomY = strutTopY - strutLength;

    // Metal Landing Gear Strut
    glColor3f(0.8f, 0.8f, 0.82f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(strutX, strutTopY);
        glVertex2f(strutX, strutBottomY);
    glEnd();

    // Tire Outer Rubber (Black)
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(strutX, strutBottomY); // Center of the tire
        for (int i = 0; i <= 20; i++) {
            float angle = i * 2.0f * 3.14159f / 20.0f;
            glVertex2f(strutX + cos(angle) * wheelRadius, strutBottomY + sin(angle) * wheelRadius);
        }
    glEnd();

    // Inside Wheel Hub Rim (Light Grey)
    glColor3f(0.8f, 0.8f, 0.82f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(strutX, strutBottomY);
        for (int i = 0; i <= 20; i++) {
            float angle = i * 2.0f * 3.14159f / 20.0f;
            glVertex2f(strutX + cos(angle) * (wheelRadius * 0.45f), strutBottomY + sin(angle) * (wheelRadius * 0.45f));
        }
    glEnd();

    // Fine rim detail outline
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(0.8f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= 20; i++) {
            float angle = i * 2.0f * 3.14159f / 20.0f;
            glVertex2f(strutX + cos(angle) * wheelRadius, strutBottomY + sin(angle) * wheelRadius);
        }
    glEnd();
}

void drawPlaneWindow(float cx, float cy, float radiusX, float radiusY) {
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 20; i++) {
            float angle = i * 2.0f * 3.14159f / 20.0f;
            glVertex2f(cx + cos(angle) * radiusX, cy + sin(angle) * radiusY);
        }
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= 20; i++) {
            float angle = i * 2.0f * 3.14159f / 20.0f;
            glVertex2f(cx + cos(angle) * radiusX, cy + sin(angle) * radiusY);
        }
    glEnd();
}

void drawPlane(float x, float y, float scale, float finR, float finG, float finB) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    float noseW = 0.22f;
    float bodyW = 0.45f;
    float tailW = (noseW + bodyW) * 0.25f;
    float bodyH = 0.09f;

    float tailEnd = 0.0f;
    float bodyStart = tailW;
    float noseStart = tailW + bodyW;

    // Right Wing
    float rightWingRootBackX = bodyStart + bodyW * 0.43f;
    float rightWingRootFrontX = rightWingRootBackX + 0.17f;
    float rightWingTipBackX = rightWingRootBackX - 0.12f;
    float rightWingTipFrontX = rightWingTipBackX + 0.04f;
    float rightWingTipY = bodyH * 2.3f;

    glColor3f(finR, finG, finB); // Using input color
    glBegin(GL_QUADS);
        glVertex2f(rightWingRootBackX, bodyH * 0.4f);
        glVertex2f(rightWingRootFrontX, bodyH * 0.4f);
        glVertex2f(rightWingTipFrontX, rightWingTipY);
        glVertex2f(rightWingTipBackX, rightWingTipY);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(rightWingRootBackX, bodyH * 0.4f);
        glVertex2f(rightWingRootFrontX, bodyH * 0.4f);
        glVertex2f(rightWingTipFrontX, rightWingTipY);
        glVertex2f(rightWingTipBackX, rightWingTipY);
    glEnd();

    // Wheels
    drawPlaneWheel(noseStart + noseW * 0.45f, -bodyH * 0.4f, 0.10f, 0.022f);
    drawPlaneWheel(bodyStart + bodyW * 0.20f, -bodyH, 0.05f, 0.026f);

    // Fin
    drawFin(bodyStart, bodyH, 0.5f, finR, finG, finB); // Using input color

    // Fuselage
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(tailEnd, 0.04f);
        glVertex2f(bodyStart, bodyH);
        glVertex2f(noseStart, bodyH);
        glVertex2f(noseStart, -bodyH);
        glVertex2f(bodyStart, -bodyH);
        glVertex2f(tailEnd, 0.02f);
    glEnd();

    glBegin(GL_POLYGON);
        float cx[] = {noseStart, noseStart + noseW, noseStart + noseW, noseStart};
        float cy[] = {bodyH, bodyH * 0.4f, -bodyH * 0.3f, -bodyH};
        for (int i = 0; i <= 20; i++) {
            float t = (float)i / 20.0f;
            float bx = pow(1-t, 3)*cx[0] + 3*t*pow(1-t, 2)*cx[1] + 3*pow(t, 2)*(1-t)*cx[2] + pow(t, 3)*cx[3];
            float by = pow(1-t, 3)*cy[0] + 3*t*pow(1-t, 2)*cy[1] + 3*pow(t, 2)*(1-t)*cy[2] + pow(t, 3)*cy[3];
            glVertex2f(bx, by);
        }
    glEnd();

    // Fuselage Outline
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(tailEnd, 0.04f);
        glVertex2f(bodyStart, bodyH);
        glVertex2f(noseStart, bodyH);
        for (int i = 0; i <= 20; i++) {
            float t = (float)i / 20.0f;
            glVertex2f(pow(1-t, 3)*cx[0] + 3*t*pow(1-t, 2)*cx[1] + 3*pow(t, 2)*(1-t)*cx[2] + pow(t, 3)*cx[3],
                       pow(1-t, 3)*cy[0] + 3*t*pow(1-t, 2)*cy[1] + 3*pow(t, 2)*(1-t)*cy[2] + pow(t, 3)*cy[3]);
        }
        glVertex2f(noseStart, -bodyH);
        glVertex2f(bodyStart, -bodyH);
        glVertex2f(tailEnd, 0.02f);
    glEnd();

    // Cockpit Window
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(noseStart + noseW * 0.25f, bodyH * 0.0f);
        glVertex2f(noseStart + noseW * 0.65f, bodyH * 0.0f);
        glVertex2f(noseStart + noseW * 0.50f, bodyH * 0.55f);
        glVertex2f(noseStart + noseW * 0.20f, bodyH * 0.70f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(noseStart + noseW * 0.25f, bodyH * 0.0f);
        glVertex2f(noseStart + noseW * 0.65f, bodyH * 0.0f);
        glVertex2f(noseStart + noseW * 0.50f, bodyH * 0.55f);
        glVertex2f(noseStart + noseW * 0.20f, bodyH * 0.70f);
    glEnd();

    // Passenger Door
    float doorX = noseStart - 0.05f;
    float doorW = 0.028f;
    float doorBottom = -bodyH * 0.50f;
    float doorTop = bodyH * 0.50f;

    glColor3f(0.4f, 0.4f, 0.4f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(doorX, doorBottom);
        glVertex2f(doorX + doorW, doorBottom);
        glVertex2f(doorX + doorW, doorTop);
        glVertex2f(doorX, doorTop);
    glEnd();

    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
        glVertex2f(doorX + doorW * 0.3f, bodyH * 0.10f);
        glVertex2f(doorX + doorW * 0.7f, bodyH * 0.10f);
        glVertex2f(doorX + doorW * 0.7f, bodyH * 0.35f);
        glVertex2f(doorX + doorW * 0.3f, bodyH * 0.35f);
    glEnd();

    // Passenger Windows
    float windowStartY = bodyH * 0.20f;
    float winRadiusX = 0.012f;
    float winRadiusY = 0.016f;
    int totalWindows = 6;

    for (int i = 0; i < totalWindows; i++) {
        float winX = doorX - 0.06f - (i * 0.055f);
        if (winX > bodyStart + 0.02f) {
            drawPlaneWindow(winX, windowStartY, winRadiusX, winRadiusY);
        }
    }

    // Left Wing Engine
    float wingRootBackX = bodyStart + bodyW * 0.42f;
    float wingRootFrontX = wingRootBackX + 0.15f;
    float wingTipBackX = wingRootBackX - 0.12f;
    float wingTipFrontX = wingTipBackX + 0.05f;
    float wingTipY = -bodyH * 1.5f;
    float wingRootY = -bodyH * 0.2f;

    float engX = wingRootBackX + 0.02f;
    float engW = 0.08f;
    float engH = bodyH * 0.60f;
    float engY = wingRootY - bodyH * 0.75f;

    glColor3f(0.85f, 0.85f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(engX + 0.02f, engY + engH);
        glVertex2f(engX + 0.06f, engY + engH);
        glVertex2f(engX + 0.05f, wingRootY - bodyH * 0.25f);
        glVertex2f(engX + 0.03f, wingRootY - bodyH * 0.25f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(engX, engY);
        glVertex2f(engX + engW, engY);
        glVertex2f(engX + engW, engY + engH);
        glVertex2f(engX, engY + engH);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(engX, engY);
        glVertex2f(engX + engW, engY);
        glVertex2f(engX + engW, engY + engH);
        glVertex2f(engX, engY + engH);
    glEnd();

    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
        glVertex2f(engX + engW, engY);
        glVertex2f(engX + engW + 0.012f, engY + engH * 0.08f);
        glVertex2f(engX + engW + 0.012f, engY + engH * 0.92f);
        glVertex2f(engX + engW, engY + engH);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(engX + engW, engY);
        glVertex2f(engX + engW + 0.012f, engY + engH * 0.08f);
        glVertex2f(engX + engW + 0.012f, engY + engH * 0.92f);
        glVertex2f(engX + engW, engY + engH);
    glEnd();

    // Left Wing
    glColor3f(finR, finG, finB); // Using input color
    glBegin(GL_QUADS);
        glVertex2f(wingRootBackX, wingRootY);
        glVertex2f(wingRootFrontX, wingRootY);
        glVertex2f(wingTipFrontX, wingTipY);
        glVertex2f(wingTipBackX, wingTipY);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(wingRootBackX, wingRootY);
        glVertex2f(wingRootFrontX, wingRootY);
        glVertex2f(wingTipFrontX, wingTipY);
        glVertex2f(wingTipBackX, wingTipY);
    glEnd();

    glPopMatrix();
}

void updateScene2(int value) {
    windmillRotation += 2.0f;
    if (windmillRotation > 360.0f) {
        windmillRotation -= 360.0f;
    }

    // Stage A: Acceleration roll on the ground
    if (mainPlaneX < -0.1f) {
        mainPlaneX += 0.01f; // High-speed ground roll
    }
    // Stage B: Rotation (Nose wheels lift, plane tilts up)
    else if (mainPlaneX >= -0.1f && mainPlanePitch < 15.0f) {
        mainPlaneX += 0.010f;
        mainPlanePitch += 1.2f;
    }
    // Stage C: Climb out
    else {
        mainPlaneX += 0.014f;
        mainPlaneY += 0.006f;
    }

    if (mainPlaneX > 1.3f || mainPlaneY > 1.2f) {
        mainPlaneX = -1.2f;
        mainPlaneY = -0.45f;
        mainPlanePitch = 0.0f;
    }
    plane2x += 0.005f; // Simply moves forward horizontally using its speed factor

    if (plane2x > 1.3f) {
        plane2x = -1.2f; // Resets to the far left to cross the screen again
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateScene2, 0);
}
//-------------------SadiEnd--------------------
//secene1
//scene1end
//scene3
//scene3End
//scene4
//Scene4End
void handleKeyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '4') {
        currentScene = key - '0';
    }
    else if (key == 'n' || key == 'N') {
        isDayMode = false;
    }
    else if (key == 'd' || key == 'D') {
        isDayMode = true;
    }

    glutPostRedisplay();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    switch(currentScene) {
        case 1:
            if (isDayMode) {
                    //
            }
            else {
                //night mode
            }

        case 2:
            if(isDayMode)
            {
                drawSky(0.6f, 1.0f, 1.0f); // light blue
                drawGround(0.3f, 0.8f, 0.3f);
                drawRunway(0.4f, 0.7f, 1.0f);
                drawTaxiway();

                // Golden sun at top-right
                drawSun(0.2f, 0.8f, 0.08f);

                drawCloud(-0.65f, 0.8f, 0.8f);
                drawCloud(-0.2f, 0.8f, 0.6f);
                drawCloud(0.1f, 0.7f, 0.5f);
                drawCloud(0.55f, 0.65f, 0.8f);

                // Windmill at top-left edge of ground
                drawWindmill(-0.95f, 0.4f, 0.3f);
                drawWindmill(-0.87f, 0.4f, 0.22f);
                drawWindmill(-0.8f, 0.4f, 0.3f);
                drawWindmill(-0.75f, 0.4f, 0.25f);

                // Draw a building at position (-0.5, -0.5)
                drawBuilding(-0.9f, 0.15f);
                // Tower near building
                drawTower(0.75f, 0.15f);

                drawTree(-0.95f, 0.15f);
                drawTree(0.55f, 0.15f);
                drawTree(0.8f, 0.15f);
                drawTree(0.85f, 0.15f);

                glPushMatrix();
                    glTranslatef(mainPlaneX, mainPlaneY, 0.0f);
                    glRotatef(mainPlanePitch, 0.0f, 0.0f, 1.0f);
                    drawPlane(0.0f, 0.0f, 0.8f, 0.15f, 0.25f, 0.45f);
                glPopMatrix();

                // Parked Planes on the Taxiway
                drawPlane(-1.0f, 0.10f, 0.45f, 0.80f, 0.10f, 0.10f);
                drawPlane(-0.6f, 0.10f, 0.45f, 0.15f, 0.60f, 0.25f);
                glPushMatrix();
                glTranslatef(plane2x, plane2y, 0.0f);
                    drawPlane(0.0f, 0.0f, 0.45f, 0.90f, 0.45f, 0.05f);
                glPopMatrix();
            }
            else
            {
                // Night Mode Colors
                drawSky(0.05f, 0.05f, 0.15f);      // Deep dark navy sky
                drawGround(0.1f, 0.3f, 0.1f);      // Dim, dark forest green grass
                drawRunway(1.0f, 1.0f, 1.0f);
                drawTaxiway();

                drawMoon(0.2f, 0.8f, 0.08f);

                // Clouds tinted slightly darker for night ambient matching
                drawCloud(-0.65f, 0.8f, 0.8f);
                drawCloud(-0.2f, 0.8f, 0.6f);
                drawCloud(0.1f, 0.7f, 0.5f);
                drawCloud(0.55f, 0.65f, 0.8f);

                // Windmills
                drawWindmill(-0.95f, 0.4f, 0.3f);
                drawWindmill(-0.87f, 0.4f, 0.22f);
                drawWindmill(-0.8f, 0.4f, 0.3f);
                drawWindmill(-0.75f, 0.4f, 0.25f);

                // Infrastructure structures
                drawBuilding(-0.9f, 0.15f);
                drawTower(0.75f, 0.15f);

                // Trees
                drawTree(-0.95f, 0.15f);
                drawTree(0.55f, 0.15f);
                drawTree(0.8f, 0.15f);
                drawTree(0.85f, 0.15f);

                // Main Taking-off Plane
                glPushMatrix();
                    glTranslatef(mainPlaneX, mainPlaneY, 0.0f);
                    glRotatef(mainPlanePitch, 0.0f, 0.0f, 1.0f);
                    drawPlane(0.0f, 0.0f, 0.8f, 0.15f, 0.25f, 0.45f);
                glPopMatrix();

                // Parked Planes on the Taxiway (Colors dimmed for night lighting)
                drawPlane(-1.0f, 0.10f, 0.45f, 0.50f, 0.05f, 0.05f); // Dimmed Dark Red
                drawPlane(-0.6f, 0.10f, 0.45f, 0.08f, 0.35f, 0.15f); // Dimmed Dark Green
                //glPushMatrix();
                    //glScalef(-1.0f,-1.0f,-1.0f);
                //glPopMatrix();
                glPushMatrix();
                glTranslatef(plane2x, plane2y, 0.0f);
                    glScalef(-1.0f, 1.0f, -1.0f);
                    drawPlane(0.0f, 0.0f, 0.45f, 0.90f, 0.45f, 0.05f);
                glPopMatrix();


            }

        case 3:
            if(isDayMode)
            {

            }
            else
            {
                //Scene
            }

        case 4:
            if(isDayMode)
            {
                //Scene
            }
            else
            {
                //Scene
            }

        default:
            break;
        }
    glutSwapBuffers();

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Airport Project");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


    glutKeyboardFunc(handleKeyboard);
    glutDisplayFunc(display);
    glutTimerFunc(16, updateScene2, 0);

    glutMainLoop();
    return 0;
}
