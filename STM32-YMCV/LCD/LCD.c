#include "lcd_cfg.h"
#include "delay.h"
#include "./LCD.h"
#include "./font.h" 
#include "stdio.h" 	 
#include "MemDbg.h"

//��ͨ��תRGB565
#define  R_TORGB565(R)   ((((CVrgb16)R) << 8) & 0xF800)
#define  G_TORGB565(G)   ((((CVrgb16)G) << 3) & 0x7E0)
#define  B_TORGB565(B)   ((((CVrgb16)B) >> 3) & 0x001F)
//�ڴ�
#define  LCD_Malloc(size)  YMem_malloc_nt(0,size)
#define  LCD_Free(ptr)     YMem_free_nt(0,ptr)
//����
#define  LCD_DebugPrintf(tipStr)  printf(tipStr)

uint16_t LCD_W=240,LCD_H=240;
color24_t LCD_POINT_COLOR = { 0,0,0 }, LCD_BCAK_COLOR = { 255,255,255 };


//���û�����ɫ
void LCD_Point_Color_Set(color24_t thiscolor)
{
    LCD_POINT_COLOR = thiscolor;
		//תrgb565
	  POINT_COLOR = (R_TORGB565(thiscolor.R) + G_TORGB565(thiscolor.G) + B_TORGB565(thiscolor.B));
}

color24_t LCD_Pen_Color_Get()
{
    return LCD_POINT_COLOR;
}
//����
//x1,y1:�������
//x2,y2:�յ����� 
void LCD_Draw_Line(uint32 x1, uint32 y1, uint32 x2, uint32 y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}


//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(uint32 x0, uint32 y0, uint32 r)
{
    
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
}

//����
void LCD_Draw_Point(uint32 x, uint32 y, color24_t thiscolor)
{
		LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	  //תrgb565
	  uint16 rgb565 = (R_TORGB565(thiscolor.R) + G_TORGB565(thiscolor.G) + B_TORGB565(thiscolor.B));
		LCD_WR_DATA(rgb565);
}
// ��ͣ�ȴ�����
void PAUSE()
{
      //getch();	
}
// ��ʱ
void Delay(int ms)
{
	  HAL_Delay_ms(ms);
}


//��ȡ��������
static int mouse_x, mouse_y;
int LCD_GetXY(int startx,int starty,int rew,int reh,int* px,int* py)
{
    //mousepos(&mouse_x, &mouse_y);

    uint16 cx = mouse_x - startx;
    uint16 cy = mouse_y - starty;
    int geflag = 0;
    if ((cx<rew) && (cy<reh))geflag = 1;

    if (geflag)
    {
        *px = cx;
        *py = cy;
    }
    return geflag;
}
//��ȡ��������
int LCD_GetChar(char* c)
{
//    int flag = kbhit();

//    if (flag)
//    {
//        *c = getch();
//    }
//    return flag;
}

//��������
//Color:Ҫ���������ɫ
void LCD_Clear(uint16 Color)
{
	uint16_t i,j;
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
  for(i=0;i<LCD_W;i++)
	{
		for (j=0;j<LCD_H;j++)
		{
			LCD_WR_DATA(Color);	 			 
		}
	}
}

//������
void LCD_Draw_Rect(int startx, int starty, int width, int height, color24_t thiscolor)
{
    uint16 i, j;
		//תrgb565
		uint16 rgb565 = (R_TORGB565(thiscolor.R) + G_TORGB565(thiscolor.G) + B_TORGB565(thiscolor.B));
	
    for (i = 0; i < height; i++)
    {
        j = 0;
			  uint16 x = startx + j;
			  uint16 y = starty + i;
				LCD_Address_Set(x,y,x,y);//���ù��λ��
				LCD_WR_DATA(rgb565); 
    }

    for (i = 0; i < height; i++)
    {
        j = width;
			  uint16 x = startx + j;
			  uint16 y = starty + i;
				LCD_Address_Set(x,y,x,y);//���ù��λ��
				LCD_WR_DATA(rgb565); 
    }

    for (j = 0; j < width; j++)
    {
        i = height;
			  uint16 x = startx + j;
			  uint16 y = starty + i;
				LCD_Address_Set(x,y,x,y);//���ù��λ��
				LCD_WR_DATA(rgb565); 
    }
    for (j = 0; j < width; j++)
    {
        i = 0;
			  uint16 x = startx + j;
			  uint16 y = starty + i;
				LCD_Address_Set(x,y,x,y);//���ù��λ��
				LCD_WR_DATA(rgb565); 
    }
}

