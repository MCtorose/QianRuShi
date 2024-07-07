#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<windef.h>
#include<math.h>
#include<string.h>

#include <graphics.h>
#include "Global.h"

int FaceCount = 0;
int angle = 0;
#define PI 3.1415926;
typedef enum 
{
    FACE_RECOGNITION_STEP_1 = 0,
    FACE_RECOGNITION_STEP_2_1,
    FACE_RECOGNITION_STEP_2_2,
    FACE_RECOGNITION_STEP_3_1,
    FACE_RECOGNITION_STEP_3_2,
    FACE_RECOGNITION_STEP_4_1,
    FACE_RECOGNITION_STEP_4_2,
    FACE_RECOGNITION_STEP_5,
    FACE_RECOGNITION_STEP_6,
    FACE_RECOGNITION_STEP_7,
    FACE_RECOGNITION_STEP_8,
    FACE_RECOGNITION_STEP_9,
    FACE_RECOGNITION_STEP_MAX,
}FACE_RECOGNITION_STEP;

// 获取文件的后缀名
char* GetFlieExta(char* filename)
{
    int fileLen = strlen(filename);
    int exLen = 0;
    char *fileExta = (char *)malloc(255);
    memset(fileExta, 0, 255);

    for (int i = fileLen-1; i > 0; i--)
        if (filename[i] == '.')
		{
            exLen = fileLen - i;
            break;
        }
    strncpy(fileExta, filename + fileLen - exLen, exLen);
    return fileExta;
}


// BGRA颜色结构体
typedef struct tagBGRA
{
	unsigned char blue;          // 该颜色的蓝色分量  (值范围为0-255)
	unsigned char green;         // 该颜色的绿色分量  (值范围为0-255)
	unsigned char red;           // 该颜色的红色分量  (值范围为0-255)
	unsigned char transparency;  // 透明度，在bmp中是保留值，无实际效果
}BGRA, * PBGRA;

// 图像结构体
typedef struct tagIMAGE_SELF
{
	unsigned int width;
	unsigned int height;
    BGRA* data;
}IMAGE_SELF, * PIMAGE_SELF;


// BMP文件的处理

// BMP文件头结构体
typedef struct tagBITMAP_HEAD_INFO
{
    /* bmp文件头的信息，有#的是重点!!*/
    // bmp文件头
    unsigned short  bfType;             // 0x424D，即BM字符串，表明是bmp格式文件
    unsigned int    bfSize;             // ###总的bmp文件大小 以字节为单位     
    unsigned short  bfReserved1;        // 保留，必须设置为0                     
    unsigned short  bfReserved2;        // 保留，必须设置为0 
    unsigned int    bfOffBits;          // ###总的bmp头部的大小（包括位图信息头），即到像素数据的偏移  
    // 位图信息头
    unsigned int    biSize;             // 位图信息头的大小
    unsigned int    biWidth;            // ###图像的宽  
    int    biHeight;           // ###图像的高  
    unsigned short  biPlanes;           // 颜色平面数，即调色盘数，恒等于1 
    unsigned short  biBitCount;         // ###图片颜色的位数,一般为32
    unsigned int    biCompression;      // 说明图象数据压缩的类型，0为不压缩
    unsigned int    biSizeImage;        // 像素数据所占大小,因为使用BI_RGB，所以设置为0
    unsigned int    biXPelsPerMeter;    // 说明水平分辨率，缺省为0
    unsigned int    biYPelsPerMeter;    // 说明垂直分辨率，缺省为0
    unsigned int    biClrUsed;          // 说明本位图实际使用调色盘的颜色索引数，0表示全部
    unsigned int    biClrImportant;     // 说明本位图重要调色盘的颜色索引数，0表示全都重要
}BITMAP_HEAD_INFO,*PBITMAP_HEAD_INFO;

// 加载BMP图片
IMAGE_SELF *Image_bmp_load(char* filename)
{
    IMAGE_SELF *pimageOld;
    BITMAP_HEAD_INFO bmpHeadInfo;
    FILE* fp;

    pimageOld = (IMAGE_SELF *)malloc(sizeof(IMAGE_SELF));
    
	DEBUG_PRINT_WITH_TIME("filename: %s", filename);
    if ((fp = fopen(filename, "rb")) == NULL)
    {
		printf("打开%s文件失败！\n", filename);
		exit(0);
	}    
    
    // 读取bmp头部
    // bmp文件头
    fread(& bmpHeadInfo.bfType, 1, sizeof(bmpHeadInfo.bfType), fp);
    fread(& bmpHeadInfo.bfSize, 1, sizeof(bmpHeadInfo.bfSize), fp);
    fread(& bmpHeadInfo.bfReserved1, 1, sizeof(bmpHeadInfo.bfReserved1), fp);
    fread(& bmpHeadInfo.bfReserved2, 1, sizeof(bmpHeadInfo.bfReserved2), fp);
    fread(& bmpHeadInfo.bfOffBits, 1, sizeof(bmpHeadInfo.bfOffBits), fp);
    // 位图信息头
    fread(& bmpHeadInfo.biSize, 1, sizeof(bmpHeadInfo.biSize), fp);
    fread(& bmpHeadInfo.biWidth, 1, sizeof(bmpHeadInfo.biWidth), fp);
    fread(& bmpHeadInfo.biHeight, 1, sizeof(bmpHeadInfo.biHeight), fp);
    fread(& bmpHeadInfo.biPlanes, 1, sizeof(bmpHeadInfo.biPlanes), fp);
    fread(& bmpHeadInfo.biBitCount, 1, sizeof(bmpHeadInfo.biBitCount), fp);
    fread(& bmpHeadInfo.biCompression, 1, sizeof(bmpHeadInfo.biCompression), fp);
    fread(& bmpHeadInfo.biSizeImage, 1, sizeof(bmpHeadInfo.biSizeImage), fp);
    fread(& bmpHeadInfo.biXPelsPerMeter, 1, sizeof(bmpHeadInfo.biXPelsPerMeter), fp);
    fread(& bmpHeadInfo.biYPelsPerMeter, 1, sizeof(bmpHeadInfo.biYPelsPerMeter), fp);
    fread(& bmpHeadInfo.biClrUsed, 1, sizeof(bmpHeadInfo.biClrUsed), fp);
    fread(& bmpHeadInfo.biClrImportant, 1, sizeof(bmpHeadInfo.biClrImportant), fp);

	//DEBUG_PRINT_WITH_TIME("bfType: %d, bfSize: %d, bfOffBits: %d", bmpHeadInfo.bfType, bmpHeadInfo.bfSize, bmpHeadInfo.bfOffBits)
	//DEBUG_PRINT_WITH_TIME("bfReserved1: %d, bfReserved2: %d", bmpHeadInfo.bfReserved1, bmpHeadInfo.bfReserved2)

	DEBUG_PRINT_WITH_TIME("biWidth: %d, Height: %d", bmpHeadInfo.biWidth, bmpHeadInfo.biHeight);

    if (bmpHeadInfo.biHeight < 0)
    {
        DEBUG_PRINT_WITH_TIME("BMP Height is negative...");
        exit(0);
    }

    // 读取bmp位图数据
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * (bmpHeadInfo.biWidth * bmpHeadInfo.biHeight));
    fseek(fp, bmpHeadInfo.bfOffBits, SEEK_SET);

    if (bmpHeadInfo.biBitCount == 32)
    {
       for (unsigned int i = 0; i < bmpHeadInfo.biWidth * bmpHeadInfo.biHeight; i++)
            fread(&bgra[i], 1, sizeof(BGRA), fp);
    }
    else if (bmpHeadInfo.biBitCount == 24)
    {
		// 计算每行补几个字节零
        int k = 4 * (3 * bmpHeadInfo.biWidth / 4 + 1) - 3 * bmpHeadInfo.biWidth;
        for (unsigned int i = 0; i < bmpHeadInfo.biWidth * bmpHeadInfo.biHeight; i++)
        {
            if (k != 4 && (ftell(fp)- 54 + k ) % (3 * bmpHeadInfo.biWidth + k)==0)
                fseek(fp, ftell(fp) + k, SEEK_SET);

            fread(& bgra[i].blue, 1, sizeof(unsigned char), fp);
            fread(& bgra[i].green, 1, sizeof(unsigned char), fp);
            fread(&bgra[i].red, 1, sizeof(unsigned char), fp);
            bgra[i].transparency = (unsigned char)0xFF;
        }
    }



    pimageOld->data = bgra;
    pimageOld->width = bmpHeadInfo.biWidth;
    pimageOld->height = bmpHeadInfo.biHeight;

    fclose(fp);
    return pimageOld;
}

