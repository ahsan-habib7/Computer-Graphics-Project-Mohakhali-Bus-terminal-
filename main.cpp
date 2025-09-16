#include <GL/glut.h>
#include <windows.h>
#include<math.h>
#include <stdlib.h>
#include<iostream>
#define MAX_DROPS 1000
using namespace std;

enum TimeOfDay { DAY, EVENING, NIGHT, CLOUDY, RAIN};
TimeOfDay current_time = DAY;

float dropX[MAX_DROPS];
float dropY[MAX_DROPS];
int totalDrops = 0;
bool isRaining = false;

const int NUM_SNOW = 400;
float snowX[NUM_SNOW];
float snowY[NUM_SNOW];
float snowSpeed = 0.15f;
int frameCounter = 0;

enum EnvironmentMode { CLEAR, SNOW, FOGGY};
EnvironmentMode current_weather = CLEAR;

float plane_400_x = 0.0f, plane_400_y = 0.0f;
float truck1_400_x = 0.0f, truck1_400_speed = 0.4f;
float truck2_400_x = 0.0f, truck2_400_speed = 0.4f;
float train_400_x = 0.0f, train_400_speed = 0.4f;
float car1_400_x = 0.0f, car1_400_speed = 0.3f;
float car2_400_x = 0.0f, car2_400_speed = 0.35f;
float bus1_400_x = 0.0f, bus1_400_speed = 0.6f;
float busBRTC_400_x = 0.0f, busBRTC_400_speed = 0.4f;
float cng_400X = 0.0f, cng_400Speed = 0.5f;
float srplus_bus_400_x = 0.0f;
bool busBRTC_400_stop = false;

float _angle1 = 0.0f;
static float busWheelAngle_400 = 0.0f;
float BRTCWheelAngle_400 = 0.0f;
float BRTCWheelSpeed_400 = 2.0f;
float trainWheelAngle_400 = 0.0f;
float truckWheelAngle_400 = 0.0f;
float cngWheelAngle_400 = 0.0f;
bool planeMoving_400 = false;
bool trainMoving_400 = false;

void addRainDrop_400() {
    if (totalDrops < MAX_DROPS) {
        dropX[totalDrops] = rand() % 160;
        dropY[totalDrops] = 120.0f;
        totalDrops++;
    }
}

void updateRainDrop_400(int value) {
    if (isRaining) {
        for (int i = 0; i < totalDrops; i++) {
            dropY[i] -= 1.5f;

            if (dropY[i] < 0) {
                dropX[i] = rand() % 160;
                dropY[i] = 120.0f;
            }
        }
        for (int i = 0; i < 5; i++) {
            addRainDrop_400();
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateRainDrop_400, 0);
}

void drawRaindrops_400() {
    if (!isRaining) return;

    glColor3f(0.2f, 0.4f, 1.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < totalDrops; i++) {
        glVertex2f(dropX[i], dropY[i]);
        glVertex2f(dropX[i], dropY[i] - 3.0f);
    }
    glEnd();
}

void drawFogOverlay_400() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.9f, 0.9f, 0.9f, 0.4f);

    glBegin(GL_QUADS);
        glVertex2f(-200, -200);
        glVertex2f( 200, -200);
        glVertex2f( 200,  200);
        glVertex2f(-200,  200);
    glEnd();

    glDisable(GL_BLEND);
}

void drawSnow_400() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);

    for (int i = 0; i < NUM_SNOW; i++) {
        if (frameCounter == 0) {
            snowX[i] = rand() % 160;
            snowY[i] = rand() % 120;
        }
        snowY[i] -= snowSpeed;
        if (snowY[i] < 0) {
            snowY[i] = 120;
            snowX[i] = rand() % 160;
        }
        glVertex2f(snowX[i], snowY[i]);
    }
    glEnd();
    frameCounter++;
}

void circle_400(float radius, float x, float y, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        float angle = i * (M_PI / 180.0f);
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

void semicircle_400(float cx, float cy, float radius, float red, float green, float blue){
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    float pi = 3.14159265359;
    for (int i = 0; i <= 180; i++) {
        float angle = i * pi / 180.0;
        float x = cos(angle) * radius + cx;
        float y = sin(angle) * radius + cy;
        glVertex2f(x, y);
    }
    glEnd();
}

void semicircle_arc_400(float cx, float cy, float radius, int startingAngle, int endingAngle, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    float pi = 3.14159265359;

    for (int i = startingAngle; i <= endingAngle; i++) {
        float angle = i * pi / 180.0;
        float x = cos(angle) * radius + cx;
        float y = sin(angle) * radius + cy;
        glVertex2f(x, y);
    }

    glEnd();
}

void renderText_400(float x, float y, const char* text, int size) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {

        if(size == 12)
        {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
        if(size == 10)
        {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
        }
        if(size == 18)
        {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        if(size == 11)
        {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
        }
    }
}

void BusWheel_400(float radius, float xc, float yc, float angle){
    glBegin(GL_POLYGON);
    for (int i = 0; i < 200; i++) {
        glColor3ub(0, 0, 0);
        float A = (i * 2 * M_PI) / 200;
        float x = radius * cos(A);
        float y = radius * sin(A);
        glVertex2f(x + xc, y + yc);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 200; i++) {
        glColor3ub(255, 255, 255);
        float A = (i * 2 * M_PI) / 200;
        float x = (radius - 0.35) * cos(A);
        float y = (radius - 0.35) * sin(A);
        glVertex2f(x + xc, y + yc);
    }
    glEnd();

    glPushMatrix();
    glTranslatef(xc, yc, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-xc, -yc, 0.0f);

    glColor3ub(255, 0, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(xc + 0.10, yc);
    glEnd();

    glPopMatrix();
}

void BRTCWheel_400(float xc, float yc, float angle) {
    float pi = 3.1416f;

    glColor3ub(30, 30, 30);//Outer tire
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i <= 200; i++) {
        float A = (2 * pi * i) / 200;
        glVertex2f(xc + 1.2f * cos(A), yc + 1.2f * sin(A));
    }
    glEnd();

    glColor3ub(255, 200, 200);// Rim
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i <= 200; i++) {
        float A = (2 * pi * i) / 200;
        glVertex2f(xc + 0.58f * cos(A), yc + 0.58f * sin(A));
    }
    glEnd();

    glColor3ub(150, 150, 150);// Inner hub
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i <= 200; i++) {
        float A = (2 * pi * i) / 200;
        glVertex2f(xc + 0.15f * cos(A), yc + 0.15f * sin(A));
    }
    glEnd();

    glPushMatrix();// Bolt holes (rotating)
    glTranslatef(xc, yc, 0.0f);
    glRotatef(-angle, 0.0f, 0.0f, 1.0f); // animate rotation
    for (int j = 0; j < 8; j++) {
        float theta = j * (2 * pi / 8);
        float bx = 0.8f * cos(theta);
        float by = 0.8f * sin(theta);

        glColor3ub(255, 0, 0);
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= 20; i++) {
            float A = (2 * pi * i) / 20;
            glVertex2f(bx + 0.15f * cos(A), by + 0.15f * sin(A));
        }
        glEnd();
    }
    glPopMatrix();

    // Center cap
    glColor3ub(100, 100, 100);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i <= 200; i++) {
        float A = (2 * pi * i) / 200;
        glVertex2f(xc + 0.3f * cos(A), yc + 0.3f * sin(A));
    }
    glEnd();
}

void TrainWheel_400(float radius, float xc, float yc, float angle) {
    float pi = 3.1416f;
    // === Outer Tire ===
    glColor3ub(30, 30, 30); // Dark gray/black
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float A = (2 * pi * i) / 100;
        glVertex2f(xc + radius * cos(A), yc + radius * sin(A));
    }
    glEnd();

    // === Inner Plate ===
    glColor3ub(180, 180, 180); // Metallic silver
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float A = (2 * pi * i) / 100;
        glVertex2f(xc + (radius * 0.75f) * cos(A), yc + (radius * 0.75f) * sin(A));
    }
    glEnd();
    // === Spokes (rotating with wheel) ===
    glPushMatrix();
    glTranslatef(xc, yc, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f); // Spokes rotate with wheel

    glColor3ub(1, 1, 1);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 6; i++) { // 6 spokes
        float A = (2 * pi * i) / 6;
        glVertex2f(0.0f, 0.0f);
        glVertex2f(radius * 0.7f * cos(A), radius * 0.7f * sin(A));
    }
    glEnd();
    glPopMatrix();
    // === Center Hub ===
    glColor3ub(255, 60, 60);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 50; i++) {
        float A = (2 * pi * i) / 50;
        glVertex2f(xc + (radius * 0.2f) * cos(A), yc + (radius * 0.2f) * sin(A));
    }
    glEnd();
}

void Truck1Wheel(float xc, float yc, float radius, float angle) {
    float pi = 3.1416f;

    glColor3ub(25, 25, 25);
    glBegin(GL_TRIANGLE_FAN);// Outer Tire
    for (int i = 0; i <= 100; i++) {
        float A = (2 * pi * i) / 100;
        glVertex2f(xc + radius * cos(A), yc + radius * sin(A));
    }
    glEnd();

    glColor3ub(60, 60, 60);
    glBegin(GL_LINES);
    for (int i = 0; i < 20; i++) {
        float A = (2 * pi * i) / 20;
        glVertex2f(xc + radius * cos(A), yc + radius * sin(A));
        glVertex2f(xc + (radius * 0.85f) * cos(A), yc + (radius * 0.85f) * sin(A));
    }
    glEnd();

    glColor3ub(190, 190, 190);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float A = (2 * pi * i) / 100;
        glVertex2f(xc + (radius * 0.70f) * cos(A), yc + (radius * 0.70f) * sin(A));
    }
    glEnd();

    glColor3ub(100, 100, 100);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float A = (2 * pi * i) / 100;
        glVertex2f(xc + (radius * 0.40f) * cos(A), yc + (radius * 0.40f) * sin(A));
    }
    glEnd();

    // Central Hub with bolts
    glColor3ub(60, 60, 60);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 40; i++) {
        float A = (2 * pi * i) / 40;
        glVertex2f(xc + (radius * 0.20f) * cos(A), yc + (radius * 0.20f) * sin(A));
    }
    glEnd();

    glColor3ub(230, 230, 230);
    for (int i = 0; i < 6; i++) {
        float A = (2 * pi * i) / 6;
        float bx = xc + (radius * 0.32f) * cos(A);
        float by = yc + (radius * 0.32f) * sin(A);

        glBegin(GL_TRIANGLE_FAN);
        for (int j = 0; j <= 20; j++) {
            float B = (2 * pi * j) / 20;
            glVertex2f(bx + 0.05f * cos(B), by + 0.05f * sin(B));
        }
        glEnd();
    }

    glPushMatrix();//rotating wheel
    glTranslatef(xc, yc, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    for (int i = 0; i < 6; i++) {
        float A = (2 * pi * i) / 6;
        glVertex2f(0.0f, 0.0f);
        glVertex2f(radius * 0.65f * cos(A), radius * 0.65f * sin(A));
    }
    glEnd();

    glPopMatrix();
}

void Truck2Wheel_400(float xc, float yc, float radius, float angle) {
    float pi = 3.1416f;
    // === Outer black tire ===
    glColor3ub(20, 20, 20);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float A = (2 * pi * i) / 100;
        glVertex2f(xc + radius * cos(A), yc + radius * sin(A));
    }
    glEnd();

    // === Inner gray rim ===
    glColor3ub(180, 180, 180);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float A = (2 * pi * i) / 100;
        glVertex2f(xc + (radius * 0.65f) * cos(A), yc + (radius * 0.65f) * sin(A));
    }
    glEnd();

    // === Central hub ===
    glColor3ub(100, 100, 100);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 40; i++) {
        float A = (2 * pi * i) / 40;
        glVertex2f(xc + (radius * 0.25f) * cos(A), yc + (radius * 0.25f) * sin(A));
    }
    glEnd();

    // === Indicator spokes (rotating) ===
    glPushMatrix();
    glTranslatef(xc, yc, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    for (int i = 0; i < 4; i++) {
        float A = (2 * pi * i) / 4;
        glVertex2f(0.0f, 0.0f);
        glVertex2f(radius * 0.65f * cos(A), radius * 0.65f * sin(A));
    }
    glEnd();

    // === Orbiting white dot (replaces red) ===
    float bx = radius * 0.55f * cos(angle * pi / 180.0f);
    float by = radius * 0.55f * sin(angle * pi / 180.0f);

    glColor3ub(240, 240, 240); // white/gray dot
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 20; i++) {
        float A = (2 * pi * i) / 20;
        glVertex2f(bx + 0.06f * cos(A), by + 0.06f * sin(A));
    }
    glEnd();

    glPopMatrix();
}

void CNGWheel_400(float xc, float yc, float radius, float angle) {
    float pi = 3.1416f;

    // === Outer Black Tire ===
    glColor3ub(25, 25, 25);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 120; i++) {
        float A = (2 * pi * i) / 120;
        glVertex2f(xc + radius * cos(A), yc + radius * sin(A));
    }
    glEnd();

    // === Sidewall Line (subtle ring) ===
    glColor3ub(60, 60, 60);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 120; i++) {
        float A = (2 * pi * i) / 120;
        glVertex2f(xc + (radius * 0.9f) * cos(A), yc + (radius * 0.9f) * sin(A));
    }
    glEnd();

    // === Rim (Shiny Gray Circle) ===
    glColor3ub(200, 200, 200);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 120; i++) {
        float A = (2 * pi * i) / 120;
        glVertex2f(xc + (radius * 0.65f) * cos(A), yc + (radius * 0.65f) * sin(A));
    }
    glEnd();

    // === Patterned Rim Holes ===
    glPushMatrix();
    glTranslatef(xc, yc, 0);
    glRotatef(angle, 0, 0, 1);

    int holes = 6;
    for (int i = 0; i < holes; i++) {
        float A = (2 * pi * i) / holes;
        float hx = (radius * 0.45f) * cos(A);
        float hy = (radius * 0.45f) * sin(A);

        glColor3ub(100, 100, 100);
        glBegin(GL_TRIANGLE_FAN);
        for (int j = 0; j <= 20; j++) {
            float B = (2 * pi * j) / 20;
            glVertex2f(hx + 0.07f * cos(B), hy + 0.07f * sin(B));
        }
        glEnd();
    }

    glPopMatrix();

    // === Central Hub ===
    glColor3ub(120, 120, 120);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 40; i++) {
        float A = (2 * pi * i) / 40;
        glVertex2f(xc + (radius * 0.2f) * cos(A), yc + (radius * 0.2f) * sin(A));
    }
    glEnd();

    // === Rotating Spokes (indicator of movement) ===
    glPushMatrix();
    glTranslatef(xc, yc, 0);
    glRotatef(angle, 0, 0, 1);

    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    for (int i = 0; i < 3; i++) { // 3 slim spokes
        float A = (2 * pi * i) / 3;
        glVertex2f(0.0f, 0.0f);
        glVertex2f(radius * 0.65f * cos(A), radius * 0.65f * sin(A));
    }
    glEnd();

    // === Orbiting Yellow Dot (movement indicator) ===
    float bx = radius * 0.55f * cos(angle * pi / 180.0f);
    float by = radius * 0.55f * sin(angle * pi / 180.0f);

    glColor3ub(255, 215, 0); // gold/yellow
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 20; i++) {
        float A = (2 * pi * i) / 20;
        glVertex2f(bx + 0.05f * cos(A), by + 0.05f * sin(A));
    }
    glEnd();

    glPopMatrix();
}

///AH401
void sunMoon_401(){
    ///Sun& moon
    if(current_time == DAY)
        circle_400(3, 10, 115, 1, 1, 0);
    else if(current_time == EVENING)
        circle_400(3, 10, 115, 1.0f, 0.65f, 0.0f);
    else if (current_time == CLOUDY || current_time == RAIN)
        circle_400(3, 10, 115, 0.55f, 0.55f, 0.55f);
    else
        circle_400(3, 10, 115, 1, 1, 1);
}

/// AH402
void Cloud_402(){
    float r = 1.0, g = 1.0, b = 1.0;
    if (current_time == CLOUDY || current_time == RAIN) {
        r = g = b = 0.70f;

        for(int i=0; i<3; i++){
            float offset = i * (12+30);

            circle_400(1.5, 21 + offset, 112,  r, g, b);
            circle_400(2.8, 15.0 + offset, 112.0, r, g, b);
            circle_400(2.5, 11.0 + offset, 112.0, r, g, b);
            circle_400(2.2, 19.0 + offset, 112, r, g, b);
            circle_400(1.2, 9 + offset, 112,  r, g, b);
        }
    }else if(current_time == NIGHT){
        r = g = b = 0.588;

        for(int i=0; i<3; i++){
            float offset = i * (12+30);

            circle_400(1.5, 81, 117,  r, g, b);
            circle_400(2.8, 75.0, 117.0, r, g, b);
            circle_400(2.5, 71.0, 117.0, r, g, b);
            circle_400(2.2, 79.0, 117,  r, g, b);
            circle_400(1.2, 69, 117,  r, g, b);

            circle_400(1.5, 131, 117,  r, g, b);
            circle_400(2.8, 125.0, 117.0, r, g, b);
            circle_400(2.5, 121.0, 117.0, r, g, b);
            circle_400(2.0, 129.0, 117.0,  r, g, b);
            circle_400(1.5, 119, 117,  r, g, b);
        }
    }

///Cloud1
    // first cloud
    circle_400(1.5, 81, 117,  r, g, b);
    circle_400(2.8, 75.0, 117.0, r, g, b);
    circle_400(2.5, 71.0, 117.0, r, g, b);
    circle_400(2.2, 79.0, 117,  r, g, b);
    circle_400(1.2, 69, 117,  r, g, b);
///Cloud2
    // second cloud
    circle_400(1.5, 131, 117,  r, g, b);
    circle_400(2.8, 125.0, 117.0, r, g, b);
    circle_400(2.5, 121.0, 117.0, r, g, b);
    circle_400(2.0, 129.0, 117.0,  r, g, b);
    circle_400(1.5, 119, 117,  r, g, b);
}

///AH403
void Sky_403(){
    switch(current_time){
        case DAY: glColor3ub(190, 229, 244); break;
        case NIGHT: glColor4ub(37,20,40,255);break;
        case EVENING: glColor4ub(255, 209, 153, 255);break;
        case CLOUDY: glColor3ub(119, 136, 153); break;    // deep slate gray-blue
        case RAIN: glColor3ub(119, 136, 153); break;
    }
    switch(current_weather){
        case SNOW: glColor4ub(255, 242, 217, 255); break;
    }

    glBegin(GL_POLYGON);
    glVertex2f(0,80);
    glVertex2f(160,80);
    glVertex2f(160,120);
    glVertex2f(0,120);
    glEnd();

    glColor3ub(201, 201, 201);
    glBegin(GL_POLYGON);
    glVertex2f(0,40);
    glVertex2f(160,40);
    glVertex2f(160,80);
    glVertex2f(0,80);
    glEnd();
}

///AH404
void rareView_404(){
    float r = 0.0f, g = 0.6f, b = 0.0f;

    semicircle_arc_400(2.5, 85, 2.4, 0, 180, r, g, b);
    semicircle_arc_400(6.1, 83.5, 2.15, -60, 160, r, g, b);
    semicircle_arc_400(10, 83, 2.2, 0, 225, r, g, b);
    semicircle_arc_400(15, 83.8, 3, 0, 210, r, g, b);
    semicircle_arc_400(22.55, 83, 5, -20, 180, r, g, b);
    semicircle_arc_400(30.1, 82.8, 2.6, -15, 180, r, g, b);
    semicircle_arc_400(35, 81.8, 2.5, -20, 165, r, g, b);
    semicircle_arc_400(40.2, 81, 2.7, -5, 180, r, g, b);
    semicircle_arc_400(46.4, 80.8, 3.4, -2, 178, r, g, b);
    semicircle_arc_400(51.8, 82.3, 2.45, 25, 225, r, g, b);
    semicircle_arc_400(55.7, 83.9, 2.1, 20, 200, r, g, b);
    semicircle_arc_400(58.4, 83.2, 1.6, -40, 180, r, g, b);
    semicircle_arc_400(63.3, 81.9, 4.2, 0, 180, r, g, b);
    semicircle_arc_400(71.8, 82.4, 4.3, 0, 190, r, g, b);
    semicircle_arc_400(79, 82, 3.1, -10, 170, r, g, b);
    semicircle_arc_400(84.9, 81.4, 2.95, -5, 175, r, g, b);
    semicircle_arc_400(90.7, 82.8, 3.25, 25, 210, r, g, b);
    semicircle_arc_400(97.6, 83.9, 4.3, -5, 175, r, g, b);
    semicircle_arc_400(105.5, 83, 3.5, -10, 178, r, g, b);
    semicircle_arc_400(113, 82.5, 4, 0, 182, r, g, b);
    semicircle_arc_400(119.5, 83.4, 2.5, 20, 205, r, g, b);
    semicircle_arc_400(123, 83.3, 1.7, -40, 155, r, g, b);
    semicircle_arc_400(127.7, 82.2, 3.5, 0, 180, r, g, b);
    semicircle_arc_400(133.6, 82.85, 2.5, 20, 200, r, g, b);
    semicircle_arc_400(138.2, 82.8, 2.8, -30, 160, r, g, b);
    semicircle_arc_400(143.2, 81.6, 2.4, 0, 185, r, g, b);
    semicircle_arc_400(147.6, 82.8, 2.2, 45, 225, r, g, b);
    semicircle_arc_400(152.6, 85.1, 3.55, 15, 195, r, g,b);
    semicircle_arc_400(158, 88, 2.75, 45, 225, r, g, b);

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);//R19, V19, W19, Z19, G26
    glVertex2f(0, 80);
    glVertex2f(0, 85);
    glVertex2f(4.59262,85);
    glVertex2f(8.18024,82.62236);
    glVertex2f(7.98, 80);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);//G26, Z19, A20, B20, C20, D20, E20, F20, G20, I26
    glVertex2f(7.98, 80);
    glVertex2f(8.18024,82.62236);
    glVertex2f(11.97889,83.36099);
    glVertex2f(17.99343,84.12649);
    glVertex2f(27.59558,82.86027);
    glVertex2f(32.82354,82.35404);
    glVertex2f(37.40418,81.13877);
    glVertex2f(43.01312,80.9518);
    glVertex2f(49.74385,80.67136);
    glVertex2f(50, 80);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);//I26, G20, H20, I20, J20, J26
    glVertex2f(50, 80);
    glVertex2f(49.74385,80.67136);
    glVertex2f(53.85707,83.66279);
    glVertex2f(57.59636,84.41065);
    glVertex2f(59.09208,81.88663);
    glVertex2f(59.19, 80);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);//J26, J20, K20, L20, M20, N20, L26
    glVertex2f(59.19, 80);
    glVertex2f(59.09208,81.88663);
    glVertex2f(67.50549,81.88663);
    glVertex2f(76.19935,82.72797);
    glVertex2f(81.99525,81.69966);
    glVertex2f(87.79116,81.23225);
    glVertex2f(87.75, 80);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);//L26, N20, O20, P20, Q20, R20, H26
    glVertex2f(87.75, 80);
    glVertex2f(87.79116,81.23225);
    glVertex2f(93.49358,84.41065);
    glVertex2f(102.00047,83.47583);
    glVertex2f(108.91816,82.35404);
    glVertex2f(117.14461,82.44752);
    glVertex2f(117.11, 80);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);//H26, R20, S20, T20, K26
    glVertex2f(117.11, 80);
    glVertex2f(117.14461,82.44752);
    glVertex2f(121.63176,84.1302);
    glVertex2f(124.24926,82.44752);
    glVertex2f(124.3, 80);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);//K26, T20, U20, V20, W20, M26
    glVertex2f(124.3, 80);
    glVertex2f(124.24926,82.44752);
    glVertex2f(131.10892,82.20029);
    glVertex2f(135.75694,83.78432);
    glVertex2f(140.92211,81.56718);
    glVertex2f(140.82, 80);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);//M26, W20, Z20, A21, B21, C21, S19
    glVertex2f(140.82, 80);
    glVertex2f(140.92211,81.56718);
    glVertex2f(145.52584,81.83054);
    glVertex2f(149.22774,83.88715);
    glVertex2f(156.11739,86.14942);
    glVertex2f(160, 90);
    glVertex2f(160, 80);
    glEnd();
}