//��ָ�����������ָ����ɫ
//�����С: width height
void LCD_Fill_RgbRect(int startx, int starty, int width, int height , color24_t* data)
{
	  uint16_t xend = startx + width - 1;
		uint16_t yend = starty + height - 1;
		LCD_Address_Set(startx,starty,xend,yend);      //���ù��λ��
    
    color24_t thiscolor;
		for (uint16 i = 0; i < width; i++)
    {
        for (uint16 j = 0; j < height; j++)
        {
            thiscolor = data[i * width + j];
						//תrgb565
						uint16 rgb565 = (R_TORGB565(thiscolor.R) + G_TORGB565(thiscolor.G) + B_TORGB565(thiscolor.B));
						LCD_WR_DATA(rgb565);
        }
    }	
}

void LCD_Fill_GrayRect(int startx, int starty, int width, int height, _color8_t* data)
{
	  uint16_t xend = startx + width - 1;
		uint16_t yend = starty + height - 1;
		LCD_Address_Set(startx,starty,xend,yend);      //���ù��λ��
    
    _color8_t gval;
		for (uint16 i = 0; i < width; i++)
    {
        for (uint16 j = 0; j < height; j++)
        {
            gval = data[i * width + j];
						//תrgb565
						uint16 rgb565 = (R_TORGB565(gval) + G_TORGB565(gval) + B_TORGB565(gval));
						LCD_WR_DATA(rgb565);
        }
    }	
}

void LCD_Fill_ConstRgbRect(int startx, int starty, int width, int height, color24_t thiscolor)
{
	  uint16_t xend = startx + width - 1;
		uint16_t yend = starty + height - 1;
		LCD_Address_Set(startx,starty,xend,yend);      //���ù��λ��
    
		//תrgb565
		uint16 rgb565 = (R_TORGB565(thiscolor.R) + G_TORGB565(thiscolor.G) + B_TORGB565(thiscolor.B));
	
		for (uint16 i = 0; i < width; i++)
    {
        for (uint16 j = 0; j < height; j++)
        {
						LCD_WR_DATA(rgb565);
        }
    }	
}



//��ָ��λ����ʾһ���ַ�
//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�
//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
    uint8_t temp;
    uint8_t pos,t;
		uint16_t x0=x;
		uint16_t colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
		//���ô���		   
		num=num-' ';//�õ�ƫ�ƺ��ֵ
		LCD_Address_Set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
		if(!mode) //�ǵ��ӷ�ʽ
		{
			for(pos=0;pos<16;pos++)
			{ 
				temp=asc2_1608[(uint16_t)num*16+pos];		 //����1608����
				for(t=0;t<8;t++)
					{                 
							if(temp&0x01)POINT_COLOR=colortemp;
					else POINT_COLOR=BACK_COLOR;
					LCD_WR_DATA(POINT_COLOR);	
					temp>>=1; 
					x++;
					}
				x=x0;
				y++;
			}	
		}
		else//���ӷ�ʽ
		{
			for(pos=0;pos<16;pos++)
			{
					temp=asc2_1608[(uint16_t)num*16+pos];		 //����1608����
				for(t=0;t<8;t++)
					{                 
							if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
							temp>>=1; 
					}
			}
		}
		POINT_COLOR=colortemp;	    	   	 	  
}

