#ifndef	_MATHCLASS_H_
#define	_MATHCLASS_H_

#include <iostream>
#include <math.h>
#include <assert.h>

typedef double m_real;

#ifndef	M_PI
#define	M_PI	3.14159265358979323846
#endif

extern m_real EPS;

#include "math_macro.h"

#include "position.h"
#include "vector.h"
#include "unit_vector.h"
#include "matrix.h"
#include "quater.h"

#include "transf.h"
#include "transq.h"

#include "complex.h"

#include "interval.h"
#include "box.h"

#include "vectorN.h"
#include "matrixN.h"
#include "smatrixN.h"

#include "optimize.h"

#endif	// _MATHCLASS_H_