void mainRoad_400(){
    if(current_time == DAY)
        glColor4ub(68,67,65,255);
    else
       glColor4ub(30,28,27,255);

    glBegin(GL_POLYGON);
    glVertex2f(0,20);
    glVertex2f(160,20);
    glVertex2f(160,40);
    glVertex2f(0,40);
    glEnd();

}

void mainRoadExten_400(){
    glMatrixMode(GL_MODELVIEW);
    ///Sidewalk-Bottom side
    glPushMatrix();

    for (int i=0; i<110; i++)
    {
         glBegin(GL_POLYGON);
        glColor4ub(188,105,99,255);
        glVertex2f(0,20);
        glVertex2f(0.5,20);
        glVertex2f(0.5,20.5);
        glVertex2f(0,20.5);
        glEnd();

          glBegin(GL_POLYGON);
        glColor3ub(209, 194, 135);
        glVertex2f(0.5,20);
        glVertex2f(1,20);
        glVertex2f(1,20.5);
        glVertex2f(0.5,20.5);
        glEnd();

         glBegin(GL_POLYGON);
        glColor4ub(107,62,43,255);
        glVertex2f(1,20);
        glVertex2f(1.5,20);
        glVertex2f(1.5,20.5);
        glVertex2f(1,20.5);
        glEnd();
        ///2nd layer
        glBegin(GL_POLYGON);
        glColor4ub(188,176,164,255);

        glVertex2f(0,20.5);
        glVertex2f(0.5,20.5);
        glVertex2f(0.5,21);
        glVertex2f(0,21);
        glEnd();

        glBegin(GL_POLYGON);
        glColor4ub(107,62,43,255);

        glVertex2f(0.5,20.5);
        glVertex2f(1,20.5);
        glVertex2f(1,21);
        glVertex2f(0.5,21);
        glEnd();

         glBegin(GL_POLYGON);
        glColor3ub(209, 194, 135);
        glVertex2f(1,20.5);
        glVertex2f(1.5,20.5);
        glVertex2f(1.5,21);
        glVertex2f(1,21);
        glEnd();


    glTranslatef(1.5f, 0.0f, 0.0f);

    }

    glPopMatrix();

    ///Top-side sidewalk
    glPushMatrix();

    for (int i=0; i<19; i++)
    {
        glBegin(GL_POLYGON);
        glColor4ub(188,105,99,255);
        glVertex2f(0,40);
        glVertex2f(0.5,40);
        glVertex2f(0.5,39.5);
        glVertex2f(0,39.5);
        glEnd();

          glBegin(GL_POLYGON);
        glColor3ub(209, 194, 135);
        glVertex2f(0.5,40);
        glVertex2f(1,40);
        glVertex2f(1,39.5);
        glVertex2f(0.5,39.5);
        glEnd();

         glBegin(GL_POLYGON);
        glColor4ub(107,62,43,255);
        glVertex2f(1,40);
        glVertex2f(1.5,40);
        glVertex2f(1.5,39.5);
        glVertex2f(1,39.5);
        glEnd();
        ///2nd layer
        glBegin(GL_POLYGON);
        glColor4ub(188,176,164,255);

        glVertex2f(0,39.5);
        glVertex2f(0.5,39.5);
        glVertex2f(0.5,39);
        glVertex2f(0,39);
        glEnd();

          glBegin(GL_POLYGON);
        glColor4ub(107,62,43,255);

        glVertex2f(0.5,39.5);
        glVertex2f(1,39.5);
        glVertex2f(1,39);
        glVertex2f(0.5,39);
        glEnd();

         glBegin(GL_POLYGON);
        glColor3ub(209, 194, 135);
        glVertex2f(1,39.5);
        glVertex2f(1.5,39.5);
        glVertex2f(1.5,39);
        glVertex2f(1,39);
        glEnd();

        glTranslatef(1.5f, 0.0f, 0.0f);
        }

    glPopMatrix();
    glPushMatrix();

    for (int i=0; i<59; i++)
    {
        glBegin(GL_POLYGON);
        glColor4ub(188,105,99,255);
        glVertex2f(36,40);
        glVertex2f(36.5,40);
        glVertex2f(36.5,39.5);
        glVertex2f(36,39.5);
        glEnd();

          glBegin(GL_POLYGON);
        glColor3ub(209, 194, 135);
        glVertex2f(36.5,40);
        glVertex2f(37,40);
        glVertex2f(37,39.5);
        glVertex2f(36.5,39.5);
        glEnd();

         glBegin(GL_POLYGON);
        glColor4ub(107,62,43,255);
        glVertex2f(37,40);
        glVertex2f(37.5,40);
        glVertex2f(37.5,39.5);
        glVertex2f(37,39.5);
        glEnd();
        ///2nd layer
        glBegin(GL_POLYGON);
        glColor4ub(188,176,164,255);

        glVertex2f(36,39.5);
        glVertex2f(36.5,39.5);
        glVertex2f(36.5,39);
        glVertex2f(36,39);
        glEnd();

          glBegin(GL_POLYGON);
        glColor4ub(107,62,43,255);

        glVertex2f(36.5,39.5);
        glVertex2f(37,39.5);
        glVertex2f(37,39);
        glVertex2f(36.5,39);
        glEnd();

         glBegin(GL_POLYGON);
        glColor3ub(209, 194, 135);
        glVertex2f(37,39.5);
        glVertex2f(37.5,39.5);
        glVertex2f(37.5,39);
        glVertex2f(37,39);
        glEnd();

        glTranslatef(1.5f, 0.0f, 0.0f);
        }

    glPopMatrix();

    glPushMatrix();

    for (int i=0; i<20; i++)
    {
        glBegin(GL_POLYGON);
        glColor4ub(188,105,99,255);
        glVertex2f(131,40);
        glVertex2f(131.5,40);
        glVertex2f(131.5,39.5);
        glVertex2f(131,39.5);
        glEnd();

          glBegin(GL_POLYGON);
        glColor3ub(209, 194, 135);
        glVertex2f(131.5,40);
        glVertex2f(132,40);
        glVertex2f(132,39.5);
        glVertex2f(131.5,39.5);
        glEnd();

         glBegin(GL_POLYGON);
        glColor4ub(107,62,43,255);
        glVertex2f(132,40);
        glVertex2f(132.5,40);
        glVertex2f(132.5,39.5);
        glVertex2f(132,39.5);
        glEnd();
        ///2nd layer
        glBegin(GL_POLYGON);
        glColor4ub(188,176,164,255);

        glVertex2f(131,39.5);
        glVertex2f(131.5,39.5);
        glVertex2f(131.5,39);
        glVertex2f(131,39);
        glEnd();

          glBegin(GL_POLYGON);
        glColor4ub(107,62,43,255);

        glVertex2f(131.5,39.5);
        glVertex2f(132,39.5);
        glVertex2f(132,39);
        glVertex2f(131.5,39);
        glEnd();

         glBegin(GL_POLYGON);
        glColor3ub(209, 194, 135);
        glVertex2f(132,39.5);
        glVertex2f(132.5,39.5);
        glVertex2f(132.5,39);
        glVertex2f(132,39);
        glEnd();

        glTranslatef(1.5f, 0.0f, 0.0f);
        }

    glPopMatrix();
    ///MidLines of road
    glLineWidth(1.2);
        glBegin(GL_LINE_LOOP);
        glColor4ub(251,246,100,255);
        glVertex2f(0,30.2);
        glVertex2f(160,30.2);
        glEnd();

    glLineWidth(1.2);
        glBegin(GL_LINE_LOOP);
        glColor4ub(251,246,100,255);
        glVertex2f(0,29.8);
        glVertex2f(160,29.8);
        glEnd();

    ///MidBorder of two lane Road
    glPushMatrix();

    for (int i=0; i<40; i++)
    {
        glLineWidth(1.1);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(1,25.5);
        glVertex2f(4,25.5);
        glEnd();

        glLineWidth(1.1);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(1,34.5);
        glVertex2f(4,34.5);
        glEnd();

    glTranslatef(6.0f, 0.0f, 0.0f);

    }
    glPopMatrix();
}

///AH405
void FullRoad_405(){
    mainRoad_400();
    mainRoadExten_400();
}

void busCounter_400(float x, float y, const char* label) {
    float h = 7.0f;
    float deskH = h * 0.3f;
    float glassH = h * 0.5f;
    float signH = h * 0.2f;
    float w = 10.0f;

    glColor3ub(64, 64, 64);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y+deskH);
        glVertex2f(x, y+deskH);
    glEnd();

    glColor3ub(180, 220, 255);
    glBegin(GL_POLYGON);
        glVertex2f(x+1, y+deskH);
        glVertex2f(x+w-1, y+deskH);
        glVertex2f(x+w-1, y+deskH+glassH);
        glVertex2f(x+1, y+deskH+glassH);
    glEnd();

    // Signboard
    glColor3ub(16, 220, 172);
    glBegin(GL_POLYGON);
        glVertex2f(x, y+deskH+glassH);
        glVertex2f(x+w, y+deskH+glassH);
        glVertex2f(x+w, y+h);
        glVertex2f(x, y+h);
    glEnd();

    // Border
    glColor3ub(255, 255, 255);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y+h);
        glVertex2f(x, y+h);
    glEnd();

    // Label
    glColor3ub(0, 0, 0);
    glRasterPos2f(x+3, y+deskH+glassH+signH/2);
    for (const char* c = label; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
    }
}

///AH414
void TrainArea_400(){
    if(current_time == DAY)
        glColor3ub(182,206,118);
    else
        glColor4ub(19,55,15,255);

    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(160,0);
    glVertex2f(160,20);
    glVertex2f(0,20);
    glEnd();
}

///AH415
void TrainLine_400(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    for (int i=0; i<82; i++)
    {
        glLineWidth(7.5);
        glBegin(GL_LINES);
        glColor3ub(144,107,73);
        glVertex2f(0,2);
        glVertex2f(1,7);
        glEnd();
        glTranslatef(2.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glLineWidth(3.5);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(0,3);
        glVertex2f(160,3);
        glEnd();

        glLineWidth(3.5);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(0,6);
        glVertex2f(160,6);
        glEnd();
}

void train_Compartment_400(){

    //Bogy2
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);//A34, B34, C34, D34
    glVertex2f(81.3, 9.15);
    glVertex2f(81.1, 8.34);
    glVertex2f(111, 8.34);
    glVertex2f(110.8, 9.15);
    glEnd();

    glColor3ub(0, 100, 0);
    glBegin(GL_POLYGON);//B34, H34, G34, C34
    glVertex2f(81.1, 8.34);
    glVertex2f(81.1, 5.85);
    glVertex2f(111, 5.85);
    glVertex2f(111, 8.34);
    glEnd();

    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);//H34, I34, J34, G34
    glVertex2f(81.1, 5.85);
    glVertex2f(81.1, 5.13);
    glVertex2f(111, 5.13);
    glVertex2f(111, 5.85);
    glEnd();

    glColor3ub(0, 100, 0);
    glBegin(GL_POLYGON);//I34, E34, F34, J34
    glVertex2f(81.1, 5.13);
    glVertex2f(81.1, 4.66);
    glVertex2f(111, 4.66);
    glVertex2f(111, 5.13);
    glEnd();

    for(int i=0; i<2; i++){
        float offset = i * (0.94 + 21.75);

        //Window
        glColor3ub(255, 255, 228);
        glBegin(GL_POLYGON);//K34, L34, M34, N34
        glVertex2f(84.36 + offset, 7.79);
        glVertex2f(84.36 + offset, 6.34);
        glVertex2f(85.3 + offset, 6.34);
        glVertex2f(85.3 + offset, 7.79);
        glEnd();
    }

    for(int i=0; i<9; i++){
        float offset = i * (1.72 + 0.56);

        //Top part (Silver)
        glColor3ub(192, 192, 192);
        glBegin(GL_POLYGON);//O34, G35, H35, R34
        glVertex2f(86.18 + offset, 7.79);
        glVertex2f(86.18 + offset, 7.39);
        glVertex2f(87.9 + offset, 7.39);
        glVertex2f(87.9 + offset, 7.79);
        glEnd();

        glColor3ub(255, 255, 228);
        glBegin(GL_POLYGON);//G35, P34, Q34, H35
        glVertex2f(86.18 + offset, 7.39);
        glVertex2f(86.18 + offset, 6.34);
        glVertex2f(87.9 + offset, 6.34);
        glVertex2f(87.9 + offset, 7.39);
        glEnd();
    }

    //door
    for(int i=0; i<2; i++){
        float offset = i * (1.28 + 26.53);

        glColor3ub(255, 0, 0);
        glBegin(GL_POLYGON);//J35, K35, L35, M35, N35, O35
        glVertex2f(81.47 + offset, 7.72);
        glVertex2f(81.47 + offset, 4.5);
        glVertex2f(82.75 + offset, 4.5);
        glVertex2f(82.75 + offset, 7.72);
        glVertex2f(82.66 + offset, 7.86);
        glVertex2f(81.62 + offset, 7.86);
        glEnd();
    }

    //Lower Part
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);//Q35, R35, S35, T35, U35, V35, W35, Z35
    glVertex2f(88.3, 4.65);
    glVertex2f(88.95, 4.21);
    glVertex2f(91.65, 4.21);
    glVertex2f(92.24, 4.28);
    glVertex2f(99.81, 4.28);
    glVertex2f(100.41, 4.21);
    glVertex2f(103.13, 4.21);
    glVertex2f(103.84, 4.65);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//G36, B36
    glVertex2f(81.53, 4.4);
    glVertex2f(82.84, 4.4);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//B36, C36
    glVertex2f(82.84, 4.4);
    glVertex2f(82.84, 4.63);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//C36, D36
    glVertex2f(82.84, 4.63);
    glVertex2f(83.6, 4.63);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//D36, E36
    glVertex2f(83.6, 4.63);
    glVertex2f(83.36, 4.19);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//E36, F36
    glVertex2f(83.36, 4.19);
    glVertex2f(81.51, 4.19);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//G36, F36
    glVertex2f(81.53, 4.4);
    glVertex2f(81.51, 4.19);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//Q36, P36
    glVertex2f(83.04, 4.19);
    glVertex2f(83.04, 3.8);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//N36, O36
    glVertex2f(81.59, 4.19);
    glVertex2f(81.59, 3.8);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//O36, P36
    glVertex2f(81.59, 3.8);
    glVertex2f(83.04, 3.8);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//H36, I36
    glVertex2f(81.73, 4.4);
    glVertex2f(81.73, 3.67);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//K36, J36
    glVertex2f(82.46, 4.19);
    glVertex2f(82.45, 3.67);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//I36, J36
    glVertex2f(81.73, 3.67);
    glVertex2f(82.45, 3.67);
    glEnd();

    TrainWheel_400(0.8, 87.4575, 3.88777, trainWheelAngle_400);
    TrainWheel_400(0.8, 84.21372, 3.88777, trainWheelAngle_400);
    TrainWheel_400(0.8, 104.62117, 3.88777, trainWheelAngle_400);
    TrainWheel_400(0.8, 107.86505, 3.88777, trainWheelAngle_400);
}

///AH416
void train_head_400(){
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);//U36, T36, D37, E37, F37, G37
    glVertex2f(111.54, 8.33);
    glVertex2f(111.54, 9);
    glVertex2f(134, 9);
    glVertex2f(134.76, 8.92);
    glVertex2f(135.68, 8.58);
    glVertex2f(136.02, 8.4);
    glEnd();

    glColor3ub(0, 100, 0);
    glBegin(GL_POLYGON);//V36, U36, G37, H37, I37, J37, K37
    glVertex2f(111.54, 4.72);
    glVertex2f(111.54, 8.33);
    glVertex2f(136.02, 8.4);
    glVertex2f(138.62, 6.43);
    glVertex2f(138.92, 6.13);
    glVertex2f(139.14, 5.75);
    glVertex2f(139.29, 4.78);
    glEnd();

    //lower part
    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//V36, W36, Z36, A37, B38
    glVertex2f(111.54, 4.72);
    glVertex2f(111.54, 3.74);
    glVertex2f(113.74, 3.74);
    glVertex2f(114, 4.43);
    glVertex2f(113.89, 4.74);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//B38, A37, B37, C37, C38
    glVertex2f(113.89, 4.74);
    glVertex2f(114, 4.43);
    glVertex2f(119.11, 4.43);
    glVertex2f(119.39, 3.74);
    glVertex2f(119.44548,4.74083);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//C38, C37, Q37, P37, R37
    glVertex2f(119.44548,4.74083);
    glVertex2f(119.39, 3.74);
    glVertex2f(130.83, 3.78);
    glVertex2f(131.1, 4.46);
    glVertex2f(131.64, 4.79);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//R37, P37, O37, N37, D38
    glVertex2f(131.64, 4.79);
    glVertex2f(131.1, 4.46);
    glVertex2f(136.2, 4.48);
    glVertex2f(136.49, 3.85);
    glVertex2f(136.6025,4.77514);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//D38, N37, M37, L37, K37
    glVertex2f(136.6025,4.77514);
    glVertex2f(136.49, 3.85);
    glVertex2f(138.86, 3.86);
    glVertex2f(139.22, 4.19);
    glVertex2f(139.29, 4.78);
    glEnd();

    for(int i=0; i<10; i++){
        float offset = i * (1.44 + 0.29);

        glColor3ub(255, 255, 228);
        glBegin(GL_POLYGON);//V37, W37, Z37, A38
        glVertex2f(115.63 + offset, 6.86);
        glVertex2f(115.63 + offset, 5.84);
        glVertex2f(114.19 + offset, 5.84);
        glVertex2f(114.19 + offset, 6.86);
        glEnd();
    }

    for(int i=0; i<2; i++){
        float offset = i * (17.75 + 1.46);

        glColor3ub(255, 0, 0);
        glBegin(GL_POLYGON);//S37, T37, U37, B38
        glVertex2f(113.89 + offset, 7.63);
        glVertex2f(112.43 + offset, 7.63);
        glVertex2f(112.43 + offset, 4.74);
        glVertex2f(113.89 + offset, 4.74);
        glEnd();
    }

    //front glass
    glColor3ub(255, 255, 228);
    glBegin(GL_POLYGON);//F38, G38, H38, I38, H37
    glVertex2f(136.41917, 8.11281);
    glVertex2f(136.21943, 8.00184);
    glVertex2f(137.89505, 6.78119);
    glVertex2f(138.19466, 6.60364);
    glVertex2f(138.62, 6.43);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//J38, K38, L38, M38, N38, O38, P38, Q38, R38, S38, T38, U38, V38, W38
    glVertex2f(133.50687,8.37041);
    glVertex2f(133.5,7);
    glVertex2f(133.58347,6.57031);
    glVertex2f(133.75582,6.12986);
    glVertex2f(133.9569,5.81389);
    glVertex2f(134.2633,5.51706);
    glVertex2f(134.62715,5.32556);
    glVertex2f(135.0293,5.13407);
    glVertex2f(135.51762,5.06704);
    glVertex2f(138.74439,5.06704);
    glVertex2f(138.62949,5.61281);
    glVertex2f(138.39969,6.07241);
    glVertex2f(138.19861,6.35009);
    glVertex2f(135.54634,8.37998);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//Z38, A39, B39, C39
    glVertex2f(134.79074,8.05174);
    glVertex2f(134.80376,7.63493);
    glVertex2f(136.21052,7.64795);
    glVertex2f(135.57227,8.10384);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);//D39, E39
    glBegin(GL_LINES);
    glVertex2f(133.82297,6.80417);
    glVertex2f(135.53751,6.82274);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);//F39, G39
    glBegin(GL_LINES);
    glVertex2f(133.88487,6.5442);
    glVertex2f(135.5994,6.55658);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);//H39, I39
    glBegin(GL_LINES);
    glVertex2f(133.9901,6.28424);
    glVertex2f(135.69225,6.28424);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);//J39, K39
    glBegin(GL_LINES);
    glVertex2f(134.20054,6.03046);
    glVertex2f(135.9027,6.04903);
    glEnd();

    TrainWheel_400(0.8, 114.5957, 3.88777, trainWheelAngle_400);
    TrainWheel_400(0.8, 118.52641, 3.88777, trainWheelAngle_400);
    TrainWheel_400(0.8, 131.67147, 3.88777, trainWheelAngle_400);
    TrainWheel_400(0.8, 135.59667, 3.88777, trainWheelAngle_400);
}

void train_400(){
    glPushMatrix();
    glTranslatef(train_400_x, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-31.0f, 0.0f, 0.0f);
    ///AH417
    train_Compartment_400();//train_Compartment1
    glPopMatrix();
    ///AH418
    train_Compartment_400();//train_Compartment2
    train_head_400();
    glPopMatrix();
}

