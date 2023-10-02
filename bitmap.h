#ifndef __BMP_H__
#define __BMP_H__

typedef struct{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} __attribute__((__packed__)) BITMAPFILEHEADER;

typedef struct {
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPixPerMeter;
	int32_t biYPixPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} __attribute__((__packed__)) BITMAPINFOHEADER;

typedef struct {
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
} __attribute__((__packed__)) RGBQUAD;
typedef struct {
//	uint8_t rgbBlue;
//	uint8_t rgbGreen;
//	uint8_t rgbRed;
	uint8_t rgbBGR[3];
}  __attribute__((__packed__)) RGBarray;


#endif	//__BMP_H__

