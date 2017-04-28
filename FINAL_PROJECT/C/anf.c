#include "math.h"
#include "anf.h"

/*
 * CONSTANTS
 */
#define PI  (3.1415927)

/*
 * GLOBAL VARIABLES
 */
static float gBetaPrev1 = 0.0;   /* previous two gradients */
static float gBetaPrev2 = 0.0;

static float gXprev1 = 0.0;
static float gXprev2 = 0.0;

static float gYprev1 = 0.0;
static float gYprev2 = 0.0;

static float gMu;              /* step size            */
static float gAngle;           /* current estimate     */
static float gAngle_init;      /* initial angle        */

static float gR = 0.85;

/*
 * DESCRIPTION:
 *   Initialize Direct Frequency Estimation algorithm
 */
void anf_init( float _mu, float _angle )
{
  gMu = _mu;
  gAngle = _angle;  
  gAngle_init = _angle;
  dfe_reset();
}

void anf_reset()
{
  gAngle = gAngle_init;
  gXprev1 = 0.0;
  gXprev2 = 0.0;
  gYprev1 = 0.0;
  gYprev2 = 0.0;
  gBetaPrev1 = 0.0;
  gBetaPrev2 = 0.0;  
}

/*
 * DESCRIPTION:
 *   DFE iteration function
 */
float anf_step( float _x )
{
  float beta, y; 
    
  /* compute the new filter output */
  float cosVal = cosf( gAngle );
  float sinVal = sinf( gAngle );
  
  y = _x - 2 * cosVal * gXprev1 + gXprev2 + 2 * gR * cosVal * gYprev1 - ( gR * gR ) * gYprev2;
  
  /* compute the gradient function */
  beta = 2 * sinVal * gXprev1 - 2 * gR * sinVal * gYprev1 + 2 * gR * cosVal * gBetaPrev1 - gR * gR * gBetaPrev2;
  
  /* compute the new angle */
  gAngle = gAngle - ( 2 * gMu * y * beta );
  
  /* update the previous values */
  gXprev2 = gXprev1;
  gXprev1 = _x;
  
  gYprev2 = gYprev1;
  gYprev1 = y;
  
  gBetaPrev2 = gBetaPrev1;
  gBetaPrev1 = beta;
  gXprev1 = _x;
  
  return( gAngle );
}

/*
 * DESCRIPTION:
 *   Update step size 
 */
void anf_update( float _mu )
{
  gMu = _mu;
}


