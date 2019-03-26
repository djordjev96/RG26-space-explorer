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
Radius Sunca - 	696.392 km - stavljamo na 100
Radius Zemlje - 6371 km - 109 puta manji od Sunca - stavljamo na 0.917
Udaljenost Zemlje od Sunca - 149.600.000 km - prevelika vrednost
    pa cemo da stavimo kao tri puta radius sunca
Radius Merkura - 2439,7 km - 285 puta manji od sunca - stavljamo na 0.351
Udaljenost Merkura od Sunca - 57.910.000 km - stavljamo na 120
Radius Venere - 6051,8 km - 115 puta manji od Sunca - stavljamo na 0.87
Udaljenost Venere od Sunca - 108.200.000 km - stavljamo na 217.4
Radius Marsa - 3.389,5 km - 205 puta manji od Sunca - stavljamo na 0.488
Udaljenost Marsa od Sunca - 227.900.000 km - stavljamo na 461.5
Radius Jupitera - 69.911 km - 10 puta manji od sunca - stavljamo na 10
Udaljenost Jupitera od Sunca - 778.500.000 km - (1562.5) ali zbog prevelike vrednosti cemo staviti na 600
Radius Saturna - 58.232 km  - 12 puta manji od Sunca - stavljamo na 8.33
Udaljenost Saturna od Sunca - 1.434.000.000 km - (prevelika vrednost) stavicemo na 750
Radius Urana - 25.362 km - 27 puta manji od Sunca - stavljamo na 3.7
Udaljenost Urana od Sunca - 2.871.000.000 km - stavljamo na 850
Radius Neptuna - 24.622 km - 28 puta manji od Sunca - stavljamo na 3.57
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

    hours += 100;
    
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
    gluLookAt(0, 0, 800, 300, 0, 0, 0, 1, 0);

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
        glTranslatef(200,0,0);  
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
        glTranslatef(400,0,0); 
        glRotatef(mars_rotation, 0,0,1);
        glColor3f(1,0,0);
        glutSolidSphere(0.488,50,50);
    glPopMatrix();

    // Uglovi Jupiterove revolucije i rotacije
    float jupiter_revolution = 360 * hours / (4332.59 * 24);
    float jupiter_rotation = 360*hours/9.912;

    // Jupiter
    glPushMatrix();
        glRotatef(jupiter_revolution, 0,0,1);
        glTranslatef(500,0,0); 
        glRotatef(jupiter_rotation, 0,0,1);
        glColor3f(0,1,0);
        glutSolidSphere(10,50,50);
    glPopMatrix();

    // Uglovi Saturnove revolucije i rotacije
    float saturn_revolution = 360 * hours / (10759 * 24);
    float saturn_rotation = 360*hours/10.656;

    // Saturn
    glPushMatrix();
        glRotatef(saturn_revolution, 0,0,1);
        glTranslatef(600,0,0); 
        glRotatef(saturn_rotation, 0,0,1);
        glColor3f(0,0,1);
        glutSolidSphere(8.33,50,50);
    glPopMatrix();

    // Uglovi Uranove revolucije i rotacije
    float uranus_revolution = 360 * hours / (30685 * 24);
    float uranus_rotation = 360*hours/17.24;

    // Uran
    glPushMatrix();
        glRotatef(uranus_revolution, 0,0,1);
        glTranslatef(700,0,0); 
        glRotatef(uranus_rotation, 0,0,1);
        glColor3f(1,0,0);
        glutSolidSphere(3.7,50,50);
    glPopMatrix();

    // Uglovi Neptunove revolucije i rotacije
    float neptune_revolution = 360 * hours / (60190 * 24);
    float neptune_rotation = 360*hours/19.1;

    // Uran
    glPushMatrix();
        glRotatef(neptune_revolution, 0,0,1);
        glTranslatef(750,0,0); 
        glRotatef(neptune_rotation, 0,0,1);
        glColor3f(0,0,1);
        glutSolidSphere(3.57,50,50);
    glPopMatrix();



    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}