// 保存BMP图片
void Image_bmp_save(char* filename, IMAGE_SELF *imageOld)
{
    FILE* fp = fopen(filename, "wb");

    unsigned short  bfType = 0x4D42;                // 0x424D，即BM字符串，表明是bmp格式文件
    unsigned int    bfSize = imageOld->width * imageOld->height * 4 + 54;  // ###总的bmp文件大小 以字节为单位     
    unsigned short  bfReserved1 = 0;                // 保留，必须设置为0                     
    unsigned short  bfReserved2 = 0;                // 保留，必须设置为0 
    unsigned int    bfOffBits = 54;                 // ###总的bmp头部的大小（包括位图信息头），即到像素数据的偏移  
    unsigned int    biSize = 40;                    // 位图信息头的大小
    unsigned int    biWidth = imageOld->width;                 // ###图像的宽  
    unsigned int    biHeight = imageOld->height;                // ###图像的高  
    unsigned short  biPlanes = 1;                   // 颜色平面数，即调色盘数，恒等于1 
    unsigned short  biBitCount = 32;                // ###图片颜色的位数,一般为32
    unsigned int    biCompression = 0;              // 说明图象数据压缩的类型，0为不压缩
    unsigned int    biSizeImage = 0;                // 像素数据所占大小,因为使用BI_RGB，所以设置为0
    unsigned int    biXPelsPerMeter = 0;            // 说明水平分辨率，缺省为0
    unsigned int    biYPelsPerMeter = 0;            // 说明垂直分辨率，缺省为0
    unsigned int    biClrUsed = 0;                  // 说明本位图实际使用调色盘的颜色索引数，0表示全部
    unsigned int    biClrImportant = 0;             // 说明本位图重要调色盘的颜色索引数，0表示全都重要

    fwrite(& bfType, 2, 1, fp);
    fwrite(&bfSize, 4, 1, fp);
    fwrite(& bfReserved1, 2, 1, fp);
    fwrite(& bfReserved2, 2, 1, fp);
    fwrite(& bfOffBits, 4, 1, fp);
    fwrite(& biSize, 4, 1, fp);
    fwrite(& biWidth, 4, 1, fp);
    fwrite(& biHeight, 4, 1, fp);
    fwrite(& biPlanes, 2, 1, fp);
    fwrite(& biBitCount, 2, 1, fp);
    fwrite(& biCompression, 4, 1, fp);
    fwrite(& biSizeImage, 4, 1, fp);
    fwrite(& biXPelsPerMeter, 4, 1, fp);
    fwrite(& biYPelsPerMeter, 4, 1, fp);
    fwrite(& biClrUsed, 4, 1, fp);
    fwrite(& biClrImportant, 4, 1, fp);

    fwrite(imageOld->data, sizeof(BGRA) * imageOld->width * imageOld->height, 1, fp);

    fclose(fp);
}


// 加载图片
IMAGE_SELF *Image_load(char* filename)
{
    IMAGE_SELF *im;
    //char* fileEx= GetFlieExta(filename);
	//DEBUG_PRINT_WITH_TIME("fileEx: %s", fileEx);
    //if (strcmp(fileEx, ".bmp") == 0)
        im = Image_bmp_load(filename);

    return im;
}

char OutputFileCount = '1';

// 保存图片
void Image_save(char* filename, IMAGE_SELF *imageOld)
{
    char* fileEx = GetFlieExta(filename);

    filename[strlen(filename) - 5] = OutputFileCount++;

    if (strcmp(fileEx, ".bmp") == 0)
        Image_bmp_save(filename, imageOld);
}

// 释放图像结构体
void Image_free(IMAGE_SELF *imageOld)
{
    free(imageOld->data);
    free(imageOld);
}

#define GRAY_MODE_WEIGHT 1           // 加权法(推荐使用)

// 彩色图转灰度图
IMAGE_SELF *Transform_color_grayscale(IMAGE_SELF *imageOld, int grayscale_mode)
{
    int color = 0;
    
    IMAGE_SELF *imageNew;
	imageNew = (IMAGE_SELF *)malloc(sizeof(IMAGE_SELF));
	
    imageNew->width = imageOld->width;
    imageNew->height = imageOld->height;
	imageNew->data = (BGRA*)malloc(sizeof(BGRA) * imageOld->width * imageOld->height);
	
    switch (grayscale_mode)
    {
	    case GRAY_MODE_WEIGHT:
	    {
	        for (unsigned int i = 0; i < imageOld->width * imageOld->height; i++)
	        {
	            color = (imageOld->data[i].blue * 114 + imageOld->data[i].green * 587 + imageOld->data[i].red * 299) / 1000;
	            imageNew->data[i].blue = color;
	            imageNew->data[i].green = color;
	            imageNew->data[i].red = color;
	        }
	        break;
	    }

	    default: DEBUG_PRINT_WITH_TIME("error:  switch default branch....");
    }
	return imageNew;
}