///AH413
void plane_413(){
    glPushMatrix();
    glTranslatef(plane_400_x, plane_400_y, 0.0f);

    glColor3ub(0, 255, 255);//Cyan
    glBegin(GL_POLYGON);
    glVertex2f(134.640725910465, 114.7322752576226);//F26
    glVertex2f(134.8100607369482, 114.5651747698902);//N26
    glVertex2f(135.4673551080351, 114.388588222434);//O26
    glVertex2f(136.0596512728239, 114.3589350235202);//P26
    glVertex2f(137.0124320423244, 114.3221323328528);//Q26
    glVertex2f(139.2328610459241, 114.2771512664815);//R26
    glVertex2f(142.456870625028, 114.2755453412209);//S26
    glVertex2f(144.0486098339602, 114.3063531968777);//T26
    glVertex2f(145.9696606614494, 114.3773728432001);//U26
    glVertex2f(146.942939208831, 114.5043222189455);//V26
    glVertex2f(147.8950595269217, 114.7053253972091);//W26
    glVertex2f(148.6884931253306, 114.9274868047637);//Z26
    glVertex2f(150.2133664342879, 115.4499696037715);//A27
    glVertex2f(150.3634275851301, 115.5670905019898);//B27
    glVertex2f(150.362379715673, 115.7039049299562);//C27
    glVertex2f(148.9945770872041, 115.9623735334765);//F27
    glVertex2f(146.8695447368191, 116.1631170758977);//G27
    glVertex2f(139.7767637594508, 116.1631170758977);//H27
    glVertex2f(138.1986527811889, 116.1266739061181);//I27
    glVertex2f(137.1911204884653, 116.0892512781027);//J27
    glVertex2f(136.8737520182478, 116.0443217178885);//K27
    glVertex2f(136.5245766039387, 115.9935694774366);//L27
    glVertex2f(138.1986527811889, 116.1266739061181);//M27
    glVertex2f(135.789684162195, 115.7296578270866);//N27
    glVertex2f(135.4960160555346, 115.4771035758383);//O27
    glVertex2f(134.751187737319, 115.0384123206181);//P27
    glVertex2f(134.6375698582691, 114.8774536586308);//Q27
    glEnd();

    glColor3ub(255, 128, 0);//Bright Orange
    glBegin(GL_POLYGON);
    glVertex2f(134.751187737319, 115.0384123206181);//R27
    glVertex2f(134.6375698582691, 114.8774536586308);//S27
    glVertex2f(134.640725910465, 114.7322752576226);//T27
    glVertex2f(134.8100607369482, 114.5651747698902);//U27
    glVertex2f(135.4673551080351, 114.388588222434);//V27
    glVertex2f(136.0596512728239, 114.3589350235202);//W27
    glVertex2f(137.0124320423244, 114.3221323328528);//Z27
    glVertex2f(139.2328610459241, 114.2771512664815);//A28
    glVertex2f(142.456870625028, 114.2755453412209);//B28
    glVertex2f(144.0486098339602, 114.3063531968777);//C28
    glVertex2f(145.9696606614494, 114.3773728432001);//D28
    glVertex2f(146.1035419560368, 114.7598826453117);//E28
    glVertex2f(143.6334410171837, 114.6241309858526);//F28
    glVertex2f(140.604083165105, 114.6879069406337);//G28
    glVertex2f(138.8343004199435, 114.7038509293289);//H28
    glVertex2f(137.1123496408674, 114.8393748332376);//I28
    glVertex2f(136.2752902343721, 114.9430107597561);//J28
    glVertex2f(135.7810265848225, 115.0067867145369);//K28
    glEnd();

    glColor3ub(76, 0, 130);//Plane Body3 (Indigo)
    glBegin(GL_POLYGON);
    glVertex2f(145.9696606614494, 114.3773728432001);//L28
    glVertex2f(146.942939208831, 114.5043222189455);//M28
    glVertex2f(147.8950595269217, 114.7053253972091);//N28
    glVertex2f(148.6884931253306, 114.9274868047637);//O28
    glVertex2f(150.2133664342879, 115.4499696037715);//P28
    glVertex2f(150.3634275851301, 115.5670905019898);//Q28
    glVertex2f(150.362379715673, 115.7039049299562);//R28
    glVertex2f(148.9945770872041, 115.9623735334765);//S28
    glVertex2f(147.9781779366118, 116.0989499401543);//T28
    glVertex2f(146.1035419560368, 114.7598826453117);//U28
    glEnd();

    glColor4ub(166,166,168,255);//Main wings
    glBegin(GL_POLYGON);
    glVertex2f(143.5102079924511, 114.7820307049692);//V28
    glVertex2f(146.3300037695918, 115.519921188707);//W28
    glVertex2f(141.0242198150965, 114.7908151154899);//Z28
    glEnd();

    glColor4ub(233,228,225,255);//Tail wings
    glBegin(GL_POLYGON);
    glVertex2f(149, 116);//A29
    glVertex2f(149.7172878870436, 118.8248403820001);//B29
    glVertex2f(149.132472524637, 118.8325862808399);//C29
    glVertex2f(148.9786762542821, 118.6940125986187);//D29
    glVertex2f(146.8695447368191, 116.1631170758977);//E29
    glEnd();

    glColor4ub(238,88,87,255);//Tail wings--red
    glBegin(GL_POLYGON);
    glVertex2f(147.9781779366118, 116.0989499401543);//F29
    glVertex2f(149, 116);//G29
    glVertex2f(149.7172878870436, 118.8248403820001);//H29
    glVertex2f(149.4992688180398, 118.8248403820001);//I29
    glEnd();

    glColor4ub(189,67,66,255);//Tail wings2--red
    glBegin(GL_POLYGON);
    glVertex2f(148.102757770896, 115.4801883433125);//J29
    glVertex2f(148.7716130618967, 115.4948884595982);//K29
    glVertex2f(150.491526667327, 116.1269934598846);//L29
    glEnd();

    glColor4ub(217,217,217,255);//Front engine
    glBegin(GL_POLYGON);
    glVertex2f(140.0622819117824, 113.735709071018);//M29
    glVertex2f(140.8192225520984, 113.7080582713717);//N29
    glVertex2f(141.5899885922376, 113.735709071018);//O29
    glVertex2f(141.9494489876388, 113.8324868697798);//P29
    glVertex2f(142.3814927321114, 113.8843321191166);//Q29
    glVertex2f(142.3780363821556, 114.4546298618204);//R29
    glVertex2f(141.9494489876388, 114.4788243115109);//S29
    glVertex2f(141.6314647917069, 114.5721457603169);//T29
    glVertex2f(140.7742900026733, 114.5997965599632);//U29
    glVertex2f(140.0415438120478, 114.5928838600516);//V29
    glVertex2f(139.9724168129322, 114.461542561732);//W29
    glVertex2f(139.958591413109, 114.2887250639429);//Z29
    glVertex2f(139.9516787131975, 114.1677528154906);//A30
    glVertex2f(139.9655041130206, 113.98802261779);//B30
    glVertex2f(139.9896985627111, 113.8359432197356);//C30
    glEnd();

    glColor4ub(217,217,217,255);//Front engine2
    glBegin(GL_POLYGON);
    glVertex2f(139.958591413109, 114.2887250639429);//D30
    glVertex2f(139.9516787131975, 114.1677528154906);//E30
    glVertex2f(139.9655041130206, 113.98802261779);//F30
    glVertex2f(139.9896985627111, 113.8359432197356);//G30
    glVertex2f(139.3478670290971, 113.8397748714205);//H30
    glVertex2f(139.2531793902461, 113.9216668833997);//I30
    glVertex2f(139.2045560081335, 114.077773531235);//J30
    glVertex2f(139.2, 114.2);//K30
    glVertex2f(139.2328610459241, 114.2771512664815);//L30
    glEnd();

    glLineWidth(1);
    glColor4ub(217,217,217,255);//Door
    glBegin(GL_LINE_LOOP);
    glVertex2f(137.2, 115.0366568177078);//M30
    glVertex2f(137.6047651153731, 115.0366568177078);//N30
    glVertex2f(137.6047651153731, 115.8);//O30
    glVertex2f(137.2, 115.8);//P30
    glEnd();

    glLineWidth(1);
    glColor4ub(81,81,81,255);//Door
    glBegin(GL_LINES);
    glVertex2f(137.5023559087713, 115.3811241490047);//Q30
    glVertex2f(137.5023559087713, 115.4788783916701);//R30
    glEnd();

    glColor4ub(112,111,171,255);//Cocpit window1
    glBegin(GL_POLYGON);
    glVertex2f(135.4960160555346, 115.4771035758383);//S30
    glVertex2f(135.798580954814, 115.4804131372293);//T30
    glVertex2f(135.9494739876967, 115.7046231496287);//U30
    glVertex2f(135.789684162195, 115.7296578270866);//V30
    glEnd();

    glColor4ub(112,111,171,255);//Cocpit window2
    glBegin(GL_POLYGON);
    glVertex2f(135.8704027566694, 115.4674094565469);//W30
    glVertex2f(136.2242337985508, 115.4363800844289);//Z30
    glVertex2f(136.2866139566846, 115.6859007169643);//A31
    glVertex2f(136.0038354590279, 115.6897972938111);//B31
    glEnd();

    glColor4ub(112,111,171,255);//Cocpit window3
    glBegin(GL_POLYGON);
    glVertex2f(136.3343164305516, 115.4180329790954);//C31
    glVertex2f(136.631539536954, 115.56847924283);//D31
    glVertex2f(136.6205312737539, 115.726264348698);//E31
    glVertex2f(136.3550909506343, 115.6921098535396);//F31
    glEnd();

    glPushMatrix();
     for (int i=0; i<17; i++)
    {
        //passenger window
        circle_400(0.17, 137.9760476299097, 115.5233437438844, 0.439, 0.435, 0.671);
        glTranslatef(0.5f, 0.0f, 0.0f);
    }
    glPopMatrix();
    glPopMatrix();
}

///AH430
void car1_400(){
    glPushMatrix();
    glTranslatef(car1_400_x, 0.0f, 0.0f);

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(11.94, 22.58);
    glVertex2f(11.49, 21.66);
    glVertex2f(14.27, 21.65);
    glVertex2f(14, 22.57);
    glEnd();

	glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(11.49, 21.66);
    glVertex2f(10.81, 21.53);
    glVertex2f(10.67, 21.45);
    glVertex2f(10.55, 21.32);
    glVertex2f(10.48, 21.15);
    glVertex2f(10.46, 21.01);
    glVertex2f(10.37, 20.99);
    glVertex2f(10.36, 20.91);
    glVertex2f(10.35, 20.69);
    glVertex2f(10.65, 20.69);
    glVertex2f(10.65, 20.82);
    glVertex2f(10.69, 20.93);
    glVertex2f(10.77, 21.04);
    glVertex2f(10.9, 21.11);
    glVertex2f(11, 21.14);
    glVertex2f(11.1, 21.15);
    glVertex2f(11.2, 21.12);
    glVertex2f(11.28, 21.09);
    glVertex2f(11.33, 21.05);
    glVertex2f(11.39, 20.97);
    glVertex2f(11.43, 20.92);
    glVertex2f(11.47, 20.84);
    glVertex2f(11.48, 20.76);
    glVertex2f(11.48, 20.68);
    glVertex2f(13.43, 20.69);
    glVertex2f(13.42, 20.76);
    glVertex2f(13.44, 20.87);
    glVertex2f(13.49, 20.97);
    glVertex2f(13.6, 21.07);
    glVertex2f(13.71, 21.13);
    glVertex2f(13.8, 21.14);
    glVertex2f(13.91, 21.14);
    glVertex2f(14, 21.12);
    glVertex2f(14.07, 21.07);
    glVertex2f(14.14, 21.02);
    glVertex2f(14.19, 20.95);
    glVertex2f(14.23, 20.87);
    glVertex2f(14.25, 20.81);
    glVertex2f(14.26, 20.73);
    glVertex2f(14.25, 20.68);
    glVertex2f(14.57, 20.68);
    glVertex2f(14.57, 21.02);
    glVertex2f(14.46, 21.01);
    glVertex2f(14.46, 21.52);
    glVertex2f(14.43, 21.61);
    glVertex2f(14.34, 21.66);
    glVertex2f(14.27, 21.65);
	glEnd();

	glColor3ub(135, 206, 250);
    glBegin(GL_POLYGON);
    glVertex2f(11.93, 22.47);
    glVertex2f(11.55, 21.66);
    glVertex2f(12.82, 21.65);
    glVertex2f(12.82, 22.46);
    glEnd();

    glColor3ub(135, 206, 250);
    glBegin(GL_POLYGON);
    glVertex2f(12.93, 22.47);
    glVertex2f(12.92, 21.66);
    glVertex2f(14.15, 21.66);
    glVertex2f(13.95, 22.47);
    glEnd();

    glLineWidth(7.5);
    circle_400(0.41, 11.07, 20.74, 0.1f, 0.1f, 0.1f);  // Outer dark gray/black
    circle_400(0.25, 11.07, 20.74, 0.6f, 0.6f, 0.6f);  // Inner light gray
    circle_400(0.41, 13.83, 20.74, 0.1f, 0.1f, 0.1f);  // Outer dark gray/black
    circle_400(0.25, 13.83, 20.74, 0.6f, 0.6f, 0.6f);  // Inner light gray
    glPopMatrix();
}

///AH431
void car2_400(){
    glPushMatrix();
    glTranslatef(car2_400_x, 0.0f, 0.0f);

    glColor3ub(255, 255, 0);
    glBegin(GL_POLYGON);//O41, P41, Q41, R41, S41, T41, N42, O42, P42, Q42
    glVertex2f(14.23659,26.80446);
    glVertex2f(14.33115,27.20451);
    glVertex2f(14.57119,27.48819);
    glVertex2f(14.97124,27.7064);
    glVertex2f(15.3422,27.81551);
    glVertex2f(17.0879,27.83005);
    glVertex2f(21.29211,27.7355);
    glVertex2f(21.5,27.5);
    glVertex2f(21.51069,27.24355);
    glVertex2f(21.34303,26.78264);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//T41, U41, V41, W41, Z41, A42, B42, L42, M42, N42
    glVertex2f(17.0879,27.83005);
    glVertex2f(18.25169,28.96476);
    glVertex2f(18.5,29);
    glVertex2f(18.85541,28.96476);
    glVertex2f(19.34275,28.84838);
    glVertex2f(19.81555,28.69563);
    glVertex2f(20.26652,28.50651);
    glVertex2f(20.6593,28.26648);
    glVertex2f(21,28);
    glVertex2f(21.29211,27.7355);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//O41, Q42, R42, S42, T42, A43, B43
    glVertex2f(14.23659,26.80446);
    glVertex2f(21.34303,26.78264);
    glVertex2f(21.4885,26.68808);
    glVertex2f(21.5,26.5);
    glVertex2f(21.33576,26.41895);
    glVertex2f(14.15658,26.44077);
    glVertex2f(14.0402,26.62261);
    glEnd();

    //Window
    glColor3ub(199, 227, 225);
    glBegin(GL_POLYGON);//H43, I43, J43, K43, L43, M43, N43, O43
    glVertex2f(17.35703,27.80823);
    glVertex2f(20.33926,27.82278);
    glVertex2f(20.35143,27.97479);
    glVertex2f(19.90407,28.27676);
    glVertex2f(19.5,28.5);
    glVertex2f(19.01495,28.61228);
    glVertex2f(18.46694,28.69056);
    glVertex2f(18.27682,28.65701);
    glEnd();

    glColor3ub(192, 192, 192);
    glBegin(GL_POLYGON);//G43, P43, Q43, R43, S43, H43
    glVertex2f(17.34248,28.23011);
    glVertex2f(17.86301,28.25998);
    glVertex2f(18.06432,28.1817);
    glVertex2f(18.13143,28.03631);
    glVertex2f(17.99722,27.84618);
    glVertex2f(17.35703,27.80823);
    glEnd();

    glLineWidth(7.5);
    circle_400(0.8, 15.46795, 26.85, 0.1f, 0.1f, 0.1f);  // Outer dark gray/black
    circle_400(0.45, 15.46795, 26.85, 0.6f, 0.6f, 0.6f);  // Inner light gray
    circle_400(0.8, 20.12894, 26.85, 0.1f, 0.1f, 0.1f);  // Outer dark gray/black
    circle_400(0.45, 20.12894, 26.85, 0.6f, 0.6f, 0.6f);  // Inner light gray
    glPopMatrix();
}

