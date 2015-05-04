
#include "mathclass.h"

vectorM
interpolate( m_real t, vectorM const& a, vectorM const& b )
{
	return (1.0-t)*a + t*b;
}

m_real len( vectorM const& v )
{
    return sqrt( v.p[0]*v.p[0] + v.p[1]*v.p[1] + v.p[2]*v.p[2] );
}

m_real
vectorM::length() const
{
    return sqrt( p[0]*p[0] + p[1]*p[1] + p[2]*p[2] );
}

matrix
vectorM::cross() const
{
    return matrix(	vectorM(  0 ,-z(), y()),
					vectorM( z(),  0 ,-x()),
					vectorM(-y(), x(),  0 ) );
}

unit_vector
vectorM::normalize() const
{
	m_real d = p[0]*p[0] + p[1]*p[1] + p[2]*p[2];
	return unit_vector( p[0]/d, p[1]/d, p[2]/d );
}

m_real angle( vectorM const& a, vectorM const& b )
{
    return ACOS( (a%b)/(len(a)*len(b)) );
}

position vectorM2position( vectorM const& v )
{
    return position( v.x(), v.y(), v.z() );
}

vectorM position2vectorM( position const& p )
{
    return vectorM( p.x(), p.y(), p.z() );
}

std::ostream& operator<<( std::ostream& os, vectorM const& a )
{
    os << "( " << a.p[0] << " , " << a.p[1] << " , " << a.p[2] << " )";
    return os;
}

std::istream& operator>>( std::istream& is, vectorM& a )
{
	static char buf[256];

    is >> buf >> a.p[0] >> buf >> a.p[1] >> buf >> a.p[2] >> buf;
    return is;
}