// 二值图(自适应阈值法，areaSize=25较合适，当图片线条多且密时，不推荐使用)
IMAGE_SELF *Transform_color_BW_Adaptive(IMAGE_SELF *imageOld, int areaSize)
{
	IMAGE_SELF *imageNew;
	imageNew = (IMAGE_SELF *)malloc(sizeof(IMAGE_SELF) );	
	
    imageNew->width = imageOld->width;
    imageNew->height = imageOld->height;
	
    // areaSize为区域的大小，区域越大，效果图的细节越好，areaSize=25较合适
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * imageOld->width * imageOld->height);
    int* p = (int*)malloc(sizeof(int) * areaSize); // p->position 位置坐标
    int k = (int)(sqrt((double)areaSize)) / 2;  // 重合区域边长的一半

    for (unsigned int i = 0; i < imageOld->width * imageOld->height; i++)
    {
        // 计算与卷积和对应重合区域的坐标
        int t = 0; // 记录p的下标
        for (int n = k; n >= -k; n--)
            for (int m = -k; m <= k; m++)
            {
                p[t] = ((i % imageOld->width) + m) + (i / imageOld->width + n) * imageOld->width;
                t++;
            }

        // 判断是否越界
        for (int j = 0; j < areaSize; j++)
            if (p[j] < 0 || p[j] >= imageOld->width * imageOld->height)
                p[j] = i;

        unsigned int color = 0;
        for (int j = 0; j < areaSize; j++)
            color += imageOld->data[p[j]].blue;
        color /= areaSize;

        if (imageOld->data[i].blue >= color)
            bgra[i].blue = 255;
        else
            bgra[i].blue = 0;

        bgra[i].green = bgra[i].blue;
        bgra[i].red = bgra[i].blue;
    }

    free(p);
    imageNew->data = bgra;
    return imageNew;
}

// 判断像素值的范围
unsigned char Tool_RBG(int BRRA)
{
    if (BRRA > 255)
        return (unsigned char)255;
    else if (BRRA < 0)
        return (unsigned char)0;
    else
        return (unsigned char)BRRA;
}

// 卷积操作（自定义）
IMAGE_SELF *Kernels_use_DIY(IMAGE_SELF *imageOld, double* kernels, int areaSize, double modulus)
{
	IMAGE_SELF *imageNew;
	imageNew = (IMAGE_SELF *)malloc(sizeof(IMAGE_SELF) );	
    imageNew->width = imageOld->width;
    imageNew->height = imageOld->height;
	imageNew->data = (BGRA*)malloc(sizeof(BGRA) * imageOld->width * imageOld->height);	
	memcpy(imageNew->data, imageOld->data, sizeof(BGRA) * imageOld->width * imageOld->height);
	
	// kernels卷积核
	// areaSize区域的大小
	// modulus最后乘的系数
	
    BGRA* bgra = (BGRA*)malloc(sizeof(BGRA) * imageOld->width * imageOld->height);
    int* p = (int*)malloc(sizeof(int) * areaSize); // p->position 位置坐标
    int k = (int)(sqrt((double)areaSize)) / 2;  // 重合区域边长的一半

    for (unsigned int i = 0; i < imageOld->width * imageOld->height; i++)
    {
        // 计算与卷积和对应重合区域的坐标
        int t = 0; // 记录p的下标
        for(int n = k; n >= -k; n--)
            for (int m = -k; m <= k; m++)
                p[t] = ((i % imageOld->width) + m) + (i / imageOld->width + n) * imageOld->width, t++;
                
        // 判断是否越界
        for (int j = 0; j < areaSize; j++) 
            if (p[j] < 0 || p[j] >= imageOld->width * imageOld->height)
                p[j] = i;

		// 相乘相加
        int blue = 0, green = 0, red = 0;
        for (int j = 0; j < areaSize; j++)
        {
            blue += imageOld->data[p[j]].blue * kernels[j];
            green += imageOld->data[p[j]].green * kernels[j];
            red += imageOld->data[p[j]].red * kernels[j];
        }
        
        bgra[i].blue = Tool_RBG(blue * modulus);
        bgra[i].green = Tool_RBG(green * modulus);
        bgra[i].red = Tool_RBG(red * modulus);
    }

    free(p);
    imageNew->data = bgra;
    return imageNew;
}

// 均值滤波卷积核
double KERNELS_Wave_Average[25] =
{
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1
};

// 均值滤波
IMAGE_SELF *Wavefiltering_Average(IMAGE_SELF *imageOld)
{
    return Kernels_use_DIY(imageOld, KERNELS_Wave_Average, 25, 1.0 / 25);
}


// 积分图结构体
typedef struct tagIGIMAGE_SELF
{
    unsigned int width;
    unsigned int height;
    unsigned int *data;
}IGIMAGE_SELF, *PIGIMAGE_SELF;


typedef struct tagEXPECTANT_ITEM
{
    float ExpectantValue; //期望值
    int Type; //1: 大于，2：小于
    char Note[100]; //注释
}EXPECTANT_ITEM;

EXPECTANT_ITEM gExpectants[FACE_RECOGNITION_STEP_MAX];


// 图像的任意角度的旋转
IMAGE_SELF *Transform_shape_whirl(IMAGE_SELF *imageOld, float angle)
{
    // 转角度换成弧度
    angle = PI * angle / 180;
    
    IMAGE_SELF *imageNew;
	imageNew = (IMAGE_SELF *)malloc(sizeof(IMAGE_SELF) );	
    //imageNew->width = imageOld->width;
    //imageNew->height = imageOld->height;
	//imageNew->data = (BGRA*)malloc(sizeof(BGRA) * imageOld->width * imageOld->height);	

    float cosnum = (float)cos(angle);   
    float sinnum = (float)sin(angle);

    // 计算原图的四个角的坐标
    int fx2 = imageOld->width - 1;
    int fy2 = 0;
    int fx3 = 0;
    int fy3 = imageOld->height - 1;
    int fx4 = imageOld->width - 1;
    int fy4 = imageOld->height - 1;

    // 计算旋转后的图像四个角的坐标
    int nx1 = 0;
    int ny1 = 0;
    int nx2 = (int)(fx2 * cosnum + fy2 * sinnum);
    int ny2 = (int)(fy2 * cosnum - fx2 * sinnum);
    int nx3 = (int)(fx3 * cosnum + fy3 * sinnum);
    int ny3 = (int)(fy3 * cosnum - fx3 * sinnum);
    int nx4 = (int)(fx4 * cosnum + fy4 * sinnum);
    int ny4 = (int)(fy4 * cosnum - fx4 * sinnum);

    // 计算旋转后的图像的宽和高
    unsigned int width = abs(max(max(nx1, nx2), max(nx3, nx4))) + abs(min(min(nx1, nx2), min(nx3, nx4))) + 1;
    unsigned int hight = abs(max(max(ny1, ny2), max(ny3, ny4))) + abs(min(min(ny1, ny2), min(ny3, ny4))) + 1;

    // 计算旋转后的图像到第一象限的位置偏移
    int bx = abs(min(min(nx1, nx2), min(nx3, nx4)));
    int by = abs(min(min(ny2, ny3), ny4));

    // 申请并初始化内存空间
    BGRA* bgra = (BGRA*)calloc(width * hight, sizeof(BGRA));
    
    for (unsigned int i = 0; i < imageOld->width * imageOld->height; i++)
    {
        // 注意(int)放的位置，不能随便移动,因为y可能为负的小数 
        unsigned int k = ((i % imageOld->width) * cosnum + (i / imageOld->width) * sinnum + bx) + (int)((i / imageOld->width) * cosnum - (i % imageOld->width) * sinnum + by) * width;

        // 判断是否越界
        if (k >= width * hight)
            k = width * hight - 1;

        bgra[k].blue = imageOld->data[i].blue;
        bgra[k].green = imageOld->data[i].green;
        bgra[k].red = imageOld->data[i].red;
        bgra[k].transparency = 255;
    }

    // 用邻近的像素填充空白区域
    for (unsigned int i = 0; i < width * hight; i++)
    {
        if (bgra[i].transparency != 255)
        {
            bgra[i].blue = 255;
            bgra[i].green = 255;
            bgra[i].red = 255;
            bgra[i].transparency = 255;
        }
    }

    //free(imageOld.data);
    imageNew->data = bgra;
    imageNew->width = width;
    imageNew->height = hight;
    return imageNew;
}


