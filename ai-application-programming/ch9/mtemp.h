#include "fuzzy.h"

#define TRICKLE_CHARGE	0
#define FAST_CHARGE	1

int normalize( fuzzyType );

fuzzyType m_temp_hot( float temp );

fuzzyType m_temp_warm( float temp );

fuzzyType m_temp_cold( float temp );

fuzzyType m_voltage_low( float temp );

fuzzyType m_voltage_medium( float temp );

fuzzyType m_voltage_high( float temp );
