#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#define PRINTF_IMPLEMENTATION
#include "printf.h"

#include <math.h>
#include <unistd.h>

#define GLIPH_SPACEPADDED // Needed to avoid write() that does not existe with avr-gcc
#define GLIPH_IMPLEMENTATION
#include "gliph.h"

#define HEIGHT (3*9)
#define WIDTH (3*16)

#define BUFFER_LENGTH_TRAINE 10
#define carre(x) ((x)*(x))
#define UNUSED(x) (void)(x)
#ifndef ABS
# define ABS(x) ((x < 0) ? -(x) : (x))
#endif


double equ_psi_1(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return ((-g*(2.f*m1 + m2)*sin(th1)) - m2*g*sin(th1 - 2.f*th2) - 2.f*sin(th1 - th2)*m2*(ph2*ph2*l2 + ph1*ph1*l1*cos(th1 - th2)))/(l1*(2*m1 + m2 - m2*cos(2.f*th1 - 2.f*th2)));
}

double equ_psi_2(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return (2.f*sin(th1 - th2)*(ph1*ph1*l1*(m1 + m2) + g*(m1 + m2)*cos(th1) + ph2*ph2*l2*m2*cos(th1 - th2)))/(l2*(2.f*m1 + m2 - m2*cos(2.f*th1 - 2.f*th2)));
}


typedef struct
{
    const float  l1;
    const float  l2;
    const float  m1;
    const float  m2;

    const float  g;

    double t;
    double e;
    double theta_1;
    double phi_1;
    double psi_1;

    double theta_2;
    double phi_2;
    double psi_2;
} Var_Dp;

Var_Dp Var_Dp1 = {.l1       = 1.0f,
.l2       = 1.0f,
.m1       = 1.f,
.m2       = 1.f,
.g        = 9.8f,
.t        = 0,
.theta_1  = M_PI-0.1f,
.phi_1    = 0,
.psi_1    = 0,
.theta_2  = M_PI,
.phi_2    = 0,
.psi_2    = 0,
};


double equ_var1_psi_1(double x, double th1, double y)
{
    UNUSED(x);
    UNUSED(y);
    return equ_psi_1(th1, Var_Dp1.theta_2, Var_Dp1.phi_1, Var_Dp1.phi_2, Var_Dp1.g, Var_Dp1.l1, Var_Dp1.l2, Var_Dp1.m1, Var_Dp1.m2);
}
double equ_var1_psi_2(double x, double th2, double y)
{
    UNUSED(x);
    UNUSED(y);
    return equ_psi_2(Var_Dp1.theta_1, th2, Var_Dp1.phi_1, Var_Dp1.phi_2, Var_Dp1.g, Var_Dp1.l1, Var_Dp1.l2, Var_Dp1.m1, Var_Dp1.m2);
}
    
typedef struct
{
    int x;
    int y;
} Vector2;
	

typedef struct
{
    Vector2 boule_1;
    Vector2 boule_2;
    Vector2 buf_traine[BUFFER_LENGTH_TRAINE];
} Double_pendule;


void save_state(Var_Dp Dp, Var_Dp *buf)
{    
    buf->e = Dp.e;
    buf->t = Dp.t;
    buf->theta_1 = Dp.theta_1;
    buf->theta_2 = Dp.theta_2;
    buf->phi_1 = Dp.phi_1;
    buf->phi_2 = Dp.phi_2;
    buf->psi_1 = Dp.psi_1;
    buf->psi_2 = Dp.psi_2;
}

void restaure_state(Var_Dp *Dp, Var_Dp buf)
{    
    Dp->e = buf.e;
    Dp->t = buf.t;
    Dp->theta_1 = buf.theta_1;
    Dp->theta_2 = buf.theta_2;
    Dp->phi_1 = buf.phi_1;
    Dp->phi_2 = buf.phi_2;
    Dp->psi_1 = buf.psi_1;
    Dp->psi_2 = buf.psi_2;
}

