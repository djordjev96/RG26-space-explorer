#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "image.h"

#define TIMER_ID 1
#define TIMER_INTERVAL 20
#define FILENAME0 "textures/sky.bmp"
#define FILENAME1 "textures/sun.bmp"
#define FILENAME2 "textures/mercury.bmp"
#define FILENAME3 "textures/venus.bmp"
#define FILENAME4 "textures/earth.bmp"
#define FILENAME5 "textures/mars.bmp"
#define FILENAME6 "textures/jupiter.bmp"
#define FILENAME7 "textures/saturn.bmp"
#define FILENAME8 "textures/uranus.bmp"
#define FILENAME9 "textures/neptune.bmp"

GLuint names[9];
float hours;
static int animation_ongoing;
static int motor = 0;
static int motor2 = 0;

static int widthW,heightW;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int id);
static void initialize(void);
static void draw_planet(float planet_revolution, float planet_rotation, float distance, float size, int planet_num, GLUquadric* quad);
static void create_texture(int num, char* name, Image * image);


/* 
Radius Sunca - 	696.392 km - stavljamo na 1000
Radius Zemlje - 6371 km - 109 puta manji od Sunca - stavljamo na 9.17
Udaljenost Zemlje od Sunca - 149.600.000 km - prevelika vrednost
    pa cemo da stavimo kao tri puta radius sunca
Radius Merkura - 2439,7 km - 285 puta manji od sunca - stavljamo na 3.51
Udaljenost Merkura od Sunca - 57.910.000 km - stavljamo na 120
Radius Venere - 6051,8 km - 115 puta manji od Sunca - stavljamo na 8.7
Udaljenost Venere od Sunca - 108.200.000 km - stavljamo na 217.4
Radius Marsa - 3.389,5 km - 205 puta manji od Sunca - stavljamo na 4.88
Udaljenost Marsa od Sunca - 227.900.000 km - stavljamo na 461.5
Radius Jupitera - 69.911 km - 10 puta manji od sunca - stavljamo na 100
Udaljenost Jupitera od Sunca - 778.500.000 km - (1562.5) ali zbog prevelike vrednosti cemo staviti na 600
Radius Saturna - 58.232 km  - 12 puta manji od Sunca - stavljamo na 83.3
Udaljenost Saturna od Sunca - 1.434.000.000 km - (prevelika vrednost) stavicemo na 750
Radius Urana - 25.362 km - 27 puta manji od Sunca - stavljamo na 37
Udaljenost Urana od Sunca - 2.871.000.000 km - stavljamo na 850
Radius Neptuna - 24.622 km - 28 puta manji od Sunca - stavljamo na 35.7
Udaljenost Neptuna od Sunca - 4.495.000.000 km - stavljamo na 900
*/


int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Space Explorer");

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);

    /* Inicijalizujemo promenljive */
    hours = 0;
    animation_ongoing = 0;

    initialize();

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void initialize(void)
{
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Postavlja se boja pozadine. */
    glClearColor(0,0,0,0);

    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(9, names);

    create_texture(0, FILENAME0, image);
    create_texture(1, FILENAME1, image);
    create_texture(2, FILENAME2, image);
    create_texture(3, FILENAME3, image);
    create_texture(4, FILENAME4, image);
    create_texture(5, FILENAME5, image);
    create_texture(6, FILENAME6, image);
    create_texture(7, FILENAME7, image);
    create_texture(8, FILENAME8, image);
    create_texture(9, FILENAME9, image);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

static void on_timer(int id)
{
    if (TIMER_ID != id)
        return;

    hours += 0.1;
    
    glutPostRedisplay();

    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;

    case 'g':
    case 'G':
        /* Pokrecemo animaciju */
        if (!animation_ongoing) {
            animation_ongoing = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
        }
        break;
    case 'w':
    case 'W':
        motor+=1;
        break;
    case 'd':
    case 'D':
        motor2+=1;
        break;
    case 'a':
    case 'A':
        motor2-=1;
        break;
    case 's':
    case 'S':
        motor-=1;
        break;
    case 'h':
    case 'H':
        animation_ongoing = 0;
        break;
    }
}

static void draw_planet(float planet_revolution, float planet_rotation, float distance, float size, int planet_num, GLUquadric* quad) {
    glPushMatrix();
        glRotatef(planet_revolution, 0,0,1);
        glTranslatef(distance,0,0); 
        glRotatef(planet_rotation, 0,0,1);
        gluQuadricNormals(quad, GLU_SMOOTH);
	    gluQuadricTexture(quad, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, names[planet_num]);
        gluSphere(quad,size,50,50);
    glPopMatrix();
glBindTexture(GL_TEXTURE_2D, 0);
}

