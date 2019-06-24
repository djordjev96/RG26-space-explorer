#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "src/image.h"
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

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
static float goUD = 0;
static float goLR = 0;
static float camera = 0;
static bool foundedItems[] = {false, false, false};
static int speedOfGame = 1;
static GLUquadric* quad;

static int widthW,heightW;
GLdouble ship_position[] = {940.0, 450.0, 0};

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int id);
static void initialize(void);
static void draw_planet(float planet_revolution, float planet_rotation, float distance, float size, int planet_num, float position);
static void create_texture(int num, char* name, Image * image);
static void create_map();
void createItem(int x, int y, int i);
void SpecialInput(int key, int x, int y);

/* 
Radius Sunca - 	696.392 km - stavljamo na 1000 - zbog prevelike vrednosti stavljamo na 300
Radius Zemlje - 6371 km - 109 puta manji od Sunca - stavljamo na 9.17
Radius Merkura - 2439,7 km - 285 puta manji od sunca - stavljamo na 3.51
Radius Venere - 6051,8 km - 115 puta manji od Sunca - stavljamo na 8.7
Radius Marsa - 3.389,5 km - 205 puta manji od Sunca - stavljamo na 4.88
Radius Jupitera - 69.911 km - 10 puta manji od sunca - stavljamo na 100
Radius Saturna - 58.232 km  - 12 puta manji od Sunca - stavljamo na 83.3
Radius Urana - 25.362 km - 27 puta manji od Sunca - stavljamo na 37
Radius Neptuna - 24.622 km - 28 puta manji od Sunca - stavljamo na 35.7
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
    glutSpecialFunc(SpecialInput);
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

    quad = gluNewQuadric();

    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);


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

    // menjamo brzinu simulacije u zavisnosti od odabira
    if(speedOfGame == 1) {
        hours += 0.2;
    }
    else if(speedOfGame == 2) {
        hours += 2;    
    }
    else if(speedOfGame == 3) {
        hours += 10;    
    }

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
    case 's':
    case 'S':
        // zaustavljamo animaciju
        animation_ongoing = 0;
        break;
    case 'a':
    case 'A':
        // pomeramo kameru ulevo
        camera -= 5;
        break;
    case 'd':
    case 'D':
        // pomeramo kameru udesno
        camera += 5;
        break;
    // biramo brzinu simulacije
    case '1':
        speedOfGame = 1;
        break;
    case '2':
        speedOfGame = 2;
        break;
    case '3':
        speedOfGame = 3;
        break;
    }
}

void SpecialInput(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            goUD -= 2;
            break;	
        case GLUT_KEY_DOWN:
            goUD += 2;
            break;
        case GLUT_KEY_LEFT:
            goLR -= 2;
            break;
        case GLUT_KEY_RIGHT:
            goLR += 2;    
            break;
    }
}
// f-ja koja iscrtava planetu
static void draw_planet(float planet_revolution, float planet_rotation, float distance, float size, int planet_num, float position) {
    glPushMatrix();
        // rotacija oko Sunca
        glRotatef(planet_revolution,0,0,1);
        // udaljenost od Sunca
        glTranslatef(distance,position,0);
        // rotacija oko svoje ose 
        glRotatef(planet_rotation, 0,0,1);
        gluQuadricNormals(quad, GLU_SMOOTH);
	    gluQuadricTexture(quad, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, names[planet_num]);
        // ukoliko je u pitanju Saturn crtamo i disk oko njega
        if(planet_num == 7) {
            gluDisk(quad, 90, 150, 50, 50);
        }
        gluSphere(quad,size,50,50);
    glPopMatrix();
glBindTexture(GL_TEXTURE_2D, 0);
}

// f-ja koja kreira texturu
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
    widthW = width;
	heightW = height;
}

static void on_display(void)
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0 , widthW, 0 , heightW, -1, 1);

    // Iscrtavamo pozadinu
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
    // kamera nam prati nasu letelicu
    gluLookAt(ship_position[0] + 60 , ship_position[1] + 30, 20, 0, camera, 0, 0, 0, 1);
    create_map();

    // letelica (polovimo sferu)
    double clip_plane[] = {0, 0, 1, 0.2};
    glClipPlane(GL_CLIP_PLANE0, clip_plane);
    glEnable(GL_CLIP_PLANE0);

    glPushMatrix();
        glTranslatef(ship_position[0],ship_position[1],ship_position[2]); 
        gluSphere(quad,1.5,50,50);
        ship_position[0] = 940 + goUD;
        ship_position[1] = 450 + goLR; 
    glPopMatrix();

    glDisable(GL_CLIP_PLANE0);

    // ukoliko pronadjemo sve resurse neka nam izbaci poruku u terminalu
    if(foundedItems[0] && foundedItems[1] && foundedItems[2]) {
        printf("Uspesno si pronasao sve resurse\n");
    }

    // ukoliko dodjemo previse blizu Suncu neka nam izbaci poruku i vrati nas na pocetnu poziciju
    if(ship_position[0] < 350) {    
        printf("Bio si previse blizu Sunca\n");
        goUD = 0;
        goLR = 0;
    }
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}

static void create_map() {
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
    draw_planet(mercury_revolution, mercury_rotation, 350, 3.51, 2, 0);
    
    // Uglovi Venerine revolucije i rotacije
    float venus_revolution = 360*hours/(224.7*24);
    float venus_rotation = 360*hours/(243*24); 

    // Venera
    draw_planet(venus_revolution, venus_rotation, 400, 8.7, 3, 80);

    // Uglovi Zemljine revolucije i rotacije
    float earth_revolution = 360 * hours / (365 * 24);
    float earth_rotation = 360*hours/24;

    // Zemlja
    draw_planet(earth_revolution, earth_rotation, 450, 9.17, 4, 200);    

    // Uglovi Marsove revolucije i rotacije
    float mars_revolution = 360 * hours / (686.98 * 24);
    float mars_rotation = 360*hours/24.623;

    // Mars
    draw_planet(mars_revolution, mars_rotation, 485, 4.88, 5, 400);

    // Uglovi Jupiterove revolucije i rotacije
    float jupiter_revolution = 360 * hours / (4332.59 * 24);
    float jupiter_rotation = 360*hours/9.912;

    // Jupiter
    draw_planet(jupiter_revolution, jupiter_rotation, 600, 100, 6,550);
    
    // Uglovi Saturnove revolucije i rotacije
    float saturn_revolution = 360 * hours / (10759 * 24);
    float saturn_rotation = 360*hours/10.656;

    // Saturn
    draw_planet(saturn_revolution, saturn_rotation, 900, 83.3, 7, 200);

    // Uglovi Uranove revolucije i rotacije
    float uranus_revolution = 360 * hours / (30685 * 24);
    float uranus_rotation = 360*hours/17.24;

    // Uran
    draw_planet(uranus_revolution, uranus_rotation, 1050, 37, 8,0);

    // Uglovi Neptunove revolucije i rotacije
    float neptune_revolution = 360 * hours / (60190 * 24);
    float neptune_rotation = 360*hours/19.1;

    // Neptun
    draw_planet(neptune_revolution, neptune_rotation, 1130, 35.7, 9, 600);

    // Iscrtavamo item-e koje treba da sakupimo
    createItem(900,500,0);
    createItem(450,250,1);
    createItem(1150,500,2);
}

// Kreiramo item koji treba da se pokupi i proveravamo da li ga je korisnik pokupio
void createItem(int x, int y, int i) {
    // ukoliko smo sakupili predmet, necemo ga vise prikazivati
    if(((ship_position[0] > x-5) && (ship_position[0] < x+5) && (ship_position[1] > y-5) && (ship_position[1] < y+5)) || (foundedItems[i] == true)) {
        if(foundedItems[i] == false) {
            foundedItems[i] = true;
        }
    }
    else {
        glPushMatrix();
            glColor3f(0,1,1);
            glTranslatef(x,y,0); 
            gluSphere(quad,0.5,50,50);
        glPopMatrix();
    }
}