double get_energie_pendule(Var_Dp Dp)
{
    float T_v2 = (1.f/2.f)*(Dp.m1 + Dp.m2)*carre(Dp.l1)*carre(Dp.phi_1) + (1.f/2.f)*Dp.m2*carre(Dp.l2)*carre(Dp.phi_2) + Dp.m2*Dp.l1*Dp.l2*Dp.phi_1*Dp.phi_2*cos(Dp.theta_1 - Dp.theta_2);
    float V_v2 = -(Dp.m1 + Dp.m2)*Dp.g*Dp.l1*cos(Dp.theta_1) - Dp.m2*Dp.g*Dp.l2*cos(Dp.theta_2);
    return ABS(T_v2 + V_v2);
}

int methode_RK4_f(const double h, double t, double *x, double *v)
{
    // avec equ_var1_psi_1
    struct
    {
    	double tn;
    	double xn;
    	double vn;
    	double an;
    } P[4] = {0};

    P[0].tn = t;
    P[0].xn = (*x);
    P[0].vn = (*v);
    P[0].an = equ_var1_psi_1(P[0].tn, P[0].xn, P[0].vn);

    P[1].tn = t + h*0.5f;
    P[1].xn = (*x) + h*0.5f*P[0].vn;
    P[1].vn = (*v) + h*0.5f*P[0].an;
    P[1].an = equ_var1_psi_1(P[1].tn, P[1].xn, P[1].vn);

    P[2].tn = t + h*0.5f;
    P[2].xn = (*x) + h*0.5f*P[1].vn;
    P[2].vn = (*v) + h*0.5f*P[1].an;
    P[2].an = equ_var1_psi_1(P[2].tn, P[2].xn, P[2].vn);

    P[3].tn = t + h;
    P[3].xn = (*x) + h*P[2].vn;
    P[3].vn = (*v) + h*P[2].an;
    // P[3].xn = (*x) + 0.5f*h*P[2].vn;                // Meilleur coeficient que RK4 classique
    // P[3].vn = (*v) + 0.5f*h*P[2].an;                // Meilleur coeficient que RK4 classique
    P[3].an = equ_var1_psi_1(P[3].tn, P[3].xn, P[3].vn);

    (*x) = (*x) + h*(1.f/6.f)*(P[0].vn + 2.f*P[1].vn + 2.f*P[2].vn + P[3].vn);
    if (isnan(*x)) return -1;
    (*v) = (*v) + h*(1.f/6.f)*(P[0].an + 2.f*P[1].an + 2.f*P[2].an + P[3].an);
    if (isnan(*v)) return -1;

    return 0;
}

int methode_RK4_g(const double h, double t, double *x, double *v)
{
    // avec  equ_var1_psi_2
    struct
    {
    	double tn;
    	double xn;
    	double vn;
    	double an;
    } P[4] = {0};
    P[0].tn = t;
    P[0].xn = (*x);
    P[0].vn = (*v);
    P[0].an = equ_var1_psi_2(P[0].tn, P[0].xn, P[0].vn);

    P[1].tn = t + h*0.5f;
    P[1].xn = (*x) + h*0.5f*P[0].vn;
    P[1].vn = (*v) + h*0.5f*P[0].an;
    P[1].an = equ_var1_psi_2(P[1].tn, P[1].xn, P[1].vn);

    P[2].tn = t + h*0.5f;
    P[2].xn = (*x) + h*0.5f*P[1].vn;
    P[2].vn = (*v) + h*0.5f*P[1].an;
    P[2].an = equ_var1_psi_2(P[2].tn, P[2].xn, P[2].vn);

    P[3].tn = t + h;
    P[3].xn = (*x) + h*P[2].vn;
    P[3].vn = (*v) + h*P[2].an;
    // P[3].xn = (*x) + 0.5f*h*P[2].vn;                // Meilleur coeficient que RK4 classique
    // P[3].vn = (*v) + 0.5f*h*P[2].an;                // Meilleur coeficient que RK4 classique
    P[3].an = equ_var1_psi_2(P[3].tn, P[3].xn, P[3].vn);

    (*x) = (*x) + h*(1.f/6.f)*(P[0].vn + 2.f*P[1].vn + 2.f*P[2].vn + P[3].vn);
    if (isnan(*x)) return -1;
    (*v) = (*v) + h*(1.f/6.f)*(P[0].an + 2.f*P[1].an + 2.f*P[2].an + P[3].an);
    if (isnan(*v)) return -1;

    return 0;
}


