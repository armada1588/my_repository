typedef struct{
	char Identifier[6];
	char FontName[8];
	unsigned char Xsize;
	unsigned char Ysize;
	unsigned char CodeType;
}fontheader;

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
	*fx = fh.Xsize / 8;//x������8����Cfh.Xsize��8�ȏ�15�ȉ��Ȃ�x�͕K��1(int�^�ŏ����_�͐؂�̂�)
	if(fh.Xsize % 8){//fh.Xsize%8�ŗ]������߂邠�܂肪1�ȏ�Ȃ�x���C���N�������g
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
	ifp = fopen(fpath,"r");//���ƂȂ�bmp�t�@�C�������[�h���[�h�œǂݍ���
	//fread(�i�[��̃o�b�t�@,�ǂݍ��ރf�[�^��̃o�C�g��,�ǂݍ��ރf�[�^�̌�,�t�@�C���|�C���^)
	//��̍s�œǂݍ���bmp�t�@�C������t�@�C���w�b�_�����Ə��w�b�_����(���ۂ�RGB����)���o�b�t�@�Ɋi�[����C���[�W
	
	
	fread(bfh,sizeof(BITMAPFILEHEADER),1,ifp);
	fread(bih,sizeof(BITMAPINFOHEADER),1,ifp);
	
	*bmp = calloc(bih->biWidth*bih->biHeight,sizeof(RGBarray));//bmp�t�@�C���̈�m��
	if(bmp==NULL){
		printf("�̈�m�ێ��s\n");
		exit(EXIT_FAILURE);
	}

	fread(*bmp,sizeof(RGBarray),bih->biWidth*bih->biHeight,ifp);//RGBarray��1�P�ʂƂ���bmp�t�@�C���̍���*�������m�ۂ���,����bmp�����ۂɑ��삷�镔��

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
					printf("��");
				}else{
					printf("��");
				}
				mask = mask >> 1;
			}
			printf("\n");
		}
		printf("\n");
}
void ShapeBitmapData_Output(char filepath[],FILE **ofp,BITMAPFILEHEADER *bfh,BITMAPINFOHEADER *bih){
	
	*ofp = fopen(filepath,"w");//�A�E�g�v�b�g�t�@�C����
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
				printf("��");
			}else{
				printf("��");
			}
			mask = mask >> 1;
		}
		printf("\n");
	}
	*Bmp_sp_x = *Bmp_sp_x + fhX;
	
}
