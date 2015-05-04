
#ifndef TRANSF_H
#define TRANSF_H

class quater;

class transf
{
  private:
    matrix m;
    vectorM v;

    // multiplication
    friend transf&      operator*=( transf &, transf const& );
    friend transf       operator*( transf const&, transf const& );
    friend vectorM&      operator*=( vectorM&, transf const& );
    friend vectorM       operator*( vectorM const&, transf const& );
    friend position&    operator*=( position&, transf const& );
    friend position     operator*( position const&, transf const& );
    friend unit_vector& operator*=( unit_vector&, transf const& );
    friend unit_vector  operator*( unit_vector const&, transf const& );

    // functions
    friend transf       inverse( transf const& );
    friend transf       interpolate( m_real, transf const&, transf const& );

    // stream
    friend std::ostream& operator<<( std::ostream&, transf const& );
    friend std::istream& operator>>( std::istream&, transf& );

  public:
    // constructors
    transf() {}
    transf( matrix const& a, vectorM const& b ) { m = a; v = b; }
    transf( quater const& a, vectorM const& b ) { m = Quater2Matrix(a); v = b; }

    // inquiry functions
    const matrix&	getAffine() const { return m; };
    const vectorM&	getTranslation() const { return v; };
	const quater	getRotation() const { return Matrix2Quater(m); }
	const matrix&	affine() const { return m; };
	const vectorM&	translation() const { return v; }
	const position  getPosition() const { return position(v.x(), v.y(), v.z()); }

	void			setAffine( matrix const& a ) { m = a; }
	void			setTranslation( vectorM const& a ) { v = a; }
	void			setRotation( quater const& q ) { m = Quater2Matrix(q); }

    transf			inverse() const;
};

// identity transform
extern transf identity_transf;

// generator
extern transf scale_transf( m_real );
extern transf scale_transf( m_real, m_real, m_real );
extern transf rotate_transf( m_real, vectorM const& );
extern transf reflect_transf( vectorM const& );
extern transf translate_transf( vectorM const& );
extern transf translate_transf( m_real, m_real, m_real );
extern transf coordinate_transf( position const&,
                  unit_vector const&, unit_vector const& );

#endif
