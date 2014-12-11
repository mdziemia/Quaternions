// -----------------------------------------------------------------------------
// Quaternions in OpenGL (+GLUT)
// Maciej Dziemianczuk
//
// References:
// 1. http://www.gamasutra.com/features/19980703/quaternions_01.htm
// 2. http://www.gamedev.net/reference/articles/article1095.asp
//
// An example how to use Quaternions in 3D rotating
// It is also shown how to create spherical linear interpolation between
// two quaternions.
//
// Controlls:
//
// - Left,Right,Up,Down,M,N - control the First Quaternion
// - W,S,A,D,Z,X            - control the Second Quaternion
//
// -----------------------------------------------------------------------------
#include "quaternions.h"

int idWindow;

GLUquadricObj *sphere = gluNewQuadric();

GLfloat LightAmbient[]=		{ 0.0f, 0.2f, 0.8f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ 0.0f, 2.0f, -2.0f, 1.0f };
GLfloat matSpacular[] =     { 0.8f, 0.8f, 0.8f, 1.0f };


// Two main quaternions, for which we shall create the path on unite sphere
// (Spherical Linear intERPolation)
CQuaternion qPoint(1.0, 0.0, 0.0, 0.0);
CQuaternion qPoint2(1.0, 0.0, 0.0, 0.0);

// Two Points according to the above quaternions which we will draw on the screen
CVector3 Point(1.0, 0.0, 0.0);
CVector3 Point2(1.0, 0.0, 0.0);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Spherical Linear interpolation
// Array for that quaternions and 3D points between two main above points
#define _MAX_POINTS 6
CVector3 arrPoints[_MAX_POINTS+1] = {};
CQuaternion arrQuat[_MAX_POINTS+1] = {};


// Additional quaternion and 3D point which will be roatete around the vector
// (Point - (0,0,0))
CQuaternion qSatelite(CVector3(0.995, 0.1, 0.0));
CVector3 Satelite(1.0, 0.1, 0.0);

// Angles for rotating the scene
CVector3 Rotate(45.0f, 45.0f, 45.0f);


// Increase or derease depending on your computer
float How_Fast_Coeff = 1.0f;

#define TIMERSECS 100

// For keyboard control
float angle = 0.0f, angle2 = 0.0f;
CVector3 vec;

// -----------------------------------------------------------------------------
// Create path between two points using SLERP
void CreatePath(const CQuaternion &_q1, const CQuaternion &_q2)
{
    float coef = 1.0f/(float)_MAX_POINTS;

    for (int i=0; i<=_MAX_POINTS; ++i)
    {
        arrQuat[i] = _q1.QuatSlerp(_q2, coef*(float)i);
        arrPoints[i] = arrQuat[i].toPoint();
    }
}

// -----------------------------------------------------------------------------
void DrawPath(void)
{
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<=_MAX_POINTS; ++i)
    {
        glVertex3f(arrPoints[i].x, arrPoints[i].y, arrPoints[i].z);
    }
    glEnd();
}

// -----------------------------------------------------------------------------
void DrawAxis(void)
{
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0); // Axis X: red
        glVertex3f(-1.0,0.0,0.0);
        glVertex3f(1.0,0.0,0.0);

        glColor3f(0.0,1.0,0.0); // Axis Y: green
        glVertex3f(0.0,-1.0,0.0);
        glVertex3f(0.0,1.0,0.0);

        glColor3f(0.0,0.0,1.0); // Axis Z: blue
        glVertex3f(0.0,0.0,-1.0);
        glVertex3f(0.0,0.0,1.0);
    glEnd();
}

// -----------------------------------------------------------------------------
void DrawGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -3.0);
    glRotatef(Rotate.x, 1.0, 0.0, 0.0);


    glColor4f(1.0, 1.0, 1.0, 0.1);
    gluSphere(sphere, 1.0, 20, 20);

    // Rotate scene by Quaternions
    // glRotatef(Rotate.y, 0.0, 1.0, 0.0);
    float Matrix[16];
    CQuaternion q(Rotate.y, CVector3(0.0,1.0,0.0));
    q.getMatrix(Matrix);
    glMultMatrixf(Matrix);


    // draw Axis
    DrawAxis();

    // get 3D Points according to their Quaternions
    Point = qPoint.toPoint();
    Point2 = qPoint2.toPoint();


    // Draw auxiliary lines
    glBegin (GL_LINES);
        glColor4f   (0.5f, 0.0f, 0.0f, 0.5f);
        glVertex3f  (Point.x, 0.0f, 0.0f);
        glVertex3fv ((GLfloat *)&Point);

        glColor4f   (0.0f, 0.5f, 0.0f, 0.5f);
        glVertex3f  (0.0f, Point.y, 0.0f);
        glVertex3fv ((GLfloat *)&Point);

        glColor4f   (0.0f, 0.0f, 0.5f, 0.5f);
        glVertex3f  (0.0f, 0.0f, Point.z);
        glVertex3fv ((GLfloat *)&Point);

        glColor3f   (1.0f, 1.0f, 1.0f);
        glVertex3f  (0.0f, 0.0f, 0.0f);
        glVertex3fv ((GLfloat *)&Point);
    glEnd();


    // Draw the First Point
    glPointSize(8.0);
    glColor3f(1.0, 1.0, 0.0);
    qPoint.glDraw();
    qPoint2.glDraw();


    // Update satelite position
    qSatelite *= CQuaternion(How_Fast_Coeff, Point);
    Satelite = qSatelite.toPoint();

    // Draw the path between Point and Point2
    DrawPath();


    // Draw Satelite
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 1.0);
    qSatelite.glDraw();

    glBegin (GL_LINES);
        glColor3f (0.5f, 0.5f, 0.5f);
        glVertex3fv ((GLfloat *)&Satelite);
        glVertex3fv ((GLfloat *)&Point);

        glColor3f (0.5f, 0.5f, 0.5f);
        glVertex3fv ((GLfloat *)&Point);
        glVertex3fv ((GLfloat *)&Point2);
    glEnd();

	glutSwapBuffers();
}