///AH427
void busBRTC_400(){
    glPushMatrix();
    glTranslatef(busBRTC_400_x, 0.0f, 0.0f);

    //Body back
    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//N45, O45, P45, Q45, J54, G46
    glVertex2f(15.37136,39.2001);
    glVertex2f(15.20489,38.65907);
    glVertex2f(15.23263,33.35982);
    glVertex2f(16.63374,32.97139);
    glVertex2f(20.18809,32.93182);
    glVertex2f(19.26115,39.13358);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//G46, J54, K54, L54, M54, N54, O54, P54, Q54, R54, J46
    glVertex2f(19.26115,39.13358);
    glVertex2f(20.18809,32.93182);
    glVertex2f(20.19791,33.72716);
    glVertex2f(20.26664,34.00701);
    glVertex2f(20.54648,34.44396);
    glVertex2f(20.54648,34.44396);
    glVertex2f(20.70359,34.60106);
    glVertex2f(20.94416,34.71889);
    glVertex2f(21.2,34.8);
    glVertex2f(21.41056,34.83181);
    glVertex2f(23.62599,39.13358);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//J46, R54, S54, T54, U54, V54, W54, Z54, A55, B55, C55, D55, E55, J46
    glVertex2f(23.62599,39.13358);
    glVertex2f(21.41056,34.83181);
    glVertex2f(21.66095,34.8269);
    glVertex2f(21.86715,34.79253);
    glVertex2f(22.04389,34.72871);
    glVertex2f(22.21606,34.63199);
    glVertex2f(22.36301,34.52251);
    glVertex2f(22.48575,34.40468);
    glVertex2f(22.56921,34.30158);
    glVertex2f(22.66249,34.13465);
    glVertex2f(22.73614,33.96773);
    glVertex2f(22.79505,33.77135);
    glVertex2f(22.81469,32.94655);
    glVertex2f(23.62599,39.13358);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//E55, R45, S45, J46
    glVertex2f(22.81469,32.94655);
    glVertex2f(31.38472,33.46645);
    glVertex2f(31.41493,38.65888);
    glVertex2f(23.62599,39.13358);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//R45, S45, T45, U45--
    glVertex2f(31.38472,33.46645);
    glVertex2f(31.41493,38.65888);
    glVertex2f(34.08284,38.66477);
    glVertex2f(34.06413,33.49421);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//T45, U45, F55, G55, H55, I55, J55, K55, L55, M55, N55, O55, F46
    glVertex2f(34.08284,39.16477);
    glVertex2f(34.06413,32.99421);
    glVertex2f(34.68314,32.98964);
    glVertex2f(34.67588,33.70148);
    glVertex2f(34.75578,33.98476);
    glVertex2f(34.86473,34.26804);
    glVertex2f(35.0268,34.46574);
    glVertex2f(35.15381,34.58909);
    glVertex2f(35.32821,34.73296);
    glVertex2f(35.4808,34.78964);
    glVertex2f(35.677,34.85504);
    glVertex2f(35.86883,34.88556);
    glVertex2f(39.44034,39.18011);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//F46, O55, P55, Q55, R55, S55, T55, U55, V55, W55, Z55, A56, B56, C56, D56, F46
    glVertex2f(39.44034,39.18011);
    glVertex2f(35.86883,34.88556);
    glVertex2f(36.06503,34.87684);
    glVertex2f(36.24814,34.85504);
    glVertex2f(36.43998,34.79836);
    glVertex2f(36.62309,34.71988);
    glVertex2f(36.77133,34.61961);
    glVertex2f(36.8934,34.51933);
    glVertex2f(37,34.4);
    glVertex2f(37.08882,34.30331);
    glVertex2f(37.1565,34.19502);
    glVertex2f(37.21065,34.06868);
    glVertex2f(37.25577,33.97393);
    glVertex2f(37.3054,33.78893);
    glVertex2f(37.31442,33.00383);
    glVertex2f(39.44034,39.18011);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//F46, D56, E56, F57
    glVertex2f(39.44034,39.18011);
    glVertex2f(37.31442,33.00383);
    glVertex2f(46,33);
    glVertex2f(47.75348,39.25024);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//F57, E56, F56, G56, H56, I56, J56, K56, L56, M56, N56, F57
    glVertex2f(47.75348,39.25024);
    glVertex2f(46,33);
    glVertex2f(46.03665,33.81338);
    glVertex2f(46.11253,34.16028);
    glVertex2f(46.21669,34.35226);
    glVertex2f(46.37929,34.52066);
    glVertex2f(46.52446,34.65423);
    glVertex2f(46.71029,34.75875);
    glVertex2f(46.91934,34.85747);
    glVertex2f(47.05532,34.89451);
    glVertex2f(47.20102,34.91751);
    glVertex2f(47.75348,39.25024);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//F57, N56, O56, P56, Q56, R56, S56, T56, U56, V56, W56, Z56, A57, B57, C57, D57, E57, F57
    glVertex2f(47.75348,39.25024);
    glVertex2f(47.20102,34.91751);
    glVertex2f(47.38505,34.91368);
    glVertex2f(47.54225,34.90601);
    glVertex2f(47.69178,34.86384);
    glVertex2f(47.84131,34.81399);
    glVertex2f(47.97934,34.75265);
    glVertex2f(48.07826,34.68266);
    glVertex2f(48.16588,34.61607);
    glVertex2f(48.25,34.53896);
    glVertex2f(48.3201,34.46185);
    glVertex2f(48.40071,34.37072);
    glVertex2f(48.4603,34.26557);
    glVertex2f(48.50586,34.17094);
    glVertex2f(48.56194,34.04827);
    glVertex2f(48.63204,33.80993);
    glVertex2f(48.64956,33.01431);
    glVertex2f(47.75348,39.25024);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//F57, E57, V45, W45, Z45, A46, B46
    glVertex2f(47.75348,39.25024);
    glVertex2f(48.64956,33.01431);
    glVertex2f(52.98232,32.99421);
    glVertex2f(53.01243,35.59898);
    glVertex2f(52.79497,38.86084);
    glVertex2f(52.71343,39.0783);
    glVertex2f(52.50956,39.2278);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//CDEF(46)
    glVertex2f(35.10267,39.17027);
    glVertex2f(35.7125,39.63257);
    glVertex2f(38.89936,39.63257);
    glVertex2f(39.44034,39.18011);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//GHIJ(46)
    glVertex2f(19.26115,39.13358);
    glVertex2f(19.86472,39.61202);
    glVertex2f(23.05923,39.62674);
    glVertex2f(23.62599,39.13358);
    glEnd();

    //Window
    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//KLMN(46)
    glVertex2f(15.55015,38.33807);
    glVertex2f(15.5306,35.89438);
    glVertex2f(16.40055,35.52294);
    glVertex2f(16.42988,38.31852);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//OPQR(46)
    glVertex2f(16.93515,38.35888);
    glVertex2f(16.93515,35.47835);
    glVertex2f(20.3907,35.4892);
    glVertex2f(20.37985,38.33718);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//STUV(46)
    glVertex2f(20.53717,38.33718);
    glVertex2f(20.56891,35.48834);
    glVertex2f(23.20071,35.50547);
    glVertex2f(23.18292,38.34955);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//W46, Z46, A47, B47
    glVertex2f(26.52049,38.37069);
    glVertex2f(26.5,35.5);
    glVertex2f(29.19293,35.5151);
    glVertex2f(29.16741,38.37912);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//CDEF(47)
    glVertex2f(29.36605,38.34908);
    glVertex2f(29.36605,35.51792);
    glVertex2f(31.21026,35.53233);
    glVertex2f(31.20306,38.40671);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//GHIJ(47)
    glVertex2f(34.23783,38.39189);
    glVertex2f(34.22732,35.53689);
    glVertex2f(37.65873,35.54114);
    glVertex2f(37.65787,38.39281);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//KLMN(47)
    glVertex2f(37.85899,38.39281);
    glVertex2f(37.8475,35.53114);
    glVertex2f(40.43334,35.55413);
    glVertex2f(40.45058,38.4043);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//OPQR(47)
    glVertex2f(43.68472,38.40632);
    glVertex2f(43.70364,35.56204);
    glVertex2f(46.74974,35.56204);
    glVertex2f(46.73712,38.40632);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//STUV(47)
    glVertex2f(46.95155,38.39371);
    glVertex2f(46.94208,35.56149);
    glVertex2f(49.06426,35.57465);
    glVertex2f(49.03904,38.41263);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//W47, Z47, A48, B48, C48, D48, E48
    glVertex2f(52.8218,38.5145);
    glVertex2f(52.5,38.5);
    glVertex2f(52.36321,38.37998);
    glVertex2f(52.3571,36.0259);
    glVertex2f(52.60168,35.27993);
    glVertex2f(52.74231,35.14541);
    glVertex2f(53.01135,35.09038);
    glEnd();

    //door
    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//FGJK(48)
    glVertex2f(23.38077,38.37935);
    glVertex2f(23.37066,37.88379);
    glVertex2f(26.29345,37.88379);
    glVertex2f(26.29345,38.38947);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//G48, H48
    glVertex2f(23.37066,37.88379);
    glVertex2f(23.38077,32.95854);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//I48, J48
    glVertex2f(26.30356,32.97877);
    glVertex2f(26.29345,37.88379);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//L48, M48
    glVertex2f(24.84722,37.85345);
    glVertex2f(24.84722,32.97877);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//N48, O48, P48, Q48
    glVertex2f(23.5527,37.75232);
    glVertex2f(23.5699,34.95833);
    glVertex2f(24.68541,34.97112);
    glVertex2f(24.68541,37.74221);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//R48, S48, T48, U48
    glVertex2f(25.00904,37.72198);
    glVertex2f(25,35);
    glVertex2f(26.14124,34.94979);
    glVertex2f(26.14175,37.74221);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//V48, W48, Z48, A49
    glVertex2f(23.58304,34.70817);
    glVertex2f(23.58304,33.40353);
    glVertex2f(24.66518,33.3833);
    glVertex2f(24.67529,34.71828);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//B49, C49, D49, E49
    glVertex2f(25.02927,34.69805);
    glVertex2f(25.01915,33.40353);
    glVertex2f(26.14175,33.39342);
    glVertex2f(26.14175,34.71828);
    glEnd();

    for(int i=0; i<2; i++){
        float offset = i * (2.87585 + 5.70568);

        glColor3ub(1, 1, 1);
        glBegin(GL_POLYGON);//FGHI49
        glVertex2f(40.6435 + offset,38.42492);
        glVertex2f(40.6435 + offset,37.90707);
        glVertex2f(43.51935 + offset,37.90707);
        glVertex2f(43.51935 + offset,38.42492);
        glEnd();

        glColor3ub(1, 1, 1);
        glLineWidth(1);
        glBegin(GL_LINES);//G49, K49
        glVertex2f(40.6435 + offset,37.90707);
        glVertex2f(40.6435 + offset,33.00754);
        glEnd();

        glColor3ub(1, 1, 1);
        glLineWidth(1);
        glBegin(GL_LINES);//H49, L49
        glVertex2f(43.51935 + offset,37.90707);
        glVertex2f(43.51935 + offset,33);
        glEnd();

        glColor3ub(1, 1, 1);
        glLineWidth(1);
        glBegin(GL_LINES);//M49, N49
        glVertex2f(42.08707 + offset,37.91579);
        glVertex2f(42.10263 + offset,33.02547);
        glEnd();

        glColor4ub(147,204,222,255);//Window
        glBegin(GL_POLYGON);//OPQR49
        glVertex2f(40.79779 + offset,37.78241);
        glVertex2f(40.81261 + offset,34.99637);
        glVertex2f(41.93147 + offset,34.99637);
        glVertex2f(41.93147 + offset,37.76759);
        glEnd();

        glColor4ub(147,204,222,255);//Window
        glBegin(GL_POLYGON);//STUV49
        glVertex2f(42.24268 + offset,37.76759);
        glVertex2f(42.26491 + offset,35.01119);
        glVertex2f(43.36895 + offset,35.02601);
        glVertex2f(43.36895 + offset,37.78241);
        glEnd();

        glColor4ub(147,204,222,255);//Window
        glBegin(GL_POLYGON);//W49, Z49, A50, B50
        glVertex2f(40.84225 + offset,34.75186);
        glVertex2f(40.84225 + offset,33.44034);
        glVertex2f(41.92406 + offset,33.45516);
        glVertex2f(41.90924 + offset,34.79631);
        glEnd();

        glColor4ub(147,204,222,255);//Window
        glBegin(GL_POLYGON);//CDEF50
        glVertex2f(42.26491 + offset,34.75927);
        glVertex2f(42.26491 + offset,33.46998);
        glVertex2f(43.36154 + offset,33.44034);
        glVertex2f(43.35413 + offset,34.77408);
        glEnd();
    }

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//G50, H50, I50, J50
    glVertex2f(52.26227,33.68896);
    glVertex2f(52.27775,32.98192);
    glVertex2f(53.18606,32.98192);
    glVertex2f(53.19122,33.7096);
    glEnd();

    //Headlight
    glColor3ub(255, 255, 0);
    glBegin(GL_POLYGON);//KLMNOP50
    glVertex2f(53.0235,34.54841);
    glVertex2f(52.5,34.5);
    glVertex2f(52.42011,34.4014);
    glVertex2f(52.5,34);
    glVertex2f(52.63076,33.88764);
    glVertex2f(53.02121,33.81058);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);//QRSTUV50
    glVertex2f(53.02361,34.43459);
    glVertex2f(52.63062,34.42949);
    glVertex2f(52.55661,34.35548);
    glVertex2f(52.6051,34.04926);
    glVertex2f(52.71228,33.9727);
    glVertex2f(53.02871,33.92422);
    glEnd();

    glColor3ub(192, 192, 192);
    glBegin(GL_POLYGON);//W50, Z50, A51, B51, C51, D51
    glVertex2f(53.17038,37.77711);
    glVertex2f(53.32942,37.83529);
    glVertex2f(53.55827,37.62971);
    glVertex2f(53.55439,36.62509);
    glVertex2f(53.36821,36.41951);
    glVertex2f(53.18978,36.43503);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//EF51
    glVertex2f(53.2436,37.82656);
    glVertex2f(53.10429,38.41677);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//FG51
    glVertex2f(53.10429,38.41677);
    glVertex2f(53.02478,38.51855);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//GH51
    glVertex2f(53.02478,38.51855);
    glVertex2f(52.94621,38.55782);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//HI51
    glVertex2f(52.94621,38.55782);
    glVertex2f(52.82664,38.57641);
    glEnd();

    BRTCWheel_400(21.48779, 33.51788, BRTCWheelAngle_400); // left wheel
    BRTCWheel_400(35.98998, 33.56258, BRTCWheelAngle_400);   // middle wheel
    BRTCWheel_400(47.32345, 33.58214, BRTCWheelAngle_400);   // right wheel
    glPopMatrix();
}

///AH438, AH439, AH440, AH441, AH442, AH443, AH444, AH445, AH446
void Bus1_400(GLubyte r, GLubyte g, GLubyte b, const char* busName, bool rotating){
    //Front
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//U10, J10, K10, V10
    glVertex2f(81.74, 73.13);
    glVertex2f(81.95, 73);
    glVertex2f(82.09, 73.04);
    glVertex2f(82.06, 73.23);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//T10, I10, J10, U10
    glVertex2f(81.56, 72.89);
    glVertex2f(81.83, 72.9);
    glVertex2f(81.95, 73);
    glVertex2f(81.74, 73.13);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//T10, S10, R10, A10, Z9, W9, G10, H10, I10
    glVertex2f(81.56, 72.89);
    glVertex2f(81.33, 71.65);
    glVertex2f(81.25, 70.9);
    glVertex2f(81.22, 70.44);
    glVertex2f(81.48, 70.43);
    glVertex2f(81.47, 70.89);
    glVertex2f(81.51, 71.45);
    glVertex2f(81.71, 72.6);
    glVertex2f(81.83, 72.9);
    glEnd();

    //Upper side
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//V10, K10, E8, W10
    glVertex2f(82.06, 73.23);
    glVertex2f(82.09, 73.04);
    glVertex2f(93.93, 73.04);
    glVertex2f(94.11, 73.23);
    glEnd();

    //Back
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//E8, D8, Q8, P8, C11, B11, A11, Z10, W10
    glVertex2f(93.93, 73.04);
    glVertex2f(93.95, 71.77);
    glVertex2f(93.93, 70.6);
    glVertex2f(93.94, 69.52);
    glVertex2f(94.32, 69.6);
    glVertex2f(94.41, 69.7);
    glVertex2f(94.33, 72.93);
    glVertex2f(94.28, 73.12);
    glVertex2f(94.11, 73.23);
    glEnd();

    //Looking Glass
    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//L11, K11, J11, I11
    glVertex2f(80.7, 72.4);
    glVertex2f(80.69, 71.96);
    glVertex2f(80.86, 71.96);
    glVertex2f(80.86, 72.4);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//N11, M11, L11, I11, H11
    glVertex2f(80.97, 72.63);
    glVertex2f(80.8, 72.52);
    glVertex2f(80.7, 72.4);
    glVertex2f(80.86, 72.4);
    glVertex2f(81.15, 72.53);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//O11, N11, H11, G11
    glVertex2f(81.31, 72.77);
    glVertex2f(80.97, 72.63);
    glVertex2f(81.15, 72.53);
    glVertex2f(81.42, 72.58);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//E11, O11, G11, F11
    glVertex2f(81.72, 72.76);
    glVertex2f(81.31, 72.77);
    glVertex2f(81.42, 72.58);
    glVertex2f(81.66, 72.58);
    glEnd();

    //Front Side Glass
    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//K10, J10, I10, H10, G10, W9, F10, E10
    glVertex2f(82.09, 73.04);
    glVertex2f(81.95, 73);
    glVertex2f(81.83, 72.9);
    glVertex2f(81.71, 72.6);
    glVertex2f(81.51, 71.45);
    glVertex2f(81.47, 70.89);
    glVertex2f(82.02, 70.9);
    glVertex2f(82.17, 70.95);
    glEnd();

    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//K10, E10, Q10, P10, O10, N10, M10, L10
    glVertex2f(82.09, 73.04);
    glVertex2f(82.17, 70.95);
    glVertex2f(82.3, 71.01);
    glVertex2f(82.42, 71.12);
    glVertex2f(82.5, 71.22);
    glVertex2f(82.61, 71.3);
    glVertex2f(82.78, 71.38);
    glVertex2f(82.78, 73.04);
    glEnd();

    //side glass
    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//B7, C7, D8, E8
    glVertex2f(82.92, 73.04);
    glVertex2f(82.92, 71.77);
    glVertex2f(93.95, 71.77);
    glVertex2f(93.93, 73.04);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//C7, B7, L10, I14
    glVertex2f(82.92, 71.77);
    glVertex2f(82.92, 73.04);
    glVertex2f(82.78, 73.04);
    glVertex2f(82.78, 71.75);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//E7, D7, F7, G7
    glVertex2f(84.49, 73.04);
    glVertex2f(84.49, 71.77);
    glVertex2f(84.58, 71.77);
    glVertex2f(84.58, 73.04);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//I7, H7, J7, K7
    glVertex2f(86.04, 73.04);
    glVertex2f(86.04, 71.77);
    glVertex2f(86.16, 71.77);
    glVertex2f(86.16, 73.04);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//M7, L7, N7, O7
    glVertex2f(87.61, 73.04);
    glVertex2f(87.61, 71.77);
    glVertex2f(87.69, 71.77);
    glVertex2f(87.69, 73.04);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//Q7, P7, R7, S7
    glVertex2f(89.17, 73.04);
    glVertex2f(89.17, 71.77);
    glVertex2f(89.26, 71.77);
    glVertex2f(89.26, 73.04);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//U7, T7, V7, W7
    glVertex2f(90.73, 73.04);
    glVertex2f(90.73, 71.77);
    glVertex2f(90.84, 71.77);
    glVertex2f(90.84, 73.04);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//A8, Z7, B8, C8
    glVertex2f(92.28, 73.04);
    glVertex2f(92.28, 71.77);
    glVertex2f(92.38, 71.77);
    glVertex2f(92.38, 73.04);
    glEnd();

    //Middle Body
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//H14, B14, C14, D14, E14
    glVertex2f(82.9, 71.75);
    glVertex2f(82.8, 71.74);
    glVertex2f(82.81, 71.34);
    glVertex2f(82.3, 70.98);
    glVertex2f(83.05, 70.62);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//H14, E14, F14, G14
    glVertex2f(82.9, 71.75);
    glVertex2f(83.05, 70.62);
    glVertex2f(93.9, 70.62);
    glVertex2f(93.93, 71.75);
    glEnd();

    glColor3ub(255, 255, 255);
    renderText_400(84, 70.6, busName, 11);

    //lower body
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//Z9, A10, B10, C10, D10
    glVertex2f(81.48, 70.43);
    glVertex2f(81.22, 70.44);
    glVertex2f(81.19, 69.88);
    glVertex2f(81.27, 69.74);
    glVertex2f(81.4, 69.66);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//W9, Z9, D10, U8, T8, E10, F10
    glVertex2f(81.47, 70.89);
    glVertex2f(81.48, 70.43);
    glVertex2f(81.4, 69.66);
    glVertex2f(82.33, 69.4);
    glVertex2f(82.33, 70.95);
    glVertex2f(82.17, 70.95);
    glVertex2f(82.02, 70.9);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//T8, U8, V8, W8
    glVertex2f(82.33, 70.95);
    glVertex2f(82.33, 69.4);
    glVertex2f(83.04, 69.4);
    glVertex2f(83.04, 70.6);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//W8, V8, Z8, A9, B9, C9, D9, E9, F9, O14
    glVertex2f(83.04, 70.6);
    glVertex2f(83.04, 69.4);
    glVertex2f(83.36, 69.4);
    glVertex2f(83.37, 69.7);
    glVertex2f(83.4, 69.8);
    glVertex2f(83.5, 69.95);
    glVertex2f(83.65, 70.05);
    glVertex2f(83.83, 70.1);
    glVertex2f(83.95, 70.11);
    glVertex2f(83.93, 70.6);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//O14, F9, G9, H9, I9, J9, K9, S8, R8
    glVertex2f(83.93, 70.6);
    glVertex2f(83.95, 70.11);
    glVertex2f(84.16, 70.08);
    glVertex2f(84.31, 69.98);
    glVertex2f(84.44, 69.8);
    glVertex2f(84.52, 69.6);
    glVertex2f(84.52, 69.4);
    glVertex2f(84.85, 69.4);
    glVertex2f(84.85, 70.6);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//R8, S8, K8, J8
    glVertex2f(84.85, 70.6);
    glVertex2f(84.85, 69.4);
    glVertex2f(86.53, 69.4);
    glVertex2f(86.53, 70.6);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//J8, K8, G8, F8
    glVertex2f(86.53, 70.6);
    glVertex2f(86.53, 69.4);
    glVertex2f(88.2, 69.4);
    glVertex2f(88.2, 70.6);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//F8, G8, H8, I8
    glVertex2f(88.2, 70.6);
    glVertex2f(88.2, 69.4);
    glVertex2f(89.82, 69.4);
    glVertex2f(89.82, 70.6);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//I8, H8, L9, M9, N9, O9, P9, Q9, N14
    glVertex2f(89.82, 70.6);
    glVertex2f(89.82, 69.4);
    glVertex2f(90.2, 69.4);
    glVertex2f(90.21, 69.6);
    glVertex2f(90.25, 69.78);
    glVertex2f(90.39, 69.95);
    glVertex2f(90.58, 70.05);
    glVertex2f(90.83, 70.1);
    glVertex2f(90.8, 70.6);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//N14, Q9, R9, S9, T9, U9, V9, M8, L8
    glVertex2f(90.8, 70.6);
    glVertex2f(90.83, 70.1);
    glVertex2f(91.08, 70.04);
    glVertex2f(91.25, 69.9);
    glVertex2f(91.34, 69.76);
    glVertex2f(91.36, 69.6);
    glVertex2f(91.38, 69.4);
    glVertex2f(91.7, 69.4);
    glVertex2f(91.7, 70.6);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//L8, M8, N8, O8, P8, Q8
    glVertex2f(91.7, 70.6);
    glVertex2f(91.7, 69.4);
    glVertex2f(92.15, 69.4);
    glVertex2f(92.77, 69.45);
    glVertex2f(93.94, 69.52);
    glVertex2f(93.93, 70.6);
    glEnd();

    //Wheel
    if (rotating) {
        BusWheel_400(0.5f, 83.93f, 69.4f, busWheelAngle_400);
        BusWheel_400(0.5f, 90.78f, 69.4f, busWheelAngle_400);
    } else {
        circle_400(0.5, 83.93f, 69.4f, 0.2f, 0.2f, 0.2f);
        circle_400(0.25, 83.93f, 69.4f, 1.0f, 1.0f, 1.0f);
        circle_400(0.5, 90.78f, 69.4f, 0.2f, 0.2f, 0.2f);
        circle_400(0.25, 90.78f, 69.4f, 1.0f, 1.0f, 1.0f);
    }
}

