//////////////////////////////////////////////////////////////////////////
//
// 이 화일은 함수 등록시 자동으로 생성된 프로젝트입니다.
//
// 사용자가 임으로 만든 경우, Project의 Setting...의 설정이 필요하므로,
//
// 이는 도움말의 '함수 작성법'을 참조 바랍니다.
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// KSClass 라이브러리의 링크
//
//////////////////////////////////////////////////////////////////////////
//
// 헤더 파일과 라이브러리 파일 검색을 위하여
// Visual Studio에서 메뉴 Tools - Options - Directories 탭에서
// Show Directories for: 부분의
//     Include files에는 KSClass의 Include의 디렉토리를
//     Library files에는 KSClass의 Lib       디렉토리를 설정하여야 한다.
//
//////////////////////////////////////////////////////////////////////////
//
// * 주의점
//
// Show Directories for: 부분의
//     Include files에 KSClass의 Include와 Lib 디렉토리를 모두 설정할 경우,
//     링크 에러 발생한다.
//
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "KSClass.h" // KSClass들에 대한 헤더
#include <math.h>
#include <vector>
#include "KScOpenCvUtils.h"

// DLL 외부에서 호출하기 위해서 필요한 부분 ...
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


