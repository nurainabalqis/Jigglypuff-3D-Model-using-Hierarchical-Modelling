/*******************************************************
* ASSIGNMENT 3                                         *
* GROUP NAME:                                          *
* NUR AINA BALQIS BINTI MOHAMAD ZAPARIN - A23CS0151    *
* KRISTINE ELLE BENJAMIN - A23CS0095                   *
* NUR AMIERA ZULAIKHA BINTI HARDI - A23CS0153          *
* SECTION : 01                                         *
*******************************************************/
#include <GL/glut.h>
#include <cmath>

// Rotation state
float rotX = 0.0f, rotY = 0.0f;
int lastX, lastY;
bool isDragging = false;

void initLighting() {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void drawSphere(float radius) {
    glutSolidSphere(radius, 30, 30);
}

void drawEar(float x, float y, float z, bool left) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Tilt the ear slightly based on left/right
    glRotatef(left ? -5.0f : 5.0f, 0.0f, 0.0f, 1.0f); // Tilt outward
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f);                 // Tilt forward


    // Outer ear (matches body color)
    glPushMatrix();
    glScalef(0.25f, 0.4f, 0.15f); // Flattened ellipse
    glColor3f(0.95f, 0.6f, 0.7f); // Jigglypuff pink (same as body)
    glutSolidSphere(1.0, 20, 40);
    glPopMatrix();

    // Inner ear (black)
    glPushMatrix();
    glTranslatef(0.0f, 0.09f, 0.12f); // Offset inward
    glScalef(0.15f, 0.25f, 0.04f);   // Smaller ellipse
    glColor3f(0.0f, 0.0f, 0.0f);     // Black inner ear
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

void drawEye(bool left) {
    glPushMatrix();
    glTranslatef(left ? -0.3f : 0.3f, 0.2f, 0.55f); // Closer to face - Eye position

    // Outer white eyeball
    glPushMatrix();
    glScalef(1.0f, 1.0f, 0.8f); // Slightly flattened
    glColor3f(1, 1, 1);
    drawSphere(0.26); // Eyeball
    glPopMatrix();

    // Blue iris 
    glTranslatef(0, 0.03, 0.07);
    glPushMatrix();
    glScalef(1.0f, 1.0f, 0.8f);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawSphere(0.20);
    glPopMatrix();

    // Inner white reflection
    glTranslatef(0.015f, 0.05f, 0.12);
    glPushMatrix();
    glScalef(1.0f, 1.0f, 0.6f);
    glColor3f(1, 1, 1);
    drawSphere(0.08);
    glPopMatrix();

    glPopMatrix();
}

void drawWing(bool left) {
    glPushMatrix();
    // Position wing relative to body
    glTranslatef(left ? -0.85f : 0.85f, 0.2f, -0.3f);
    glScalef(left ? -1.0f : 1.0f, 1.0f, 1.0f);  // Mirror if left

    int layers = 3;
    int feathersPerLayer = 3;
    for (int layer = 0; layer < layers; ++layer) {
        float layerYOffset = 0.1f * layer;      // stack upward
        float layerZOffset = -0.05f * layer;    // backward for layering

        for (int i = 0; i < feathersPerLayer; ++i) {
            glPushMatrix();
            // Feather position across wing span
            float xOffset = 0.2f * i;

            // Slight upward tilt for wing shape
            float yOffset = layerYOffset + 0.05f * i;

            // Feather curvature (optional)
            float rotation = -10 + 5 * i;

            glTranslatef(xOffset, yOffset, layerZOffset);
            glRotatef(rotation, 0, 0, 1);  // Rotate feather to fan it

            // Longer feathers for outer layers
            float scaleX = 0.35f + 0.05f * layer;
            float scaleY = 0.12f;
            float scaleZ = 0.05f;

            glScalef(scaleX, scaleY, scaleZ);
            glColor3f(0.95f, 0.95f, 0.95f);
            drawSphere(1.0);
            glPopMatrix();
        }
    }
    glPopMatrix();
}

void drawLimb(float x, float y, float z, bool isArm) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.15, isArm ? 0.15 : 0.2, 0.15);
    glColor3f(1.0, 0.75, 0.8);
    drawSphere(1.0);
    glPopMatrix();
}

void drawMouth() {
    glPushMatrix();
    glTranslatef(0.0, -0.18, 0.75);
    glColor3f(0, 0, 0);
    glPointSize(2.5);

    glBegin(GL_POINTS);
    for (float angle = -120.0f; angle <= 120.0f; angle += 1.0f) {
        float rad = angle * 3.14159265f / 180.0f;
        float x = 0.12f * cos(rad);
        float y = 0.13f * sin(rad);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    glPopMatrix();
}

void drawHairCurl() {
    float angleStep = 0.15f;
    float radius = 0.18f;
    float baseRadius = 0.07f;
    float totalAngle = 7.85f; // approx 2.5 * Ï€

    glColor3f(1.0f, 0.75f, 0.8f); // Light pink

    for (float angle = 0.0f; angle <= totalAngle; angle += angleStep) {
        float x = radius * cosf(angle);

        // Start with normal wave, then curve down at the end
        float y = 0.83f + 0.04f * sinf(angle * 1.5f);

        // Add extra forward pull toward the end of curl
        float z = radius * sinf(angle) + 0.03f * (angle / totalAngle); // +z to bring it toward forehead
        y -= 0.015f * (angle / totalAngle); // gently lower end of curl

        float taper = 1.0f - (angle / totalAngle);
        float currentRadius = baseRadius * taper;

        glPushMatrix();
        glTranslatef(x, y, z);
        glutSolidSphere(currentRadius, 20, 20);
        glPopMatrix();

        radius -= 0.003f; // spiral tightens
    }
}

void drawJigglypuff() {
    glPushMatrix();
    glColor3f(1.0, 0.75, 0.8);
    drawSphere(0.8);

    drawHairCurl();
    drawEye(true); // Left Eye
    drawEye(false); // Right Eye
    drawMouth();

    drawEar(-0.45, 0.50, 0.0, true);  // Left ear
    drawEar(0.45, 0.50, 0.0, false);  // Right ear

    drawLimb(-0.45, -0.4, 0.55, true); // Left arm
    drawLimb(0.45, -0.4, 0.55, true); // Right arm
    drawLimb(-0.3, -0.7, 0.1, false); // Left leg
    drawLimb(0.3, -0.7, 0.1, false); // Right leg

    drawWing(true); // Left Wing
    drawWing(false); // Right Wing

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    drawJigglypuff();
    glutSwapBuffers();
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastX = x;
            lastY = y;
        }
        else {
            isDragging = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (isDragging) {
        rotY += (x - lastX) * 0.5f;
        rotX += (y - lastY) * 0.5f;
        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Jigglypuff - 3D");

    init();
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}