// 获得积分图（在此之前要保证图片是“白底黑字”）
IGIMAGE_SELF *IntegralImage_get(IMAGE_SELF *imageOld)
{
    IGIMAGE_SELF *IGmap;

	IGmap = (IGIMAGE_SELF *)malloc(sizeof(IGIMAGE_SELF));

    
    unsigned int* array = (unsigned int *)malloc(sizeof(unsigned int) * imageOld->width * imageOld->height);
    memset(array, 0, sizeof(int) * imageOld->width * imageOld->height);

    int k = 0; // 用于统计每一行的像素个数
    
    for(int height = imageOld->height; height > 0; height--)
	{
		k = 0;
		for(int width = 0; width < imageOld->width; width++)
		{
			int temp = imageOld->data[(height - 1) * imageOld->width + width].blue;
			if(temp == 0)
			{
				k++;
			}
			if (temp == 0)
			{
				//printf("y:");
			}
			else
			{
				//printf("n:");
			}
			
			int heightTemp = imageOld->height - height;
			if (height == imageOld->height)
			{
				array[width] = k;
			}
			else
			{
				array[heightTemp * imageOld->width + width] =  array[(heightTemp - 1) * imageOld->width + width] + k;
				//printf("%3d ", array[heightTemp * imageOld.width + width]);
			}
		}
		//printf("\n");
		//Sleep(1000);
		//pause();
		//DEBUG_PRINT_WITH_TIME("height: %d, k: %d", height, k);
	}

    IGmap->data = array;
    IGmap->width = imageOld->width;
    IGmap->height = imageOld->height;
    return IGmap;
}

// 计算积分区域像素个数
//int IntegralImage_count(IGIMAGE_SELF IGmap, int x0, int y0, int x1, int y1)
long IntegralImage_count(IGIMAGE_SELF *IGmap, int x0, int y0, int x1, int y1)
{
    long a1, a2, a3, a4;
    //int leftBottom = x0 + y1 *IGmap.width;
    //int rightTop = x1 + y0 *IGmap.width;
    
    if(x0 > IGmap->width || x1 > IGmap->width || y1 > IGmap->height || y0 > IGmap->height)
	{
		DEBUG_PRINT_WITH_TIME("x0: %d, y0: %d, x1: %d, y1: %d    width: %d, height: %d", x0, y0, x1, y1, IGmap->width, IGmap->height);
		return -1;
	}
    
	//DEBUG_PRINT_WITH_TIME("x0: %ld, y0: %ld, x1: %ld, y1: %ld    width: %d, height: %d", x0, y0, x1, y1, IGmap.width, IGmap.height)
    
    a1 = y0 * IGmap->width + x0;
    a2 = y1 * IGmap->width + x0;
    a3 = y0 * IGmap->width + x1;
    a4 = y1 * IGmap->width + x1;

	//DEBUG_PRINT_WITH_TIME("a4: %u, a3: %u, a2: %u, a1: %u", a4, a3, a2, a1)
    //  判断是否越界
    if (a1 < 0)
        a1 = 0;
    if (a2 < 0)
        a2 = 0;
    if (a3 < 0)
        a3 = 0;
    if (a3 > IGmap->width * IGmap->height - 1)
        a3 = a4;

	long result = IGmap->data[a4] + IGmap->data[a1] - IGmap->data[a3]  - IGmap->data[a2];
	//DEBUG_PRINT_WITH_TIME("IGmap.data a4: %u, a3: %u, a2: %u, a1: %u result: %u\n", IGmap.data[a4], IGmap.data[a3], IGmap.data[a2], IGmap.data[a1], result)
		
    // 计算区域中的像素数
    return  result;
}

// 释放积分图结构体
void IntegralImage_free(IGIMAGE_SELF *IGimage)
{
    free(IGimage->data);
	free(IGimage);
}

