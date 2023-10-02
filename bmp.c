#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bitmap.h"


typedef struct{
	char Identifier[6];
	char FontName[8];
	unsigned char Xsize;
	unsigned char Ysize;
	unsigned char CodeType;
} fontheader;

fontheader CreateFontPointer(unsigned char **p,unsigned char **p0,int *fx){
	fontheader fh;
	for(int i=0;i<6;i++){
		fh.Identifier[i] = getchar();
	}

	for(int i=0;i<8;i++){
		fh.FontName[i] = getchar();
	}
	fh.Xsize = getchar();
	fh.Ysize = getchar();
	fh.CodeType = getchar();
	*fx = fh.Xsize / 8;//xを割る8する，fh.Xsizeが8以上15以下ならxは必ず1(int型で小数点は切り捨て)
	if(fh.Xsize % 8){//fh.Xsize%8で余りを求めるあまりが1以上ならxをインクリメント
		(*fx++);
	}
	*p = (unsigned char *)malloc(256*(*fx)*fh.Ysize);
	if(*p==NULL){
		exit(EXIT_FAILURE);
	}

	return fh;
}
void CreateBitmapPointer(char fpath[],RGBarray **bmp,BITMAPFILEHEADER *bfh,BITMAPINFOHEADER *bih){

	FILE *ifp;
	ifp = fopen(fpath,"r");//元となるbmpファイルをリードモードで読み込み
	//fread(格納先のバッファ,読み込むデータ一つのバイト数,読み込むデータの個数,ファイルポインタ)
	//上の行で読み込んだbmpファイルからファイルヘッダ部分と情報ヘッダ部分(実際のRGB部分)をバッファに格納するイメージ
	
	
	fread(bfh,sizeof(BITMAPFILEHEADER),1,ifp);
	fread(bih,sizeof(BITMAPINFOHEADER),1,ifp);
	
	*bmp = calloc(bih->biWidth*bih->biHeight,sizeof(RGBarray));//bmpファイル領域確保
	if(bmp==NULL){
		printf("領域確保失敗\n");
		exit(EXIT_FAILURE);
	}

	fread(*bmp,sizeof(RGBarray),bih->biWidth*bih->biHeight,ifp);//RGBarrayを1単位としてbmpファイルの高さ*横分を確保する,このbmpが実際に操作する部分

	fclose(ifp);
}
void CreateOrgFontArray(unsigned char *FontOrgBox,unsigned char *p1,fontheader fh){
		for(int k=0;k<fh.Ysize;k++){
			FontOrgBox[k] = *p1++;
		}
		for(int k=0;k<fh.Ysize;k++){
			unsigned int mask = 0x80;
			for(int l=0;l<fh.Xsize;l++){
				if(mask & FontOrgBox[k]){
					printf("■");
				}else{
					printf("□");
				}
				mask = mask >> 1;
			}
			printf("\n");
		}
		printf("\n");
}
void ShapeBitmapData_Output(char filepath[],FILE **ofp,BITMAPFILEHEADER *bfh,BITMAPINFOHEADER *bih){
	
	*ofp = fopen(filepath,"w");//アウトプットファイル名
	fwrite(bfh,sizeof(BITMAPFILEHEADER),1,*ofp);
	fwrite(bih,sizeof(BITMAPINFOHEADER),1,*ofp);
	fseek(*ofp,bfh->bfOffBits,SEEK_SET);
	
}
void EditBmp(int *Bmp_sp_x,int Bmp_sp_y,int fhX,int fhY,BITMAPINFOHEADER *bih,unsigned char *FontOrgBox,RGBarray **bmp){
	for(int y=0; y<fhY;y++){
		int edit_y = y + Bmp_sp_y;
		//printf("\nedit_y=%d\n",edit_y);
		unsigned char mask=0x80;
		for(int x=0; x<fhX;x++){
			int edit_x = x + *Bmp_sp_x;
			//printf("\nedit_x=%d\n",edit_x);
			if(mask & FontOrgBox[(fhY -1)-y]){
				(*bmp + edit_x + edit_y * bih->biWidth)-> rgbBGR[0] = 0;
				(*bmp + edit_x + edit_y * bih->biWidth)-> rgbBGR[1] = 0;
				(*bmp + edit_x + edit_y * bih->biWidth)-> rgbBGR[2] = 0xff;
				printf("■");
			}else{
				printf("□");
			}
			mask = mask >> 1;
		}
		printf("\n");
	}
	*Bmp_sp_x = *Bmp_sp_x + fhX;
	
}
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