///AH481, AH482, AH483, AH484, AH485
void Bus2_400(GLubyte r, GLubyte g, GLubyte b){
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//F52, T51, G52, L52, U52
    glVertex2f(49.5,68.5);
    glVertex2f(42.59429,67.66173);
    glVertex2f(42.54356,66.90594);
    glVertex2f(48.68689,68.1137);
    glVertex2f(49.38981,68.1988);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//T51, U51, V51, W51, I53, J53, K53, L53, V53
    glVertex2f(42.59429,67.16173);
    glVertex2f(42.39331,66.50019);
    glVertex2f(42.43518,64.0215);
    glVertex2f(42.80364,63.75354);
    glVertex2f(43.28484,63.74238);
    glVertex2f(43.28017,64.51792);
    glVertex2f(43.33156,64.69078);
    glVertex2f(43.45986,64.7074);
    glVertex2f(43.70592,65.93483);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//V53, L53, M53, N53, O53
    glVertex2f(43.70592,65.93483);
    glVertex2f(43.45986,64.7074);
    glVertex2f(43.61736,64.49841);
    glVertex2f(43.71428,64.22884);
    glVertex2f(43.78092,63.93505);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//V53, O53, P53, Q53, W53
    glVertex2f(43.70592,65.93483);
    glVertex2f(43.78092,63.93505);
    glVertex2f(43.8,64.6);
    glVertex2f(43.88996,64.72558);
    glVertex2f(43.9677,65.94254);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//W53, Q53, R53, S53, T53, U53, Z53
    glVertex2f(43.9677,65.94254);
    glVertex2f(43.88996,64.72558);
    glVertex2f(44.04443,64.71649);
    glVertex2f(44.15044,64.5711);
    glVertex2f(44.23828,64.4136);
    glVertex2f(44.27462,63.6261);
    glVertex2f(45.69833,66.11318);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//Z53, U53, Z52, A53, B53, C53, D53, I52
    glVertex2f(45.69833,66.11318);
    glVertex2f(44.27462,63.6261);
    glVertex2f(47.31517,63.26738);
    glVertex2f(47.30487,64.22583);
    glVertex2f(47.37496,64.44796);
    glVertex2f(47.51801,64.60712);
    glVertex2f(47.78394,64.66252);
    glVertex2f(47.65004,66.25206);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//I52, D53, E53, F53, G53, H53, Z51, A52, B52, C52, D52, E52, F52
    glVertex2f(47.65004,66.25206);
    glVertex2f(47.78394,64.66252);
    glVertex2f(48,64.5);
    glVertex2f(48.11637,64.29686);
    glVertex2f(48.19947,64.11956);
    glVertex2f(48.25488,63.15);
    glVertex2f(49.68701,63.025);
    glVertex2f(53.2429,63.21876);
    glVertex2f(53.44714,63.42612);
    glVertex2f(53.41589,65.54061);
    glVertex2f(52.66694,68.17941);
    glVertex2f(52.46004,68.3937);
    glVertex2f(49.5,68.5);
    glEnd();

    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//G52, H52, I52, J52, K52, L52
    glVertex2f(42.54356,67.20594);
    glVertex2f(42.49167,65.82652);
    glVertex2f(47.65004,66.25206);
    glVertex2f(48.58415,66.2313);
    glVertex2f(48.59074,67.26758);
    glVertex2f(48.68689,68.1137);
    glEnd();

    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//M52, N52, O52, P52, Q52, R52, S52
    glVertex2f(49.56745,67.6727);
    glVertex2f(49.72459,65.26771);
    glVertex2f(49.98422,65.11739);
    glVertex2f(50.74262,65.01491);
    glVertex2f(53.00414,65.06273);
    glVertex2f(53.37308,65.19255);
    glVertex2f(52.85382,67.72736);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//TMSD52
    glVertex2f(49.54012,68.15097);
    glVertex2f(49.56745,67.6727);
    glVertex2f(52.85382,67.72736);
    glVertex2f(52.66694,68.17941);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//Z51, U52
    glVertex2f(49.68701,63.025);
    glVertex2f(49.38981,68.1988);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(0.7);
    glBegin(GL_LINES);//L52, U52
    glVertex2f(48.68689,68.1137);
    glVertex2f(49.38981,68.1988);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//K52, V52
    glVertex2f(48.59074,67.26758);
    glVertex2f(48.75454,63.11842);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//Z51, W52
    glVertex2f(49.68701,63.025);
    glVertex2f(49.42059,67.24395);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//V52, Z51
    glVertex2f(48.75454,63.11842);
    glVertex2f(49.68701,63.025);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);//K52, W52
    glVertex2f(48.59074,67.26758);
    glVertex2f(49.42059,67.24395);
    glEnd();

    glColor3ub(192, 192, 192);
    glBegin(GL_POLYGON);//FGHI54
    glVertex2f(51.07557,64.41339);
    glVertex2f(51.16943,64.05879);
    glVertex2f(52.78079,64.09529);
    glVertex2f(52.85901,64.41339);
    glEnd();

    glColor3ub(r, g, b);
    glLineWidth(1.5);
    glBegin(GL_LINES);//Z51, E54
    glVertex2f(49.68701,63.025);
    glVertex2f(49.55298,65.76055);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(47.82228,64.36491);//J23
    glVertex2f(47.74377,64.32329);//F24
    glVertex2f(47.70783,64.26819);//G24
    glVertex2f(47.68627,64.21069);//H24
    glVertex2f(47.65033,64.15558);//I24
    glVertex2f(47.62877,64.10527);//J24
    glVertex2f(47.61679,64.0286);//K24
    glVertex2f(47.59283,63.95912);//L24
    glVertex2f(47.57366,63.86568);//P25
    glVertex2f(47.56648,63.801);//Q25
    glVertex2f(48.14627,63.84891);//L63
    glVertex2f(48.12471,63.95433);//M63
    glVertex2f(48.10554,64.01662);//P63
    glVertex2f(48.08398,64.07412);//Q63
    glVertex2f(48.04804,64.146);//R63
    glVertex2f(48.01375,64.20764);//S63
    glVertex2f(47.9546,64.27537);//T63
    glVertex2f(47.92998,64.30337);//U63
    glVertex2f(47.88383,64.33756);//Z63
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(47.56648,63.801);//Q25
    glVertex2f(47.58325,63.35297);//R25
    glVertex2f(48.16304,62.96245);//K63
    glVertex2f(48.25488,63.15);//H53
    glVertex2f(48.14627,63.84891);//L63
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(47.58325,63.35297);//R25
    glVertex2f(47.58396,63.2639);//T25
    glVertex2f(47.6,63.2);//V25
    glVertex2f(47.63117,63.12297);//G62
    glVertex2f(47.66231,63.02953);//H62
    glVertex2f(47.71742,62.9337);//I62
    glVertex2f(47.76533,62.88578);//J62
    glVertex2f(47.80367,62.83547);//F63
    glVertex2f(47.86117,62.79235);//G63
    glVertex2f(47.93065,62.73245);//H63
    glVertex2f(48.04804,62.69409);//I63
    glVertex2f(48.49294,62.69704);//J63
    glVertex2f(48.62552,62.77659);//K63
    glVertex2f(48.74338,62.92096);//F64
    glVertex2f(48.75454,63.11842);//V52
    glVertex2f(48.25488,63.15);//H53
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(44.79452,63.56287);//G64
    glVertex2f(44.77572,63.47265);//H64
    glVertex2f(44.71181,63.37867);//I64
    glVertex2f(44.65167,63.31852);//J64
    glVertex2f(44.5652,63.25462);//K64
    glVertex2f(44.25319,63.23582);//L64
    glVertex2f(44.17425,63.28845);//M64
    glVertex2f(44.09906,63.3486);//N64
    glVertex2f(44.05019,63.41626);//O64
    glVertex2f(43.99756,63.46889);//P64
    glVertex2f(43.95621,63.40499);//Q64
    glVertex2f(43.91486,63.35987);//R64
    glVertex2f(43.79456,63.34108);//S64
    glVertex2f(43.6254,63.34108);//T64
    glVertex2f(43.55022,63.38995);//U64
    glVertex2f(43.50135,63.48017);//V64
    glVertex2f(43.44872,63.58167);//W64
    glVertex2f(43.38105,63.74331);//Z64
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(43.38105,63.74331);//Z64
    glVertex2f(43.32354,64.26139);//D65
    glVertex2f(43.34077,64.40064);//E65
    glVertex2f(43.40522,64.49731);//F65
    glVertex2f(43.49796,64.52752);//G65
    glVertex2f(43.57294,64.5207);//H65
    glVertex2f(43.61736,64.49841);//M53
    glVertex2f(43.71428,64.22884);//N53
    glVertex2f(43.78092,63.93505);//O53
    glVertex2f(43.99756,63.46889);//P64
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(43.99756,63.46889);//P64
    glVertex2f(43.78092,63.93505);//O53
    glVertex2f(43.8,64.6);//P53
    glVertex2f(43.88996,64.72558);//Q53
    glVertex2f(44.04443,64.71649);//R53
    glVertex2f(44.15044,64.5711);//S53
    glVertex2f(44.23828,64.4136);//T53
    glVertex2f(44.27462,63.6261);//U53
    glEnd();
}

///AH486, AH487, AH488
void Bus3_400(GLubyte r, GLubyte g, GLubyte b){
    //Front Glass
    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//R31, M31, N31, O31, P31, Q31
    glVertex2f(129.51022,58.85073);
    glVertex2f(129.02098,55.57655);
    glVertex2f(129.35968,55.14375);
    glVertex2f(133,55.14375);
    glVertex2f(133.61235,55.67063);
    glVertex2f(133.83816,58.86954);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//R31, S31, T31, U31, V31, W31, G33, Q31
    glVertex2f(129.51022,58.85073);
    glVertex2f(129.87729,59.17169);
    glVertex2f(130.36674,59.35296);
    glVertex2f(131.21873,59.47079);
    glVertex2f(132.2792,59.50705);
    glVertex2f(134.68319,59.45974);
    glVertex2f(134.19696,58.82321);
    glVertex2f(133.83816,58.86954);
    glEnd();

    glColor3ub(r, g, b);
    glLineWidth(2);
    glBegin(GL_LINES);//R31, M31
    glVertex2f(129.51022,58.85073);
    glVertex2f(129.02098,55.57655);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//M31, F33, E33, D33, H33, O31
    glVertex2f(129.02098,55.57655);
    glVertex2f(128.91903,54.36717);
    glVertex2f(129.1905,53.0236);
    glVertex2f(134.00043,52.86548);
    glVertex2f(134.00043,54.73701);
    glVertex2f(133,55.14375);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//Q31, G33, H33, O31
    glVertex2f(133.83816,58.86954);
    glVertex2f(134.19696,58.82321);
    glVertex2f(134.00043,54.73701);
    glVertex2f(133,55.14375);
    glEnd();

    //side Glass
    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//M33, G33, H33, O33, P33, I33, J33, K33, L33, N33
    glVertex2f(134.5,59);
    glVertex2f(134.19696,58.82321);
    glVertex2f(134.00043,54.73701);
    glVertex2f(134.63698,55.447);
    glVertex2f(134.89375,56.22683);
    glVertex2f(135.09267,56.71802);
    glVertex2f(140.14166,56.56586);
    glVertex2f(140.14166,57.94343);
    glVertex2f(139.9644,58.12069);
    glVertex2f(135.1315,59.02278);
    glEnd();

    //Upper Body
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//N33, M33, W31, Z31, A32, B32, Q33, J33, K33, L33
    glVertex2f(135.1315,59.02278);
    glVertex2f(134.5,59);
    glVertex2f(134.68319,59.45974);
    glVertex2f(140.09525,58.30432);
    glVertex2f(140.25746,58.10967);
    glVertex2f(139.9644,58.12069);
    glEnd();

    //Upper Body Back
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//A32, B32, Q33, J33, K33, L33
    glVertex2f(140.25746,58.10967);
    glVertex2f(140.32234,57.85942);
    glVertex2f(140.35089,56.52461);
    glVertex2f(140.14166,56.56586);
    glVertex2f(140.14166,57.94343);
    glVertex2f(139.9644,58.12069);
    glEnd();

    //Middle Body
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//I33, P33, O33, H33, D33, C33, B33, A33, Z32, W32
    glVertex2f(135.09267,56.71802);
    glVertex2f(134.89375,56.22683);
    glVertex2f(134.63698,55.447);
    glVertex2f(134.00043,54.73701);
    glVertex2f(134.00043,52.86548);
    glVertex2f(134.75836,53.11135);
    glVertex2f(134.76529,53.66645);
    glVertex2f(134.82774,54.1591);
    glVertex2f(134.98733,54.60318);
    glVertex2f(135.26488,54.83215);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//I33, W32, V32, U32, T32, S32, R32, Q32, P32, O32, N32, M32, L32, R33
    glVertex2f(135.09267,56.71802);
    glVertex2f(135.26488,54.83215);
    glVertex2f(135.44529,54.81134);
    glVertex2f(135.58833,54.57402);
    glVertex2f(135.71866,54.11062);
    glVertex2f(135.71866,53.22001);
    glVertex2f(138.5889,53.60759);
    glVertex2f(138.63172,54.16416);
    glVertex2f(138.67453,54.42104);
    glVertex2f(138.73569,54.64734);
    glVertex2f(138.80908,54.77578);
    glVertex2f(138.91918,54.91033);
    glVertex2f(139.02685,55.02594);
    glVertex2f(139.0172,56.51965);
    glEnd();

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);//R33, L32, K32, J32, I32, H32, G32, F32, E32, D32, C32, Q33, J33
    glVertex2f(139.0172,56.51965);
    glVertex2f(139.02685,55.02594);
    glVertex2f(139.15771,54.92868);
    glVertex2f(139.26168,54.63511);
    glVertex2f(139.33508,54.3293);
    glVertex2f(139.31061,53.63205);
    glVertex2f(139.48187,53.60759);
    glVertex2f(139.76321,53.69321);
    glVertex2f(140.00786,53.8033);
    glVertex2f(140.20358,53.99291);
    glVertex2f(140.30143,54.2559);
    glVertex2f(140.35089,56.52461);
    glVertex2f(140.14166,56.56586);
    glEnd();

    //looking glass
    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//LMNO54
    glVertex2f(128.43152,57.30168);
    glVertex2f(128.52149,57.18547);
    glVertex2f(128.68269,57.19671);
    glVertex2f(128.75104,57.31104);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//KLOP54
    glVertex2f(128.46901,57.98021);
    glVertex2f(128.43152,57.30168);
    glVertex2f(128.75104,57.31104);
    glVertex2f(128.76512,57.89406);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//IJKPQ54
    glVertex2f(128.74642,58.32134);
    glVertex2f(128.57772,58.1489);
    glVertex2f(128.46901,57.98021);
    glVertex2f(128.76512,57.89406);
    glVertex2f(128.87497,58.05741);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//HIQR54
    glVertex2f(128.93386,58.42256);
    glVertex2f(128.74642,58.32134);
    glVertex2f(128.87497,58.05741);
    glVertex2f(129.06649,58.20387);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//FGHRS54
    glVertex2f(129.33123,58.47879);
    glVertex2f(129.1063,58.48254);
    glVertex2f(128.93386,58.42256);
    glVertex2f(129.06649,58.20387);
    glVertex2f(129.29463,58.23204);
    glEnd();

    //Wheel Front
    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//C33, B33, A33, Z32, W32, V32, U32, T32, H66, G66
    glVertex2f(134.75836,53.11135);
    glVertex2f(134.76529,53.66645);
    glVertex2f(134.82774,54.1591);
    glVertex2f(134.98733,54.60318);
    glVertex2f(135.26488,54.83215);
    glVertex2f(135.44529,54.81134);
    glVertex2f(135.58833,54.57402);
    glVertex2f(135.71866,54.11062);
    glVertex2f(135.70711,53.40421);
    glVertex2f(135.65031,53.23134);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//C33, I65, J65, K65, L65, M65, N65, O65, P65, Q65, R65, S65, T65, U65, V65, W65, Z65, A66, B66, C66, D66, E66, F66, G66
    glVertex2f(134.75836,53.11135);
    glVertex2f(134.24721,52.93963);
    glVertex2f(134.28906,52.85212);
    glVertex2f(134.32711,52.77221);
    glVertex2f(134.38038,52.69611);
    glVertex2f(134.41843,52.64284);
    glVertex2f(134.47551,52.58576);
    glVertex2f(134.51736,52.5401);
    glVertex2f(134.58585,52.49444);
    glVertex2f(134.65815,52.464);
    glVertex2f(134.73425,52.45259);
    glVertex2f(134.82177,52.44117);
    glVertex2f(134.90928,52.44498);
    glVertex2f(135.02343,52.4602);
    glVertex2f(135.1169,52.48555);
    glVertex2f(135.21074,52.51518);
    glVertex2f(135.30211,52.5621);
    glVertex2f(135.3762,52.62631);
    glVertex2f(135.43053,52.70286);
    glVertex2f(135.49967,52.78683);
    glVertex2f(135.54906,52.90043);
    glVertex2f(135.6,53);
    glVertex2f(135.6355,53.12762);
    glVertex2f(135.65031,53.23134);
    glEnd();

    //Wheel Back
    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//K67, I32, J32, K32, L32, M32, N32, O32, P32, Q32, R32
    glVertex2f(139.28227,53.78962);
    glVertex2f(139.33508,54.3293);
    glVertex2f(139.26168,54.63511);
    glVertex2f(139.15771,54.92868);
    glVertex2f(139.02685,55.02594);
    glVertex2f(138.91918,54.91033);
    glVertex2f(138.80908,54.77578);
    glVertex2f(138.73569,54.64734);
    glVertex2f(138.67453,54.42104);
    glVertex2f(138.63172,54.16416);
    glVertex2f(138.5889,53.60759);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//R32, IJKLMNOPQRSTUVWZ66, ABCDEFGHIJK67
    glVertex2f(138.5889,53.60759);
    glVertex2f(138.23777,53.51335);
    glVertex2f(138.26004,53.4521);
    glVertex2f(138.27396,53.40476);
    glVertex2f(138.2851,53.35743);
    glVertex2f(138.30459,53.30452);
    glVertex2f(138.32687,53.25162);
    glVertex2f(138.35471,53.2015);
    glVertex2f(138.39091,53.14859);
    glVertex2f(138.44103,53.09569);
    glVertex2f(138.48558,53.05671);
    glVertex2f(138.54405,53.02329);
    glVertex2f(138.6,53);
    glVertex2f(138.66657,52.9871);
    glVertex2f(138.72504,52.97874);
    glVertex2f(138.79465,52.97317);
    glVertex2f(138.8521,52.96598);
    glVertex2f(138.92142,52.97822);
    glVertex2f(139,53);
    glVertex2f(139.05801,53.05569);
    glVertex2f(139.1049,53.11685);
    glVertex2f(139.14975,53.19024);
    glVertex2f(139.18645,53.27791);
    glVertex2f(139.20888,53.34722);
    glVertex2f(139.23334,53.441);
    glVertex2f(139.2415,53.51847);
    glVertex2f(139.25373,53.6306);
    glVertex2f(139.28227,53.78962);
    glEnd();

    glColor3ub(192, 192, 192);
    glBegin(GL_POLYGON);//LMNOPQRSTUVWZ67, ABCDEF68
    glVertex2f(138.79094,54.12376);
    glVertex2f(138.7928,53.52698);
    glVertex2f(138.80948,53.46952);
    glVertex2f(138.83172,53.4028);
    glVertex2f(138.86508,53.34349);
    glVertex2f(138.91141,53.28419);
    glVertex2f(138.96887,53.26565);
    glVertex2f(139.03744,53.31755);
    glVertex2f(139.08192,53.38056);
    glVertex2f(139.11899,53.46767);
    glVertex2f(139.1505,53.61408);
    glVertex2f(139.17644,54.20716);
    glVertex2f(139.1505,54.29056);
    glVertex2f(139.09304,54.34061);
    glVertex2f(139.03559,54.37953);
    glVertex2f(138.97999,54.3647);
    glVertex2f(138.904,54.32392);
    glVertex2f(138.85766,54.26091);
    glVertex2f(138.81874,54.1979);
    glEnd();

    glColor3ub(192, 192, 192);
    glBegin(GL_POLYGON);//GHIJKLMNOPQRSTUVWZ68
    glVertex2f(134.93188,53.77766);
    glVertex2f(134.92845,53.14939);
    glVertex2f(134.95071,53.08091);
    glVertex2f(135,53);
    glVertex2f(135.04829,52.92855);
    glVertex2f(135.13046,52.90972);
    glVertex2f(135.24344,52.94738);
    glVertex2f(135.32048,53.02271);
    glVertex2f(135.37526,53.14254);
    glVertex2f(135.42148,53.24868);
    glVertex2f(135.45059,53.3805);
    glVertex2f(135.44887,53.79136);
    glVertex2f(135.40436,53.93173);
    glVertex2f(135.34616,54.02589);
    glVertex2f(135.3,54.1);
    glVertex2f(135.14758,54.12175);
    glVertex2f(135.06198,54.03616);
    glVertex2f(134.99864,53.9266);
    glEnd();
}

///AH428
void truck1_400(){
    glPushMatrix();
    glTranslatef(truck1_400_x, 0.0f, 0.0f);
    //Main body
    glColor4ub(92,76,102,255);
    glBegin(GL_POLYGON);
    glVertex2f(111.7216192373069, 32.4192824188013);
    glVertex2f(115.1251962536539, 32.4614070396603);
    glVertex2f(115.1251962536539, 35.1005038030414);
    glVertex2f(113.9902400432318, 36.7847884391204);
    glVertex2f(113.6297104755453, 37.0191326581166);
    glVertex2f(111.7216192373069, 37.0191326581166);
    glEnd();

    glColor4ub(92,76,102,255);
    glBegin(GL_POLYGON);
    glVertex2f(97.6812314933094, 32.4614070396603);
    glVertex2f(111.7216192373069, 32.4614070396603);
    glVertex2f(111.7216192373069, 33.6997331978843);
    glVertex2f(97.6812314933094, 33.6997331978843);
    glEnd();

    ///Payload
    glColor4ub(243,239,240,255);
    glBegin(GL_POLYGON);
    glVertex2f(98.104650699739, 33.6997331978843);
    glVertex2f(111.3384434283056, 33.6997331978843);
    glVertex2f(111.3384434283056, 38.1874201377858);
    glVertex2f(98.104650699739, 38.1874201377858);
    glEnd();

    ///Window
    glColor4ub(210,237,246,255);
    glBegin(GL_POLYGON);
    glVertex2f(112.452235144923, 35.1437936098002);
    glVertex2f(114.6178994175021, 34.7582556452568);
    glVertex2f(114.8244814855163, 34.7662011094112);
    glVertex2f(114.9674998402954, 34.8774376075727);
    glVertex2f(114.9833907686042, 35.0919651397413);
    glVertex2f(113.9345895002244, 36.6492761140025);
    glVertex2f(113.7756802171366, 36.7922944687815);
    glVertex2f(113.5663970222538, 36.8723829930122);
    glVertex2f(112.452235144923, 36.8723829930122);
    glEnd();

    ///Window side lines
    glLineWidth(4);
    glColor4ub(46,45,41,255);
    glBegin(GL_LINES);
    glVertex2f(112.2234059396675, 36.894058956835);
    glVertex2f(112.2282777832564, 35.1937855442905);
    glEnd();

    glLineWidth(2.5);
    glColor4ub(168,161,179,255);
    glBegin(GL_LINES);
    glVertex2f(112.0123428883572, 35.1291161658037);
    glVertex2f(114.6874568989323, 34.6540528156497);
    glEnd();

    glLineWidth(0.8);
    glColor4ub(168,161,179,255);
    glBegin(GL_LINES);
    glVertex2f(112.0123428883572, 35.1291161658037);
    glVertex2f(112.0123428883572, 36.8725525382133);
    glEnd();

    ///Door handle
    glLineWidth(3.5);
    glColor4ub(46,45,41,255);
    glBegin(GL_LINES);
    glVertex2f(111.9413948854505, 34.3354576985083);
    glVertex2f(112.4183299063433, 34.3354576985083);
    glEnd();

    ///White Headlights
    glColor4ub(202,202,204,255);
    glBegin(GL_POLYGON);
    glVertex2f(115.1251962536539, 32.5011801625979);
    glVertex2f(115.3313169902735, 32.6923032989542);
    glVertex2f(115.3313169902735, 33.1650182593601);
    glVertex2f(115.1251962536539, 33.2844956669352);
    glEnd();

    ///Red Headlights
    glColor4ub(183,42,51,255);
    glBegin(GL_POLYGON);
    glVertex2f(115.1251962536539, 33.7260426079737);
    glVertex2f(115.3313169902735, 33.565007841242);
    glVertex2f(115.3313169902735, 33.1650182593601);
    glVertex2f(115.1251962536539, 33.2844956669352);
    glEnd();

    ///Back lignts
    glColor4ub(183,42,51,255);
    glBegin(GL_POLYGON);
    glVertex2f(97.6643558030399, 32.814126554161);
    glVertex2f(97.9022460811503, 32.814126554161);
    glVertex2f(97.9022460811503, 33.6929989705134);
    glVertex2f(97.7040041827249, 33.4749328822455);
    glEnd();

    glPushMatrix();
    glTranslatef(99.705857227896, 32.4167903883552, 0);
    glRotatef(-_angle1, 0, 0, 1);
    glTranslatef(-99.705857227896, -32.4167903883552, 0);
    glPopMatrix();

    glPointSize(3);
    glBegin(GL_POINTS);
    glColor4ub(58,49,47,255);
    glVertex2f(99.705857227896, 32.4167903883552);
    glEnd();

    glPushMatrix();
    glTranslatef(101.9997807106927, 32.4382289255776, 0);
    glRotatef(-_angle1, 0, 0, 1);
    glTranslatef(-101.9997807106927, -32.4382289255776, 0);
    glPopMatrix();

    glPointSize(3);
    glBegin(GL_POINTS);
    glColor4ub(58,49,47,255);
    glVertex2f(101.9997807106927, 32.4382289255776);
    glEnd();

    glPushMatrix();
    glTranslatef(112.7833649335594, 32.3953518511328, 0);
    glRotatef(-_angle1, 0, 0, 1);
    glTranslatef(-112.7833649335594, -32.3953518511328, 0);
    glPopMatrix();

    glPointSize(3);
    glBegin(GL_POINTS);
    glColor4ub(58,49,47,255);
    glVertex2f(112.7833649335594, 32.3953518511328);
    glEnd();

    Truck1Wheel(99.705857227896f, 32.4167903883552f, 1.1f, _angle1);
    Truck1Wheel(101.9997807106927f, 32.4382289255776f, 1.1f, _angle1);
    Truck1Wheel(112.7833649335594f, 32.3953518511328f, 1.1f, _angle1);
    glPopMatrix();
}