// -----------------------------------------------------------------------------
void Animate()
{
  Rotate.y += 0.1*How_Fast_Coeff;
  glutPostRedisplay();
}

// -----------------------------------------------------------------------------
void InitGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//glOrtho( -1.2, 1.2, -1.0, 1.0, 0.0, 10.0 );
	gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);

	glPointSize(8.0);
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    glPolygonMode(GL_FRONT,GL_LINE);
	glutSwapBuffers();
}

// -----------------------------------------------------------------------------
void ReshapeGL(int width, int height)
{
  glViewport(0, 0, width, height);
}




void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key)
	{

	  	case 27: // Escape key
      		glutDestroyWindow ( idWindow );
      		exit (0);
      		break;

		case ' ' :
			Point = CVector3(1.0,0.0,0.0);
			Satelite = CVector3(1.0, 0.1, 0.0);
			break;

		case 'm' :
			angle =  How_Fast_Coeff; vec = CVector3(0.0, 0.0, 1.0);
			break;

		case 'n' :
			angle = -How_Fast_Coeff; vec = CVector3(0.0, 0.0, 1.0);
			break;

		case 'd' :
			angle2 =  How_Fast_Coeff; vec = CVector3(0.0, 0.0, 1.0);
			break;

		case 'a' :
			angle2 = -How_Fast_Coeff; vec = CVector3(0.0, 0.0, 1.0);
			break;

		case 'w' :
			angle2 =  How_Fast_Coeff; vec = CVector3(0.0, 1.0, 0.0);
			break;

		case 's' :
			angle2 = -How_Fast_Coeff; vec = CVector3(0.0, 1.0, 0.0);
			break;

		case 'x' :
			angle2 =  How_Fast_Coeff; vec = CVector3(1.0, 0.0, 0.0);
			break;

		case 'z' :
			angle2 = -How_Fast_Coeff; vec = CVector3(1.0, 0.0, 0.0);
			break;

	}

	if (angle)
	{
		// Multiple quaternions (rotate)
		qPoint *= CQuaternion(angle, vec);
		qSatelite *= CQuaternion(angle, vec);

		// Create path between Point and Point2
		CreatePath(qPoint, qPoint2);
		angle = 0.0f;
	}

	if (angle2)
	{
		// Multiple quaternions (rotate)
		qPoint2 *= CQuaternion(angle2, vec);
		angle2 = 0.0f;

		// Create path between Point and Point2
		CreatePath(qPoint, qPoint2);
	}
}




void processSpecialKeys(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_LEFT :
			angle =  How_Fast_Coeff; vec = CVector3(0.0, 1.0, 0.0);
			break;
		case GLUT_KEY_RIGHT :
			angle = -How_Fast_Coeff; vec = CVector3(0.0, 1.0, 0.0);
			break;

		case GLUT_KEY_UP :
			angle =  How_Fast_Coeff; vec = CVector3(1.0, 0.0, 0.0);
			break;

		case GLUT_KEY_DOWN :
			angle = -How_Fast_Coeff; vec = CVector3(1.0, 0.0, 0.0);
			break;



		case GLUT_KEY_F1 :
				break;

		case GLUT_KEY_F10:
			exit(0);
			break;
	}

	if (angle)
	{
		// Multiple quaternions (rotate)
		qPoint *= CQuaternion(angle, vec);
		qSatelite *= CQuaternion(angle, vec);

		// Create path between Point and Point2
		CreatePath(qPoint, qPoint2);
		angle = 0.0f;
	}

	if (angle2)
	{
		// Multiple quaternions (rotate)
		qPoint2 *= CQuaternion(angle2, vec);
		angle2 = 0.0f;

		// Create path between Point and Point2
		CreatePath(qPoint, qPoint2);
	}

}

// -----------------------------------------------------------------------------
int main (int argn, char **args)
{
	glutInit(&argn, args);
	glutInitWindowPosition(100, 100);
  	glutInitWindowSize(500, 500);
  	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	idWindow = glutCreateWindow(args[0]);


	glutDisplayFunc(DrawGL);
	glutReshapeFunc(ReshapeGL);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutIdleFunc(Animate);


	glutReshapeWindow(640,480);
	printf("GL_RENDERER = %s\n",glGetString(GL_RENDERER));

	InitGL();
  	glutMainLoop();

  	printf("The end...\n");

}



