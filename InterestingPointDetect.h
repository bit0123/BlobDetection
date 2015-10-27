//////////////////////////////////////////////////////////////////////////
//
// �� ȭ���� �Լ� ��Ͻ� �ڵ����� ������ ������Ʈ�Դϴ�.
//
// ����ڰ� ������ ���� ���, Project�� Setting...�� ������ �ʿ��ϹǷ�,
//
// �̴� ������ '�Լ� �ۼ���'�� ���� �ٶ��ϴ�.
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// KSClass ���̺귯���� ��ũ
//
//////////////////////////////////////////////////////////////////////////
//
// ��� ���ϰ� ���̺귯�� ���� �˻��� ���Ͽ�
// Visual Studio���� �޴� Tools - Options - Directories �ǿ���
// Show Directories for: �κ���
//     Include files���� KSClass�� Include�� ���丮��
//     Library files���� KSClass�� Lib       ���丮�� �����Ͽ��� �Ѵ�.
//
//////////////////////////////////////////////////////////////////////////
//
// * ������
//
// Show Directories for: �κ���
//     Include files�� KSClass�� Include�� Lib ���丮�� ��� ������ ���,
//     ��ũ ���� �߻��Ѵ�.
//
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "KSClass.h" // KSClass�鿡 ���� ���
#include <math.h>
#include <vector>
#include "KScOpenCvUtils.h"

// DLL �ܺο��� ȣ���ϱ� ���ؼ� �ʿ��� �κ� ...
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


