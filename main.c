#include <stdlib.h>
#include <GL/glut.h>

#define TIMER_ID 1
#define TIMER_INTERVAL 20

float hours;
int animation_ongoing;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int id);

/* 
Radius Sunca - 	696,392 km - stavljamo na 100
Radius Zemlje - 6371 km - 109 puta manji od Sunca - stavljamo na 0.917
Udaljenost Zemlje od Sunca - 149.600.000 km - prevelika vrednost
    pa cemo da stavimo kao tri puta radius sunca
Radius Merkura - 2439,7 km - 285 puta manji od sunca - stavljamo na 0.351
Udaljenost Merkura od Sunca - 57.910.000 km - stavljamo na 120
Radius Venere - 6051,8 km - 115 puta manji od Sunca - stavljamo na 0.87
Udaljenost Venere od Sunca - 108.200.000 km - stavljamo na 217.4
Radius Marsa - 3.389,5 km - 205 puta manji od Sunca - stavljamo na 0.488
Udaljenost Marsa od Sunca - 227.900.000 km - stavljamo na 461.5
*/


int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    /* Inicijalizujemo promenljive */
    hours = 0;
    animation_ongoing = 0;

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_timer(int id)
{
    if (TIMER_ID != id)
        return;

    hours += 18;
    
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
        animation_ongoing = 0;
        break;
    }
}

static void on_reshape(int width, int height)
{
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 1500);
}

static void on_display(void)
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se vidna tacka. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(2, 5, 4, 0, 0, 0, 0, 1, 0);
    gluLookAt(0, 0, 700, 0, 0, 0, 0, 1, 0);

    // Ugao rotacije Sunca oko svoje ose 
    float sun_rotation = 360*hours/(15*24);

    // Sunce
    glPushMatrix();
        glRotatef(sun_rotation, 0,0,1);
        glColor3f(1,1,0);
        glutSolidSphere(100,50,50);
    glPopMatrix();

    // Uglovi Merkurove revolucije i rotacije
    float mercury_revolution = 360*hours/(88*24);
    float mercury_rotation = 360*hours/(58.7*24);
    
    // Merkur
    glPushMatrix();
        glRotatef(mercury_revolution, 0,0,1);
        glTranslatef(120,0,0);  
        glRotatef(mercury_rotation, 0,0,1);
        glColor3f(0,0,1);
        glutSolidSphere(0.351,50,50);
    glPopMatrix();


    // Uglovi Venerine revolucije i rotacije
    float venus_revolution = 360*hours/(224.7*24);
    float venus_rotation = 360*hours/(243*24); 

    // Venera
    glPushMatrix();
        glRotatef(venus_revolution, 0,0,1);
        glTranslatef(217.4,0,0);  
        glRotatef(venus_rotation, 0,0,1);
        glColor3f(1,0,0);
        glutSolidSphere(0.87,50,50);
    glPopMatrix();

    // Uglovi Zemljine revolucije i rotacije
    float earth_revolution = 360 * hours / (365 * 24);
    float earth_rotation = 360*hours/24;

    // Zemlja
    
    glPushMatrix();
        glRotatef(earth_revolution, 0,0,1);
        glTranslatef(300,0,0); 
        glRotatef(earth_rotation, 0,0,1);
        glColor3f(0,0,1);
        glutSolidSphere(0.917,50,50);
    glPopMatrix();

    // Uglovi Marsove revolucije i rotacije
    float mars_revolution = 360 * hours / (686.98 * 24);
    float mars_rotation = 360*hours/24.623;

    // Mars
    glPushMatrix();
        glRotatef(mars_revolution, 0,0,1);
        glTranslatef(461.5,0,0); 
        glRotatef(mars_rotation, 0,0,1);
        glColor3f(1,0,0);
        glutSolidSphere(0.488,50,50);
    glPopMatrix();

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}