int methode_RK_adaptative_pendule(double stepSize, double err, Var_Dp *Dp)
{
    // sauvegarde_etat_equadiff;
    double dt = stepSize;
    long pas = 0;
    Var_Dp DpB = {0};
    save_state(*Dp, &DpB); // Sauvegarder *Dp dans le buffer pour modifier Dp comme on le souhaite

    double startTime = DpB.t;
    const double EnergieDebut = DpB.e;
    
    double DernierDifEnergie = 10e10;
    double valeur = 10e10;
    int firstTime = 1;

    if (stepSize < 1e-15)
	return -1;
    do
    {
	double t = startTime;
	if (!firstTime) {
	    restaure_state(Dp, DpB); // restaurer le Dp comme avant
	    // sauvegarde de toute les donner du Dp
	    dt /= 5;
	    if (dt < 1e-10) {
		fprintf(stderr, "pas de temps trop petit\n");
		return -1;
	    }
	}
	pas = 0;

	while (t < (startTime + stepSize)) {
	    double h = dt;

	    if ((t + h) > (startTime + stepSize - 1e-10))
		h = startTime + stepSize - t;
	    pas++;
	    if (methode_RK4_f(h, t, &Dp->theta_1, &Dp->phi_1) < 0) return -1; // New step
	    if (methode_RK4_g(h, t, &Dp->theta_2, &Dp->phi_2) < 0) return -1; // New step

	    Dp->t = t; // sauvegarde de toute les donner du Dp
	    t = t + h;
	    if (pas > (stepSize/dt + 2)){ 
	    	fprintf(stderr, "Problème nombre de pas trop grand: %ld, %f\n", pas, stepSize/dt);
		return -1;
	    }
	}

	const float EnergieFinale = get_energie_pendule(*Dp);
	if (isnan(EnergieFinale)) return -1;
	const float difEnergie = ABS(EnergieDebut - EnergieFinale);
	if (!firstTime)
	    valeur = ABS(difEnergie - DernierDifEnergie);
	// test code
	DernierDifEnergie = difEnergie;
	firstTime = 0;
    } while (valeur > err);
    return 0;
}




Vector2 adaptation_coord_fenetre(Vector2 a)
{
    if ((a.x + WIDTH/2) < 1)
	a.x = -WIDTH/2 + 1;
    if ((a.x + WIDTH/2) > WIDTH-1)
	a.x = WIDTH/2 - 1;
    if ((a.y + HEIGHT/2) < 1)
	a.y = -HEIGHT/2 + 1;
    if ((a.y + HEIGHT/2) > HEIGHT-1)
	a.y = HEIGHT/2 - 1;
    return a;
}

