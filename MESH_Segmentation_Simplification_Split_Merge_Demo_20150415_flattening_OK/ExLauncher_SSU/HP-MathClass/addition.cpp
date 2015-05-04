
#include "mathclass.h"

position& operator+=( position& a, vectorM const& b )
{
    a.p[0] += b.p[0];
    a.p[1] += b.p[1];
    a.p[2] += b.p[2];

    return a;
}

vectorM& operator+=( vectorM& a, vectorM const& b )
{
    a.p[0] += b.p[0];
    a.p[1] += b.p[1];
    a.p[2] += b.p[2];

    return a;
}

position operator+( position const& a, vectorM const& b )
{
    position c;

    c.p[0] = a.p[0] + b.p[0];
    c.p[1] = a.p[1] + b.p[1];
    c.p[2] = a.p[2] + b.p[2];

    return c;
}

position operator+( vectorM const& b, position const& a )
{
    position c;

    c.p[0] = a.p[0] + b.p[0];
    c.p[1] = a.p[1] + b.p[1];
    c.p[2] = a.p[2] + b.p[2];

    return c;
}

vectorM operator+( vectorM const& a, vectorM const& b )
{
    vectorM c;

    c.p[0] = a.p[0] + b.p[0];
    c.p[1] = a.p[1] + b.p[1];
    c.p[2] = a.p[2] + b.p[2];

    return c;
}

