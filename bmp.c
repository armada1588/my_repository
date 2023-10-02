#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bitmap.h"
#include "bitmapfunc.h"

int main(int argc,char *argv[])
{

	if(argc!=7){
			printf("Usage:%s 入力ファイル名 出力ファイル名 X座標 Y座標 入力する文字 倍率 < フォントファイル名\n",argv[0]);
			exit(EXIT_FAILURE);
	}
	
	/*フォント読み込み*/
	fontheader fh;//フォントヘッダ
	unsigned char *p,*p0,*p1;//フォントポインタ,p=フォントのボディ部分の読み込み用,p0=フォントボディ部分の一番初めを記録する用
	int fx;//フォントファイルのx軸(横幅)倍率	
	
	fh = CreateFontPointer(&p,&p0,&fx);//getchar()使用のため必ず最初に実行
	
	p0 = p;//関数化できない助けて
	for(int j=0;j<256;j++){//ファイルのボディをpで読み込んでおく
		for(int k=0;k<fx*fh.Ysize;k++){
			*p++ = (unsigned char)getchar();
		}
	}

	
	RGBarray *bmp;//bmpで直接操作する部分のバッファ
	BITMAPFILEHEADER bfh;//bmpのヘッダ部分
	BITMAPINFOHEADER bih;//bmpの情報部分
	char ibmpfilepath[64];//インプットファイル名というかファイルパスというか家内というか奥さんなんですけど
	strncpy(ibmpfilepath, argv[1], sizeof(ibmpfilepath));//インプットファイル名を実行引数からibmpfilepathに格納
	CreateBitmapPointer(ibmpfilepath,&bmp,&bfh,&bih);//bmpファイルを直接操作する部分のポインタ作成

	FILE *ofp;	
	char obmpfilepath[64];
	strncpy(obmpfilepath, argv[2], sizeof(obmpfilepath));//アウトプットファイル名を実行引数からobmpfilepathに格納
	ShapeBitmapData_Output(obmpfilepath,&ofp,&bfh,&bih);//出力用のbmpを形成しておく

	
	
	RGBQUAD rgbq;
	int Bmp_sp_x = atoi(argv[3]);//bitmap steart point x
	int Bmp_sp_y = atoi(argv[4]);//bitmap steart point y
	int Font_Scale = atoi(argv[6]);
	char StrBuf[strlen(argv[5]) + 1]; // +1 は終端文字用の領域を確保するためです
	strncpy(StrBuf, argv[5], sizeof(StrBuf) - 1); // 終端文字の分を除いてコピー
	StrBuf[sizeof(StrBuf) - 1] = '\0'; // 終端文字を手動で追加
	
	for(int j = 0; j < strlen(StrBuf);j++){
		p=p0;
		char c = StrBuf[j];
		for(int j=0;j<c;j++){		
			for(int k=0;k<fx*fh.Ysize;k++){
				p++;
			}
		}
		p1 = p;
		unsigned char FontOrgBox[fh.Ysize];
		CreateOrgFontArray(FontOrgBox,p1,fh);
		EditBmp(&Bmp_sp_x,Bmp_sp_y,(int)fh.Xsize,(int)fh.Ysize,&bih,FontOrgBox,&bmp);

	}

	fwrite(bmp,sizeof(RGBarray),bih.biWidth*bih.biHeight,ofp);
	fclose(ofp);
	free(bmp);
	return 0;

}


//bmp.exe Balloon.bmp Balloon_A.bmp 100 100 test 1 < ILGH16XB.FNT
//./xxxxx.c 入力ファイル名 出力ファイル名 X座標 Y座標 入力する文字 倍率 < フォントファイル名