// 单分支决策树分类器
double Classifier_decisionStump(IGIMAGE_SELF *IGmap, int x0, int y0, int x1, int y1, float *parameters)
{   
    //DEBUG_PRINT_WITH_TIME("x0: %d, y0: %d, x1: %d, y1: %d\n", x0, y0, x1, y1);
    int areaW = abs(x0-x1);
    int areaH = abs(y0-y1);

	int xStep = abs(x0-x1)/5;
	int yStep = abs(y0-y1)/5;
	
	long Areas[25] = {0};
	double ReturnFlag = 0;
	
	for(int j = 0; j < 5; j++)
	{
		for(int i = 0; i < 5; i++)
		{
			Areas[j * 5 + i] = IntegralImage_count(IGmap, x0 + i * xStep, y0 + yStep * j, x0 + (i + 1) * xStep, y0 + yStep * (j + 1));
			//DEBUG_PRINT_WITH_TIME("j: %d, i: %d, num: %d\n", j, i, num);
		}
	}

	for(int i=0;i<25;i++)
	{
		//printf("%d: %d, ", i, Areas[i]);
	}
	
	// 计算25个区域的像素个数
	int w_all = IntegralImage_count(IGmap, x0, y0, x1, y1);
	
	//DEBUG_PRINT_WITH_TIME("if: Step 1 计算25个区域的像素个数占面积像素比( > 0.19) : %f", (double)w_all / (areaW * areaH));
	parameters[FACE_RECOGNITION_STEP_1] = (float)w_all / (areaW * areaH);
    if ((double)w_all / (areaW * areaH) < gExpectants[FACE_RECOGNITION_STEP_1].ExpectantValue)
    {
    	//DEBUG_PRINT_WITH_TIME("if return");
		ReturnFlag = FACE_RECOGNITION_STEP_1;
		return ReturnFlag;
	}

	//2、两只眼睛，与人中的比例，正常来说，眼睛比人中的像素点多些
	float d1 = (float)(Areas[0] + Areas[1] + Areas[5] + Areas[6]);        
	float d2 = (float)(Areas[2] + Areas[7]);  
	float d3 = (float)(Areas[3] + Areas[4] + Areas[8] + Areas[9]);
    //DEBUG_PRINT_WITH_TIME("if: Step 2 两只眼睛，与人中的比例，正常来说，眼睛比人中的像素点多些( > 2.6) : %f,  %f", d1 / d2 ,  d3 / d2);
    parameters[FACE_RECOGNITION_STEP_2_1] = d1 / d2;
    parameters[FACE_RECOGNITION_STEP_2_2] = d1 / d2;
    float temp = gExpectants[FACE_RECOGNITION_STEP_2_1].ExpectantValue;
	if (d1 / d2 < temp || d3 / d2 < temp)
    {
    	//DEBUG_PRINT_WITH_TIME("if return");
		ReturnFlag = FACE_RECOGNITION_STEP_2_1;
		return ReturnFlag;
	}
	
	//3、鼻子的像素正常比鼻子两边像素多些
	d1 = (double)(Areas[12] + Areas[17]);
	d2 = (double)(Areas[11] + Areas[16]);
	float d4 = (double)(Areas[13] + Areas[18]);
    //DEBUG_PRINT_WITH_TIME("if: Step 3 鼻子的像素正常比鼻子两边像素多些( > 1): %f,  %f",  d1 / d2 , d3 / d4);
    parameters[FACE_RECOGNITION_STEP_3_1] = d1 / d2;
    parameters[FACE_RECOGNITION_STEP_3_2] = d3 / d4;

    temp = gExpectants[FACE_RECOGNITION_STEP_3_1].ExpectantValue;
	if (d1 / d2 < temp || d1 / d4 < temp)
    {
    	//DEBUG_PRINT_WITH_TIME("if return");
		ReturnFlag = FACE_RECOGNITION_STEP_3_1;
		return ReturnFlag;
	}
	
	//4、眼睛的像素比眼睛下边的脸像素少些，两边都是，因为考虑到还有胡子
	d1 = (double)(Areas[0] + Areas[1] + Areas[5] + Areas[6]);
	d2 = (double)(Areas[10] + Areas[11] + Areas[15]);
	d3 = (double)(Areas[3] + Areas[4] + Areas[8] + Areas[9]);
	d4 = (double)(Areas[13] + Areas[14] + Areas[19]);   
    //DEBUG_PRINT_WITH_TIME("if: Step 4 眼睛的像素比眼睛下边的脸像素少些，两边都是，因为考虑到还有胡子( > 1.3) : %f,  %f",  d1 / d2 , d3 / d4);
    parameters[FACE_RECOGNITION_STEP_4_1] = d1 / d2;
    parameters[FACE_RECOGNITION_STEP_4_2] = d3 / d4;
    temp = gExpectants[FACE_RECOGNITION_STEP_4_1].ExpectantValue;
    if (d1 / d2 < temp || d3 / d4 < temp)
    {
    	//DEBUG_PRINT_WITH_TIME("if return");
		ReturnFlag = FACE_RECOGNITION_STEP_4_1;
		return ReturnFlag;
	}

    d1 = 0;
    d2 = 0;
    for (int i = 0; i < 10; i++)
    {
        d1 += Areas[i];
        d2 += Areas[15 + i];
    }

	//5、上边脸跟嘴和下巴像素的比值
	//d1 = (double)(Areas[0] + Areas[1] + Areas[2] + Areas[3] + Areas[4] + Areas[5] + Areas[6] + Areas[7] + Areas[8] + Areas[9]);
	//d2 = (double)(Areas[15] + Areas[16] + Areas[17] + Areas[18] + Areas[19] + Areas[20] + Areas[21] + Areas[22] + Areas[23] + Areas[24]);
    //DEBUG_PRINT_WITH_TIME("if: Step 5 上边脸跟嘴和下巴像素的比值( > 2): %f", d1 / d2);
    parameters[FACE_RECOGNITION_STEP_5] = d1 / d2;

    if ( d1 / d2 > gExpectants[FACE_RECOGNITION_STEP_5].ExpectantValue)
    {
        //DEBUG_PRINT_WITH_TIME("if return");
        ReturnFlag = FACE_RECOGNITION_STEP_5;
        return ReturnFlag;
    }

	//6、眼睛和嘴巴所占的像素比值不能太小
	d1 = (double)(Areas[0] + Areas[1] + Areas[3] + Areas[4] + Areas[5] + Areas[6] + Areas[8] + Areas[9] 
				+ Areas[12] + Areas[16] + Areas[17] + Areas[18] + Areas[22]);
    //DEBUG_PRINT_WITH_TIME("if: Step 6 眼睛和嘴巴所占的像素比值不能太小( > 0.5) : %f", d1 / w_all);
    parameters[FACE_RECOGNITION_STEP_6] = d1 / d2;
	if (d1/ w_all < gExpectants[FACE_RECOGNITION_STEP_6].ExpectantValue)
    {
        //DEBUG_PRINT_WITH_TIME("if return");
        ReturnFlag = FACE_RECOGNITION_STEP_6;
        return ReturnFlag;
    }
    
	//7、脸左边跟脸右边的比值不能差太多
	d1 = (double)(Areas[0] + Areas[1] + Areas[5] + Areas[6] + Areas[10] + Areas[11]  + Areas[15] + Areas[16]  + Areas[20] + Areas[21]);
	d2 = (double)(Areas[3] + Areas[4] + Areas[8] + Areas[9] + Areas[13] + Areas[14] + Areas[18] + Areas[19] + Areas[23] + Areas[24]);
    double PCT_1 = (double)min(d1, d2)/ max(d1, d2);
    parameters[FACE_RECOGNITION_STEP_7] = PCT_1;
    PCT_1 = exp(-3.125 * (PCT_1 - 1) * (PCT_1 - 1)) * 100;

	//8、两只眼睛的比值不能差太多
	d1 = (double)(Areas[0] + Areas[1] + Areas[5] + Areas[6]);
	d2 = (double)(Areas[3] + Areas[4] + Areas[8] + Areas[9]);
    double PCT_2 = (double)min(d1, d2) / max(d1, d2);
    parameters[FACE_RECOGNITION_STEP_8] = PCT_2;
    PCT_2 = exp(-3.125 * (PCT_2 - 1) * (PCT_2 - 1)) * 100;

	//9、两腮的比值不能差太多
	d1 = (double)(Areas[15] + Areas[20]);
	d2 = (double)(Areas[19] + Areas[24]);
    double PCT_3 = (double)min(d1, d2) / max(d1, d2);
    parameters[FACE_RECOGNITION_STEP_9] = PCT_3;
    PCT_3 = exp(-3.125 * (PCT_3 - 1) * (PCT_3 - 1)) * 100;

    // 计算总的概率
    double PCT_all = (PCT_1 + PCT_2 + PCT_3) / 3;

	//DEBUG_PRINT_WITH_TIME("PCT_all: %f, PCT_1: %f, PCT_2: %f, PCT_3: %f", PCT_all, PCT_1, PCT_2, PCT_3)
        
    // 1、判断是否为人脸，整个区域有的像素个数占整个区域面积的比例
	if (ReturnFlag != 0)
	{
	    return ReturnFlag;
	}
	else if (PCT_all > 60)
	{
		return PCT_all;
	}
	else 
	{
		return -1;
	}    
}

// 人脸数据结构体
typedef struct tagFACE_RESULT
{
    int x0;
    int y0;
    int x1;
    int y1;
    float parameters[FACE_RECOGNITION_STEP_MAX];
    int angle; //旋转角度
    double confidence;
}FACE_RESULT;

int GraphicsInitial(IMAGE_SELF *imageOld)
{    
	setinitmode(0);
	initgraph(imageOld->width * 2, imageOld->height * 2,1);
	
	setcolor(RED);
	
	setlinewidth(2);
}

