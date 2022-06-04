#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL\glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};

double wheelRadius = 50;
double wheelWidth = 25;
double xMax = 200, xMin = -200;
double yMax = 200, yMin = -200;
double planeZ = -wheelRadius;

point moveDirection;
point rotationAxis;
point verticalAxis;

point newOrigin;

double rotationAngel = 0;
double rotationUnit = 5;
double translationUnit;
double directionChange = 0;



void printPoint(point p){
    printf("%lf %lf %lf\n",p.x,p.y,p.z);
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}


void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}


point rotateVectorCCW(point vect, point helper, double sign = 1.0){
    double rotationSinA = sin(rotationUnit*pi/180);
    double rotationCosA = cos(rotationUnit*pi/180);

    point temp;
    // perp(vect) is helper
    point perpVectsinA;
    point vectCosA;

    perpVectsinA.x = helper.x * rotationSinA * sign;
    perpVectsinA.y = helper.y * rotationSinA * sign;
    perpVectsinA.z = helper.z * rotationSinA * sign;

    vectCosA.x = vect.x * rotationCosA;
    vectCosA.y = vect.y * rotationCosA;
    vectCosA.z = vect.z * rotationCosA;

    temp.x = perpVectsinA.x + vectCosA.x;
    temp.y = perpVectsinA.y + vectCosA.y;
    temp.z = perpVectsinA.z + vectCosA.z;

    return temp;
}

point rotateVectorCW(point vect, point helper, double sign = 1.0){
    double rotationSinA = sin(-rotationUnit*pi/180);
    double rotationCosA = cos(-rotationUnit*pi/180);
    point temp;
    // perp(vect) is helper
    point perpVectsinA;
    point vectCosA;

    perpVectsinA.x = helper.x * rotationSinA * sign;
    perpVectsinA.y = helper.y * rotationSinA * sign;
    perpVectsinA.z = helper.z * rotationSinA * sign;

    vectCosA.x = vect.x * rotationCosA;
    vectCosA.y = vect.y * rotationCosA;
    vectCosA.z = vect.z * rotationCosA;

    temp.x = perpVectsinA.x + vectCosA.x;
    temp.y = perpVectsinA.y + vectCosA.y;
    temp.z = perpVectsinA.z + vectCosA.z;

    return temp;
}

void moveRight(){
    directionChange -= rotationUnit;
    point temp = rotateVectorCW(rotationAxis, moveDirection, -1.0);
    moveDirection = rotateVectorCW(moveDirection, rotationAxis);
    rotationAxis = temp;
    //printPoint(moveDirection);
    //printPoint(rotationAxis);
}

void moveLeft(){
    directionChange += rotationUnit;
    point temp = rotateVectorCCW(moveDirection, rotationAxis);
    rotationAxis = rotateVectorCCW(rotationAxis, moveDirection,-1.0);
    moveDirection = temp;
    //printPoint(moveDirection);
    //printPoint(rotationAxis);
}

void drawPlane(){
    point points[2][25];
    for(int i=0;i<=20;i++){
        points[0][i].x = points[1][i].x = xMin + 20.0*i;
        points[0][i].y = yMin;
        points[1][i].y = yMax;
        points[0][i].z = points[1][i].z = planeZ;
    }

    glColor3f(1.0,1.0,1.0);
    for(int i=1;i<=19;i++){
        glBegin(GL_LINES);
        {
			glVertex3f(points[0][i].x, points[0][i].y, points[0][i].z);
			glVertex3f(points[1][i].x, points[1][i].y, points[1][i].z);
        }
        glEnd();
    }

    for(int i=0;i<=20;i++){
        points[0][i].y = points[1][i].y = yMin + 20.0*i;
        points[0][i].x = xMin;
        points[1][i].x = xMax;
        points[0][i].z = points[1][i].z = planeZ;
    }

    for(int i=1;i<=19;i++){
        glBegin(GL_LINES);
        {
			glVertex3f(points[0][i].x, points[0][i].y, points[0][i].z);
			glVertex3f(points[1][i].x, points[1][i].y, points[1][i].z);
        }
        glEnd();
    }
}


void drawCylinder(double radius, double height, int slices, int stacks){
    struct point points[stacks + 1][slices + 1];
	int i,j;

	//generate points
	for(i=0;i<=stacks;i++){
		for(j=0;j<=slices;j++){
			points[i][j].x=radius*cos(((double)j/(double)slices)*pi*2.0);
            points[i][j].y=(double)i*height/stacks;
            points[i][j].z=radius*sin(((double)j/(double)slices)*pi*2.0);
		}
	}

	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks, (double)i/(double)stacks, (double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    double colorSegment;
		    if(j < slices/2){
                colorSegment = 2.0*(double)j/(double)slices;
		    }
		    else{
                colorSegment = 2.0*(1 - (double)j/(double)slices);
		    }

		    glColor3f(colorSegment,colorSegment,colorSegment);

			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y, points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y, points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y, points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawWheel(){
    drawCylinder(wheelRadius,wheelWidth,50,50);
    glPushMatrix();

    glColor3f(0.5,0.5,0.5);
    glRotatef(90,0,0,1);
    glBegin(GL_QUADS);{
        //upper hemisphere
        glVertex3f(wheelWidth/4,0,planeZ);
        glVertex3f(wheelWidth*0.75,0,planeZ);
        glVertex3f(wheelWidth*0.75,0,wheelRadius);
        glVertex3f(wheelWidth/4,0,wheelRadius);

    }glEnd();

    glPopMatrix();

    glColor3f(0.75,0.75,0.75);
    glRotatef(90,0,1,0);
    glRotatef(90,0,0,1);
    glBegin(GL_QUADS);{
        //upper hemisphere
        glVertex3f(wheelWidth/4,0,planeZ);
        glVertex3f(wheelWidth*0.75,0,planeZ);
        glVertex3f(wheelWidth*0.75,0,wheelRadius);
        glVertex3f(wheelWidth/4,0,wheelRadius);

    }glEnd();
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'w':
            rotationAngel += rotationUnit;
            newOrigin.x += moveDirection.x * translationUnit;
            newOrigin.y += moveDirection.y * translationUnit;
            newOrigin.z += moveDirection.z * translationUnit;
            break;
        case 's':
            rotationAngel -= rotationUnit;
            newOrigin.x -= moveDirection.x * translationUnit;
            newOrigin.y -= moveDirection.y * translationUnit;
            newOrigin.z -= moveDirection.z * translationUnit;
            break;
        case 'd':
            moveRight();
            break;
        case 'a':
            moveLeft();
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);


    drawPlane();



    ///translation
    glTranslatef(newOrigin.x, newOrigin.y, newOrigin.z);

    ///rotation
    glRotatef(rotationAngel, rotationAxis.x, rotationAxis.y, rotationAxis.z);

    /// direction change
    glRotatef(directionChange,0,0,1);

    /// move wheel center
    glTranslatef(0,-wheelWidth/2,0);
    drawWheel();

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	newOrigin.x = 0;
	newOrigin.y = 0;
	newOrigin.z = 0;

	moveDirection.x = 1;
	moveDirection.y = 0;
	moveDirection.z = 0;

	rotationAxis.x = 0;
	rotationAxis.y = 1;
	rotationAxis.z = 0;

	verticalAxis.x = 0;
	verticalAxis.y = 0;
	verticalAxis.z = 1;

	//calculate translation unit
	translationUnit = rotationUnit / 360.0 * 2.0 * pi * wheelRadius;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
