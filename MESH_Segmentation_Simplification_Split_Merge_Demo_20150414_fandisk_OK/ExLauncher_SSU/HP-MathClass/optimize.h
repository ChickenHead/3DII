
#ifndef __OPTIMIZE_H
#define __OPTIMIZE_H

void mnbrak(m_real&, m_real&, m_real&,
			m_real&, m_real&, m_real&, m_real (*func)(m_real)) ;

m_real brent(m_real, m_real, m_real,
			m_real (*f)(m_real), m_real, m_real&) ;

void linmin(vectorN&, vectorN&, int, m_real&, m_real (*func)(vectorN const&));

void gradient_descent(	vectorN&, int, m_real, int&, m_real&,
            m_real (*func)(vectorN const&),
            m_real (*dfunc)(vectorN const&, vectorN&));

void frprmn(vectorN&, int, m_real, int&, m_real&,
            m_real (*func)(vectorN const&),
            m_real (*dfunc)(vectorN const&, vectorN&));

void error( char* );

#endif
