
#ifndef POSITION_H
#define POSITION_H

class vectorM;
class unit_vector;
class matrix;
class transf;

class position
{
  private:
    m_real p[3];
    
    // addtion
    friend position& operator+=( position&, vectorM const& );
    friend position  operator+( position const&, vectorM const& );
    friend position  operator+( vectorM const&, position const& );

    // subtraction
    friend position& operator-=( position&, vectorM const& );
    friend vectorM    operator-( position const&, position const& );
    friend position  operator-( position const&, vectorM const& );

    // multiplication
    friend position  operator*( matrix const&, position const& );
    friend position  operator*( position const&, matrix const& );

    friend position& operator*=( position&, transf const& );
    friend position  operator*( position const&, transf const& );

    // multiplication by scalar
    friend position& operator*=( position&, m_real );
    friend position  operator*( position const&, m_real );
    friend position  operator*( m_real, position const& );

    // dot product
    friend m_real    operator%( position const&, vectorM const& );
    friend m_real    operator%( vectorM const&, position const& );

    // cross product
    friend position  operator*( position const&, unit_vector const& );
    friend position  operator*( unit_vector const&, position const& );

    // functions

    friend position  interpolate( m_real, position const&, position const& );
    friend m_real    distance( position const&, position const& );
    friend m_real    distance( position const&, position const&, position const& );

	friend position	affineCombination( position const&, position const&, m_real );
	friend position	affineCombination( position const&, position const&, position const&,
									   m_real, m_real );
	friend position	affineCombination( position const&, position const&, position const&, position const&,
									   m_real, m_real, m_real );
	friend position	affineCombination( int, position[], m_real[] );

    friend position     vectorM2position( vectorM const& );
    friend vectorM       position2vectorM( position const& );

    // stream
    friend std::ostream& operator<<( std::ostream&, position const& );
    friend std::istream& operator>>( std::istream&, position& );

  public:
    // constructors
    position() {};
    position( m_real x, m_real y, m_real z ) { p[0]=x; p[1]=y; p[2]=z; };
    position( m_real a[3] ) { p[0]=a[0]; p[1]=a[1]; p[2]=a[2]; };

    // inquiry functions
    m_real& operator[] (int i) { return p[i]; }

    m_real x() const { return p[0]; }
    m_real y() const { return p[1]; }
    m_real z() const { return p[2]; }
	void   getValue( m_real d[3] ) { d[0]=p[0]; d[1]=p[1]; d[2]=p[2]; }
	void   setValue( m_real d[3] ) { p[0]=d[0]; p[1]=d[1]; p[2]=d[2]; }
	m_real getValue( int n ) const { return p[n]; }

    m_real coordinate( int i ) const { return p[i]; };
    m_real norm() const { return (m_real)sqrt( p[0]*p[0] + p[1]*p[1] + p[2]*p[2] ); };

    // chang functions
    void set_x( m_real x ) { p[0]=x; };
    void set_y( m_real x ) { p[1]=x; };
    void set_z( m_real x ) { p[2]=x; };
};

#endif