///AH429
void truck2_400(){
    glPushMatrix();
    glTranslatef(truck2_400_x, 0.0f, 0.0f);

    glColor3ub(255,239,0);
    glBegin(GL_POLYGON);//H44, I44, J44, S44, M44, N44
    glVertex2f(63.40959,24.68511);
    glVertex2f(62.88233,23.29865);
    glVertex2f(62.87181,21.51925);
    glVertex2f(66.2165,21.53429);
    glVertex2f(66.23488,22.49258);
    glVertex2f(66.20582,24.6862);
    glEnd();

    glColor3ub(255,239,0);
    glBegin(GL_POLYGON);//O44, P44, Q44, R44
    glVertex2f(64.44802,24.70072);
    glVertex2f(65.16712,25.35445);
    glVertex2f(66,25.5);
    glVertex2f(66.03842,24.68699);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//M45, N45, O45, P45
    glVertex2f(65.33,24.32);
    glVertex2f(65.33,23.5);
    glVertex2f(66,23.5);
    glVertex2f(66,24.31592);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//Q45, R45
    glVertex2f(63.48301,24.46043);
    glVertex2f(63.05452,23.35241);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//R45, S45
    glVertex2f(63.05452,23.35241);
    glVertex2f(63.05452,21.98903);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//S45, T45
    glVertex2f(63.05452,21.98903);
    glVertex2f(63.18436,21.92843);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//T45, U45
    glVertex2f(63.18436,21.92843);
    glVertex2f(63.31421,21.94142);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//U45, V45
    glVertex2f(63.31421,21.94142);
    glVertex2f(63.43973,22.16216);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//V45, W45
    glVertex2f(63.43973,22.16216);
    glVertex2f(63.66047,22.40021);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//W45, Z45
    glVertex2f(63.66047,22.40021);
    glVertex2f(64.04135,22.57333);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//Z45, A46
    glVertex2f(64.04135,22.57333);
    glVertex2f(65.03683,22.58632);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//A46, B46
    glVertex2f(65.03683,22.58632);
    glVertex2f(65.15369,22.71184);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//B46, C46
    glVertex2f(65.15369,22.71184);
    glVertex2f(65.15802,24.34789);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//C46, D46
    glVertex2f(65.15802,24.34789);
    glVertex2f(65.05414,24.45177);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(1.5);
    glBegin(GL_LINES);//D46, Q45
    glVertex2f(65.05414,24.45177);
    glVertex2f(63.48301,24.46043);
    glEnd();

    glColor4ub(147,204,222,255);
    glBegin(GL_POLYGON);//E46, F46, G46, H46
    glVertex2f(63.60853,24.30894);
    glVertex2f(63.29257,23.48225);
    glVertex2f(64.95027,23.46927);
    glVertex2f(64.96325,24.32625);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//I46, J46, K46, L46
    glVertex2f(64.59968,23.36106);
    glVertex2f(64.6,23.2);
    glVertex2f(64.97191,23.19659);
    glVertex2f(64.96758,23.35241);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//M44, S44, K44, L44
    glVertex2f(66.23488,22.49258);
    glVertex2f(66.2165,21.53429);
    glVertex2f(72.62688,21.57009);
    glVertex2f(72.60509,22.48531);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//T44, U44, V44, W44
    glVertex2f(74.56,22.48224);
    glVertex2f(74.57061,21.54872);
    glVertex2f(81.25371,21.58055);
    glVertex2f(81.24311,22.50345);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//Z44, A45, B45, C45
    glVertex2f(66.48894,26.30503);
    glVertex2f(66.48894,22.73353);
    glVertex2f(82.40681,22.75222);
    glVertex2f(82.39412,26.33116);
    glEnd();

    glColor3ub(255,239,0);
    glBegin(GL_POLYGON);//D45, E45, F45, G45
    glVertex2f(66.67577,26.12212);
    glVertex2f(66.66974,22.89796);
    glVertex2f(82.24648,22.91142);
    glVertex2f(82.23199,26.11484);
    glEnd();

    for(int i=0; i<12; i++){
        float offset = i * (0.23 + 1.05);

        glColor3ub(1, 1, 1);
        glBegin(GL_POLYGON);//H45, I45, J45, K45
        glVertex2f(67.25 + offset,25.72);
        glVertex2f(67.48 + offset,25.72);
        glVertex2f(67.48 + offset,23.3);
        glVertex2f(67.25 + offset,23.3);
        glEnd();
    }

    Truck2Wheel_400(64.2734, 21.5561, 0.75, truckWheelAngle_400);
    Truck2Wheel_400(69.17664, 21.5444, 0.75, truckWheelAngle_400);
    Truck2Wheel_400(71.25404, 21.55945, 0.75, truckWheelAngle_400);
    Truck2Wheel_400(75.75542, 21.55042, 0.75, truckWheelAngle_400);
    Truck2Wheel_400(77.81815, 21.54685, 0.75, truckWheelAngle_400);
    Truck2Wheel_400(79.87081, 21.53313, 0.75, truckWheelAngle_400);
    glPopMatrix();
}

///AH432, AH433
void cng_400(){
    glPushMatrix();
    glTranslatef(cng_400X, 0.0f, 0.0f);

    glColor3ub(139, 150, 139);
    glBegin(GL_POLYGON);
    glVertex2f(72.42958,37.83121);
    glVertex2f(73.86755,37.83121);
    glVertex2f(73.96005,37.79337);
    glVertex2f(74.0248,37.724);
    glVertex2f(74.09497,37.34795);
    glVertex2f(74.21347,37.35178);
    glVertex2f(74.24537,36.94206);
    glVertex2f(74.30855,36.79991);
    glVertex2f(74.40332,36.73357);
    glVertex2f(74.48229,36.69566);
    glVertex2f(74.91823,36.7083);//M
    glVertex2f(75.33521,36.63248);//V
    glVertex2f(73.6,36.6);
    glVertex2f(73.52513,36.69882);
    glVertex2f(73.41773,36.84097);
    glVertex2f(72.86175,36.85361);
    glVertex2f(72.8,36.8);
    glVertex2f(72.68801,36.66723);
    glVertex2f(72.43213,36.65776);
    glEnd();

    glColor3ub(139, 150, 139);
    glBegin(GL_POLYGON);//MNOPQRSTUV
    glVertex2f(74.91823,36.7083);//M
    glVertex2f(74.9972,36.76516);
    glVertex2f(75.04143,36.86625);
    glVertex2f(75.05722,36.98313);
    glVertex2f(75.0509,37.71285);
    glVertex2f(75.15831,37.71285);
    glVertex2f(75.16147,37.31798);
    glVertex2f(75.40471,37.31482);
    glVertex2f(75.41418,36.72093);
    glVertex2f(75.33521,36.63248);//V
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//Q, R2
    glVertex2f(75.0509,37.71285);
    glVertex2f(75.07223,39.23579);
    glEnd();
    glColor3ub(139, 150, 139);
    glBegin(GL_POLYGON);//W, H1, G1, F1
    glVertex2f(73.6,36.6);
    glVertex2f(73.61201,36.4671);
    glVertex2f(75.80401,36.49779);
    glVertex2f(76.29501,36.63807);
    glEnd();

    glColor3ub(139, 150, 139);
    glBegin(GL_POLYGON);//E3, U, V, F1, I1, J1
    glVertex2f(75.41, 36.8);
    glVertex2f(75.41418,36.72093);
    glVertex2f(75.33521,36.63248);
    glVertex2f(76.29501,36.63807);
    glVertex2f(76.41777,36.72137);
    glVertex2f(76.45583,36.80894);
    glEnd();

    glColor3ub(139, 150, 139);
    glBegin(GL_POLYGON);//LKJIFG1, JIHGFEDCBA2, ZW1
    glVertex2f(76.54052,38.07602);
    glVertex2f(76.50106,37.00194);
    glVertex2f(76.45583,36.80894);
    glVertex2f(76.41777,36.72137);
    glVertex2f(76.29501,36.63807);
    glVertex2f(75.80401,36.49779);
    glVertex2f(76.70213,36.4853);
    glVertex2f(76.8,36.6);
    glVertex2f(76.88666,36.67834);
    glVertex2f(76.97182,36.7919);
    glVertex2f(77.03996,36.90829);
    glVertex2f(77.11377,37.0843);
    glVertex2f(77.16487,37.22057);
    glVertex2f(77.20461,37.35967);
    glVertex2f(77.25003,37.53569);
    glVertex2f(77.26139,37.66344);
    glVertex2f(77.2983,37.80538);
    glVertex2f(77.30422,38.08024);
    glEnd();

    glColor3ub(139, 150, 139);
    glBegin(GL_POLYGON);//LMNOVW1
    glVertex2f(76.54052,38.07602);
    glVertex2f(76.4353,39.10626);
    glVertex2f(76.44407,39.22024);
    glVertex2f(76.55689,39.33949);
    glVertex2f(77.18652,38.31602);
    glVertex2f(77.30422,38.08024);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//KLMZ2,ABCD3
    glVertex2f(72.39321,37.89904);
    glVertex2f(73.81558,37.91285);
    glVertex2f(73.8211,39.15017);
    glVertex2f(72.8,39.4);
    glVertex2f(72.55907,39.02839);
    glVertex2f(72.44879,38.57055);
    glVertex2f(72.41871,38.3266);
    glVertex2f(72.39866,38.07261);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);//MNOPQRS2, F3, TUVWZ2
    glVertex2f(73.8211,39.15017);
    glVertex2f(74.12767,39.17226);
    glVertex2f(74.27957,39.18607);
    glVertex2f(74.43424,39.20817);
    glVertex2f(74.65519,39.23026);
    glVertex2f(75.07223,39.23579);
    glVertex2f(76.37883,39.23558);
    glVertex2f(76.65858,39.41507);
    glVertex2f(76.37883,39.56309);
    glVertex2f(75.06213,39.59985);
    glVertex2f(73.85906,39.59316);
    glVertex2f(73.12719,39.58982);
    glVertex2f(72.8,39.4);
    glEnd();

    glColor3ub(139, 150, 139);
    glBegin(GL_POLYGON);//E2, GHIJKLMNOPQRSTUVWZ3, ABCDEFGHIJKLM4, J2
    glVertex2f(77.11377,37.0843);
    glVertex2f(77.25443,37.10074);
    glVertex2f(77.33303,37.1153);
    glVertex2f(77.41453,37.10948);
    glVertex2f(77.49022,37.10074);
    glVertex2f(77.56881,37.07455);
    glVertex2f(77.62994,37.03379);
    glVertex2f(77.69399,36.99013);
    glVertex2f(77.73183,36.93773);
    glVertex2f(77.76385,36.88242);
    glVertex2f(77.79587,36.84167);
    glVertex2f(77.8,36.8);
    glVertex2f(77.84262,36.72056);
    glVertex2f(77.76967,36.73979);
    glVertex2f(77.71727,36.75143);
    glVertex2f(77.65032,36.76599);
    glVertex2f(77.60084,36.7689);
    glVertex2f(77.53388,36.77472);
    glVertex2f(77.46984,36.78345);
    glVertex2f(77.40289,36.77472);
    glVertex2f(77.32721,36.7689);
    glVertex2f(77.25734,36.74852);
    glVertex2f(77.21077,36.7427);
    glVertex2f(77.12344,36.72814);
    glVertex2f(77.07395,36.70194);
    glVertex2f(76.98954,36.6641);
    glVertex2f(76.93423,36.62335);
    glVertex2f(76.84981,36.29441);
    glVertex2f(76.76539,36.27112);
    glVertex2f(76.71882,36.2915);
    glVertex2f(76.66351,36.27112);
    glVertex2f(76.63149,36.32352);
    glVertex2f(76.70213,36.4853);
    glEnd();

    CNGWheel_400(73.1163,36.32464, 0.5, cngWheelAngle_400);
    CNGWheel_400(77.37296,36.31694, 0.5, cngWheelAngle_400);
    glPopMatrix();
}

///AH406, AH407, AH408, AH409
void Bench_400(){
    glColor3ub(166, 123, 191);
    glBegin(GL_POLYGON);
    glVertex2f(112, 63.4);
    glVertex2f(111.9, 63);
    glVertex2f(114.8, 63);
    glVertex2f(114.8, 63.4);
    glEnd();

    glColor3ub(166, 123, 191);
    glBegin(GL_POLYGON);
    glVertex2f(112.2, 63);
    glVertex2f(112.2, 62.55);
    glVertex2f(112.25, 62.55);
    glVertex2f(112.25, 63);
    glEnd();

    glColor3ub(166, 123, 191);
    glBegin(GL_POLYGON);
    glVertex2f(114.44, 63);
    glVertex2f(114.44, 62.55);
    glVertex2f(114.51, 62.55);
    glVertex2f(114.51, 63);
    glEnd();

    glColor3ub(166, 123, 191);
    glBegin(GL_POLYGON);
    glVertex2f(111.88, 62.57);
    glVertex2f(111.67, 62.53);
    glVertex2f(115, 62.53);
    glVertex2f(114.79, 62.57);
    glEnd();

    glColor3ub(166, 123, 191);
    glBegin(GL_POLYGON);
    glVertex2f(111.67, 62.53);
    glVertex2f(111.67, 62.4);
    glVertex2f(115, 62.4);
    glVertex2f(115, 62.53);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(112.08, 62.4);
    glVertex2f(112.08, 61.84);
    glVertex2f(112, 61.84);
    glVertex2f(112, 61.77);
    glVertex2f(112.22, 61.77);
    glVertex2f(112.22, 61.84);
    glVertex2f(112.13, 61.84);
    glVertex2f(112.13, 62.4);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(114.46, 62.4);
    glVertex2f(114.46, 61.84);
    glVertex2f(114.38, 61.84);
    glVertex2f(114.38, 61.77);
    glVertex2f(114.62, 61.77);
    glVertex2f(114.62, 61.84);
    glVertex2f(114.51, 61.84);
    glVertex2f(114.51, 62.4);
    glEnd();
}

///AH410
void TerminalBuilding_1_400(){
    //Floor
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//U, W, Z, A1
    glVertex2f(105, 65),
    glVertex2f(98, 58);
    glVertex2f(140.01756,58);
    glVertex2f(132.01, 65);
    glEnd();

    //Floor
    glColor3ub(178, 190, 181);
    glBegin(GL_POLYGON);//U, V, B1, A1
    glVertex2f(105, 65),
    glVertex2f(100, 60);
    glVertex2f(137.8, 60);
    glVertex2f(132.01, 65);
    glEnd();

    //Roof
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//C1, D1, E1, F1
    glVertex2f(100, 75);
    glVertex2f(137.34553,75.04617);
    glVertex2f(137.32418,76.01514);
    glVertex2f(100, 76);
    glEnd();

    //CounterRoom1:
    glColor3ub(219, 215, 210);
    glBegin(GL_POLYGON);//Z1, A2, B2, C2
    glVertex2f(110, 62.5);
    glVertex2f(110, 72.5);
    glVertex2f(128, 72.5);
    glVertex2f(128, 62.5);
    glEnd();

    //CounterRoom1: Window1
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);//D2, E2, F2, G2
    glVertex2f(112, 68.3);
    glVertex2f(112, 64.6);
    glVertex2f(114, 64.6);
    glVertex2f(114, 68.3);
    glEnd();

    //CounterRoom1: Window2
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);//H2, I2, J2, K2
    glVertex2f(115, 68.3);
    glVertex2f(115, 64.6);
    glVertex2f(117, 64.6);
    glVertex2f(117, 68.3);
    glEnd();

    //CounterRoom1: Window3
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);//L2, M2, N2, O2
    glVertex2f(119.6, 68.3);
    glVertex2f(119.6, 64.6);
    glVertex2f(121.5, 64.6);
    glVertex2f(121.5, 68.3);
    glEnd();

    //CounterRoom1: Window4
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);//P2, Q2, R2, S2
    glVertex2f(122.4, 68.3);
    glVertex2f(122.4, 64.6);
    glVertex2f(124.4, 64.6);
    glVertex2f(124.4, 68.3);
    glEnd();

    //CounterRoom1: Window5
    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);// T2, U2, V2, W2
    glVertex2f(125.4, 68.3);
    glVertex2f(125.4, 64.6);
    glVertex2f(127.3, 64.6);
    glVertex2f(127.3, 68.3);
    glEnd();


    ///AH406
    glPushMatrix();//Bench1
    glTranslatef(-1, 0, 0);
    Bench_400();
    glPopMatrix();

    ///AH407
    glPushMatrix();//Bench2
    glTranslatef(3, 0, 0);
    Bench_400();
    glPopMatrix();

    ///AH408
    glPushMatrix();//Bench3
    glTranslatef(8, 0, 0);//Move (write, up)
    Bench_400();
    glPopMatrix();

    ///AH409
    glPushMatrix();//Bench4
    glTranslatef(13, 0, 0);
    Bench_400();
    glPopMatrix();

    //Pillar1
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//P1, Q1, R1, S1
    glVertex2f(102, 75);
    glVertex2f(102, 62);
    glVertex2f(102.76686,62);
    glVertex2f(102.76686,75);
    glEnd();

    //Pillar2
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//I, J, K, L
    glVertex2f(105, 75);
    glVertex2f(105, 60);
    glVertex2f(105.91735517328193,60.048638680888686);
    glVertex2f(105.96238283177422,74.99398415482804);
    glEnd();

    //Pillar3
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//Q, R, S, T
    glVertex2f(118.01227,75.02391);
    glVertex2f(118.01227,60);
    glVertex2f(118.98272,60);
    glVertex2f(118.98272,75.02391);
    glEnd();

    //Pillar4
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//Q, R, S, T
    glVertex2f(131.00974,75);
    glVertex2f(131.00974,60);
    glVertex2f(132.02976,60);
    glVertex2f(132.02976,75);
    glEnd();

    //Pillar5
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//T1, U1, V1, W1
    glVertex2f(135.57, 62);
    glVertex2f(135.64, 75);
    glVertex2f(134.88, 75);
    glVertex2f(134.88, 62);
    glEnd();

    //Banner
    glColor3ub(255, 255, 204);
    glBegin(GL_POLYGON);//C1, Z13, A14, D1
    glVertex2f(100, 75);
    glVertex2f(100, 73.03);
    glVertex2f(137.35, 73.03);
    glVertex2f(137.34553,75.04617);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    renderText_400(113.0f, 73.5f, "Mohakhali Bus Terminal", 10);

    float y = 76.0f;
    semicircle_400(102.0f, y, 2, 0.2f, 0.2f, 0.2f);
    semicircle_400(106.0f, y, 2, 0.2f, 0.2f, 0.2f);
    semicircle_400(110.13f, y, 2.13, 0.2f, 0.2f, 0.2f);
    semicircle_400(114.265f, y, 2.005, 0.2f, 0.2f, 0.2f);
    semicircle_400(118.42f, y, 2.15, 0.2f, 0.2f, 0.2f);
    semicircle_400(122.645f, y, 2.075, 0.2f, 0.2f, 0.2f);
    semicircle_400(126.775f, y, 2.055, 0.2f, 0.2f, 0.2f);
    semicircle_400(130.965f, y, 2.135, 0.2f, 0.2f, 0.2f);
    semicircle_400(135.215f, y, 2.115, 0.2f, 0.2f, 0.2f);
}

///AH411
void TerminalBuilding_2_400(){
    //Floor
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//L13, M13, P13, K13
    glVertex2f(38, 66),
    glVertex2f(70, 66);
    glVertex2f(69.96, 69.08);
    glVertex2f(40.4, 69.08);
    glEnd();

    //Floor
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//M13, N13, O13, P13
    glVertex2f(70, 66);
    glVertex2f(87.13, 74.69);
    glVertex2f(79.9, 74.65);
    glVertex2f(69.96, 69.08);
    glEnd();

    //Roof
    glColor3ub(89,95,91);
    glBegin(GL_POLYGON);//J12, K12, L12, M12
    glVertex2f(39.86, 73.84);
    glVertex2f(64.5, 73.84);
    glVertex2f(77.52, 80.15);
    glVertex2f(52.5, 78.76);
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(62.86, 78.38);
    glVertex2f(60.78, 73.84);
    glVertex2f(64.55, 73.84);
    glEnd();

    semicircle_400(43.21f, 73.84, 3.35, 0.2f, 0.2f, 0.2f);
    semicircle_400(50.125f, 73.84, 3.565, 0.2f, 0.2f, 0.2f);
    semicircle_400(57.235f, 73.84, 3.545, 0.2f, 0.2f, 0.2f);
    semicircle_arc_400(67.88f, 75.48, 3.53, 28, 208, 0.2f, 0.2f, 0.2f);
    semicircle_arc_400(74.39f, 78.635, 3.28, 28, 208, 0.2f, 0.2f, 0.2f);

    //Pillar2.1
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//S12, T12, U12, V12
    glVertex2f(40.35, 73.84);
    glVertex2f(40.35, 67.24);
    glVertex2f(40.89, 67.24);
    glVertex2f(40.87, 73.84);
    glEnd();

    //Pillar2.2
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//W12, Z12, A13, B13
    glVertex2f(48.02, 73.84);
    glVertex2f(48.02, 67.2);
    glVertex2f(48.38, 67.2);
    glVertex2f(48.38, 73.84);
    glEnd();

    //Pillar2.3
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//C13, D13, E13, F13
    glVertex2f(54.82, 73.84);
    glVertex2f(54.8, 67.2);
    glVertex2f(55.21, 67.2);
    glVertex2f(55.21, 73.84);
    glEnd();

    //Pillar2.4
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//G13, H13, I13, J13
    glVertex2f(62.4, 73.84);
    glVertex2f(62.4, 67.2);
    glVertex2f(62.8, 67.2);
    glVertex2f(62.8, 73.84);
    glEnd();

    //Pillar2.5
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//Q13, P13, R13, S13
    glVertex2f(69.97, 76.49);
    glVertex2f(69.96, 69.08);
    glVertex2f(70.53, 69.08);
    glVertex2f(70.56, 76.78);
    glEnd();

    //Pillar2.6
    glColor3ub(61, 72, 64);
    glBegin(GL_POLYGON);//T13, U13, V13, W13
    glVertex2f(75.97, 79.4);
    glVertex2f(76, 72);
    glVertex2f(76.46, 72);
    glVertex2f(76.45, 79.63);
    glEnd();
}

