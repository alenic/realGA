import re

text = """
#include "ok_ks33_io.h"
#include "ok_ks33_io.hpp"
#include "ok_ks33_io.hsfe.hh"
#include "ok_ks33_io.hsfe.cpp"
 #include "ok_ks33_io.hsfe.h"
    #include "ok_ks33_io.hsfe.h"

#include <ok_ks33_io.h>
#include <ok_ks33_io.hpp>
#include <ok_ks33_io.hsfe.hh>
#include <ok_ks33_io.hsfe.cpp>
 #include <ok_ks33_io.hsfe.h>
    #include <ok_ks33_io.hsfe.h>
#define include 234

class C {
    public:
        C() {int include=23;};
    private:
    int a;
};
"""

def remove_includes(code: str):
    pattern = re.compile(
        r'#include.*["|>]'
    )

    return re.sub(pattern, "", code)

print(remove_includes(text))

