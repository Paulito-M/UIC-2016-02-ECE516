#include "math.h"
#include "dfe.h"

/*
 * CONSTANTS
 */
#define PI  (3.1415927)

/*
 * GLOBAL VARIABLES
 */
static float gXprev1 = 0.0;    /* previous two samples */
static float gXprev2 = 0.0;

static float gMu;              /* step size            */
static float gAngle;           /* current estimate     */
static float gAngle_init;      /* initial angle        */

/*
 * DESCRIPTION:
 *   Initialize Direct Frequency Estimation algorithm
 */
void dfe_init( float _mu, float _angle )
{
  gMu = _mu;
  gAngle = _angle;  
  gAngle_init = _angle;
  gXprev1 = 0.0;
  gXprev2 = 0.0;
}

void dfe_reset()
{
  gAngle = gAngle_init;
  gXprev1 = 0.0;
  gXprev2 = 0.0;
}

/*
 * DESCRIPTION:
 *   DFE iteration function
 */
float dfe_step( float _x )
{
  float s,e; 
    
  /* compute the predicted signal from past two samples */
  float cosVal = cosf( gAngle );
  s = 2.0 * cosVal * gXprev1 - gXprev2;

  /* compute the error signal */
  e = _x - s;

  /* compute the new angle */
  /*  
  gAngle = gAngle - ( gMu * e * ( ( ( _x + gXprev2 ) * cosf( gAngle ) ) + gXprev1 ) );  
 */
  float gAngle_delta = ( gMu * e * ( ( ( _x + gXprev2 ) * cosVal ) + gXprev1 ) );
  gAngle = gAngle - gAngle_delta;

  /* update state variables */
  gXprev2 = gXprev1;
  gXprev1 = _x;
  
  return( gAngle );  
}

/*
 * DESCRIPTION:
 *   Update step size 
 */
void dfe_update( float _mu )
{
  gMu = _mu;
}


