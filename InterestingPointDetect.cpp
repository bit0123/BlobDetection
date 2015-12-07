//////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "InterestingPointDetect.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"

BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH: break;
		case DLL_THREAD_ATTACH:  break;
		case DLL_THREAD_DETACH:  break;
		case DLL_PROCESS_DETACH: break;
	}

	return TRUE;
}

//Generating Gaussian mask/kernel
std::vector<std::vector<double>> generateGaussianKernel(int kSize, double sigma)
{
	double r, s = 2.0 * sigma * sigma;
	 // sum is for normalization
	double sum = 0.0;

	std::vector<std::vector<double>> gKernel(kSize, std::vector<double>(kSize, 0.0));
 
    // generate (kSize x kSize) kernel
	int halfofKernelSize = kSize/2;
    for (int x = -1*halfofKernelSize; x <= halfofKernelSize; x++)
    {
        for(int y = -1*halfofKernelSize; y <= halfofKernelSize; y++)
        {
            r = x*x + y*y;
			gKernel[x + halfofKernelSize][y + halfofKernelSize] = (exp(-(r)/s))/(M_PI * s);
            sum += gKernel[x + halfofKernelSize][y + halfofKernelSize];
        }
    }
 
    // normalize the Kernel. Total sum should be exactr 1. but it's not 1 for some precession issues. that's wht need to normalize
   for(int i = 0; i < kSize; ++i)
   {
        for(int j = 0; j < kSize; ++j)
		{
            gKernel[i][j] /= sum;
			std::cout <<  gKernel[i][j] << " ";
		}
		std::cout << std::endl <<sum;
   }

   return gKernel;
}

double getMean(std::vector<double> dogImg)
{
	double sum = 0.0;
	for(int i=0; i<dogImg.size(); i++)
		sum += dogImg[i];

	return sum/dogImg.size();
}

std::vector< std::vector<int> > getMaximaMinima( std::vector< std::vector<double> > dogImg1,  std::vector< std::vector<double> > dogImg2, std::vector< std::vector<double> > dogImg3, int dx, int dy, int index)
{
	bool maxFlag;
	bool minFlag;
	int tx, ty;
	double interstValue; 
	
	std::vector<std::vector<int>> pTmpImg(dy, std::vector<int>(dx, 0));

	for(int i=0; i<dy; i++)
		for(int j=0; j<dx; j++)
		{
			interstValue = dogImg2[i][j];
			maxFlag = true;
			minFlag = true;
			pTmpImg[i][j] = -1;
			for(int x=-1; x<=1; x++)
				for(int y=-1; y<=1; y++)
				{
					tx=i+x;
                    ty=j+y;
					if(tx>=0 && tx<dy && ty>=0 && ty<dx)
					{
						if(minFlag)if(interstValue > dogImg1[tx][ty] || interstValue > dogImg2[tx][ty] || interstValue > dogImg3[tx][ty]){ minimaInterestingPointSet[index].push_back(interstValue); minFlag = false;}
						if(maxFlag)if(interstValue < dogImg1[tx][ty] || interstValue < dogImg2[tx][ty] || interstValue < dogImg3[tx][ty]){ maximaInterestingPointSet[index].push_back(interstValue); maxFlag = false;}
				
					}
				}
			if(minFlag) pTmpImg[i][j] = MINIMA;
			if(maxFlag) pTmpImg[i][j] = MAXIMA;
		
		}

	return pTmpImg;
}

std::vector< std::vector<double> > getDOGImage( std::vector< std::vector<double> > bluredImg1, std::vector< std::vector<double> > bluredImg2, int dx, int dy)
{
	std::vector<std::vector<double>> pTmpImg(dy, std::vector<double>(dx, 0.0));
	double dogValue;
	for(int i=0; i<dy; i++)
		for(int j=0; j<dx; j++)
		{
			dogValue = bluredImg1[i][j] - bluredImg2[i][j]; 
			//pTmpImg[i][j] = (dogValue < 0)? -1*dogValue : dogValue;
			pTmpImg[i][j] = dogValue ;
		}

	return pTmpImg;
}

std::vector< std::vector<double> > smoothImageWithKernel(KSdUint8 ** pSrcImg, int dx, int dy, std::vector<std::vector<double>> kernel, int kSize)
{
	int halfofKernelSize = kSize/2, tx, ty;
	double cSum;
	std::vector<std::vector<double>> pTmpImg(dy, std::vector<double>(dx, 0.0));

	for(int i=0; i<dy; i++)
	{
		for(int j=0; j<dx; j++)
		{
			cSum=0.0;

			for (int x = -halfofKernelSize; x <=halfofKernelSize; x++)
                for (int y = -halfofKernelSize; y <=halfofKernelSize; y++)
				{
					tx=i+x;
                    ty=j+y;
					if(tx>=0 && tx<dy && ty>=0 && ty<dx)
                    {
                        cSum += kernel[x + halfofKernelSize][y + halfofKernelSize]* pSrcImg[tx][ty];
                    } 
				}
			pTmpImg[i][j] = cSum;
		}
	}

	return pTmpImg;

}

