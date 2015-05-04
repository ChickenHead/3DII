
#ifndef MATRIX_H
#define MATRIX_H

class transf;
class quater;

class matrix
{
  private:
    vectorM p[3];

    // addition & subtraction
    friend matrix    operator+( matrix const&, matrix const& );
    friend matrix&   operator+=( matrix&, matrix const& );
    friend matrix    operator-( matrix const&, matrix const& );
    friend matrix&   operator-=( matrix&, matrix const& );

    // multiplication
    friend matrix    operator*( m_real, matrix const& );
    friend matrix    operator*( matrix const&, m_real );
    friend matrix    operator*( matrix const&, matrix const& );
    friend vectorM    operator*( matrix const&, vectorM const& );
    friend vectorM    operator*( vectorM const&, matrix const& );
    friend position  operator*( matrix const&, position const& );
    friend position  operator*( position const&, matrix const& );
    friend unit_vector& operator*=( unit_vector&, matrix const& );
    friend unit_vector  operator*( unit_vector const&, matrix const& );

    // transform with quaternions
    friend matrix    Quater2Matrix( quater const& );
    friend quater    Matrix2Quater( matrix const& );

    // functions
    friend matrix    inverse( matrix const& );
    friend matrix    interpolate( m_real, matrix const&, matrix const& );
	friend matrix    mexp( vectorM const& );
	friend vectorM    mlog( matrix const& );

    // stream
    friend std::ostream& operator<<( std::ostream&, matrix const& );
    friend std::istream& operator>>( std::istream&, matrix& );

  public:
    // constructors
    matrix() {};
    matrix( vectorM const& a, vectorM const& b, vectorM const& c )
    {
        p[0][0]=a.x(); p[0][1]=a.y(); p[0][2]=a.z();
        p[1][0]=b.x(); p[1][1]=b.y(); p[1][2]=b.z();
        p[2][0]=c.x(); p[2][1]=c.y(); p[2][2]=c.z();
    };
    matrix( m_real a00, m_real a01, m_real a02,
            m_real a10, m_real a11, m_real a12,
            m_real a20, m_real a21, m_real a22 )
    {
        p[0][0]=a00; p[0][1]=a01; p[0][2]=a02;
        p[1][0]=a10; p[1][1]=a11; p[1][2]=a12;
        p[2][0]=a20; p[2][1]=a21; p[2][2]=a22;
    };

    // inquiry functions
    void   getValue( m_real d[3][3] )
			{ d[0][0]=p[0][0]; d[0][1]=p[0][1]; d[0][2]=p[0][2];
			  d[1][0]=p[1][0]; d[1][1]=p[1][1]; d[1][2]=p[1][2];
			  d[2][0]=p[2][0]; d[2][1]=p[2][1]; d[2][2]=p[2][2]; }
    void   setValue( m_real d[3][3] )
			{ p[0][0]=d[0][0]; p[0][1]=d[0][1]; p[0][2]=d[0][2];
			  p[1][0]=d[1][0]; p[1][1]=d[1][1]; p[1][2]=d[1][2];
			  p[2][0]=d[2][0]; p[2][1]=d[2][1]; p[2][2]=d[2][2]; }
	m_real getValue( int row, int col ) const { return p[row].getValue( col );}
	vectorM getValue( int row ) const { return p[row];}
	matrix setValue( m_real x00, m_real x01, m_real x02,
					 m_real x10, m_real x11, m_real x12,
					 m_real x20, m_real x21, m_real x22 )
			{ p[0][0]=x00, p[0][1]=x01, p[0][2]=x02,
			  p[1][0]=x10, p[1][1]=x11, p[1][2]=x12,
			  p[2][0]=x20, p[2][1]=x21, p[2][2]=x22;
			  return *this; }

	vectorM setValue( int row, vectorM const& v )
			{ return p[row]=v; }
	m_real setValue( int row, int col, m_real x )
			{ return p[row][col]=x; }

    vectorM& operator[](int i) { return p[i]; };
    matrix transpose() const;
    matrix inverse() const;
    m_real det() const;
};

extern matrix scaling( m_real );
extern matrix scaling( m_real, m_real, m_real );
extern matrix rotation( m_real, vectorM const& );
extern matrix reflection( vectorM const& );
extern vectorM Matrix2EulerAngle( const matrix& m );
extern matrix EulerAngle2Matrix( const vectorM& m );

#endif
