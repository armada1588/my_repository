#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bitmap.h"
#include "bitmapfunc.h"

int main(int argc,char *argv[])
{

	if(argc!=7){
			printf("Usage:%s ���̓t�@�C���� �o�̓t�@�C���� X���W Y���W ���͂��镶�� �{�� < �t�H���g�t�@�C����\n",argv[0]);
			exit(EXIT_FAILURE);
	}
	
	/*�t�H���g�ǂݍ���*/
	fontheader fh;//�t�H���g�w�b�_
	unsigned char *p,*p0,*p1;//�t�H���g�|�C���^,p=�t�H���g�̃{�f�B�����̓ǂݍ��ݗp,p0=�t�H���g�{�f�B�����̈�ԏ��߂��L�^����p
	int fx;//�t�H���g�t�@�C����x��(����)�{��	
	
	fh = CreateFontPointer(&p,&p0,&fx);//getchar()�g�p�̂��ߕK���ŏ��Ɏ��s
	
	p0 = p;//�֐����ł��Ȃ�������
	for(int j=0;j<256;j++){//�t�@�C���̃{�f�B��p�œǂݍ���ł���
		for(int k=0;k<fx*fh.Ysize;k++){
			*p++ = (unsigned char)getchar();
		}
	}

	
	RGBarray *bmp;//bmp�Œ��ڑ��삷�镔���̃o�b�t�@
	BITMAPFILEHEADER bfh;//bmp�̃w�b�_����
	BITMAPINFOHEADER bih;//bmp�̏�񕔕�
	char ibmpfilepath[64];//�C���v�b�g�t�@�C�����Ƃ������t�@�C���p�X�Ƃ������Ɠ��Ƃ�����������Ȃ�ł�����
	strncpy(ibmpfilepath, argv[1], sizeof(ibmpfilepath));//�C���v�b�g�t�@�C���������s��������ibmpfilepath�Ɋi�[
	CreateBitmapPointer(ibmpfilepath,&bmp,&bfh,&bih);//bmp�t�@�C���𒼐ڑ��삷�镔���̃|�C���^�쐬

	FILE *ofp;	
	char obmpfilepath[64];
	strncpy(obmpfilepath, argv[2], sizeof(obmpfilepath));//�A�E�g�v�b�g�t�@�C���������s��������obmpfilepath�Ɋi�[
	ShapeBitmapData_Output(obmpfilepath,&ofp,&bfh,&bih);//�o�͗p��bmp���`�����Ă���

	
	
	RGBQUAD rgbq;
	int Bmp_sp_x = atoi(argv[3]);//bitmap steart point x
	int Bmp_sp_y = atoi(argv[4]);//bitmap steart point y
	int Font_Scale = atoi(argv[6]);
	char StrBuf[strlen(argv[5]) + 1]; // +1 �͏I�[�����p�̗̈���m�ۂ��邽�߂ł�
	strncpy(StrBuf, argv[5], sizeof(StrBuf) - 1); // �I�[�����̕��������ăR�s�[
	StrBuf[sizeof(StrBuf) - 1] = '\0'; // �I�[�������蓮�Œǉ�
	
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
//./xxxxx.c ���̓t�@�C���� �o�̓t�@�C���� X���W Y���W ���͂��镶�� �{�� < �t�H���g�t�@�C����
