#include<graphics.h>//1.4
#include<conio.h>
#include<iostream>
#include<time.h>
#include<windows.h>
using namespace std;
COLORREF  darkorange= COLORREF RGB(250,140,0);
COLORREF  blueviolet= COLORREF RGB(138,43,226 );
COLORREF Back_color=DARKGRAY;
int i,j;
DWORD Time_speed;
IMAGE img_gameover;
DWORD t1 = GetTickCount();
int score=0;
char main_s[26][10]={0};
class BLOCK
{
 public:
	WORD dir[4];	// 方块的四个旋转状态
	COLORREF color;	// 方块的颜色
};
BLOCK g_Blocks[7] = {	
	{0x0F00, 0x4444, 0x0F00, 0x4444, RED},		// I
	{0x0660, 0x0660, 0x0660, 0x0660, BLUE},		// 口
	{0x4460, 0x02E0, 0x0622, 0x0740, blueviolet},	// L
	{0x2260, 0x0E20, 0x0644, 0x0470, darkorange},	// 反L
	{0x0C60, 0x2640, 0x0C60, 0x2640, CYAN},		// Z
	{0x0360, 0x4620, 0x0360, 0x4620, GREEN},	// 反Z
	{0x4E00, 0x4C40, 0x0E40, 0x4640, BROWN}};	// T
	class BLOCKINFO
	{
	public:
		char id;	// 方块 ID
		char x, y;	// 方块在游戏区中的坐标
		WORD dire;	// 方向
	}	g_CurBlock, g_NextBlock;
	
	bool Checkdown(BLOCKINFO tmp);//检测方块是否可以落下
	void Checkend(BLOCKINFO tmp);//将下沉方块的坐标写入二维数组
	bool GameOver(BLOCKINFO tmp);//显示game over
	void click();   //获取按键
	void Draw_next();//画出预览区的方块
	void Draw_next2();//擦除预览区的方块
	int draw_rectangle(int a,int b,COLORREF x,COLORREF y);//画出单元方格
	int draw_rectangle2(int l,int t,int r,int b,COLORREF x,COLORREF y);//画出方格
	void DrawBlock(BLOCKINFO a,char b_states);//画出28种方块
	void initblock();//随机生成一个俄罗斯方块
	void ON_SHIFT();//逆时针变换方块的方向
	void ON_SINK();              //方块下沉，并消除
	void ON_LEFT();//方块的左移
	void ON_RIGHT();//方块的右移
	void ON_DOWN();//方块的下移
	void level();//根据等级设置固定下来的方块的颜色
	void ON_ESC();//关闭图形界面
	void Time_down();//根据等级设置下降速度
	int initwindows();//初始化界面和一个俄罗斯方块
	int draw_rectangle(int a,int b,COLORREF x,COLORREF y)
	{
		setcolor(x);
		setfillcolor(y);
		fillrectangle (155+(b-1)*20, 75+(a-1)*20, 155+b*20,75+a*20);
		return 0;
	}
	int draw_rectangle2(int l,int t,int r,int b,COLORREF x,COLORREF y)
	{
		setcolor(x);
		setfillcolor(y);
		fillrectangle (l, t, r,b);
		return 0;
	}
	// 画方块
	void DrawBlock(BLOCKINFO a,char b_states)
	{
		WORD b = g_Blocks[a.id].dir[a.dire];
		int x, y;
		for(int i = 0; i < 16; i++, b <<= 1)
			if (b&0x8000)
			{
				x = a.x + i % 4;
				y = a.y - i / 4;
				//	if (y < 26)
				switch(b_states)
				{
				case 1:	draw_rectangle(x, y,WHITE ,g_Blocks[a.id].color);break;
				case 2:	draw_rectangle(x, y,0 ,0);break;
				case 3:	draw_rectangle(x, y,WHITE ,Back_color);break;
					//	case 4:draw_rectangle2( l,t,int r,int b,COLORREF x,COLORREF y);break;
				}
			}
	}
	void initblock()
	{
		srand(time(NULL));
		Draw_next2();
		g_CurBlock.id=g_NextBlock.id;g_NextBlock.id=rand() % 7;
		g_CurBlock.dire=g_NextBlock.dire;g_NextBlock.dire=rand()%4;
		g_CurBlock.x=1;
		g_CurBlock.y=7;
		DrawBlock(g_CurBlock,1);
		Draw_next();
	}

	void Draw_next()
	{
		WORD b = g_Blocks[g_NextBlock.id].dir[g_NextBlock.dire];
		int x, y;
		for(int i = 0; i < 16; i++, b <<= 1)
			if (b&0x8000)
			{
				x = g_NextBlock.x + i % 4;
				y = g_NextBlock.y - i / 4;
				
				draw_rectangle(x,y+16,WHITE ,g_Blocks[g_NextBlock.id].color);
			}
	}
	void Draw_next2()
	{
		WORD b = g_Blocks[g_NextBlock.id].dir[g_NextBlock.dire];
		int x, y;
		for(int i = 0; i < 16; i++, b <<= 1)
			if (b&0x8000)
			{
				x = g_NextBlock.x + i % 4;
				y = g_NextBlock.y - i / 4;
				
				draw_rectangle(x,y+16,0,0);
			}
	}
	void ON_SHIFT()
	{	
		BLOCKINFO tmp = g_CurBlock;
		tmp.dire=(tmp.dire+1)%4;
		if (Checkdown(tmp))
		{
			DrawBlock(g_CurBlock, 2);
			g_CurBlock.dire=(g_CurBlock.dire+1)%4;
			DrawBlock(g_CurBlock,1);
		}
	}
	void ON_LEFT()
	{
		BLOCKINFO tmp = g_CurBlock;
		tmp.y--;
		if (Checkdown(tmp))
		{
			DrawBlock(g_CurBlock, 2);
			g_CurBlock.y--;
			DrawBlock(g_CurBlock,1);
		}
	}
	void ON_RIGHT()
	{
		BLOCKINFO tmp = g_CurBlock;
		tmp.y++;
		if (Checkdown(tmp))
		{
			DrawBlock(g_CurBlock, 2);
			g_CurBlock.y++;
			DrawBlock(g_CurBlock,1);
		}
	}/**/
	void ON_DOWN()
	{
		BLOCKINFO tmp = g_CurBlock;
		tmp.x++;
		if (Checkdown(tmp))
		{
			DrawBlock(g_CurBlock, 2);
			g_CurBlock.x++;
			DrawBlock(g_CurBlock,1);
		}
		else {
			ON_SINK();
			
		}
	}
	void level()
	{
		if(score>=0&&score<=20) Back_color=DARKGRAY;
		if(score>=20&&score<=40) Back_color=RED;
		if(score>=40&&score<=60) Back_color=BLUE;
		if(score>=60&&score<=80) Back_color=blueviolet;
		if(score>=80&&score<=100) Back_color=darkorange;
		if(score>=100&&score<=120) Back_color=CYAN;
		if(score>=120&&score<=140) Back_color=GREEN;
		if(score>=140&&score<=160) Back_color=BROWN;
	}
	void ON_ESC()
	{
		closegraph();
		exit(0);
	}
	void ON_SINK()
	{
		DrawBlock(g_CurBlock, 2);
		BLOCKINFO tmp = g_CurBlock;
		tmp.x++;
		while (Checkdown(tmp))
		{
			g_CurBlock.x++;
			tmp.x++;
		}
		DrawBlock(g_CurBlock, 3);
		Checkend(g_CurBlock);
		char x,y;
		x=g_CurBlock.x;
		y=g_CurBlock.y;
		IMAGE img_tu;
		for(i=0;i<4;i++)
		{int count=0,k;
		for(j=1;j<11;j++)
			if(main_s[x+i][j]==1)  
				count++;
			if(count==10)     //消除操作
			{
				getimage(&img_tu,155,75,200,(x+i-1)*20);
				for(k=0;k<x+i;k++)
					for(j=1;j<11;j++)
					{
						main_s[x+i-k][j]=main_s[x+i-k-1][j];
					}
					for(j=1;j<11;j++) main_s[0][j]=0;
					putimage(155,95,&img_tu);
					score++;
					settextstyle(27,0,_T("宋体"), 3.0, 3.0, 14,1,0 ,0 );
					char s[5];
					sprintf(s, "%d",score);
					outtextxy(41,41,s);	
			}
		}
		
		if(GameOver(g_CurBlock))
			initblock();//setbkmode(TRANSPARENT);	
		//setcolor(0xAA00AA);
		
	}
	bool Checkdown(BLOCKINFO tmp)
	{
		WORD b = g_Blocks[tmp.id].dir[tmp.dire];
		int x, y;
		for(int i = 0; i < 16; i++, b <<= 1)
			if (b & 0x8000)
			{
				x = tmp.x + i % 4;
				y = tmp.y - i / 4;
				if ((x < 1) || (x >= 27) || (y < 1)||(y>10))
					return false;
				if ( (main_s[x][y]==1)&& (x > 1))
					return false;
			}
			return true;
	}
	bool GameOver(BLOCKINFO tmp)
	{
		WORD b = g_Blocks[tmp.id].dir[tmp.dire];
		for(i = 0; i < 16; i++, b <<= 1)
			if (b & 0x8000)
			{
				if (g_CurBlock.x+ i % 4 <=4)
				{
					putimage(0,0,&img_gameover);
					return false;
				}
				else
					return true;
			}
	}
	void Checkend(BLOCKINFO tmp)
	{
		WORD b = g_Blocks[tmp.id].dir[tmp.dire];
		for(i = 0; i < 16; i++, b <<= 1)
			if (b & 0x8000)
			{
				main_s[tmp.x + i % 4][tmp.y - i / 4] = 1;
			}
	}
	
	/**/
	void Time_down()
	{	
		if(score>=0&&score<=20) Time_speed= 1000;
		if(score>=20&&score<=40) Time_speed= 600;
		if(score>=40&&score<=60) Time_speed= 500;
		if(score>=60&&score<=80) Time_speed= 400;
		if(score>=80&&score<=100) Time_speed= 300;
		if(score>=100&&score<=120)Time_speed= 250;
		if(score>=120&&score<=140) Time_speed= 200;
		if(score>=140&&score<=160) Time_speed= 100;
		DWORD t2 = GetTickCount();
		if(t2-t1>=Time_speed)
		{
			t1=t2;
			ON_DOWN();
		}
		
	}
	void click()
	{
		if(kbhit())
		{
			switch(getch())
			{
			case 72:    ON_SHIFT();break;//上
			case 75:	ON_LEFT();break; //left
			case 77:	ON_RIGHT();break;//right
			case 80:	ON_DOWN();break;//down
			case 27:	 ON_ESC();break;
			case ' ':	ON_SINK();break;//下沉
			}
		}
	}
	int initwindows()
	{
		
		initgraph(510,600);setcolor(BLUE);
		draw_rectangle2(150,70,360,600,CYAN,LIGHTBLUE);
		draw_rectangle2(155,75,355,595,0,0);
		draw_rectangle2(392,52,478,138,CYAN,LIGHTBLUE);
		draw_rectangle2(395,55,475,135,0,0);
		loadimage(&img_gameover,_T("./res/gameover.jpg"),510,620,false);	
		initblock();
		return 0;
	}
	
	int main()
	{
		initwindows(); 
		while(1)
		{
			level();
			Time_down();	
			settextstyle(27,0,_T("宋体"), 3.0, 3.0, 4,1,0 ,0 );
			char s[50];
			sprintf(s, "%d",score);
			outtextxy(41,41,s);	
			click();
			
			
		}
		getch();
		
		return 0;
	}