//期望值初始化，包含：值是多少，阈值是多少，是大于还是小于，期望值说明
int ExpectantsInitial()
{
    gExpectants[FACE_RECOGNITION_STEP_1].ExpectantValue = 0.19;
    gExpectants[FACE_RECOGNITION_STEP_1].Type = 2;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_1].Note, "Step 1 计算25个区域的像素个数占面积像素比期望 > ");

    gExpectants[FACE_RECOGNITION_STEP_2_1].ExpectantValue = 2.6;
    gExpectants[FACE_RECOGNITION_STEP_2_1].Type = 2;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_2_1].Note, "Step 2_1 两只眼睛，与人中的比例，正常来说，眼睛比人中的像素点多些期望 > ");

    gExpectants[FACE_RECOGNITION_STEP_2_2].ExpectantValue = 2.6;
    gExpectants[FACE_RECOGNITION_STEP_2_2].Type = 2;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_2_2].Note, "Step 2_2 两只眼睛，与人中的比例，正常来说，眼睛比人中的像素点多些期望 > ");

    gExpectants[FACE_RECOGNITION_STEP_3_1].ExpectantValue = 1;
    gExpectants[FACE_RECOGNITION_STEP_3_1].Type = 1;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_3_1].Note, "Step 3_1 鼻子的像素正常比鼻子两边像素多些期望 > ");

    gExpectants[FACE_RECOGNITION_STEP_3_2].ExpectantValue = 1;
    gExpectants[FACE_RECOGNITION_STEP_3_2].Type = 1;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_3_2].Note, "Step 3_2 鼻子的像素正常比鼻子两边像素多些期望 > ");

    gExpectants[FACE_RECOGNITION_STEP_4_1].ExpectantValue = 1.3;
    gExpectants[FACE_RECOGNITION_STEP_4_1].Type = 2;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_4_1].Note, "Step 4_1 眼睛的像素比眼睛下边的脸像素少些，两边都是，因为考虑到还有胡子期望 > ");

    gExpectants[FACE_RECOGNITION_STEP_4_2].ExpectantValue = 1.3;
    gExpectants[FACE_RECOGNITION_STEP_4_2].Type = 2;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_4_2].Note, "Step 4_2 眼睛的像素比眼睛下边的脸像素少些，两边都是，因为考虑到还有胡子期望 > ");

    gExpectants[FACE_RECOGNITION_STEP_5].ExpectantValue = 2;
    gExpectants[FACE_RECOGNITION_STEP_5].Type = 1;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_5].Note, "Step 5 上边脸跟嘴和下巴像素的比值期望 < ");

    gExpectants[FACE_RECOGNITION_STEP_6].ExpectantValue = 0.6;
    gExpectants[FACE_RECOGNITION_STEP_6].Type = 2;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_6].Note, "Step 6 眼睛和嘴巴所占整个区域总的像素比值不能太小期望 > ");

    gExpectants[FACE_RECOGNITION_STEP_7].ExpectantValue = 0;
    gExpectants[FACE_RECOGNITION_STEP_7].Type = 0;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_7].Note, "Step 7 脸左边跟脸右边的比值不能差太多 ");

    gExpectants[FACE_RECOGNITION_STEP_8].ExpectantValue = 0;
    gExpectants[FACE_RECOGNITION_STEP_8].Type = 0;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_8].Note, "Step 8 两只眼睛的比值不能差太多 ");

    gExpectants[FACE_RECOGNITION_STEP_9].ExpectantValue = 0;
    gExpectants[FACE_RECOGNITION_STEP_9].Type = 0;
    strcpy(gExpectants[FACE_RECOGNITION_STEP_9].Note, "Step 9 两腮的比值不能差太多 ");

	return 0;
}

//手动调试模式，用鼠标点击，这样图片太大可以使用这种方法，比较快速测试哪些参数不太合理
int ManualMode(IMAGE_SELF imageOld, IGIMAGE_SELF *IGmap)
{    
	int x0 = 0;
	int y0 = 0;
	int x1 = 0;
	int y1 = 0;
    
	int DefaultFaceWidth = 97;
	int DefaultFaceHeight = 100;
	
	setinitmode(0);
	initgraph(imageOld.width,imageOld.height,1);
	
	setcolor(RED);
	
	setlinewidth(2);
	
    //在画板画出图像，这里其实有优化的空间，当前偷懒整个画板更新
    //实际上不需要，只需要更新上次画的红色格子区域就行，当前对性能有点损耗
    for(unsigned int i = imageOld.height - 1; i > 0 ; i--)
    {
        for(unsigned int j = 0; j < imageOld.width; j++)
        {
            unsigned int point = (imageOld.height - i) * imageOld.width + j; 
            putpixel(j, i, EGERGB(imageOld.data[point].red, 
                                   imageOld.data[point].green, 
                                   imageOld.data[point].blue));
        }
    }
    
    while(1)
	{
        int  ret = mousemsg();
        if (! ret)
            continue;
        
        mouse_msg m = getmouse();
        if (!m.is_down() ||  !m.is_left())
            continue;
    					    
		x0 = m.x;   
		y0 = m.y; 
		x1 = x0 + DefaultFaceWidth;
		y1 = y0 + DefaultFaceHeight;
	    //DEBUG_PRINT_WITH_TIME("x0: %d, y0: %d, x1: %d, y1: %d", x0, y0, x1, y1);

        //在画板画出图像，这里其实有优化的空间，当前偷懒整个画板更新
        //实际上不需要，只需要更新上次画的红色格子区域就行，当前对性能有点损耗
        for(unsigned int i = imageOld.height - y0; i > imageOld.height - y1 ; i--)
        {
            for(unsigned int j = x0 - 1; j < x0 + 1; j++)
            {
                unsigned int point = (imageOld.height - i) * imageOld.width + j; 
                putpixel(j, i, EGERGB(imageOld.data[point].red, 
                                       imageOld.data[point].green, 
                                       imageOld.data[point].blue));
            }
        }
        
        //画出当前在运行的方框
        rectangle(x0, y0, x1, y1);
        
        //在红色方框里边画5*5个小方框，为了大概看到每个方框像素点个数
        int xStep = abs(x0-x1)/5;
        int yStep = abs(y0-y1)/5;
        
        for(int j = 1; j < 5; j++)
        {
            line(x0 + xStep * j, y0, x0 + xStep * j, y1 );
            line(x0, y0 + yStep * j, x1, y0 + yStep * j );
        }
		 //记录找到的脸对应的参数，用于未找到正确人脸，查看是哪些参数出了问题，可用于优化
        float parameters[FACE_RECOGNITION_STEP_MAX] = {0};      
        double confidenceTemp = Classifier_decisionStump(IGmap, x0, y0, x1, y1, parameters);
        //if (confidenceTemp > 60.2f)
        DEBUG_PRINT_WITH_TIME("confidenceTemp: %lf, x0: %d, y0: %d, x1: %d, y1: %d\n", confidenceTemp, x0, y0, x1, y1);

        for (int i = 0; i < FACE_RECOGNITION_STEP_MAX; i++)
        {
            DEBUG_PRINT_WITH_TIME("%s%f，实际: %f", gExpectants[i].Note, gExpectants[i].ExpectantValue, parameters[i]);
        } 
        DEBUG_PRINT_WITH_TIME("\n");
    
        Sleep(10);
    }

    return 0;

   

}