///AH412
void GasStation_400(){
    glColor3ub(255, 204, 153);
    glBegin(GL_POLYGON);//K15, L15, M15, N15
    glVertex2f(10.59211,77.93101);
    glVertex2f(25.55148,77.93101);
    glVertex2f(25.5162,76.27278);
    glVertex2f(10.59211,76.34334);
    glEnd();

    glColor3ub(35, 9, 0);
    glBegin(GL_POLYGON);//M15, N15, O15, P15
    glVertex2f(25.5162,76.27278);
    glVertex2f(10.59211,76.34334);
    glVertex2f(2.90074,75.28489);
    glVertex2f(20.3651,75.10849);
    glEnd();

    glColor3ub(255, 204, 153);
    glBegin(GL_POLYGON);//J15, K15, N15, O15
    glVertex2f(2.7949,76.69616);
    glVertex2f(10.59211,77.93101);
    glVertex2f(10.59211,76.34334);
    glVertex2f(2.90074,75.28489);
    glEnd();

    glColor3ub(103, 103, 103);
    glBegin(GL_POLYGON);//E16, F16, G16, H16, I16
    glVertex2f(6.1688,65.98847);
    glVertex2f(22.29271,66.02444);
    glVertex2f(22.35682,65.12702);
    glVertex2f(9.32277,65.02018);
    glVertex2f(6.18178,65.27659);
    glEnd();

    glColor3ub(35, 9, 0);
    glBegin(GL_POLYGON);//Q15, R15, S15, T15
    glVertex2f(8.51136,75.41472);
    glVertex2f(8.51136,66.17978);
    glVertex2f(9.46868,66.15692);
    glVertex2f(9.49153,75.48349);
    glEnd();

    glColor3ub(35, 9, 0);
    glBegin(GL_POLYGON);//U15, V15, W15, Z15
    glVertex2f(11.18312,75.66637);
    glVertex2f(11.16026,66.08835);
    glVertex2f(12, 66);
    glVertex2f(11.96033,75.71209);
    glEnd();

    glColor3ub(35, 9, 0);
    glBegin(GL_POLYGON);//A16, B16, C16, D16
    glVertex2f(19.82391,75.52921);
    glVertex2f(20, 66);
    glVertex2f(20.784,65.99691);
    glVertex2f(20.78785,75.52349);
    glEnd();

    glColor3ub(255, 135, 94);
    glBegin(GL_POLYGON);//J16, K16, L16, M16, N16
    glVertex2f(22.3054,73.28471);
    glVertex2f(22.3054,71.95639);
    glVertex2f(24.38091,71.93563);
    glVertex2f(32.49614,71.95639);
    glVertex2f(32.45463,73.22245);
    glEnd();

    glColor3ub(255, 135, 94);
    glBegin(GL_POLYGON);//O16, P16, Q16, L16
    glVertex2f(22.7, 71.95);
    glVertex2f(22.69975,66.85065);
    glVertex2f(24.44317,66.82989);
    glVertex2f(24.38091,71.93563);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//L16, Q16
    glVertex2f(24.38091,71.93563);
    glVertex2f(24.44317,66.82989);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//O16, P16
    glVertex2f(22.7, 71.95);
    glVertex2f(22.69975,66.85065);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//J16, N16
    glVertex2f(22.3054,73.28471);
    glVertex2f(32.45463,73.22245);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//K16, L16
    glVertex2f(22.3054,71.95639);
    glVertex2f(24.38091,71.93563);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//L16, M16
    glVertex2f(24.38091,71.93563);
    glVertex2f(32.49614,71.95639);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//P16, Q16
    glVertex2f(22.69975,66.85065);
    glVertex2f(24.44317,66.82989);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//Q16, R16
    glVertex2f(24.44317,66.82989);
    glVertex2f(32.12255,66.80914);
    glEnd();

    glColor3ub(1, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);//J16, N16
    glVertex2f(22.3054,73.28471);
    glVertex2f(32.45463,73.22245);
    glEnd();

    glColor3ub(255, 135, 94);
    glBegin(GL_POLYGON);//Q16, R16, S16, L16
    glVertex2f(24.44317,66.82989);
    glVertex2f(32.12255,66.80914);
    glVertex2f(32.15, 71.96);
    glVertex2f(24.38091,71.93563);
    glEnd();

    glColor3ub(133, 34, 0);//Door
    glBegin(GL_POLYGON);//T16, U16, V16, W16
    glVertex2f(28.93, 66.82);
    glVertex2f(28.94702,70.69033);
    glVertex2f(30.98102,70.66958);
    glVertex2f(30.96, 66.81);
    glEnd();

    glColor4ub(147,204,222,255);//Window
    glBegin(GL_POLYGON);//Z16, A17, B17, C17
    glVertex2f(25.39791,70.60731);
    glVertex2f(25.39791,68.40727);
    glVertex2f(27.76398,68.40727);
    glVertex2f(27.847,70.66958);
    glEnd();
}

///AH464, AH465, AH466, AH467
void TeaStall_400() {
    glColor3ub(139, 69, 19);// Roof (Brown)
    glBegin(GL_POLYGON);
    glVertex2f(116.5, 46.5);
    glVertex2f(124.0, 46.5);
    glVertex2f(120.25, 48.0);
    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);// Banner
    glVertex2f(117.25, 46.5);
    glVertex2f(117.25, 44.5);
    glVertex2f(123.2, 44.5);
    glVertex2f(123.2, 46.5);
    glEnd();

    glColor3ub(54, 69, 79);
    glBegin(GL_POLYGON);
    glVertex2f(116.95, 42);
    glVertex2f(116.95, 40.5);
    glVertex2f(123.5, 40.5);
    glVertex2f(123.5, 42);
    glEnd();

    glColor3ub(205, 127, 50);
    glBegin(GL_POLYGON);
    glVertex2f(117.5, 44.5);
    glVertex2f(117.5, 42);
    glVertex2f(117.69, 42);
    glVertex2f(117.69, 44.5);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(122.81, 44.5);
    glVertex2f(122.81, 42.01);
    glVertex2f(123, 42);
    glVertex2f(123, 44.5);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(119.5, 42.0);
    glVertex2f(119.5, 42.7);
    glVertex2f(120.0, 42.7);
    glVertex2f(120.0, 42.0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(120.0, 42.5);
    glVertex2f(120.5, 42.4);
    glVertex2f(120.5, 42.6);
    glVertex2f(120.0, 42.7);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(119.5, 42.5);
    glVertex2f(119.3, 42.6);
    glVertex2f(119.3, 42.4);
    glEnd();

    glColor3ub(0, 0, 0);
    renderText_400(118.5, 45, "Tea Stall", 10);
}

///AH478
void FruitShop_400() {
    glColor3ub(139, 69, 19);
    glBegin(GL_TRIANGLES);
    glVertex2f(19.5, 16.5);
    glVertex2f(30.5, 16.5);
    glVertex2f(25.0, 18.0);
    glEnd();

    glColor3ub(255, 255, 0);
    glBegin(GL_POLYGON);//banner
    glVertex2f(20.69, 16.5);
    glVertex2f(20.69, 15.0);
    glVertex2f(29.31, 15.0);
    glVertex2f(29.31, 16.5);
    glEnd();

    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
    glVertex2f(20.0, 13.0);
    glVertex2f(30.0, 13.0);
    glVertex2f(30.0, 10.0);
    glVertex2f(20.0, 10.0);
    glEnd();

    glColor3ub(205, 127, 50);
    glBegin(GL_POLYGON);
    glVertex2f(20.5, 16.5);
    glVertex2f(20.5, 13);
    glVertex2f(20.69, 13);
    glVertex2f(20.69, 16.5);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(29.5, 16.5);
    glVertex2f(29.5, 13);
    glVertex2f(29.31, 13);
    glVertex2f(29.31, 16.5);
    glEnd();

    circle_400(0.5, 21.5, 13.5, 1.0, 1.0, 0.0);   // Lemon
    circle_400(0.5, 23.0, 13.5, 0.0, 1.0, 0.0);   // Lime
    circle_400(0.5, 24.5, 13.5, 1.0, 0.0, 0.0);   // Apple
    circle_400(0.5, 26.0, 13.5, 1.0, 0.647, 0.0); // Orange
    circle_400(0.5, 27.5, 13.5, 0.5, 0.0, 0.5);   // Grape

    glColor3ub(0, 0, 0);
    renderText_400(22.5, 15.2, "Fruit Shop", 10);
}

///AH479
void FoodStall_400() {
    glColor3ub(139, 69, 19); // Brown
    glBegin(GL_POLYGON);
    glVertex2f(45.2, 18.6);
    glVertex2f(54.8, 18.6);
    glVertex2f(50.0, 20.6);
    glEnd();

    glColor3ub(178, 34, 34);
    glBegin(GL_POLYGON);
    glVertex2f(45.2, 14.3);
    glVertex2f(54.8, 14.3);
    glVertex2f(54.8, 11.5);
    glVertex2f(45.2, 11.5);
    glEnd();

    glColor3ub(205, 127, 50);
    glBegin(GL_POLYGON);
    glVertex2f(45.7, 18.6);
    glVertex2f(45.89, 18.6);
    glVertex2f(45.89, 14.3);
    glVertex2f(45.7, 14.3);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(54.3, 18.6);
    glVertex2f(54.11, 18.6);
    glVertex2f(54.11, 14.3);
    glVertex2f(54.3, 14.3);
    glEnd();

    circle_400(0.9, 47.6, 14.9, 0.5, 0.25, 0.0);
    circle_400(0.65, 47.6, 14.9, 0.25, 0.15, 0.0);

    glColor3ub(255, 140, 0);
    glBegin(GL_POLYGON);
    glVertex2f(49.8, 14.6);
    glVertex2f(51.8, 14.6);
    glVertex2f(51.8, 15.3);
    glVertex2f(49.8, 15.3);
    glEnd();

    glColor3ub(173, 216, 230);
    glBegin(GL_POLYGON);
    glVertex2f(52.6, 14.3);
    glVertex2f(53.3, 14.3);
    glVertex2f(53.3, 15.3);
    glVertex2f(52.6, 15.3);
    glEnd();

    glColor3ub(1, 1, 1);
    renderText_400(47.0, 12.4, "Tasty Bits", 10);
}