static void create_texture(int num, char* name, Image * image) {
    image_read(image, name);

    glBindTexture(GL_TEXTURE_2D, names[num]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

}

static void on_reshape(int width, int height)
{
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 1500);
    widthW = width; /*remembering width and height on reshape in global variables*/
	heightW = height;
}

static void on_display(void)
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(0, 0, 800, 300, 0, 0, 0, 1, 0);

     glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glOrtho(0 , widthW, 0 , heightW, -1, 1);



    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex2i(0, 0);

        glTexCoord2f(1, 0);
        glVertex2i(widthW, 0);

        glTexCoord2f(1, 1);
        glVertex2i(widthW, heightW);

        glTexCoord2f(0, 1);
        glVertex2i(0, heightW);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST); /*Re enabling depth testing*/
    glClear(GL_DEPTH_BUFFER_BIT); /*Clearing depth buffer*/
    glLoadIdentity();
        gluPerspective(60, (float) widthW/heightW, 1, 1000); /*Setting perspective again*/
    
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    gluLookAt(700, 0, 900, 500, 0, 0, 0, 1, 0);
//    gluLookAt(400, 0, 700, 300, 0, 0, 0, 1, 0);

    
    GLUquadric* quad = gluNewQuadric();

    // Ugao rotacije Sunca oko svoje ose 
    float sun_rotation = 360*hours/(30*24);

    // Sunce
    glPushMatrix();
        glRotatef(sun_rotation, 0,0,1);
        gluQuadricNormals(quad, GLU_SMOOTH);
	    gluQuadricTexture(quad, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, names[1]);
        gluSphere(quad,300,50,50);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Uglovi Merkurove revolucije i rotacije
    float mercury_revolution = 360*hours/(88*24);
    float mercury_rotation = 360*hours/(58.7*24);
    
    // Merkur
    draw_planet(mercury_revolution, mercury_rotation, 350, 3.51, 2, quad);
    
    // Uglovi Venerine revolucije i rotacije
    float venus_revolution = 360*hours/(224.7*24);
    float venus_rotation = 360*hours/(243*24); 

    // Venera
    draw_planet(venus_revolution, venus_rotation, 400, 8.7, 3, quad);

    // Uglovi Zemljine revolucije i rotacije
    float earth_revolution = 360 * hours / (365 * 24);
    float earth_rotation = 360*hours/24;

    // Zemlja
    draw_planet(earth_revolution, earth_rotation, 450, 9.17, 4, quad);    

    // Uglovi Marsove revolucije i rotacije
    float mars_revolution = 360 * hours / (686.98 * 24);
    float mars_rotation = 360*hours/24.623;

    // Mars
    draw_planet(mars_revolution, mars_rotation, 480, 4.88, 5, quad);

    // Uglovi Jupiterove revolucije i rotacije
    float jupiter_revolution = 360 * hours / (4332.59 * 24);
    float jupiter_rotation = 360*hours/9.912;

    // Jupiter
    draw_planet(jupiter_revolution, jupiter_rotation, 600, 100, 6, quad);
    
    // Uglovi Saturnove revolucije i rotacije
    float saturn_revolution = 360 * hours / (10759 * 24);
    float saturn_rotation = 360*hours/10.656;

    // Saturn(600)
    draw_planet(saturn_revolution, saturn_rotation, 800, 83.3, 7, quad);

    // Uglovi Uranove revolucije i rotacije
    float uranus_revolution = 360 * hours / (30685 * 24);
    float uranus_rotation = 360*hours/17.24;

    // Uran
    draw_planet(uranus_revolution, uranus_rotation, 930, 37, 8, quad);

    // Uglovi Neptunove revolucije i rotacije
    float neptune_revolution = 360 * hours / (60190 * 24);
    float neptune_rotation = 360*hours/19.1;

    // Neptun
    draw_planet(neptune_revolution, neptune_rotation, 1010, 35.7, 9, quad);

    // glPushMatrix();
    //     glTranslatef(500 - motor,300 + motor2,0); 
    //     gluSphere(quad,2,50,50);
    // glPopMatrix();
    glPushMatrix();
        glTranslatef(500 - motor,300 + motor2,0); 
        glRotatef(0, 1+motor2,1,0);
        gluSphere(quad,2,50,50);
    glPopMatrix();

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}