//滑动窗口区域（训练用）
int MoveWindowArea(IMAGE_SELF *imageOld, IGIMAGE_SELF *IGmap, FACE_RESULT *FoundFaces)
{    	
	int FaceWidth = 97;
	int FaceHeight = 100;
	int MoveStepSize = 8;
	
	int x0 = 0;
	int y0 = 0;
	for(int i = 0; i * MoveStepSize < imageOld->width - FaceWidth; i++) //height
	{
	    x0 += MoveStepSize;
	    y0 = 0;
		for(int j = 0 ; j * MoveStepSize < imageOld->height - FaceHeight; j++)
		{
			y0 +=  MoveStepSize;
			int x1 = x0 + 97;
			int y1 = y0 + 100;

            DEBUG_PRINT_WITH_TIME();
            
			//超过阈值，直接跑下一次循环
			if(x1 > imageOld->width || y1 > imageOld->height)
			{
				continue;	//TODO
			}
			

            //在画板画出图像，这里其实有优化的空间，当前偷懒整个画板更新
            //实际上不需要，只需要更新上次画的红色格子区域就行，当前对性能有点损耗
			for(unsigned int i = imageOld->height - 1; i > 0 ; i--)
			{
				for(unsigned int j = 0; j < imageOld->width; j++)
				{
					unsigned int point = (imageOld->height - i) * imageOld->width + j; 
					putpixel(j, i, EGERGB(imageOld->data[point].red, 
									       imageOld->data[point].green, 
										   imageOld->data[point].blue));
				}
			}

			//画出当前在运行的方框
			rectangle(x0, y0, x1, y1);

            //在红色方框里边画5*5个小方框，为了大概看到每个方框像素点个数
			int xStep = abs(x0-x1)/5;
			int yStep = abs(y0-y1)/5;

			for(int j = 1; j < 5; j++)
			{
				line(x0 + xStep * j, y0, x0 + xStep * j, y1 );
				line(x0, y0 + yStep * j, x1, y0 + yStep * j );
			}

			//记录找到的脸对应的参数，用于未找到正确人脸，查看是哪些参数出了问题，可用于优化
			float parameters[FACE_RECOGNITION_STEP_MAX] = {0};		
			double confidenceTemp = Classifier_decisionStump(IGmap, x0, y0, x1, y1, parameters);
			if (confidenceTemp > 60.2f)
			{
			    DEBUG_PRINT_WITH_TIME("FaceCount: %d, confidenceTemp: %lf, x0: %d, y0: %d, x1: %d, y1: %d\n", FaceCount + 1, confidenceTemp, x0, y0, x1, y1);
			    FoundFaces[FaceCount].x0 = x0;
			    FoundFaces[FaceCount].x1 = x1;
			    FoundFaces[FaceCount].y0 = y0;
			    FoundFaces[FaceCount].y1 = y1;
			    FoundFaces[FaceCount].angle = angle;
			    for (int i = 0; i < FACE_RECOGNITION_STEP_MAX; i++)
    			{
     			    FoundFaces[FaceCount].parameters[i] = parameters[i];
     			    DEBUG_PRINT_WITH_TIME("%s%f，实际: %f", gExpectants[i].Note, gExpectants[i].ExpectantValue, parameters[i]);
    			} 
			    FaceCount++;
			    y0 += FaceHeight;
			    x0 += FaceWidth;
			    DEBUG_PRINT_WITH_TIME("\n");
			    //getch();
			}

			//Sleep(100);	
		}
		
		Sleep(1);
	}

    for (int i = 0; i < FaceCount; i++)
    {
        rectangle(FoundFaces[i].x0, FoundFaces[i].y0, FoundFaces[i].x1, FoundFaces[i].y1);
        DEBUG_PRINT_WITH_TIME("FoundFaces: %d, angle: %d: x0: %d, y0: %d, x1: %d, y1: %d", i + 1, angle, FoundFaces[i].x0,  FoundFaces[i].y0, FoundFaces[i].x1, FoundFaces[i].y1);
        for (int j = 0; j < FACE_RECOGNITION_STEP_MAX; j++)
            DEBUG_PRINT_WITH_TIME("%s%f，实际: %f", gExpectants[j].Note, gExpectants[j].ExpectantValue, FoundFaces[i].parameters[j]);
        printf("\n");
        Sleep(1);
    }
	
    // 窗口区域的取值范围
	//getch();
	//closegraph();
    return FaceCount;
 }

// 画出人框
void Image_draw(IMAGE_SELF *imageOld, FACE_RESULT *faceResult, int FaceCount)
{
    int width = imageOld->width;
    int height = imageOld->height;

    for (int count = 0; count < FaceCount; count++)
    {
        int x0 = faceResult[count].x0;
        int y0 = faceResult[count].y0;
        int x1 = faceResult[count].x1;
        int y1 = faceResult[count].y1;
        int LineWidth = 4;
        y0 = height - y0;
        y1 = height - y1;

        int temp = y0;
        y0 = y1;
        y1 = temp;
        
        //竖
        for (int i = y0; i < y1; i++)
        {
        	for (int j = 0; j < LineWidth; j++)
        	{
	            imageOld->data[i * width + x0 + j].blue = 0;
	            imageOld->data[i * width + x0 + j].green = 0;
	            imageOld->data[i * width + x0 + j].red = 200;	
	            imageOld->data[i * width + x1 - j].blue = 0;
	            imageOld->data[i * width + x1 - j].green = 0;
	            imageOld->data[i * width + x1 - j].red = 200; 
			}         
        }
        
        //横
        for (int i = x0; i < x1; i++)
        {
        	for (int j = 0; j < LineWidth; j++)
        	{
	            imageOld->data[(y0 + j) * width + i].blue = 0;
	            imageOld->data[(y0 + j) * width + i].green = 0;
	            imageOld->data[(y0 + j) * width + i].red = 200;
		        imageOld->data[(y1 - j) * width + i].blue = 0;
	            imageOld->data[(y1 - j) * width + i].green = 0;
	            imageOld->data[(y1 - j) * width + i].red = 200;  			
			}
        }
    }
}

//积分图保存
int IG_ImageSave(IGIMAGE_SELF *IGmap1, char *OutputFile)
{
    IMAGE_SELF *image36;    
    image36 = (IMAGE_SELF *)malloc(sizeof(IMAGE_SELF) );
    image36->width = IGmap1->width;
    image36->height = IGmap1->height;
    image36->data = (BGRA *)malloc(sizeof(BGRA) * IGmap1->width * IGmap1->height);
    
    for (unsigned int i = 0; i < IGmap1->width * IGmap1->height; i++)
    {
        image36->data[i].red = IGmap1->data[i] % 256;
        image36->data[i].green = IGmap1->data[i] / 256 % 256;
        image36->data[i].blue = IGmap1->data[i] / 256 / 256 % 256;
        image36->data[i].transparency = 0;
        //if (i %1000 == 0)
        //  DEBUG_PRINT_WITH_TIME("w: %d, h: %d, i: %d", image35.width, image35.h, i);
    }
    
    Image_save(OutputFile, image36);    
    
    DEBUG_PRINT_WITH_TIME("1");
    for(unsigned int i = 0; i < IGmap1->width * IGmap1->height; i++)
    {
        //printf("%d ", IGmap1->data[i]);
        if((i + 1) % IGmap1->width == 0 )
        {
            //putchar('\n');
        }
    }

}


