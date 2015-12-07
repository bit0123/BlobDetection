//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "KSClass.h"
#include <math.h>
#include <vector>
#include "KScOpenCvUtils.h"

extern "C" __declspec( dllexport )
/*int CDECL InterestingPointDetect(KScScalarImage2dUint8* srcImg,KScScalarImage2dUint8* dstImg)*/
int CDECL InterestingPointDetect(KScScalarImage2dUint8* par0,KScScalarImage2dUint8* par1,double par2);

# define M_PI           3.14159265358979323846  /* pi */
#define MAXIMA 1
#define MINIMA 0

#define SHOW_MAXIMA 0
#define SHOW_MINIMA 1

std::vector< std::vector<double> > minimaInterestingPointSet(2);
std::vector< std::vector<double> > maximaInterestingPointSet(2);