/*int CDECL InterestingPointDetect(KScScalarImage2dUint8* srcImg,KScScalarImage2dUint8* dstImg)*/
int CDECL InterestingPointDetect(KScScalarImage2dUint8* srcImg, KScScalarImage2dUint8* dstImg, double sd)
{

	if (srcImg->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Input buffer type not suitable.",
			"iPrewittOutGradientMagPhaseInt16", MB_OK);
        return FALSE;
	}

	if (dstImg->GetId() != KS_SCALAR_IMAGE_2D_UINT8)
	{
		::MessageBox(NULL, "Output buffer type not suitable.",
			"iPrewittOutGradientMagPhaseInt16", MB_OK);
        return FALSE;
	}

	int dx = srcImg->GetMainXSize();
	int dy = srcImg->GetMainYSize();
    if (!dx || !dy)
	{
		::MessageBox(NULL, "Input buffer not allocated.",
			"iPrewittOutGradientMagPhaseInt16", MB_OK);
        return FALSE;
	}

	if (dx != dstImg->GetXSize() || dy != dstImg->GetYSize())
	{
		dstImg->Free();
		if (dstImg->Alloc(dx, dy))
		{
			::MessageBox(NULL, "Fail to allocate output buffer.",
				"iPrewittOutGradientMagPhaseInt16", MB_OK);
			return FALSE;
		}
	}
	else
	{
		dstImg->InitTo(0);
	}

	KSdUint8 **pSrcImg  = srcImg->Get2dArray();
	KSdUint8 **pDstImg  = dstImg->Get2dArray();
	KSdUint8 **pTmpImg  = dstImg->Get2dArray();

	// kernel size.  It should be odd ( ksize% 2 = 1 ) and positive.
	//double sigma[5] = {1, sqrt(2.0), 2, 2*sqrt(2.0), 4};
	int octaveSize = 5;
	std::vector<double> sigma(octaveSize);
	sigma[0] = sd;
	for(int i=1; i<octaveSize; i++) sigma[i] = sigma[i-1] * sqrt(2.0);

	std::vector<int> kSize(octaveSize);

	double kernelSize;

	std::vector< std::vector< std::vector<double> > > bluredImgs(octaveSize);
	std::vector< std::vector< std::vector<double> > > dogImgs(octaveSize-1);
	std::vector< std::vector< std::vector<int> > > interestImgs(octaveSize-3);

	for(int i=0; i<octaveSize; i++)
	{
		kernelSize = 6 * sigma[i];
		if(kernelSize > (int)kernelSize) kernelSize++;

		if((int)kernelSize % 2 == 1 ) kSize[i] = (int)kernelSize;
		else kSize[i] = (int)kernelSize + 1;

		// get the kernel to blur the image
		std::vector<std::vector<double>> kernel = generateGaussianKernel(kSize[i], sigma[i]);
		
		// get the Gausssian Blured Image
		bluredImgs[i] = smoothImageWithKernel(pSrcImg, dx, dy, kernel, kSize[i]);

	}

	// get The Difference of Gaussian Image (DOG) image
	for(int i=1; i<octaveSize; i++)
	{
		dogImgs[i-1] = getDOGImage(bluredImgs[i-1], bluredImgs[i], dx, dy);
	}
	bluredImgs.clear();

	// get Minima Maxima point in DOG image
	int minima_maxima_interestPointSet_index = 0;
	for(int i=2; i<octaveSize-1; i++)
	{
		interestImgs[i-2] = getMaximaMinima(dogImgs[i-2], dogImgs[i-1], dogImgs[i], dx, dy, minima_maxima_interestPointSet_index);
		minima_maxima_interestPointSet_index++;
	}
    
	// Convert ScalarImage to openCv Mat to draw Circle
	cv::Mat srcMat = KScScalarImage2dUint8ToMat(srcImg);

	// Set threshould to discard less important (low contrast) interesting point 
	double meanOfMaxima[2] , meanOfMinima[2];
	for(int i=0; i<2;i++)
		{
			meanOfMaxima[i] = getMean(maximaInterestingPointSet[i]);
			meanOfMinima[i]  = getMean(minimaInterestingPointSet[i]);
		}

	// Draw Circle using openCV library function
	int radious;
	for(int i=0; i<dy; i++)
		for(int j=0; j<dx; j++)
		{
			for(int x=0;x<2;x++)
			{
				//radious = sigma[x+1]/sqrt(2.0);
				radious = sigma[x+1]*sqrt(2.0);
				//if(SHOW_MAXIMA)if(interestImgs[x][i][j] == MAXIMA && dogImgs[x+1][i][j] > meanOfMaxima[x]) 
				if(SHOW_MAXIMA)if(interestImgs[x][i][j] == MAXIMA)
				{ 
					cv::circle(srcMat, cv::Point(j, i), (int)(radious+.5), cv::Scalar( 255, 255, 255 ), 2, 8, 0);
				}
				//if(SHOW_MINIMA)if(interestImgs[x][i][j] == MINIMA && dogImgs[x+1][i][j] < *meanOfMinima[x])  
				if(SHOW_MINIMA)if(interestImgs[x][i][j] == MINIMA)  
				{
					cv::circle(srcMat, cv::Point(j, i), (int)(radious+.5), cv::Scalar( 255, 255, 255 ), 2, 8, 0);
				}
			}
		}

		//  Release Memory of vector after finishing work
		for(int i=0; i<2;i++)
		{
			minimaInterestingPointSet[i].clear();
			maximaInterestingPointSet[i].clear();
		}
		dogImgs.clear();
		interestImgs.clear();

	// Show output image (Blob detected)
	//cv::imshow("Output", srcMat);
	MatToKScScalarImage2dUint8(srcMat, dstImg);

	return TRUE;
}
