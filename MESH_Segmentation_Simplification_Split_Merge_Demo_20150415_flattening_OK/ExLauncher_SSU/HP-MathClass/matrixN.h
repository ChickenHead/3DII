
#ifndef MATRIX_N_H
#define MATRIX_N_H

class vectorN;

class matrixN
{
  private:
    int      n,
             m,
			on;
    vectorN *v;

  public:
    // constructors
     matrixN();
     matrixN( int, int );
     matrixN( int, int, vectorN* );
    ~matrixN();

    // inquiry functions
    void      getValue( m_real** ) const;
    m_real    getValue( int, int ) const;
    void      setValue( m_real** );
    void      setValue( int, int, m_real );

    vectorN&  operator[](int i) const { assert(i>=0 && n>i); return v[i]; };
    int       row()    const { return n; }
    int       column() const { return m; }
    void      setSize( int, int );
	void      setRow( int, const vectorN& );
	void	  setColumn( int, const vectorN& );

    matrixN&  transpose( matrixN const& );
    m_real    det() const;

    matrixN&  assign( matrixN const& );
    matrixN&  mult( matrixN const&, matrixN const& );
    matrixN&  operator*=( m_real );
    matrixN&  operator/=( m_real );
    matrixN&  operator+=( matrixN const& );
    matrixN&  operator-=( matrixN const& );

	matrixN&  mergeUpDown( matrixN const&, matrixN const& );
	matrixN&  mergeLeftRight( matrixN const&, matrixN const& );
	void      splitUpDown( matrixN&, matrixN& );
	void      splitLeftRight( matrixN&, matrixN& );
	void      splitUpDown( matrixN&, matrixN&, int );
	void      splitLeftRight( matrixN&, matrixN&, int );

	void      LUdecompose( int* );
	void	  LUsubstitute( int*, vectorN& );
	m_real    LUinverse( matrixN& );

	void	  SVdecompose( vectorN&, matrixN& );
	void	  SVsubstitute( vectorN const&, matrixN const&,
							vectorN const&, vectorN& );
	void      SVinverse( matrixN& );

    // stream
    friend std::ostream& operator<<( std::ostream&, matrixN const& );
    friend std::istream& operator>>( std::istream&, matrixN& );
};

#endif
