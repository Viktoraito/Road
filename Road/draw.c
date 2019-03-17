#include "common.h"

#define MY_ENCODING "ISO-8859-1"

int procIsAsking;
int x,y, px[3],py[3],dx[3],dy[3], t_num;
float **map_o; float **map_r;
FILE *fmap;

void testXmlwriterFilename(const char *uri);

void drawRoads(float dtx, float dty) {
for(int i=0; i<y; i++) {
    for(int j=0; j<x; j++) {
	  glColor3f(0.0f,0.0f,0.0f);
      if(fabs(map_o[i][j]-1.0)<1)
          glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);
		glVertex3f((j+1)*dtx-dtx,(i+1)*dty-dty,0.0f); //upper left
		glVertex3f((j+1)*dtx,    (i+1)*dty-dty,0.0f); //upper right
		glVertex3f((j+1)*dtx,    (i+1)*dty,    0.0f); //bottom right
		glVertex3f((j+1)*dtx-dtx,(i+1)*dty,    0.0f); //bottom left
	glEnd();
    }
  }	
}


void drawPoints(float dtx, float dty) {
  glColor3f(1.0f,0.0f,1.0f);
	for(int i=0; i<t_num; i++) {
	glBegin(GL_QUADS);
		glVertex3f(py[i]*dtx-dtx/2,px[i]*dty-dty/2,0.2f); //upper left
		glVertex3f(py[i]*dtx,    px[i]*dty-dty/2,0.2f); //upper right
		glVertex3f(py[i]*dtx,    px[i]*dty,    0.2f); //bottom right
		glVertex3f(py[i]*dtx-dtx/2,px[i]*dty,    0.2f); //bottom left
	glEnd();
        }
}

void drawLights(float dtx, float dty) {
for(int i=0; i<y; i++) {
    for(int j=0; j<x; j++) {
      if(fabs(map_o[i][j]-3.1)<0.1) {
          glColor3f(0.0f,1.0f,0.0f);
	      glBegin(GL_QUADS);
		glVertex3f((j+1)*dtx-dtx,(i+1)*dty-dty,0.0f); //upper left
		glVertex3f((j+1)*dtx,    (i+1)*dty-dty,0.0f); //upper right
		glVertex3f((j+1)*dtx,    (i+1)*dty,    0.0f); //bottom right
		glVertex3f((j+1)*dtx-dtx,(i+1)*dty,    0.0f); //bottom left
	      glEnd();
	}
      if(fabs(map_o[i][j]-3.2)<0.1) {
          glColor3f(1.0f,0.0f,0.0f);
	      glBegin(GL_QUADS);
		glVertex3f((j+1)*dtx-dtx,(i+1)*dty-dty,0.0f); //upper left
		glVertex3f((j+1)*dtx,    (i+1)*dty-dty,0.0f); //upper right
		glVertex3f((j+1)*dtx,    (i+1)*dty,    0.0f); //bottom right
		glVertex3f((j+1)*dtx-dtx,(i+1)*dty,    0.0f); //bottom left
	      glEnd();
	}
   }
  }
}

void drawPedestr(float dtx, float dty) {
  for(int i=0; i<y; i++) {
    for(int j=0; j<x; j++) {
      if(fabs(map_o[i][j]-2.0)<1) {
          glColor3f(0.0f,0.0f,1.0f);
        glBegin(GL_QUADS);
         glVertex3f((j+1)*dtx-dtx/2,(i+1)*dty-dty/2,0.0f); //upper left
	 glVertex3f((j+1)*dtx,    (i+1)*dty-dty/2,0.0f); //upper right
	 glVertex3f((j+1)*dtx,    (i+1)*dty,    0.0f); //bottom right
	 glVertex3f((j+1)*dtx-dtx/2,(i+1)*dty,    0.0f); //bottom left
        glEnd();
	}
      if(fabs(map_o[i][j]-2.2)<0.1) {
        glColor3f(1.0f,1.0f,0.0f);
        glBegin(GL_QUADS);
         glVertex3f((j+1)*dtx-dtx/2,(i+1)*dty-dty/2,0.0f); //upper left
	 glVertex3f((j+1)*dtx,    (i+1)*dty-dty/2,0.0f); //upper right
	 glVertex3f((j+1)*dtx,    (i+1)*dty,    0.0f); //bottom right
	 glVertex3f((j+1)*dtx-dtx/2,(i+1)*dty,    0.0f); //bottom left
        glEnd();
	}
    }
  }
}

void drawVehic(float dtx, float dty) {
  glColor3f(0.0f,1.0f,1.0f);
	for(int i=0; i<t_num; i++) {
	glBegin(GL_QUADS);
		glVertex3f(dy[i]*dtx-dtx/2,dx[i]*dty-dty/2,0.2f); //upper left
		glVertex3f(dy[i]*dtx,    dx[i]*dty-dty/2,0.2f); //upper right
		glVertex3f(dy[i]*dtx,    dx[i]*dty,    0.2f); //bottom right
		glVertex3f(dy[i]*dtx-dtx/2,dx[i]*dty,    0.2f); //bottom left
	glEnd();
        }
}

void Draw() {
  FILE *fcoord;
  fcoord = fopen("coord", "r");
for(int i=0; i<3; i++) {
  fscanf(fcoord, "%d", &dx[i]);
  fscanf(fcoord, "%d", &dy[i]);
  fscanf(fcoord, "%d", &px[i]);
  fscanf(fcoord, "%d", &py[i]);
}
  fclose(fcoord);
  fmap = fopen("map", "r");
  fscanf(fmap, "%d", &x);
  fscanf(fmap, "%d", &y);
  for(int i=0; i<y; i++)
    for(int j=0; j<x; j++)
      fscanf(fmap, "%f", &map_o[i][j]);
  fclose(fmap);  

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /*clear screen*/
  glLoadIdentity(); /*move (0,0) to center of screen*/
  glTranslatef(-1.0f,1.0f,0.0f); /*move (0,0) to left upper corner*/
  glRotatef(180.0, 1.0, 0.0, 0.0); /*rotate screen over X axis*/
  //glRotatef(180.0, 0.0, 1.0, 0.0); /*rotate screen over Y axis*/
  
  float dtx=2.0/x, dty=2.0/y;

  drawRoads(dtx, dty);
  drawPoints(dtx, dty);
  drawLights(dtx, dty);
  drawPedestr(dtx,dty);

  sleep(1);

  drawVehic(dtx,dty);
  glutSwapBuffers();
}

void timer (int t) {
   glutDisplayFunc(Draw);
   glutPostRedisplay();
   glutTimerFunc(100, timer, 0); 	
}

int main(int argc, char **argv) {
  if(argc>1)
    t_num = atoi(argv[1]);
  else
    t_num = 1;

  fmap = fopen("map", "r");
  fscanf(fmap, "%d", &x);
  fscanf(fmap, "%d", &y);
  glutInit(&argc, argv);

  map_o = (float**)malloc(x * sizeof(float*));
  for(int i=0; i<y; i++) {
    map_o[i] = (float*)malloc(x * sizeof(float));
    for(int j=0; j<x; j++){
      fscanf(fmap, "%f", &map_o[i][j]);
    }
  }
  fclose(fmap);

   glutInitDisplayMode(GLUT_DEPTH| GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize(100*x, 100*y);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("ROAD DEMO");
   glutTimerFunc(1, timer, 0);
   glutMainLoop();  

  for (int i=0; i<x; i++) {
    free(map_o[i]);
  }
  free(map_o);

  exit(EXIT_SUCCESS);
}

