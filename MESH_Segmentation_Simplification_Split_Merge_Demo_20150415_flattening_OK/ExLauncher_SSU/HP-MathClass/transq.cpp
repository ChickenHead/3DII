
#include "mathclass.h"

transq identity_transq( quater(1,0,0,0),vectorM(0,0,0) );


transq
transq::inverse() const
{
    quater a = rotation.inverse();
    return transq( a, -rotate(a,translation) );
}

transq
interpolate( m_real t, transq const& a, transq const& b )
{
	quater q = ::interpolate( t, a.rotation, b.rotation );
	vectorM v = ::interpolate( t, a.translation, b.translation );

    return transq( q, v );
}


transq rotate_transq( m_real angle, vectorM const& axis )
{
    return transq( exp( angle * axis / 2.0 ),vectorM(0,0,0) );
}

transq translate_transq( vectorM const& axis )
{
    return transq( quater(1,0,0,0), axis );
}

transq translate_transq( m_real x, m_real y, m_real z )
{
    return transq( quater(1,0,0,0),vectorM(x,y,z) );
}


std::ostream& operator<<( std::ostream& os, transq const& a )
{
    os << a.rotation << " + " << a.translation;
    return os;
}

std::istream& operator>>( std::istream& is, transq& a )
{
	static char	buf[256];
    is >> a.rotation >> buf >> a.translation;
    return is;
}

//--------------------------------------------------------------------------//

position& operator*=( position& a, transq const& b )
{
    a = rotate(b.rotation,a) + b.translation;
    return a;
}

position operator*( position const& a, transq const& b )
{
    return ( rotate(b.rotation,a) + b.translation );
}

unit_vector& operator*=( unit_vector& a, transq const& b )
{
    a = rotate(b.rotation,a);
    return a;
}

unit_vector operator*( unit_vector const& a, transq const& b )
{
    return ( rotate(b.rotation,a) );
}

vectorM& operator*=( vectorM& a, transq const& b )
{
    a = rotate(b.rotation,a);
    return a;
}

vectorM operator*( vectorM const& a, transq const& b )
{
    return ( rotate(b.rotation,a) );
}

transq operator*( transq const& a, transq const& b )
{
    return transq( a.rotation * b.rotation,
				   rotate(a.rotation, b.translation) + a.translation );
}

transq& operator*=( transq& a, transq const& b )
{
	a.rotation = a.rotation * b.rotation;
	a.translation = rotate(b.rotation, a.translation) + b.translation;

    return a;
}

