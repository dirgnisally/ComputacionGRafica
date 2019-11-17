#include <GL/glut.h>
#include <iostream>

#include "camera.h"

using namespace std;

//CAMERA
static camera *objetoCamara = new camera(0.0f, 1.0f, -3.0f);
static int old_x, old_y;

//CAMERA
void MouseMotion(int x, int y) {
    old_y = y;
    old_x = x;
}

//CAMERA
void Examinar(int x, int y) {
    float rot_x, rot_y;
    rot_y = (float) (old_y - y);
    rot_x = (float) (x - old_x);
    objetoCamara->Rotar_Latitud(rot_y * DEGREE_TO_RAD);
    objetoCamara->Rotar_Longitud(rot_x * DEGREE_TO_RAD);
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

static void SpecialKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1:
        glutPassiveMotionFunc(MouseMotion);
        objetoCamara->camMovimiento = CAM_STOP;
        break;
    case GLUT_KEY_F2:
        glutPassiveMotionFunc(Examinar);
        objetoCamara->camMovimiento = CAM_EXAMINAR;
        break;
    case GLUT_KEY_F3:
        glutPassiveMotionFunc(MouseMotion);
        objetoCamara->camMovimiento = CAM_PASEAR;
        objetoCamara->camAtY = 0;
        objetoCamara->camViewY = 0;
        objetoCamara->SetDependentParametersCamera();
        break;
    case GLUT_KEY_F4: //ORTOGONAL Y PERSPECTIVA
        if(objetoCamara->camProjection == CAM_CONIC){
            objetoCamara->x1 = -3;
            objetoCamara->x2 = 3;
            objetoCamara->y1 = -3;
            objetoCamara->y2 = 3;
            objetoCamara->z1 = -5;
            objetoCamara->z2 = 5;
            objetoCamara->camProjection = CAM_PARALLEL;
        }else objetoCamara->camProjection = CAM_CONIC;
        break;
    case GLUT_KEY_F5: //MODO TRIPODE
        //
        objetoCamara->camMovimiento = CAM_TRIPODE;
        break;
    case GLUT_KEY_F6: //MODO PANORAMA
        //
        objetoCamara->camMovimiento = CAM_PAN;
        break;
    case GLUT_KEY_HOME: //Reset Camera
        objetoCamara->camAtX = 0;
        objetoCamara->camAtY = 0;
        objetoCamara->camAtZ = 0;
        objetoCamara->camViewX = 0;
        objetoCamara->camViewY = 1;
        objetoCamara->camViewZ = -3;
        objetoCamara->SetDependentParametersCamera();
        break;
    default:
        printf("key %d %c X %d Y %d\n", key, key, x, y);
    }
    glutPostRedisplay();
}

void Zoom(int x, int y) {
    float zoom;
    zoom = (float) ((y - old_y) * DEGREE_TO_RAD);
    old_y = y;
    switch (objetoCamara->camMovimiento) {
        case CAM_EXAMINAR:
            if (objetoCamara->camAperture + zoom > (5 * DEGREE_TO_RAD) &&
                    objetoCamara->camAperture + zoom < 175 * DEGREE_TO_RAD)
                objetoCamara->camAperture = objetoCamara->camAperture + zoom;
            break;
    }
    glutPostRedisplay();
}

void Andar(int x, int y) {
    float rotacion_x, avance_y;
    avance_y = (float) (y - old_y) / 10;
    rotacion_x = (float) (old_x - x) * DEGREE_TO_RAD / 5;
    objetoCamara->YawCamera(rotacion_x);
    objetoCamara->AvanceFreeCamera(avance_y);
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

void Tripode(int x, int y){
    float rotacion_x, rotacion_y;
    rotacion_x = (float)(old_x - x) * DEGREE_TO_RAD / 5;
    rotacion_y = (float)(old_y - y) * DEGREE_TO_RAD / 5;
    objetoCamara->YawCamera( rotacion_x );
    objetoCamara->PitchCamera( rotacion_y );
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

void Pan(int x, int y){
    float avance_x, avance_y;
    avance_x = (float)(old_x - x) / 10;
    avance_y = (float)(y - old_y) / 10;
    objetoCamara->PanCamera(avance_x, avance_y);
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    old_x = x;
    old_y = y;

    switch (button){
        case GLUT_LEFT_BUTTON:
        switch (objetoCamara->camMovimiento) {
            case CAM_EXAMINAR:
                if (state == GLUT_DOWN) glutMotionFunc(Zoom);
                if (state == GLUT_UP){
                    glutPassiveMotionFunc(Examinar);
                    glutMotionFunc(NULL);
                }
                break;
            case CAM_PASEAR:
                if (state == GLUT_DOWN) glutMotionFunc(Andar);
                if (state == GLUT_UP) glutMotionFunc(NULL);
                break;
            case CAM_TRIPODE:
                if (state == GLUT_DOWN) glutMotionFunc(Tripode);
                if (state == GLUT_UP) glutMotionFunc(NULL);
                break;
            case CAM_PAN:
                if (state == GLUT_DOWN) glutMotionFunc(Pan);
                if (state == GLUT_UP) glutMotionFunc(NULL);
                break;
        }
        break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void cubeColor() {
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    // Top face (y = 0.5f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out

    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f,  0.5f);
    glVertex3f( 0.5f, 0.5f,  0.5f);

    // Bottom face (y = -0.5f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);

    // Front face  (z = 0.5f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f( 0.5f,  0.5f, 0.5f);
    glVertex3f(-0.5f,  0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f( 0.5f, -0.5f, 0.5f);

    // Back face (z = -0.5f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    // Left face (x = -0.5f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);

    // Right face (x = 0.5f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0.5f,  0.5f, -0.5f);
    glVertex3f(0.5f,  0.5f,  0.5f);
    glVertex3f(0.5f, -0.5f,  0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    glEnd();  // End of drawing color-cube
}

void display(void){
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glColor3f(1.0, 1.0, 1.0);
    //glutWireCube(.60);
    //glColor3f(0.0, 0.0, 1.0);
    cubeColor();

    //CAMERA
    objetoCamara->SetGLCamera();

    glutSwapBuffers();
}

void reshape(int width, int height){
    glViewport(0, 0, width, height);
    //CAMERA
    objetoCamara->SetGLAspectRatioCamera();
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, height/width, 1.0, 128.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);*/
}

void keyboard(unsigned char key, int x, int y){
    switch (key) {
    case 'h':
        cout<<"HELP!!"<<endl;
        break;
    case 'c':
        if(glIsEnabled(GL_CULL_FACE))
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);
        break;
    case '1':
        glRotatef(1.0, 1.0, 0.0, 0.0);
        break;
    case '2':
        glRotatef(1.0, 0.0, 1.0, 0.0);
        break;
    case 'q':
        exit(0);
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pract1");

    glutDisplayFunc(display);

    //CAMERA
    glutPassiveMotionFunc(MouseMotion);
    glutSpecialFunc(SpecialKey);
    glutMouseFunc(mouse);

    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
