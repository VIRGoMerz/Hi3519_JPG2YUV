#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#ifdef __cplusplus
}
#endif

#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include "sample_comm_ive.h"

using namespace std;
using namespace cv;

typedef struct hiSAMPLE_IVE_TEST_S
{
    FILE* pFpDst;                       //save file

    IVE_SRC_IMAGE_S stDst_YUV400;       //YUV400
    IVE_SRC_IMAGE_S stDst_YUV420SP;     //YUV420SP
    IVE_SRC_IMAGE_S stDst_BGR;          //U8C3_PACKAGE(BGR)
}SAMPLE_IVE_TEST_S;

static SAMPLE_IVE_TEST_S s_stTest;


static HI_VOID SAMPLE_IVE_Test_Uninit(SAMPLE_IVE_TEST_S *pstTest)
{
    //yuv400
    IVE_MMZ_FREE(pstTest->stDst_YUV400.au64PhyAddr[0], pstTest->stDst_YUV400.au64VirAddr[0]);
    //yuv420
    IVE_MMZ_FREE(pstTest->stDst_YUV420SP.au64PhyAddr[0], pstTest->stDst_YUV420SP.au64VirAddr[0]);
    //u8c3
    IVE_MMZ_FREE(pstTest->stDst_BGR.au64PhyAddr[0], pstTest->stDst_BGR.au64VirAddr[0]);

    IVE_CLOSE_FILE(pstTest->pFpDst);
}

static HI_S32 SAMPLE_IVE_Test_Init(SAMPLE_IVE_TEST_S *pstTest, HI_U32 u32Width, HI_U32 u32Height, HI_CHAR *pchDstFileName)
{
    HI_S32 s32Ret;

    memset(pstTest, 0, sizeof(SAMPLE_IVE_TEST_S));
 
    s32Ret = HI_FAILURE;

    pstTest->pFpDst = fopen(pchDstFileName, "wb");
    SAMPLE_CHECK_EXPR_GOTO(NULL == pstTest->pFpDst, TEST_INIT_FAIL, "Error,Open file %s failed!\n", pchDstFileName);

    s32Ret = HI_SUCCESS;    

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTest->stDst_YUV400), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, TEST_INIT_FAIL, "Error(%#x),Create YUV400 image failed!\n", s32Ret);
     
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTest->stDst_YUV420SP), IVE_IMAGE_TYPE_YUV420SP, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, TEST_INIT_FAIL, "Error(%#x),Create YUV420SP image failed!\n", s32Ret);
     
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTest->stDst_BGR), IVE_IMAGE_TYPE_U8C3_PACKAGE, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, TEST_INIT_FAIL, "Error(%#x),Create BGR image failed!\n", s32Ret);

TEST_INIT_FAIL:
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_IVE_Test_Uninit(pstTest);
    }

    return s32Ret;    
}


static HI_S32 SAMPLE_IVE_ConvProc(SAMPLE_IVE_TEST_S* pstTest, Mat CV_Img)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(CV_Img.channels() == 1)
    {
        Mat2IveImage(&CV_Img, &pstTest->stDst_YUV400);

        s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstTest->stDst_YUV400, pstTest->pFpDst);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),write file failed!\n",s32Ret);
    }
    else if (CV_Img.channels() == 3)
    {
        Mat2IveImage(&CV_Img, &pstTest->stDst_BGR);
        s32Ret = SAMPLE_IVE_BGR2YUV(&pstTest->stDst_BGR, &pstTest->stDst_YUV420SP);

        s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstTest->stDst_YUV420SP, pstTest->pFpDst);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),write file failed!\n",s32Ret);
    }

    return s32Ret;
}


HI_VOID JPG2YUV(Mat CV_Img, int ImageNum)
{
    HI_S32 s32Ret = HI_SUCCESS;

    /*
    //通道数
    cout << "channels:" << CV_Img.channels() << endl;
    //列宽
    cout << "clos:" << CV_Img.cols << endl;
    //行高
    cout << "rows:" << CV_Img.rows << endl;
    */
   
    HI_U32 u32Width  = CV_Img.cols;
    HI_U32 u32Height = CV_Img.rows;

    HI_CHAR pchDstFileName[500];
    //TODO:这边的命名还有点问题
    snprintf(pchDstFileName, sizeof(pchDstFileName), "../data/images/YUV/%d.yuv", ImageNum);

    memset(&s_stTest,0,sizeof(s_stTest));
    SAMPLE_COMM_IVE_CheckIveMpiInit();

    s32Ret = SAMPLE_IVE_Test_Init(&s_stTest, u32Width, u32Height, pchDstFileName);
    SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, TEST_FAIL, "Error(%#x),SAMPLE_IVE_Test_Init failed!\n", s32Ret);

    if(CV_Img.channels() == 3 || CV_Img.channels() == 1)
    {
        //3通道的转换成YUV420SP,1通道的转换成YUV400
        s32Ret = SAMPLE_IVE_ConvProc(&s_stTest, CV_Img);
        if (HI_SUCCESS == s32Ret)
        {
            SAMPLE_PRT("Convert success!\n");
        }
    }
    else
    {
        cout << "error : image type not support !!!" << endl;
    }

    SAMPLE_IVE_Test_Uninit(&s_stTest);
    memset(&s_stTest,0,sizeof(s_stTest));    

TEST_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();

}

int main()
{
    //抓取JPG文件
    string pattern_jpg;
    vector<String> image_files;
    pattern_jpg = "../data/images/JPG/*.jpg";
    glob(pattern_jpg, image_files);
    cout << "Pic number is :" << image_files.size() << endl;

    //将JPG图像加载到Mat里,转成NV21格式保存
    Mat CV_Img;
    for(int ImageNum = 0; ImageNum < (int)image_files.size(); ImageNum++)
    {
        cout << "convert " << image_files[ImageNum] << endl;
           CV_Img = imread(image_files[ImageNum]);
        JPG2YUV(CV_Img, ImageNum);
    }
    return 0;
}