//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(uint16 x, uint16 y, const char* p)
{         
    while(*p!='\0')
    {   
				//�������з�
			  if(*p == '\n')
				{
					x=0;
					y+=16;	
					p++;					
          continue;					
				}
				
        if(x > LCD_W-16) //�ַ�����
				{
					x=0;
					y+=16;
				}
				//�������
        if(y>LCD_H-16)
				{
					y=x=0;
					LCD_Clear(RED);
				}
				//�����ַ�
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}




//				RGB565��ʽͼ����LCD����ʾ
void LCD_show_rgbbuffer(uint16 x, uint16 y, uint16 Width, uint16 Height, uint16* pic)
{
		uint16_t xend = x + Width - 1;
		uint16_t yend = y + Height - 1;
		LCD_Address_Set(x,y,xend,yend);      //���ù��λ��
		for (int i = 0, imax = Height * Width; i < imax; i++)
    {
			LCD_WR_DATA(pic[i]);//д������
		}
}

void LCD_show_graybuffer(uint16 x, uint16 y, uint16 Width, uint16 Height, uint8* pic)
{
		uint16_t xend = x + Width - 1;
		uint16_t yend = y + Height - 1;
		LCD_Address_Set(x,y,xend,yend);      //���ù��λ��
		for (int i = 0, imax = Height * Width; i < imax; i++)
    {
			uint8 gray = pic[i];
			//תrgb565
			uint16 rgb565 = (R_TORGB565(gray) + G_TORGB565(gray) + B_TORGB565(gray));
			LCD_WR_DATA(rgb565);
		}
}

//��ʾͼƬ
void LCD_show_image(uint16 x, uint16 y, CVIMAGE myimg)
{
    if (myimg->type == CVRgb16Type)
    {
        LCD_show_rgbbuffer(x, y, myimg->width, myimg->height,(uint16*) myimg->data);
    }

    else if (myimg->type <= CVGrayType)
    {
        LCD_show_graybuffer(x, y, myimg->width, myimg->height, (uint8*)myimg->data);
    }
}

//				RGB565��ʽͼ����LCD����ʾ
void YMKV_show_pic(uint16_t x,uint16_t y,uint16_t Width,uint16_t Height,uint16_t *pic)
{  
	uint32_t i,total;
	LCD_Address_Set(x,y,x+Width-1,y+Height-1);
	total=Height*Width;
 	for(i=0;i<total;i++)
		LCD_WR_DATA(pic[i]);//д������
}  

//ͨ��ת����ʾ
void LCDChannelTransformShow(CVPIX_RENDER myRender, int startx, int starty, int width)
{
    int i, endx, endy, j,k,my;
    float deta = (width - 1) / 255.0f;
    int32 num, calnum,channelnum=1;
    uint8* showHis, * thisshowHis, * RenderTrTable,* thisRenderTrTable;
    uint8* myPic=0;
    RenderTrTable = myRender->PixTrTable;

    if (myRender->ChannelNum == PIXChannelNum3)
    {
        channelnum = 3;
    }
    else if (myRender->ChannelNum == PIXChannelNum1)
    {
        channelnum = 1;
    }

    showHis = (uint8*)LCD_Malloc(channelnum * width * sizeof(uint8));//malloc 0 
		if(showHis == NULL)
		{
			LCD_DebugPrintf("�ڴ治�㻺�洴��ʧ��");
			while(1){};
		}
		
    for (int32 k = 0; k < channelnum; k++)
    {
        thisshowHis = &showHis[k * width];
        thisRenderTrTable = &RenderTrTable[k * 256];
        j = 0;
        for (i = 0; i < width; i++)
        {
            if (j < (i * 255) / (width - 1)) j++;
            calnum = thisRenderTrTable[j];
            num = 1;//num
            while (j < (i * 255) / (width - 1))
            {
                j++;
                num++;
                calnum += thisRenderTrTable[j];
            }

            calnum /= num;//ƽ��һ��
            thisshowHis[i] = (uint8)((calnum > 255) ? 255u : ((calnum < 0) ? 0u : calnum));
        }
    }
    //���뻺�廭��
    myPic = (uint8*)LCD_Malloc(width* width* channelnum * sizeof(uint8));//malloc 1 
		if(myPic == NULL)
		{
			LCD_DebugPrintf("�ڴ治�㻭�����洴��ʧ��");
			while(1){};
		}
    memset(myPic, 0, width * width * channelnum * sizeof(uint8));//�������

    //��Ӿ��ο�
    for (k = 0; k < channelnum; k++)
    {
        endx = (width - 1)* channelnum;
        endy = channelnum * width;
        my = (width - 1) * channelnum * width;
        for (i = 0; i < width; i++)
        {
            //��ɫ
            myPic[i * endy + 0 * channelnum + k] = (uint8)50;//��ֱ��
            myPic[i * endy + endx + k] = (uint8)50;
            myPic[0 * endy + i * channelnum + k] = (uint8)50;//ˮƽ��
            myPic[my + i * channelnum + k] = (uint8)50;//ˮƽ��
        }
    }

    //�������,�����ο�
    for ( k = 0; k < channelnum; k++)
    {
        thisshowHis = &showHis[k * width];//��������ĳ��ͨ��
        for (i = 0; i < width; i++)
        {
            my = (thisshowHis[i] * deta);
            myPic[(width - 1 - my) * channelnum * width + i * channelnum + k] = (uint8)255;
           //endx = (thisshowHis[i+1] * deta);
           //line(startx + i, starty - endy, startx + i+1, starty - endx);
        }
    }
    //��ʾ����
    if (myRender->ChannelNum == PIXChannelNum3)
    {
        LCD_Fill_RgbRect(startx, starty - width, width, width,(color24_t*) myPic);
    }
    else if (myRender->ChannelNum == PIXChannelNum1)
    {
        LCD_Fill_GrayRect(startx, starty - width, width, width, (_color8_t*)myPic);
    }
    //setcolor(DARKGRAY);//GRAY
		uint16  pcl_old = POINT_COLOR;
		POINT_COLOR = BROWN;
		
    endx = startx + width;
    endy = starty - width;

    //line(startx, starty, endx, starty);//��ˮƽ��
    //line(startx, endy, endx, endy);
    //line(startx, starty, startx, endy);//��ֱ��
    //line(endx, starty, endx, endy);

    //����
		POINT_COLOR = RED;//setcolor(RED);
    LCD_ShowString(startx - 8, starty - 8, "0");
    LCD_ShowString(startx - 8, endy - 8, "255");
    LCD_ShowString(endx - 8, starty - 8, "255");

    LCD_Free(myPic);
    LCD_Free(showHis);//�ͷ������ڴ�
		//��ԭ
		POINT_COLOR = pcl_old;
}

//ֱ��ͼ��ʾ
void LCDHistShow(int32* myHis, PIXChannelNum channelNum, int startx, int starty, int width)
{
    int i, endx, endy, j, k,maxH, my;
    float deta = (width - 1) / 255.0, detakh;
    int32 num, calnum, channelnum = 1,* thisHis;
    uint8* showHis, * thisshowHis;
    uint8* myPic = 0;

    if (channelNum == PIXChannelNum3)
    {
        channelnum = 3;
    }
    else if (channelNum == PIXChannelNum1)
    {
        channelnum = 1; 
    }
    showHis = (uint8*)LCD_Malloc(width * channelnum * sizeof(uint8));//malloc 0 
    if(showHis == NULL)
		{
			LCD_DebugPrintf("�ڴ治�㻺�洴��ʧ��");
			while(1){};
		}
		
    for (int32 k = 0; k < channelnum; k++)
    {
         //���з���
        thisshowHis = &showHis[k * width];
        thisHis = &myHis[k * 256]; 

        //�ҵ����ֵ
        maxH = 0;
        for (i = 0, j = 256; i < j; i++)
        {
            if (maxH< thisHis[i])
                maxH = thisHis[i];
        }
        detakh = 255.0 / maxH;

        j = 0;
        for (i = 0; i < width; i++)
        {
            if (j < (i * 255) / (width - 1)) j++;
            calnum = (thisHis[j] * detakh);
            num = 1;//num
            while (j < (i * 255) / (width - 1))
            {
                j++;
                num++;
                calnum += (thisHis[j] * detakh);
            }
            calnum /= num;//ƽ��һ��
            thisshowHis[i] = (uint8)((calnum > 255) ? 255u : ((calnum < 0) ? 0u : calnum));
        }
    }
    //���뻺�廭��
    myPic = (uint8*)LCD_Malloc(width * width * channelnum * sizeof(uint8));//malloc 1 
		if(myPic == NULL)
		{
			LCD_DebugPrintf("�ڴ治�㻭�����洴��ʧ��");
			while(1){};
		}
		
    memset(myPic, 0, width * width * channelnum * sizeof(uint8));//�������

    //��Ӿ��ο�
    for (k = 0; k < channelnum; k++)
    {
        endx = (width - 1) * channelnum;
        endy = channelnum * width;
        my = (width - 1) * channelnum * width;
        for (i = 0; i < width; i++)
        {
            //��ɫ
            myPic[i * endy + 0 * channelnum + k] = (uint8)50;//��ֱ��
            myPic[i * endy + endx + k] = (uint8)50;
            myPic[0 * endy + i * channelnum + k] = (uint8)50;//ˮƽ��
            myPic[my + i * channelnum + k] = (uint8)50;//ˮƽ��
        }
    }

    //�������,�����ο�
    for (k = 0; k < channelnum; k++)
    {
        thisshowHis = &showHis[k * width];//��������ĳ��ͨ��
        for (i = 0; i < width; i++)
        {
            my = (thisshowHis[i] * deta);
            for (j = 0; j <= my; j++)
            {
                myPic[(width - 1 - j) * channelnum * width + i * channelnum + k] = (uint8)255;
            }
        }
    }
    //��ʾ����
    if (channelNum == PIXChannelNum3)
    {
        LCD_Fill_RgbRect(startx, starty - width, width, width, (color24_t*)myPic);
    }
    else if (channelNum == PIXChannelNum1)
    {
        LCD_Fill_GrayRect(startx, starty - width, width, width, (_color8_t*)myPic);
    }
    //setcolor(DARKGRAY);//GRAY
		uint16  pcl_old = POINT_COLOR;
		POINT_COLOR = BROWN;
		
    endx = startx + width;
    endy = starty - width;

    //����
    POINT_COLOR = RED;//setcolor(RED);
		
    LCD_ShowString(startx - 8, starty - 8, "0");
    LCD_ShowString(startx - 8, endy - 8, "max");
    LCD_ShowString(endx - 8, starty - 8, "255");

    LCD_Free(myPic);
    LCD_Free(showHis);//�ͷ������ڴ�
		//��ԭ
		POINT_COLOR = pcl_old;
}

//HOGֱ��ͼ��ʾ
void LCDHogHistShow(CVVECTORS_F32 hogdescrible, int startx, int starty, int invers)
{
    int cell_size = 8;
    int lenmax = cell_size / 2;
    int sizediv2 = cell_size / 2;

    color24_t lstcolor = LCD_POINT_COLOR;//ԭʼ����ɫ
    color24_t pencolor;
    //����x,y����cell�ĸ���
    int xnum = hogdescrible->dim / (sizeof(CVhog_cell8x8) / sizeof(float32));
    int ynum = hogdescrible->dataNum;
    //����cell
    for (int y = 0; y < ynum; y++)
    {
        CVHOG_CELL8X8 cell_1 = (CVHOG_CELL8X8)&hogdescrible->datap[y * hogdescrible->dim];
        for (int x = 0; x < xnum; x++)
        {
            //�����ĵ�
            int cx = x * cell_size + sizediv2;
            int cy = y * cell_size + sizediv2;
            //9��ά��
            for (int hi = 0; hi < 9; hi++)
            {
                //���������ɫǿ��
                uint16 vcolor = cell_1[x].hist[hi]* 4 * 255;//2x2��block,ÿ��cell��ֵ����Ϊ0.25������ȡ��ɫmax����Ϊ0.75*255
                vcolor = CVLimitMaxMin(0, vcolor, 255);
                pencolor.R = vcolor;
                pencolor.G = vcolor;
                pencolor.B = vcolor;
                LCD_Point_Color_Set(pencolor);

                int x2 = lenmax * YMCV_Cos(hi * 20 * YMCV_Deg2Rad);
                int y2 = lenmax * YMCV_Sin(hi * 20 * YMCV_Deg2Rad);
                //  \ /
                //   *
                //  / \
               
                if (invers)
                {
                    LCD_Draw_Line(startx + cx + y2, starty + cy - x2, startx + cx - y2, starty + cy + x2);
                }
                else
                {
                    LCD_Draw_Line(startx + cx - x2, starty + cy - y2, startx + cx + x2, starty + cy + y2);
                }
            }
            
        }
    }
    //��ɫ��ԭ
    LCD_Point_Color_Set(lstcolor);
}

//LBPֱ��ͼ��ʾ
void LCDLbpHistShow(CV_FACE_LBP_DESC lbpdescrible, int startx, int starty)
{
    //cell��С
    int cell_size = 16;
    int lenmax = cell_size / 2;
    int sizediv2 = cell_size / 2;

    color24_t lstcolor = LCD_POINT_COLOR;//ԭʼ����ɫ
    color24_t pencolor;
    //x,y����cell�ĸ���,cell��С��16*16����
    int xnum =7;
    int ynum = 7;
    //����cell
    for (int y = 0; y < ynum; y++)
    {
        uint8* cell_1 = (uint8*)&lbpdescrible->hist[y * 7 * 59];
        for (int x = 0; x < xnum; x++)
        {
            //�����ĵ�
            int cx = x * cell_size + sizediv2;
            int cy = y * cell_size + sizediv2;
            //59��ά��,ÿ3�����ݺϳ�20��
            for (int hi = 0; hi < 18; hi++)
            {
                //���������ɫǿ��
                uint16 vcolor = (cell_1[x * 7 + 3 * hi + 0] + cell_1[x * 7 + 3 * hi + 1] + cell_1[x * 7 + 3 * hi + 2])*4;//ÿ��ͨ������Ϊ4.3��3��ͨ���ϲ���Ϊ13����������52
                vcolor = CVLimitMaxMin(0, vcolor, 255);
                pencolor.R = vcolor;
                pencolor.G = vcolor;
                pencolor.B = vcolor;
                LCD_Point_Color_Set(pencolor);

                int x2 = lenmax * YMCV_Cos(hi * 20 * YMCV_Deg2Rad);
                int y2 = lenmax * YMCV_Sin(hi * 20 * YMCV_Deg2Rad);
                //  \ /
                //-- * --

                LCD_Draw_Line(startx + cx, starty + cy, startx + cx + x2, starty + cy + y2);
            }
        }
    }
    //��ɫ��ԭ
    LCD_Point_Color_Set(lstcolor);
}

//ͶӰֱ��ͼ��ʾ
void LCDProjHistShow(CVVECTORS_U16 projhist, uint8 axis_0x_1y, int startx, int starty)
{
    color24_t lstcolor = LCD_POINT_COLOR;//ԭʼ����ɫ
    color24_t pencolor = { 255,255,255 };
    LCD_Point_Color_Set(pencolor);
    //x��ͶӰ
    if (axis_0x_1y == 0)
    {
        for (int i = 0; i < projhist->dataNum; i++)
        {
            LCD_Draw_Line(startx + i, starty, startx + i, starty + projhist->datap[i]);//����
        }
    }
    //y��ͶӰ
    else
    {
        for (int i = 0; i < projhist->dataNum; i++)
        {
            LCD_Draw_Line(startx, starty + i, startx + projhist->datap[i], starty + i);//����
        }
    }
    //��ɫ��ԭ
    LCD_Point_Color_Set(lstcolor);
}

//�������������
void LCD_FaceFeaturePoint_Show(CVpoint fupoint[40], int startx, int starty)
{
    color24_t lstcolor = LCD_POINT_COLOR;//ԭʼ����ɫ
    color24_t pencolor = { 0,255, 0 };//��ɫ
    LCD_Point_Color_Set(pencolor);
    pencolor.R = 255;//��ɫ
    pencolor.G = 0;
		
		//����������
		// 1 - 2 - 3 - 4     5 - 6 - 7 - 8 - 9 - 10 - 5
		// 11 - 12 - 13 - 14     15 - 16 - 17 - 18 - 19 - 20 - 15
		// 6 - 10  7 - 9     16 - 20  17 - 19
		// 21 - 22 - 23 - 24 - 25 - 26
		// 27 - 28 - 29 - 30 - 31 -32 - 27
		// 27 - 36 - 35 - 30   27 - 33 - 34 - 30
		// 37 - 38 - 39 - 37  39 - 40

		const static uint8 lineto[] = {
  			1,2, 2,3, 3,4, 5,6, 6,7, 7,8, 8,9, 9,10, 10,5,\
				11, 12, 12, 13, 13, 14,       15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 15, \
				6, 10, 7, 9,                  16, 20, 17, 19, \
				21, 22, 22, 23, 23, 24, 24, 25, 25, 26, \
				27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 27, \
				27, 36, 36, 35, 35, 30,   27, 33, 33, 34, 34, 30, \
				37, 38, 38, 39, 39, 37, 39, 40 };

		//�����߶�
		for (int pli = 0; pli < sizeof(lineto) / 2; pli++)
		{
				int id0 = lineto[2 * pli + 0] - 1;
				int id1 = lineto[2 * pli + 1] - 1;
				LCD_Draw_Line(startx + fupoint[id0].x, starty + fupoint[id0].y, startx + fupoint[id1].x, starty + fupoint[id1].y);
		}

    //���Ƶ�
    for (int pi = 0; pi < 40; pi++)//40��������
    {
        //����������
        LCD_Fill_ConstRgbRect(startx + fupoint[pi].x - 1, starty + fupoint[pi].y - 1, 3, 3, pencolor);
    }
    //��ɫ��ԭ
    LCD_Point_Color_Set(lstcolor);
}