int IG_ImageWhirl(IGIMAGE_SELF *IGmap1, char *OutputFile)
{
    //准备旋转积分图，用于人脸旋转识别测试
    //当前准备沿着图像横轴和纵轴中心旋转
    IMAGE_SELF *image37;    
    image37 = (IMAGE_SELF *)malloc(sizeof(IMAGE_SELF) );
    if (NULL == image37)
    {
        DEBUG_PRINT_WITH_TIME("memory allocate failed...");
        return -1;
    }
    int NewWidth = IGmap1->width * 2;
    int NewHeight = IGmap1->height * 2;
    image37->width = NewWidth;
    image37->height = NewHeight;
    DEBUG_PRINT_WITH_TIME("memory allocate: %d", 4 * NewWidth * NewWidth);
    image37->data = (BGRA *)malloc(4 * NewWidth * NewWidth);
    if (NULL == image37->data)
    {
        DEBUG_PRINT_WITH_TIME("memory allocate failed...");
        return -1;
    }

    int OldHeight = IGmap1->height;
    int OldWidth = IGmap1->width;

    int OldCenterHeight = OldHeight / 2;
    int OldCenterWidth = OldWidth / 2;
    
    int NewCenterWidth = NewWidth / 2;
    int NewCenterHeight = NewHeight / 2;
    
    //度数
    //double AngleAdd = 45 * PI / 180;
    double AngleAdd = 0;
    double Angle = 0;
    
    //旋转
    //for (int height = 2; height < OldHeight; height++)
    for (int height = 0; height < OldHeight; height++)
    {
        for (int width = 0; width < OldWidth; width++)
        //for (int width = 2; width < OldWidth; width++)
        {
            //当前点到中心点的距离
            int HeightTemp = height - OldCenterHeight;            
            int WidthTemp = width - OldCenterWidth;
            
            int HeightAbsTemp = abs(HeightTemp);
            int WidthAbsTemp = abs(WidthTemp);
            int Radius = sqrt(HeightAbsTemp * HeightAbsTemp + WidthAbsTemp * WidthAbsTemp);
            
            if(Radius != 0) //半径为0时，计算出来的Angle会有问题
            {
                Angle = asin((double)WidthAbsTemp / Radius);
            }
            
            if (height > OldCenterHeight & & width > OldCenterWidth)  //第一象限 （0，PI/2）
            {
                //不用管
            }
            else if (height >= OldCenterHeight & & width <= OldCenterWidth) //第二象限 （PI/2，PI）
            {
                Angle += PI/2;
            }           
            else if (height <= OldCenterHeight & &  width <= OldCenterWidth) //第三象限 （PI，3PI/2）
            {
                Angle += PI;
            }           
            else if (height <= OldCenterHeight & &  width >= OldCenterWidth) //第四象限 （3PI/2，2PI）
            {
                Angle += 3*PI/2;
            }
            
            double NewAngle = Angle + AngleAdd;
            int NewY, NewX;

            if (Angle > 0 && Angle < PI/2 || Angle > PI & & Angle < 3*PI/2)
            {
                //求新的x, y坐标点
                NewY = Radius * cos(NewAngle);            
                NewX = Radius * sin(NewAngle);
            }
            else
            {
                NewX = Radius * cos(NewAngle);            
                NewY = Radius * sin(NewAngle);
            }
            /*
            DEBUG_PRINT_WITH_TIME("width: %d, height: %d, Radius: %d, Angle: %lf, NewAngle: %lf, NewX: %d, NewY: %d", 
                                    width, 
                                    height, 
                                    Radius,
                                    Angle, 
                                    NewAngle, 
                                    NewX, 
                                    NewY)
                                    */
            //沿中心旋转
            int location = (NewCenterHeight + NewY) * NewWidth + NewCenterWidth + NewX;
            int location2 = height * OldWidth + width;
            //DEBUG_PRINT_WITH_TIME("location: %d, location2: %d", 
            //                                  location, 
            //                                  location2)
            if(location < 0)
            {
                DEBUG_PRINT_WITH_TIME("location < 0");
                break;
            }
            
            if(location2 < 0)
            {
                DEBUG_PRINT_WITH_TIME("location2 < 0");
                break;
            }
            image37->data[location].red   = IGmap1->data[location2] % 256;
            image37->data[location].green = IGmap1->data[location2] / 256 % 256;
            image37->data[location].blue  = IGmap1->data[location2] / 256 / 256 % 256;
            image37->data[location].transparency = 0;
        }
    }
    
    Image_save(OutputFile, image37);  

}

char InputFile[100] = {0};
char OutputFile[100] = {0};

FileNameInitial()
{
    char InputPath[100] = "../Input/";
    char OutputPath[100] = "../Output/";
    
    char InputFileName[50] = "InputTest_04.bmp";
    char OutputFilename[100] = "_Result00.bmp";

    
    DEBUG_PRINT_WITH_TIME("main start....");


    strcpy(InputFile, InputPath);
    strcpy(OutputFile, OutputPath);
    
    strcat(InputFile, InputFileName);
    
    strcat(OutputFile, InputFileName);  
    OutputFile[strlen(OutputFile) - 4] = '_';
    
    strcat(OutputFile, OutputFilename);
}


int main()
{
    FileNameInitial();
    
	// 用于处理
	IMAGE_SELF *image1 = Image_load(InputFile);
    if (image1 == NULL)
    {
        DEBUG_PRINT_WITH_TIME("readBMP file Abnormal...");
        return -1;
    }
		
	// 灰度图
	IMAGE_SELF *image3 = Transform_color_grayscale(image1, GRAY_MODE_WEIGHT);
	Image_save(OutputFile, image3);
	
	// 均值滤波
	IMAGE_SELF *image4 = Wavefiltering_Average(image3);
	Image_save(OutputFile, image4);
	
	// 二值图加边缘检测
	IMAGE_SELF *image5 = Transform_color_BW_Adaptive(image4, 25);
	Image_save(OutputFile, image5);

	// 积分图
	IGIMAGE_SELF *IGmap1 = IntegralImage_get(image5);

    IG_ImageSave(IGmap1, OutputFile);

	//假设能找到FaceCountExpect张脸
    int FaceCountExpect = 1000;
	FACE_RESULT *faceResult = (FACE_RESULT *)malloc(FaceCountExpect * sizeof(FACE_RESULT));
	memset(faceResult, 0, FaceCountExpect * sizeof(FACE_RESULT));
	
    ExpectantsInitial();
    
    GraphicsInitial(image1);

    for (angle = 10; angle < 360; angle += 10)
    {
        //旋转图像
        IMAGE_SELF *image6 = Transform_shape_whirl(image5, angle);
        Image_save(OutputFile, image6);
        
        // 旋转图像积分图
        IGIMAGE_SELF *IGmap2 = IntegralImage_get(image6);
        
        IG_ImageSave(IGmap2, OutputFile);
            
        // 滑动窗口
        FaceCount += MoveWindowArea(image6, IGmap2, faceResult);        
        Sleep(200);
    }

	// 画出人脸框
	Image_draw(image1, faceResult, FaceCount);


    //ManualMode(image5, IGmap1);
	
	// 保存图片
	Image_save(OutputFile, image1);
	
	// 释放积分图
	IntegralImage_free(IGmap1);
	
	// 释放图片资源 
	Image_free(image1);
	//Image_show(saveFilename);
	
	return 0;
}
