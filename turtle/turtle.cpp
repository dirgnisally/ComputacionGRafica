#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>

using namespace std;

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 119:{ //UP W
        glTranslatef(0.0, 0.0, -0.5);
        break;
    }
    case 100:{ //RIGHT D
        glTranslatef(0.5, 0.0, 0.0);
        break;
    }
    case 115:{ //DOWN S
        glTranslatef(0.0, 0.0, 0.5);
        break;
    }
    case 97:{ //LEFT A
        glTranslatef(-0.5, 0.0, 0.0);
        break;
    }
    case 'q':{ // GIRA IZQ
        glRotatef(1.0,0.0,1.0,0.0);
        break;
    }
    case 'e':{ // GIRA DER
        glRotatef(-1.0,0.0,1.0,0.0);
        break;
    }
    case 'r':{ // GIRA UP
        glRotatef(1.0,1.0,0.0,0.0);
        break;
    }
    case 'f':{ // GIRA DOWN
        glRotatef(-1.0,1.0,0.0,0.0);
        break;
    }
    case 'z':{ // ++
        glScalef(1.2,1.2,1.2);
        break;
    }
    case 'x':{ // --
        glScalef(0.8,0.8,0.8);
        break;
    }
    case 27:
        exit(0);

    }
    glutPostRedisplay();
}

void reshape(int width, int height){
    int w, h;
    if(width<height){
        w = h = width;
    }else{
        w = h = height;
    }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)h / (GLfloat)w, 1.0, 128.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-2.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void draw_Turtle(){
    int i;
    int npoints = 23;
    //double x[] = {0,1.5,2.5,2.5,1.5,1,1,1.5,2,3,5,3.1,4,4.5,4.5,4,3.1,5.5,5,3,2.5,1,0};
    //double z[] = {-15,-14,-13,-12,-11,-10,-9.5,-9.5,-10,-11,-9,-8.5,-7,-6,-4,-3,-2,-2,-0.5,0.5,-1,0,1};
    double x[] = {0,0.3,0.5,0.5,0.3,0.2,0.2,0.3,0.4,0.6,1,0.62,0.8,0.9,0.9,0.8,0.62,1.1,1,0.6,0.5,0.2,0};
    double z[] = {-3,-2.8,-2.6,-2.4,-2.2,-2,-1.9,-1.9,-2,-2.2,-1.8,-1.7,-1.4,-1.2,-0.8,-0.6,-0.4,-0.4,-0.1,0.1,-0.2,0,0.2};
    glBegin(GL_LINE_LOOP);
    for(i=0; i<npoints; i++){
            glVertex3d(x[i], 0, z[i]);
    }
    for(i=npoints-1; i>=0; i--){
            glVertex3d(-x[i], 0, z[i]);
    }
    glEnd();
}

void display(){
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glColor3f(1.0,0.0,0.0);
    glColor3f(1.0, 0.0, 1.0 );
     
    //glutWireTorus(0.25,0.75, 28, 28);
    //glColor3f(0.0,0.0,1.0) ;
    //glutWireCube(0.6) ;
    draw_Turtle();

    glutSwapBuffers();
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(512, 512);
    glutCreateWindow("---Tortuga--Movimiento---");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}

