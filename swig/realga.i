%module(directors="1") realga

%{
/* These are compiled into the wrapper (.cxx) */
#include "realga.h"
#include "options.h"
#include "chromosome.h"
#include "fitnessfunction.h"
%}

/* --- Common C++ std typemaps --- */
%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>
%include <stdint.i>

/* If you pass arrays as pointers sometimes */
%include <typemaps.i>

/* --- Let Python subclass FitnessFunction (virtual method) --- */
%feature("director") FitnessFunction;

/* --- Explicit STL instantiations used in your headers --- */
%template(FloatVector)  std::vector<float>;
%template(DoubleVector) std::vector<double>;
%template(IntVector)    std::vector<int>;
%template(StringVector) std::vector<std::string>;

/* ✅ This is the missing one */
%template(ChromosomeVector) std::vector<RealChromosome>;

/* --- Parse dependent public headers first --- */
%include "chromosome.h"
%include "fitnessfunction.h"
%include "options.h"

/* --- Finally, wrap the main API --- */
%include "realga.h"
