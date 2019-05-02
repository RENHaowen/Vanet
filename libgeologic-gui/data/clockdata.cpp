#include "clockdata.h"

ClockData::ClockData(): Data("ClockData") {

}

ClockData::~ClockData(){

}

void ClockData::draw() {

    this->drawBackGround();

    unsigned long long currentTime = this->s_clock->getCurrentTime();

    int hours = (currentTime / 3600) % 24; //à chercher dans SimulationClock

    int minutes = (currentTime % 3600) / 60;//à chercher dans SimulationClock

    int seconds =  (currentTime % 3600) % 60;

    char string_time[9];

    sprintf(string_time, "%02d:%02d:%02d", hours, minutes, seconds); // Clock dans un buffer text

    // dessin police / valeur compteur
    glColor4f(0.0, 0.0, 0.0, 1.0); // Couleur caractères

    glLineWidth(3);// Épaisseur des caractères

    glPointSize(3);

    char *p;

    glPushMatrix();

    glTranslatef(-125, -20.0,0.0);

    glScaled(0.6, 1.4, 1.0);

    for (p = &string_time[0]; *p; p++){

        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p); // Vectoriser et afficher chaque caractère.

    }

    glPopMatrix();
}

void ClockData::setCurrentSimuClock(SimulationClock* s_clock) {

    this->s_clock = s_clock;
}

void ClockData::draw(unsigned long long t) {

    this->drawBackGround();

    unsigned long long currentTime = this->s_clock->getCurrentTime();

    int hours = (currentTime / 3600) % 24; //à chercher dans SimulationClock

    int minutes = (currentTime % 3600) / 60;//à chercher dans SimulationClock

    int seconds =  (currentTime % 3600) % 60;

    char string_time[9];

    sprintf(string_time, "%02d:%02d:%02d", hours, minutes, seconds); // Clock dans un buffer text

    // dessin police / valeur compteur
    glColor4f(0.0, 0.0, 0.0, 1.0); // Couleur caractères

    char *p;

    glLineWidth(3);// Épaisseur des caractères

    glPointSize(3);

    glPushMatrix();

    glTranslatef(-125, -20.0,0.0);

    glScaled(0.6, 1.4, 1.0);

    for (p = &string_time[0]; *p; p++){

        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p); // Vectoriser et afficher chaque caractère.

    }
    glPopMatrix();
}

void ClockData::drawBackGround() {

    glColor4d(1.0, 1.0, 1.0, 1.0);
/*/
    glPointSize(5);
    glColor3b(1.0,0.0,0.0);
    glBegin(GL_POINTS);
    glVertex2d(-150.0, -150.0);

    glVertex2d(200.0, -150.0);

    glVertex2d(200.0, 200.0);

    glVertex2d(-150.0, 200.0);
    glEnd();
/*/
    glBegin(GL_QUADS);

    glVertex2d(-150.0, -150.0);

    glVertex2d(200.0, -150.0);

    glVertex2d(200.0, 200.0);

    glVertex2d(-150.0, 200.0);

    glEnd();
}
