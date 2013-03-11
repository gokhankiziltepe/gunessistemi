#include <Windows.h>
#include <GL/gl.h>

#include <GL/glu.h>
#include <GL/glut.h>

#include <SOIL\SOIL.h>
#include <iostream>

/*
	Gokhan Kiziltepe
	040080168
*/	
/*
	ESC - for exit
	R - reset
	P - pause
	PAGE_UP/PAGE_DOWN - zoom in/out
	UP/DOWN/LEFT/RIGHT - panoramic direction
 */
/*
	Textures are from
	* http://www.3delyvisions.com/
	* http://www.gamedev.net/topic/463506-need-a-simple-free-space-skybox-texture/
	
	drawSkyBox method is from
	* http://www.morrowland.com/apron/tut_gl.php

	Other code parts are from
	* Course :)
*/
using namespace std;

GLint textures[6];
GLint skyboxTextures[6];
string pathArray[] = {"Textures/sun.bmp", "Textures/venusmap.bmp", "Textures/earthmap.bmp", "Textures/moonmap.bmp", "Textures/mercurymap.bmp", "Textures/marsmap.bmp"};
string skyboxPathArray[] = {"Textures/front.bmp", "Textures/back.bmp", "Textures/left.bmp", "Textures/right.bmp", "Textures/up.bmp", "Textures/down.bmp"};

GLfloat distanceZ = -50;
GLfloat lightAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightBlack[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat lightPosition[] = {0, 0, distanceZ, 1.0f};

GLfloat angle = 0;
GLdouble angle2 = 0;
GLfloat cameraAngleY = 0;
GLfloat targetCameraAngleY = 0;
GLfloat cameraAngleX = 0;
GLfloat targetCameraAngleX = 0;
GLboolean paused = 0;

GLint loadGLTexture(char *filename) {
    GLuint loadedTexture;
    loadedTexture =SOIL_load_OGL_texture
		(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS
		);
    if (loadedTexture == 0) {
        cout << "Error" <<  endl;
    }
    glBindTexture(GL_TEXTURE_2D, loadedTexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return loadedTexture;
}

void drawSkybox(float x, float y, float z, float width, float height, float length) {
    x = x - width / 2;
    y = y - height / 2;
    z = z - length / 2;


    glBindTexture(GL_TEXTURE_2D, skyboxTextures[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x, y, z + length);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x + width, y, z + length);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + width, y, z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, y, z);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, y, z + length);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x, y, z);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x + width, y, z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + width, y, z + length);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x + width, y + height, z);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x + width, y + height, z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x, y + height, z);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x, y, z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x, y, z + length);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + width, y, z + length);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x + width, y, z);
    glEnd();

}

static void draw(void) {

    lightPosition[2] = distanceZ;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawSkybox(0, 0, 0, 500, 500,500);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    
    glTranslated(0, 0, distanceZ);
    glRotated(cameraAngleX, 1, 0, 0);
    glRotated(cameraAngleY, 0, 1, 0);
	
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glMaterialfv(GL_FRONT, GL_EMISSION, lightDiffuse);
    gluSphere(quadric, 3, 30, 30);
    glMaterialfv(GL_FRONT, GL_EMISSION, lightBlack);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glRotated(angle, 0, 1, 0);
    glTranslatef(7.f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 1, 30, 30);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glRotated(angle * 2, 0, 1, 0);
    glTranslatef(2.f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 0.5, 30, 30);
    glPushMatrix();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glRotated(angle * 1.5, 0, 1, 0);
    glTranslatef(20.0f, 0.f, 0.f);
    glRotated(angle * 2, 0, 1, 0);
    gluSphere(quadric, 1.5, 30, 30);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glRotated(angle, 0, 1, 0);
    glTranslatef(4.f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 0.5, 30, 30);
    glPushMatrix();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glRotated(2 * angle, 0, 1, 0);
    glTranslatef(7.f, 0.f, 0.f);
    glRotated(angle, 0, 1, 0);
    gluSphere(quadric, 1, 30, 30);
    glPushMatrix();
    glPopMatrix();
    glPopMatrix();

    glLoadIdentity();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

    glutSwapBuffers();
}

static void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < 6; i++) {

		char *path = new char[pathArray[i].length() + 1];
		strcpy(path, pathArray[i].c_str());

		char *path2 = new char[skyboxPathArray[i].length() + 1];
		strcpy(path2, skyboxPathArray[i].c_str());

        textures[i] = loadGLTexture(path);
        skyboxTextures[i] = loadGLTexture(path2);

    }

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
}

static void timerCallback(int value) {
	if(!paused)
		angle++;

    GLfloat diff = targetCameraAngleY - cameraAngleY;
    if (abs(diff) > 0.000001) {
        cameraAngleY += diff / 15;
    }
    diff = targetCameraAngleX - cameraAngleX;
    if (abs(diff) > 0.0001) {
        cameraAngleX += diff / 15;
    }
    glutTimerFunc(10, timerCallback, 0);
    glutPostRedisplay();
}

static void Key(unsigned char key, int x, int y) {
    switch (key) { 
		case 'p':
            paused = !paused;
            break;
        case 'P':
            paused = !paused;
            break;
        case 'q':
            exit(0);
            break;
        case 'Q':
            exit(0);
            break;
        case 'r':
            angle2 = 0;
            cameraAngleX = 0;
            targetCameraAngleX = 0;
            cameraAngleY = 0;
            targetCameraAngleY = 0;
            break;
        case 'R':
            angle2 = 0;
            cameraAngleX = 0;
            targetCameraAngleX = 0;
            cameraAngleY = 0;
            targetCameraAngleY = 0;
            break;
    }
}

void specialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            targetCameraAngleY += 30;
            break;
        case GLUT_KEY_RIGHT:
            targetCameraAngleY -= 30;
            break;
        case GLUT_KEY_UP:
            targetCameraAngleX += 15;
            break;
        case GLUT_KEY_DOWN:
            targetCameraAngleX -= 15;
            break;
        case GLUT_KEY_PAGE_DOWN:
            distanceZ -= 1;
            break;
        case GLUT_KEY_PAGE_UP:
            distanceZ += 1;
            break;
    }
}

int main(int argc, char ** argv) {
    GLenum type;
    glutInit(&argc, argv);
    glutInitWindowSize(1024, 1024);
    type = GLUT_RGB;
    type |= GLUT_DOUBLE;
    type |= GLUT_DEPTH;
    type |= GLUT_MULTISAMPLE;
    glutInitDisplayMode(type);
    glutCreateWindow("Gokhan Kiziltepe");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, 1, 0.1, 600);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    init();
    timerCallback(0);
    glutKeyboardFunc(Key);
    glutSpecialFunc(specialKey);
    glutDisplayFunc(draw);
    glutMainLoop();
    return 0;
}
