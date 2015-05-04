
#ifndef TRANSQ_H
#define TRANSQ_H

class transq
{
  public:
    quater rotation;
    vectorM translation;

  private:
    // multiplication
    friend transq&      operator*=( transq &, transq const& );
    friend transq       operator* ( transq const&, transq const& );
    friend vectorM&      operator*=( vectorM&, transq const& );
    friend vectorM       operator* ( vectorM const&, transq const& );
    friend position&    operator*=( position&, transq const& );
    friend position     operator* ( position const&, transq const& );
    friend unit_vector& operator*=( unit_vector&, transq const& );
    friend unit_vector  operator* ( unit_vector const&, transq const& );

    // functions
    friend transq       interpolate( m_real, transq const&, transq const& );

    // stream
    friend std::ostream& operator<<( std::ostream&, transq const& );
    friend std::istream& operator>>( std::istream&, transq& );

  public:
    // constructors
    transq() {};
    transq( quater const& a, vectorM const& b ) { rotation=a; translation=b; }

    transq			inverse() const;
};

// identity transq
extern transq identity_transq;

#endif
