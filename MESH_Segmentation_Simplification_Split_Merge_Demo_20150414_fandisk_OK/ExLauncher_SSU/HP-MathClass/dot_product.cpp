
#include "mathclass.h"

m_real operator%( position const& a, vectorM const& b )
{
    return ( a.p[0]*b.p[0] + a.p[1]*b.p[1] + a.p[2]*b.p[2] );
}

m_real operator%( vectorM const& b, position const& a )
{
    return ( a.p[0]*b.p[0] + a.p[1]*b.p[1] + a.p[2]*b.p[2] );
}

m_real operator%( vectorM const& a, vectorM const& b )
{
    return ( a.p[0]*b.p[0] + a.p[1]*b.p[1] + a.p[2]*b.p[2] );
}

