
#include <math.h>
#include <stdlib.h>
#include "mtemp.h"

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)	(int)((x) * getSRand())

float voltage = 20.0;
float temperature = 12.0;
float timer = 0.0;

#define MAX_LOADS	5
const float load[MAX_LOADS]={0.02, 0.04, 0.06, 0.08, 0.1};

static int curLoad = 0;


double charge( int t )
{
  double result;

  result = sin( (double)t/100.0 );

  if (result < 0.0) result = 0.0;

  return result;
}


int simulate( void )
{
  extern int chargeMode;
  static int t=0;

  /* First, update the loading if necessary */
  if (getSRand() < 0.02) {
    curLoad = getRand( MAX_LOADS );
  }

  /* Affect the current battery voltage given the load */
  voltage -= load[curLoad];

  /* Next, update the battery voltage given input charge */
  if (chargeMode == FAST_CHARGE) {
    voltage += (charge(t) * sqrt(timer));
  } else {
    voltage += ((charge(t) * sqrt(timer)) / 10.0 );
  }

  if      (voltage < 0.0)  voltage = 0.0;
  else if (voltage > 35.0) voltage = 35.0;

  /* Update the temperature */
  if (chargeMode == FAST_CHARGE) {
    if        (voltage > 25) {
      temperature += ((load[curLoad] * (sqrt(timer)/25.0)) * 10.0);
    } else if (voltage > 15) {
      temperature += ((load[curLoad] * (sqrt(timer)/20.0)) * 10.0);
    } else {
      temperature += ((load[curLoad] * (sqrt(timer)/15.0)) * 10.0);
    }
  } else {
    if        (temperature > 20.0) {
      temperature -= ((load[curLoad] * (sqrt(timer)/20.0)) * 10.0);
    } else {
      temperature -= ((load[curLoad] * (sqrt(timer)/100.0)) * 10.0);
    }
  }

  if      (temperature < 0.0) temperature = 0.0;
  else if (temperature > 40.0) temperature = 40.0;

  t++;

  return 0;
}

