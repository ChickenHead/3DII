
#include "mathclass.h"

vectorM& operator*=( vectorM& a, m_real b )
{
    a.p[0] = a.p[0] * b;
    a.p[1] = a.p[1] * b;
    a.p[2] = a.p[2] * b;

    return a;
}

vectorM operator*( vectorM const& a, m_real b )
{
    vectorM c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}


vectorM operator*( m_real b, vectorM const& a )
{
    vectorM c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}

position& operator*=( position& a, m_real b )
{
    a.p[0] = a.p[0] * b;
    a.p[1] = a.p[1] * b;
    a.p[2] = a.p[2] * b;

    return a;
}

position operator*( position const& a, m_real b )
{
    position c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}


position operator*( m_real b, position const& a )
{
    position c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}