// void tracage_double_pendule(int i, Double_pendule *Dp, Color cl, Var_Dp VDp)
// void tracage_double_pendule(int i, Double_pendule *Dp, Var_Dp VDp, char **cl)
void tracage_double_pendule(int i, Double_pendule *Dp, Var_Dp VDp, Screen cl)
{
    static const short midH = HEIGHT/2;
    static const short midW = WIDTH/2;
    ConsoleClear(cl, '#');
    PrintRectangle(cl, 1, 1, WIDTH-2, HEIGHT-2, ' ');

    // Calcul des nouvelles coordonnées du pendule
    Dp->boule_1.x = 7.f*VDp.l1*sinf(VDp.theta_1);
    Dp->boule_1.y = -7.f*VDp.l1*cosf(VDp.theta_1);
    Dp->boule_2.x = 7.f*VDp.l2*sinf(VDp.theta_2) + Dp->boule_1.x;
    Dp->boule_2.y = -7.f*VDp.l2*cosf(VDp.theta_2) + Dp->boule_1.y;

    Dp->boule_1 = adaptation_coord_fenetre(Dp->boule_1);
    Dp->boule_2 = adaptation_coord_fenetre(Dp->boule_2);

    // Calcul de la trajectoir
    for (short j = i-1; j > 0; j--) {
	Dp->buf_traine[j].x = Dp->buf_traine[j-1].x;
	Dp->buf_traine[j].y = Dp->buf_traine[j-1].y;
    }
	
    // trainé de la trajectoire
    for (short j = 0; j < i-1; j++){
	PrintLine(cl, Dp->buf_traine[j].x, -Dp->buf_traine[j].y, Dp->buf_traine[j+1].x, -Dp->buf_traine[j+1].y, '`');
    }

    // Tige du pendule
    PrintLine(cl, 0, 0, Dp->boule_1.x, -Dp->boule_1.y, '/');
    PrintLine(cl, Dp->boule_1.x, -Dp->boule_1.y, Dp->boule_2.x, -Dp->boule_2.y, '/');

    // Masse du pedule
    PrintCircle(cl, Dp->boule_1.x+midW, -Dp->boule_1.y+midH, VDp.m1, '*');
    PrintCircle(cl, Dp->boule_2.x+midW, -Dp->boule_2.y+midH, VDp.m2, '*');
    
    // Ligne entre le centre et le bout du pendule
    Dp->buf_traine[0].x = Dp->boule_2.x;
    Dp->buf_traine[0].y = Dp->boule_2.y;
    
    // Base du pendule
    PrintCircle(cl, midW, midH, 1, '#');
}


int main(void)
{
    USART_Init(3);

    Var_Dp Var_Dp1 = {.l1       = 1.0f,
    		      .l2       = 1.0f,
    		      .m1       = 1.f,
    		      .m2       = 1.f,
    		      .g        = 9.8f,
    		      .t        = 0,
    		      // .theta_1  = M_PI - 0.1f,
    		      .theta_1  = M_PI/2.f,
    		      .phi_1    = 0,
    		      .psi_1    = 0,
    		      .theta_2  = M_PI,
    		      // .theta_2  = 0,
    		      .phi_2    = 0,
    		      .psi_2    = 0,
    };
    Var_Dp1.e = get_energie_pendule(Var_Dp1);

    
    const float dt = 0.1f;
    // float epsilon = 0.01f;
    Double_pendule Dp1;
    
    GLIPH_INIT(console, WIDTH, HEIGHT);

    for (int i = 0;;) {

	if (methode_RK4_f(dt, 0, &Var_Dp1.theta_1, &Var_Dp1.phi_1))
	    printf("ERREUR: calcul %s\n", "methode_RK_adaptative_pendule");
	if (methode_RK4_g(dt, 0, &Var_Dp1.theta_2, &Var_Dp1.phi_2))
	    printf("ERREUR: calcul %s\n", "methode_RK_adaptative_pendule");
	
    	// if (methode_RK_adaptative_pendule(dt, epsilon, &Var_Dp1) < 0)
    	//     printf("ERREUR: calcul %s\n", "methode_RK_adaptative_pendule");

    	if (i < BUFFER_LENGTH_TRAINE) i++;
    	tracage_double_pendule(i, &Dp1, Var_Dp1, console);
	
    	PrintConsole(console);
    }
    // mem_free(console, HEIGHT);
    while(1);
}
