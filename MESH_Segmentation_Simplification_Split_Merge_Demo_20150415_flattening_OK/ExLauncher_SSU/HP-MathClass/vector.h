
#ifndef vectorM_H
#define vectorM_H

class unit_vector;
class matrix;
class transf;

class vectorM
{
  private:
    m_real p[3];

    // negation
    friend vectorM      operator-( vectorM const& );
    friend unit_vector operator-( unit_vector const& );

    // addtion
    friend position& operator+=( position&, vectorM const& );
    friend vectorM&   operator+=( vectorM&, vectorM const& );
    friend vectorM    operator+( vectorM const&, vectorM const& );
    friend position  operator+( position const&, vectorM const& );
    friend position  operator+( vectorM const&, position const& );

    // subtraction
    friend vectorM    operator-( vectorM const&, vectorM const& );
    friend position  operator-( position const&, vectorM const& );
    friend position& operator-=( position&, vectorM const& );
    friend vectorM&   operator-=( vectorM&, vectorM const& );

    // dot product
    friend m_real    operator%( vectorM const&, vectorM const& );
    friend m_real    operator%( position const&, vectorM const& );
    friend m_real    operator%( vectorM const&, position const& );

    // cross product
    friend vectorM    operator*( vectorM const&, vectorM const& );
    friend position  operator*( position const&, unit_vector const& );
    friend position  operator*( unit_vector const&, position const& );

    // scalar Multiplication
    friend vectorM    operator*( vectorM const&, m_real );
    friend vectorM    operator*( m_real, vectorM const& );
    friend vectorM&   operator*=( vectorM&, m_real );

    // scalar Division
    friend vectorM    operator/( vectorM const&, m_real );
    friend m_real    operator/( vectorM const&, vectorM const& );
    friend vectorM&   operator/=( vectorM&, m_real );

    // matrix Multiplication
    friend vectorM    operator*( vectorM const&, matrix const& );
    friend vectorM    operator*( matrix const&, vectorM const& );

    friend vectorM&   operator*=( vectorM&, transf const& );
    friend vectorM    operator*( vectorM const&, transf const& );

    // functions
    friend m_real       len( vectorM const& );
    friend unit_vector  normalize( vectorM const& );
	friend vectorM       interpolate( m_real, vectorM const&, vectorM const& );


    friend m_real       angle( vectorM const&, vectorM const& );

    friend position     vectorM2position( vectorM const& );
    friend vectorM       position2vectorM( position const& );

    // stream
    friend std::ostream& operator<<( std::ostream&, vectorM const& );
    friend std::istream& operator>>( std::istream&, vectorM& );
    friend std::ostream& operator<<( std::ostream&, unit_vector const& );
    friend std::istream& operator>>( std::istream&, unit_vector& );

  public:
    // constructors
    vectorM() {}
    vectorM( m_real x, m_real y, m_real z ) { p[0]=x; p[1]=y; p[2]=z; }
    vectorM( m_real a[3] ) { p[0]=a[0]; p[1]=a[1]; p[2]=a[2]; }

    // inquiry functions
    m_real& operator[](int i) { return p[i]; }

    m_real x() const { return p[0]; };
    m_real y() const { return p[1]; };
    m_real z() const { return p[2]; };
    void   getValue( m_real d[3] ) { d[0]=p[0]; d[1]=p[1]; d[2]=p[2]; }
    void   setValue( m_real d[3] ) { p[0]=d[0]; p[1]=d[1]; p[2]=d[2]; }
	m_real getValue( int n ) const { return p[n]; }
	vectorM setValue( m_real x, m_real y, m_real z )
								   { p[0]=x, p[1]=y, p[2]=z; return *this; }
	m_real setValue( int n, m_real x )
								   { return p[n]=x; }

	m_real length() const;
	matrix cross() const;

	unit_vector normalize() const;

    // change functions
    void set_x( m_real x ) { p[0]=x; };
    void set_y( m_real x ) { p[1]=x; };
    void set_z( m_real x ) { p[2]=x; };
};

#endif