///AH477
void IceCreamStall_400() {
    glColor3ub(200, 162, 200);
    glBegin(GL_POLYGON);
    glVertex2f(40, 10);
    glVertex2f(52, 10);
    glVertex2f(52, 14);
    glVertex2f(40, 14);
    glEnd();

    glColor3ub(255, 192, 203);
    glBegin(GL_POLYGON);
    glVertex2f(39.5, 14);
    glVertex2f(52.5, 14);
    glVertex2f(52.5, 15);
    glVertex2f(39.5, 15);
    glEnd();

    glColor3ub(255, 165, 0);
    glBegin(GL_POLYGON);
    glVertex2f(40, 11);
    glVertex2f(52, 11);
    glVertex2f(52, 11.25);
    glVertex2f(40, 11.25);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(40, 12);
    glVertex2f(52, 12);
    glVertex2f(52, 12.25);
    glVertex2f(40, 12.25);
    glEnd();

    glColor3ub(255, 192, 203);
    semicircle_400(46, 15, 7, 1, 0.7529, 0.7961);

    glColor3ub(255, 192, 203);
    glBegin(GL_POLYGON);
    glVertex2f(39, 15);
    glVertex2f(53, 15);
    glVertex2f(53, 14.5);
    glVertex2f(39, 14.5);
    glEnd();

    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
    glVertex2f(40.5, 15.0);
    glVertex2f(40.5, 17.0);
    glVertex2f(41.0, 17.0);
    glVertex2f(41.0, 15.0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(51.5, 15.0);
    glVertex2f(51.5, 17.0);
    glVertex2f(51.0, 17.0);
    glVertex2f(51.0, 15.0);
    glEnd();

    glColor3ub(100, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(41.0, 10.5);
    glVertex2f(51.0, 10.5);
    glVertex2f(51.0, 10.0);
    glVertex2f(41.0, 10.0);
    glEnd();

    glColor3ub(255, 255, 255);
    renderText_400(42.0, 10.1, "Ice Cream", 12);

    glColor3ub(210, 180, 140);
    glBegin(GL_TRIANGLES);
    glVertex2f(43.5, 15.5);
    glVertex2f(44.0, 16.5);
    glVertex2f(44.5, 15.5);
    glEnd();
    circle_400(0.4, 44.0, 16.7, 1.0, 0.0, 1.0);

    glColor3ub(210, 180, 140);
    glBegin(GL_TRIANGLES);
    glVertex2f(47.5, 15.5);
    glVertex2f(48.0, 16.5);
    glVertex2f(48.5, 15.5);
    glEnd();
    circle_400(0.4, 48.0, 16.7, 0.0, 1.0, 1.0);

    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
    glVertex2f(40.5, 15.5);
    glVertex2f(41.0, 15.5);
    glVertex2f(41.0, 16.0);
    glVertex2f(40.5, 16.0);
    glEnd();

    glColor3ub(255, 0, 127);
    glBegin(GL_POLYGON);
    glVertex2f(40.2, 16.0);
    glVertex2f(41.3, 16.0);
    glVertex2f(41.3, 17.0);
    glVertex2f(40.2, 17.0);
    glEnd();

    glColor3ub(139, 69, 19);
    circle_400(0.64, 46.5, 17.5, 0.54, 0.27, 0.07);
    glColor3ub(240, 200, 180);
    circle_400(0.56, 46.5, 17.3, 0.94, 0.78, 0.70);

    glColor3ub(65, 105, 225);
    glBegin(GL_POLYGON);
    glVertex2f(44.8, 15.5);
    glVertex2f(47.2, 15.5);
    glVertex2f(47.2, 17.0);
    glVertex2f(44.8, 17.0);
    glEnd();
}

///AH474
void CoffeeShop_400() {
    glColor3ub(60, 60, 60);
    glBegin(GL_POLYGON);
    glVertex2f(80, 8);
    glVertex2f(140, 8);
    glVertex2f(140, 18);
    glVertex2f(80, 18);
    glEnd();

    glColor3ub(255, 140, 0); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(80, 18);
    glVertex2f(140, 18);
    glVertex2f(140, 18.5);
    glVertex2f(80, 18.5);
    glEnd();

    glColor3ub(40, 40, 40); // Black
    glBegin(GL_POLYGON);
    glVertex2f(80, 18.5);
    glVertex2f(140, 18.5);
    glVertex2f(140, 19.5);
    glVertex2f(80, 19.5);
    glEnd();

    glColor3ub(173, 216, 230); // Light Blue
    glBegin(GL_POLYGON);
    glVertex2f(85, 8);
    glVertex2f(95, 8);
    glVertex2f(95, 18);
    glVertex2f(85, 18);
    glEnd();

    glColor3ub(255, 0, 0); // Red
    glBegin(GL_POLYGON);
    glVertex2f(87, 14);
    glVertex2f(93, 14);
    glVertex2f(93, 15);
    glVertex2f(87, 15);
    glEnd();
    glColor3ub(255, 255, 255);
    renderText_400(88.5, 14.25, "OPEN", 10);

    glColor3ub(173, 216, 230); // Light Blue
    glBegin(GL_POLYGON);
    glVertex2f(105, 10);
    glVertex2f(135, 10);
    glVertex2f(135, 17.5);
    glVertex2f(105, 17.5);
    glEnd();

    glColor3ub(60, 60, 60);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(115, 10);
    glVertex2f(115, 17.5);
    glVertex2f(125, 10);
    glVertex2f(125, 17.5);
    glVertex2f(105, 13.75);
    glVertex2f(135, 13.75);
    glEnd();

    glLineWidth(1.0);
    glColor3ub(50, 50, 50); // Dark Gray
    glBegin(GL_POLYGON);
    glVertex2f(115, 8);
    glVertex2f(125, 8);
    glVertex2f(125, 8.2);
    glVertex2f(115, 8.2);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(116, 7);
    glVertex2f(117, 7);
    glVertex2f(117, 8);
    glVertex2f(116, 8);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(123, 7);
    glVertex2f(124, 7);
    glVertex2f(124, 8);
    glVertex2f(123, 8);
    glEnd();

    glColor3ub(40, 40, 40); // Black background
    glBegin(GL_POLYGON);
    glVertex2f(105, 19.5);
    glVertex2f(135, 19.5);
    glVertex2f(135, 20);
    glVertex2f(105, 20);
    glEnd();
    glColor3ub(255, 255, 255); // White text
    renderText_400(102.0, 18.5, "Cup & Comfort", 12);

    glColor3ub(255, 255, 255); // White
    glBegin(GL_QUADS);
    glVertex2f(90, 18);
    glVertex2f(98, 18);
    glVertex2f(98, 19);
    glVertex2f(90, 19);
    glEnd();
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glVertex2f(98, 18.5);
    glVertex2f(99, 18.5);
    glVertex2f(99, 18.75);
    glVertex2f(98, 18.75);
    glEnd();

    // Steam
    glBegin(GL_LINES);
    glVertex2f(94.0, 19);
    glVertex2f(94.0, 19.5);
    glVertex2f(95.0, 19);
    glVertex2f(95.0, 19.5);
    glEnd();

    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);///AH475
    glVertex2f(72, 8);
    glVertex2f(74, 8);
    glVertex2f(74, 10);
    glVertex2f(72, 10);
    glEnd();
    circle_400(1.5, 73, 11, 0.2, 0.6, 0.1); // Green leaves

    glColor3ub(139, 69, 19); // Trunk
    glBegin(GL_POLYGON);///AH476
    glVertex2f(145, 8);
    glVertex2f(147, 8);
    glVertex2f(147, 10);
    glVertex2f(145, 10);
    glEnd();
    circle_400(1.5, 146.0, 11.0, 0.2, 0.6, 0.1); // Green leaves
}

///AH468, AH469, AH470, AH471, AH472, AH473, AH480
void TeaStallSeat_400(){
    glColor3ub(139, 69, 19); // Brown
    glBegin(GL_POLYGON);
    glVertex2f(1, 14);
    glVertex2f(10, 14);
    glVertex2f(10, 13);
    glVertex2f(1, 13);
    glEnd();

    glColor3ub(105, 105, 105);
    glBegin(GL_POLYGON);// Left leg
    glVertex2f(1.5, 13);
    glVertex2f(2, 13);
    glVertex2f(2, 11.5);
    glVertex2f(1.5, 11.5);
    glEnd();

    glColor3ub(105, 105, 105);
    glBegin(GL_POLYGON);// Right leg
    glVertex2f(9, 13);
    glVertex2f(9.5, 13);
    glVertex2f(9.5, 11.5);
    glVertex2f(9, 11.5);
    glEnd();
}

void buildingWindow_400(int r, int g, int b){///AH447
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(22,92);
    glVertex2f(22,90.92);
    glVertex2f(24,90.92);
    glVertex2f(24,92);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(22,92);
    glVertex2f(22,90.8);
    glVertex2f(24,90.8);
    glVertex2f(24,92);
    glEnd();
}

void buildingWindow1_400(int r, int g, int b){///AH448
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(28,92);
    glVertex2f(28,90.8);
    glVertex2f(30,90.8);
    glVertex2f(30,92);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(28,92);
    glVertex2f(28,90.8);
    glVertex2f(30,90.8);
    glVertex2f(30,92);
    glEnd();
}

void  buildingWindow2_400(int r, int g, int b){///AH449
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(22,94.3);
    glVertex2f(22,93.1);
    glVertex2f(24,93.1);
    glVertex2f(24,94.3);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(22,94.3);
    glVertex2f(22,93.1);
    glVertex2f(24,93.1);
    glVertex2f(24,94.3);
    glEnd();
}

void  buildingWindow3_400(int r, int g, int b){///AH450
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(28,94.3);
    glVertex2f(28,93.1);
    glVertex2f(30,93.1);
    glVertex2f(30,94.3);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(28,94.3);
    glVertex2f(28,93.1);
    glVertex2f(30,93.1);
    glVertex2f(30,94.3);
    glEnd();
}

void  buildingWindow4_400(int r, int g, int b){///AH451
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(22,96.8);
    glVertex2f(22,95.6);
    glVertex2f(24,95.6);
    glVertex2f(24,96.8);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(22,96.8);
    glVertex2f(22,95.6);
    glVertex2f(24,95.6);
    glVertex2f(24,96.8);
    glEnd();
}

void  buildingWindow5_400(int r, int g, int b){///AH452
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(28,96.8);
    glVertex2f(28,95.6);
    glVertex2f(30,95.6);
    glVertex2f(30,96.8);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(28,96.8);
    glVertex2f(28,95.6);
    glVertex2f(30,95.6);
    glVertex2f(30,96.8);
    glEnd();
}

void  buildingWindow6_400(int r, int g, int b){///AH453
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(22,99.2);
    glVertex2f(22,98);
    glVertex2f(24,98);
    glVertex2f(24,99.2);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(22,99.2);
    glVertex2f(22,98);
    glVertex2f(24,98);
    glVertex2f(24,99.2);
    glEnd();
}

void  buildingWindow7_400(int r, int g, int b){///AH454
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(28,99.2);
    glVertex2f(28,98);
    glVertex2f(30,98);
    glVertex2f(30,99.2);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(28,99.2);
    glVertex2f(28,98);
    glVertex2f(30,98);
    glVertex2f(30,99.2);
    glEnd();
}

///AH455, AH456, AH457, AH458
void building1_400(float r, float g, float b){
    glColor3ub(r,g,b);
    glBegin(GL_POLYGON);
    glVertex2f(20,84);
    glVertex2f(32,84);
    glVertex2f(32,99.6);
    glVertex2f(20,99.6);
    glEnd();

    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(20,84);
    glVertex2f(20,90);
    glVertex2f(20,92.4);
    glVertex2f(20,94.8);
    glVertex2f(20,97.21);
    glVertex2f(20,99.6);
    glVertex2f(32,99.6);
    glVertex2f(32,97.21);
    glVertex2f(32,94.8);
    glVertex2f(32,92.4);
    glVertex2f(32,90);
    glVertex2f(32,84);
    glEnd();

    glColor3ub(255,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(24,84);
    glVertex2f(28,84);
    glVertex2f(28,88);
    glVertex2f(24,88);
    glEnd();
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(24,84);
    glVertex2f(28,84);
    glVertex2f(28,88);
    glVertex2f(24,88);
    glEnd();

    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(20,90);
    glVertex2f(32,90);
    glEnd();

    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(20,92.4);
    glVertex2f(32,92.4);
    glEnd();

    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(20,94.8);
    glVertex2f(32,94.8);
    glEnd();

    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(20,97.21);
    glVertex2f(32,97.21);
    glEnd();

    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    glVertex2f(20,99.6);
    glVertex2f(32,99.6);
    glEnd();

    if(current_time == DAY || current_time == EVENING){
        int r=g=b=255;
        buildingWindow_400(r, g, b);
        buildingWindow1_400(r, g, b);
        buildingWindow2_400(r, g, b);
        buildingWindow3_400(r, g, b);
        buildingWindow4_400(r, g, b);
        buildingWindow5_400(r, g, b);
        buildingWindow6_400(r, g, b);
        buildingWindow7_400(r, g, b);
    }
    else {
        int r=g=00255;
        int b=0;
        buildingWindow_400(r, g, b);
        buildingWindow1_400(r, g, b);
        buildingWindow2_400(r, g, b);
        buildingWindow3_400(r, g, b);
        buildingWindow4_400(r, g, b);
        buildingWindow5_400(r, g, b);
        buildingWindow6_400(r, g, b);
        buildingWindow7_400(r, g, b);
    }
}

///AH434, AH435, AH436, AH437
void Bus_Stop_400(){
    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(65.67483,49.76407);
    glVertex2f(65.67483,49.37796);
    glVertex2f(78.4284,49.34286);
    glVertex2f(78.4401,49.77578);
    glEnd();

    glColor3ub(123, 245, 154);
    glBegin(GL_POLYGON);
    glVertex2f(66.20815,49.40758);
    glVertex2f(66.22693,48.88175);
    glVertex2f(66.31401,48.68004);
    glVertex2f(66.51345,48.52492);
    glVertex2f(66.75721,48.49169);
    glVertex2f(67.06952,48.47946);
    glVertex2f(77.04387,48.47946);
    glVertex2f(77.34302,48.49169);
    glVertex2f(77.56424,48.52492);
    glVertex2f(77.76535,48.68004);
    glVertex2f(77.92624,48.88175);
    glVertex2f(77.91618,49.40758);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(66.75721,48.49169);
    glVertex2f(66.74474,40.56774);
    glVertex2f(67.0955,40.55475);
    glVertex2f(67.06952,48.47946);
    glEnd();

    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(77.34302,48.50849);
    glVertex2f(77.04387,48.50849);
    glVertex2f(77.05867,40.5525);
    glVertex2f(77.36935,40.5525);
    glEnd();

    //Back Glass
    glColor3ub(122, 204, 184);
    glBegin(GL_POLYGON);
    glVertex2f(67.09212,47.42775);
    glVertex2f(77.03855,47.42775);
    glVertex2f(77.03664,41.48445);
    glVertex2f(67.09647,41.45473);
    glEnd();

    //chair
    glColor3ub(136,139,141);
    glBegin(GL_POLYGON);
    glVertex2f(67.68169,42.40732);
    glVertex2f(67.68169,41.9334);
    glVertex2f(76.75212,41.9334);
    glVertex2f(76.75212,42.40732);
    glEnd();

    glColor3ub(136,139,141);
    glBegin(GL_POLYGON);
    glVertex2f(68.07326,42.439);
    glVertex2f(68.07473,43.93657);
    glVertex2f(68.12226,44.06487);
    glVertex2f(68.19866,44.15124);
    glVertex2f(68.30495,44.21103);
    glVertex2f(76.13033,44.21103);
    glVertex2f(76.23662,44.15124);
    glVertex2f(76.31302,44.06487);
    glVertex2f(76.36055,43.93657);
    glVertex2f(76.36055,42.439);
    glEnd();

    //chair leg 1
    glColor3ub(136,139,141);
    glBegin(GL_POLYGON);
    glVertex2f(68.13638,41.94098);
    glVertex2f(68.4476,41.93604);
    glVertex2f(68.43278,40.538);
    glVertex2f(68.13638,40.54788);
    glEnd();

    //chair leg 2
    glColor3ub(136,139,141);
    glBegin(GL_POLYGON);
    glVertex2f(72.372515, 41.94098);
    glVertex2f(72.372515, 40.54788);
    glVertex2f(72.061295, 40.54788);
    glVertex2f(72.061295, 41.94098);
    glEnd();

    //chair leg 3
    glColor3ub(136,139,141);
    glBegin(GL_POLYGON);
    glVertex2f(76.29743,41.94098);
    glVertex2f(76.29743,40.54788);
    glVertex2f(75.98621,40.54788);
    glVertex2f(75.98621,41.94098);
    glEnd();
}

///AH459, AH460, AH461, AH462, AH463
void lampPost_400(){
    glColor3ub(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(81.14065,40.53946);
    glVertex2f(81.13669,43.32792);
    glVertex2f(81.1946,43.44785);
    glVertex2f(81.31945,43.52978);
    glVertex2f(81.42089,43.54538);
    glVertex2f(81.63937,43.54538);
    glVertex2f(81.79152,43.51417);
    glVertex2f(81.89296,43.45565);
    glVertex2f(81.94758,43.36201);
    glVertex2f(81.95631,40.5394);
    glEnd();

    glColor3ub(52, 61, 54);
    glBegin(GL_POLYGON);
    glVertex2f(81.42089,43.54538);
    glVertex2f(81.63937,43.54538);
    glVertex2f(81.64814,48.01567);
    glVertex2f(81.41333,48.01567);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(79.78926,47.58519);
    glVertex2f(79.83818,47.29168);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(79.83818,47.29168);
    glVertex2f(79.9458,47.12536);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(79.9458,47.12536);
    glVertex2f(80.15125,47.00796);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(80.15125,47.00796);
    glVertex2f(80.54547,46.93623);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(80.54547,46.93623);
    glVertex2f(80.93493,46.9193);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(80.93493,46.9193);
    glVertex2f(81.39778,46.93059);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(81.6677,46.94926);
    glVertex2f(82.09204,46.93059);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(82.09204,46.93059);
    glVertex2f(82.44764,46.93059);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(82.44764,46.93059);
    glVertex2f(82.8026,46.95904);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(82.8026,46.95904);
    glVertex2f(83.10589,47.03731);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(83.10589,47.03731);
    glVertex2f(83.23222,47.22974);
    glEnd();

    glColor3ub(52, 61, 54);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(83.23222,47.22974);
    glVertex2f(83.28302,47.59098);
    glEnd();

    if(current_time == NIGHT || current_time == CLOUDY || current_time == RAIN){
        circle_400(0.7, 79.81861, 48.35809, 255, 255, 0);
        circle_400(0.7, 81.54052, 48.769, 255, 255, 0);
        circle_400(0.7, 83.30156, 48.35809, 255, 255, 0);
    }else{
        circle_400(0.7, 79.81861, 48.35809, 255, 255, 255);
        circle_400(0.7, 81.54052, 48.769, 255, 255, 255);
        circle_400(0.7, 83.30156, 48.35809, 255, 255, 255);
    }
}

void scaleTrain_400(){
    glPushMatrix();
    glTranslatef(-40.0f, -2.0f, 0.0f);
    glScalef(1.5f, 1.5f, 1.0f);
    train_400();
    glPopMatrix();
}

void scaleBRTCBus_400(){///AH427
    glPushMatrix();
    glTranslatef(8, -3.5, 0);
    glScalef(1.2f, 1.2f, 1.0f);
    busBRTC_400();
    glPopMatrix();
}

void scaleBus_400(){
    glPushMatrix();
    glTranslatef(-43, -35, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(237, 41, 57, "Hanif Enterprise", false);///AH438
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-45, -55, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(0, 128, 255, "Shohag Paribahan", false);///AH441
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, -40, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(255, 165, 0, "Green Line", false);///AH442
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-47, -41, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(153, 50, 204, "Ena Transport", false);///AH439
    glPopMatrix();

    glPushMatrix();
    glTranslatef(bus1_400_x, 0.0f, 0.0f);
    glTranslatef(-67, -82, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(60,179,113, "Desh Travels", true);///AH446
    glPopMatrix();

    glPushMatrix();
    glTranslatef(srplus_bus_400_x, 0.0f, 0.0f);
    glTranslatef(-120, -60, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(139,104,200, "SR Plus", false);///AH445
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-115, -40, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(239,204,0, "Shyamoli Paribahan", false);///AH444
    glPopMatrix();

    glPushMatrix();
    glTranslatef(17, -45, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(255,0,144, "Grameen Travels", false);///AH443
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-47, -48, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus1_400(15, 50, 204, "National Travles", false);///AH440
    glPopMatrix();

    glPushMatrix();
    glTranslatef(43, -42, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus2_400(120, 150, 225);///AH481
    glPopMatrix();

    glPushMatrix();
    glTranslatef(33, -42, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus2_400(20, 150, 25);///AH482
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-7, -35, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus2_400(250, 150, 25);///AH483
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-17, -35, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus2_400(250, 150, 225);///AH484
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-27, -35, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    Bus2_400(250, 250, 225);///AH485
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-42, -17, 0);
    glScalef(1.3f, 1.3f, 1.0f);
    Bus3_400(220, 30, 240);///AH486
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-32, -17, 0);
    glScalef(1.3f, 1.3f, 1.0f);
    Bus3_400(144, 238, 144);///AH487
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-22, -17, 0);
    glScalef(1.3f, 1.3f, 1.0f);
    Bus3_400(144, 23, 144);///AH488
    glPopMatrix();
}

void scaleCar_400(){
    glPushMatrix();
    glTranslatef( -11, -16, 0 );
    glScalef(2.0f, 2.0f, 1.0f);
    car1_400();///AH430
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 15, -26, 0 );
    glScalef(1.8f, 1.8f, 1.0f);
    car2_400();///AH431
    glPopMatrix();
}

void scaleTruck_400(){
    truck1_400();///AH428

    glPushMatrix();
    glTranslatef(50, -5, 0 );
    glScalef(1.5f, 1.5f, 1.0f);
    truck2_400();///AH429
    glPopMatrix();
}

void scaleCNG_400(){
    glPushMatrix();
    glTranslatef(-10, -13.5, 0);
    glScalef(1.4, 1.4, 1);
    cng_400();///AH432
    glPopMatrix();

    // CNG in gas station
    glPushMatrix();
    glTranslatef(-93, 20, 0);
    glScalef(1.3, 1.3, 1);
    float tempX = cng_400X;
    cng_400X = 0;
    float tempAngle = cngWheelAngle_400;
    cngWheelAngle_400 = 0;
    cng_400();///AH433
    cng_400X = tempX;
    cngWheelAngle_400 = tempAngle;
    glPopMatrix();
}

void scaleBusStop_400(){
    glPushMatrix();
    glTranslatef(-10, 0, 0);
    Bus_Stop_400();///AH434
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 0);
    Bus_Stop_400();///AH435
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10, 0, 0);
    Bus_Stop_400();///AH436
    glPopMatrix();

    glPushMatrix();
    glTranslatef(20, 0, 0);
    Bus_Stop_400();///AH437
    glPopMatrix();
}

void scaleLampPost_400(){
    glPushMatrix();
    glTranslatef(4, -9, 0);
    glScalef(1.2, 1.2, 1);
    lampPost_400();///AH459
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-46, -9, 0);
    glScalef(1.2, 1.2, 1);
    lampPost_400();///AH460
    glPopMatrix();
}

void scaleLampPost2_400(){
    glPushMatrix();
    glTranslatef(-87, -30, 0);
    glScalef(1.2, 1.2, 1);
    lampPost_400();///AH461
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-53, -30, 0);
    glScalef(1.2, 1.2, 1);
    lampPost_400();///AH462
    glPopMatrix();

    glPushMatrix();
    glTranslatef(25, -30, 0);
    glScalef(1.2, 1.2, 1);
    lampPost_400();///AH463
    glPopMatrix();
}

void scaleTeaStall_TerminalSide_400(){
    glPushMatrix();
    glTranslatef(-34, -8, 0);
    glScalef(1.2, 1.2, 1);
    TeaStall_400();///AH466
    glPopMatrix();

    glPushMatrix();
    glTranslatef(47, 39, 0);
    glScalef(0.9, 0.9, 1);
    TeaStall_400();///AH467
    glPopMatrix();
}

void scaleTrainSideStall_400(){
    glPushMatrix();
    glTranslatef(-165, -39, 0);
    glScalef(1.5, 1.2, 1);
    TeaStall_400();///AH464
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10, -38, 0);
    glScalef(1.3, 1.2, 1);
    TeaStall_400();///AH465
    glPopMatrix();

    glPushMatrix();
    glTranslatef(27, -3, 0);
    glScalef(1.5, 1.2, 1);
    FruitShop_400();///AH478
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -2, 0);
    glScalef(1, 1, 1);
    TeaStallSeat_400();///AH468
    glPopMatrix();

    glPushMatrix();
    glTranslatef(43, -2, 0);
    glScalef(1.3, 1, 1);
    TeaStallSeat_400();///AH469
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 2, 0);
    glScalef(1, 1, 1);
    TeaStallSeat_400();///AH470
    glPopMatrix();

    glPushMatrix();
    glTranslatef(20, 0, 0);
    glScalef(1, 1, 1);
    TeaStallSeat_400();///AH480
    glPopMatrix();

    glPushMatrix();
    glTranslatef(131.5, 0, 0);
    glScalef(1, 1, 1);
    TeaStallSeat_400();///AH471
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150, -0.5, 0);
    glScalef(1, 1, 1);
    TeaStallSeat_400();///AH472
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150, 3, 0);
    glScalef(1, 1, 1);
    TeaStallSeat_400();///AH473
    glPopMatrix();

    glPushMatrix();
    glTranslatef(25, 2, 0);
    glScalef(1.25, 0.8, 1);
    IceCreamStall_400();///AH477
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-28, -2, 0);
    glScalef(1.3, 1, 1);
    FoodStall_400();///AH479
    glPopMatrix();

    glPushMatrix();
    glTranslatef(65, 1, 0);
    glScalef(0.4, 1, 1);
    CoffeeShop_400();///AH474
    glPopMatrix();
}

void scaleBuilding_400(){
    glPushMatrix();///AH455
    glTranslatef(-12, -27.8, 0);
    glScalef(1, 1.3, 1);
    building1_400(100,30,100);
    glPopMatrix();

    glPushMatrix();///AH456
    glTranslatef(5, -20, 0);
    glScalef(1, 1.2, 1);
    building1_400(0,30,100);
    glPopMatrix();

    glPushMatrix();///AH457
    glTranslatef(110, -20, 0);
    glScalef(1, 1.2, 1);
    building1_400(255,30,100);
    glPopMatrix();

    glPushMatrix();///AH458
    glTranslatef(126, -32, 0);
    glScalef(1, 1.35, 1);
    building1_400(210,105,30);
    glPopMatrix();
}

void plane_400_animation(){///Ani(4)1
    if (planeMoving_400){
        plane_400_x -= 0.3;
        plane_400_y -= 0.0;

        if (plane_400_x < -180){
            plane_400_x = 20;
            plane_400_y = 0.0f;
        }
    }
}

void car1_400_animation(){///Ani(4)2
    car1_400_x -= car1_400_speed;

    if (car1_400_x <  -20.0f) {
        car1_400_x = 83;
    }
}

void car2_400_animation(){///Ani(4)3
    car2_400_x -= car2_400_speed;

    if (car2_400_x <  -32.0f) {
        car2_400_x = 80;
    }
}

void train_400_animation(){///Ani(4)4
    train_400_x += train_400_speed;

    if (train_400_x >  90.0f) {
        train_400_x = -150;
    }
}

void truck1_400_animation(){///Ani(4)5
    truck1_400_x += truck1_400_speed;

    if (truck1_400_x >  70.0f) {
        truck1_400_x = -120;
    }
}

void truck2_400_animation(){///Ani(4)6
    truck2_400_x -= truck2_400_speed;

    if (truck2_400_x <  -120.0f) {
        truck2_400_x = 120;
    }
}

void busBRTC_400_animation(){///Ani(4)7
    busBRTC_400_x += busBRTC_400_speed;

    if (busBRTC_400_x >  120.0f) {
        busBRTC_400_x = -100;
        busBRTC_400_stop = false;
    }
}

void bus1_400_animation(){///Ani(4)8
    bus1_400_x -= bus1_400_speed;

    if (bus1_400_x <  -120.0f) {
        bus1_400_x = 120;
    }
}

void cng_400_animation(){///Ani(4)9
    cng_400X += cng_400Speed;

    if (cng_400X >  120.0f) {
        cng_400X = -120;
    }
}

void resumeBusBRTC_400(int value){///Ani(4)10
    busBRTC_400_speed = 0.4f;
}

void updateBusBRTC_400(int value){///Ani(4)11
    if (!busBRTC_400_stop && busBRTC_400_x >= 22.0f && busBRTC_400_x < 50.0f) {
        busBRTC_400_stop = true;
        busBRTC_400_speed = 0.0f;
        glutTimerFunc(3000, resumeBusBRTC_400, 0);
    }
    glutPostRedisplay();
    glutTimerFunc(30, updateBusBRTC_400, 0);
}

void updateBusWheelRotation_400(int value){///Ani(4)12
    busWheelAngle_400 += 5.0f;
    if (busWheelAngle_400 > 360.0f)
        busWheelAngle_400 -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, updateBusWheelRotation_400, 0);
}

void updateBRTCWheelRotation_400(int value){///Ani(4)13
    BRTCWheelAngle_400 += BRTCWheelSpeed_400;
    if (BRTCWheelAngle_400 > 360.0f)
        BRTCWheelAngle_400 -= 360.0f;

    glutPostRedisplay();   // redraw
    glutTimerFunc(16, updateBRTCWheelRotation_400, 0); // ~60 FPS
}

void updateTrainWheel_400(int value){///Ani(4)14
    trainWheelAngle_400 -= 2.0f;
    if (trainWheelAngle_400 < -360.0f)
        trainWheelAngle_400 += 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, updateTrainWheel_400, 0);
}

void updateTruck2Wheel_400(int value){///Ani(4)15
    truckWheelAngle_400 += 5.0f; // spin (negative = clockwise)
    if (truckWheelAngle_400 > 360.0f)
        truckWheelAngle_400 -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, updateTruck2Wheel_400, 0); // ~60 FPS
}

void updateCNGWheel_400(int value) {///Ani(4)16
    cngWheelAngle_400 -= 6.0f; // rotate clockwise
    if (cngWheelAngle_400 < -360.0f)
        cngWheelAngle_400 += 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, updateCNGWheel_400, 0);
}

void update_animations_400(int value){///Ani(4)19
    _angle1 += 8.0f;

    plane_400_animation();
    train_400_animation();
    car1_400_animation();
    car2_400_animation();
    busBRTC_400_animation();
    bus1_400_animation();
    truck1_400_animation();
    truck2_400_animation();
    cng_400_animation();

    glutPostRedisplay();
    glutTimerFunc(16, update_animations_400, 0);
}

void handleKeyboard_400(unsigned char key, int x, int y){///Ani(4)17
    key = tolower(key); // convert uppercase to lowercase
    switch(key) {
        case 'd': current_time = DAY; break;
        case 'n': current_time = NIGHT; break;
        case 'e': current_time = EVENING; break;
        case 'c': current_time = CLOUDY; break;
        case '3':
            if (current_time != RAIN) {
                current_time = RAIN;
                isRaining = true;
            } else {
                current_time = DAY;
                isRaining = false;
            }
            break;

        case '1': current_weather = CLEAR; break;
        case '2': current_weather = SNOW; break;
        case '4': current_weather = FOGGY; break;

        case 's': planeMoving_400 = true; break;
        case 'f': planeMoving_400 = false; break;

        case '+': train_400_speed = 0.4f; break;
        case '-': train_400_speed = 0.0f; break;
    }
    glutPostRedisplay();
}

void display(){
   glClear(GL_COLOR_BUFFER_BIT);

    Sky_403();
    sunMoon_401();
    Cloud_402();
    scaleBuilding_400();
    rareView_404();
    FullRoad_405();
    TrainArea_400();
    TrainLine_400();
    busCounter_400(64, 75, "National");///AH419
    busCounter_400(75, 75, "Nabil");///AH420
    busCounter_400(86, 75, "Hanif");///AH421
    busCounter_400(97, 75, "Ena");///AH422
    busCounter_400(108, 75, "Shohag");///AH423
    busCounter_400(119, 75, "Green Line");///AH424
    busCounter_400(130, 75, "Desh");///AH425
    busCounter_400(141, 75, "Grameen");///AH426
    TerminalBuilding_1_400();
    TerminalBuilding_2_400();
    GasStation_400();
    plane_413();
    scaleTrainSideStall_400();
    scaleTrain_400();
    scaleCNG_400();
    scaleBus_400();
    scaleTeaStall_TerminalSide_400();
    scaleBusStop_400();
    scaleLampPost_400();
    scaleBRTCBus_400();
    scaleTruck_400();
    scaleCar_400();
    scaleLampPost2_400();

    if (current_time == RAIN && isRaining) {
        drawRaindrops_400();
     }
     else if(current_weather == FOGGY){
        drawFogOverlay_400();
     }
     else if (current_weather == SNOW) {
        drawSnow_400();
     }
    //glFlush();
    glutSwapBuffers();
}

void mouseHandler(int button, int state, int x, int y) {///Ani(4)18
    if (state == GLUT_DOWN) {
            if (button == GLUT_LEFT_BUTTON) {
                srplus_bus_400_x -= 5.0f;   // move SR Plus left
            }
            else if (button == GLUT_RIGHT_BUTTON) {
                srplus_bus_400_x += 5.0f;   // move SR Plus right
            }
        glutPostRedisplay();
    }
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(1300, 550);
    glutCreateWindow("Mohakhali Bus Terminal");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 160, 0, 120);

    glutDisplayFunc(display);
    glutTimerFunc(16, update_animations_400, 0);
    glutTimerFunc(16, updateRainDrop_400, 0);
    glutTimerFunc(16, updateBusWheelRotation_400, 0);
    glutTimerFunc(0, updateBRTCWheelRotation_400, 0);
    glutTimerFunc(0, updateTrainWheel_400, 0);
    glutTimerFunc(0, updateTruck2Wheel_400, 0);
    glutTimerFunc(0, updateCNGWheel_400, 0);
    glutTimerFunc(30, updateBusBRTC_400, 0);
    glutMouseFunc(mouseHandler);
    glutKeyboardFunc(handleKeyboard_400);

    glutMainLoop();
    return 0;
}
