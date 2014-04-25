#include "Example.h"											/**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>	 
#include <gl\glut.h>
#include <gl\glext.h>
#include <math.h>                                 
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							


/** 定义光源的属性值 */
GLfloat LightAmbient[]= { 0.0f, 0.0f, 1.0f, 1.0f }; 	/**< 环境光参数 */
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };		/**< 漫射光参数 */
GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };	/**< 镜面光参数 */
GLfloat LightPosition[]= { 2.0f, 2.0f, -8.0f, 1.0f };	/**< 光源位置 */

GLfloat globel_ambient[] = { 1.0 , 1.0 , 1.0 , 1.0 };

HGLRC hRC = NULL;   //永久着色描述表
HDC hDC = NULL;  //私有GDI设备描述表
HWND hWnd = NULL;   //保存我们的窗口句柄
HINSTANCE hInstance;  //保存程序的实例

bool keys[256];   //用于键盘例程的数组
bool active = true;     //窗口的活动标志，缺省为true
bool fullscreen = false;

float star[2000][3];


/** 初始化OpenGL */
bool Example::Init()									
{
	/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	//gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -100.0, 0.0, 1.0, 0.0);
	ResizeDraw(true);											/**< 改变OpenGL窗口大小，直接调用子类的函数 */
	/** 载入纹理 */
	if( !LoadTexture() )
		MessageBox(NULL,"载入纹理错误!","错误",MB_OK);

	/** 设置光源的属性值 */
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);				/**< 设置环境光 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);				/**< 设置漫射光 */
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);			/**< 设置漫射光 */
	glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);			/**< 设置光源位置 */
	GLfloat light_l[]={1.0,1.0,1.0,1.0};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_l);//漫反射
	glLightfv(GL_LIGHT0,GL_SPECULAR,LightPosition);
	glLightfv(GL_LIGHT0,GL_POSITION,light_l);
	/** 启用光照和光源 */
	//	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);     

	return true;           /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Example::Uninit()									
{
	Texture.FreeImage();              /** 释放纹理图像占用的内存 */
	glDeleteTextures(1, &Texture.ID); /**< 删除纹理对象 */
}

/** 程序更新函数 */
void Example::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}

	///if (m_Keys.IsPressed(VK_F1) == true)						/**< 按F1切换窗口/全屏模式 */
	///{
	///	ToggleFullscreen();										
	///}
	/** 用户自定义的更新过程 */
	rot += milliseconds/20.0;
	day += milliseconds/20.0;
	year += milliseconds/30.0;

}

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Example * example = new Example(class_name);
	return reinterpret_cast<GLApplication *>(example);
}

/** 构造函数 */
Example::Example(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	rot = 0.0;
	day = 0.0;
	year = 0.0;
	ellx = 0.0;
	elly = 0.0;
	ellz = 0.0;
	deltay = 0.0;
	earthy = 1.0;
	starscalex = 0.0;
	starscaley = 0.0;
	starscalez = 0.0;

}

void Example::AStar()  
{  

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(1.0,1.0,1.0);
	glVertex3f(1.0,1.0,-7.0);
	glVertex3f(1.0,-1.0,-7.0);
	glVertex3f(-1.0,-1.0,-7.0);
	glVertex3f(-1.0,1.0,-7.0);
	glEnd();
	glPopMatrix();

}  

void Example::DrawStars()  
{  
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		
		GLUquadricObj *quadPlanetstar = gluNewQuadric();  //创建一个二次曲面物体
		gluQuadricTexture(quadPlanetstar,GL_TRUE); //启动该二次曲面的纹理
		glBindTexture(GL_TEXTURE_2D, 4); //绑定纹理
			glTranslatef(2.0,2.0,-8.0);
			//glScalef(0.5,1.0,1.0);
			glRotatef(-70,1.0f,0.0f,0.0f);
			glRotatef(-10,0.0,0.0,1.0);
			gluSphere(quadPlanetstar, 0.03 ,20,16);//绘制一个球体
		gluDeleteQuadric(quadPlanetstar);  //删除二次曲面对象

	glPopMatrix();

}  

void Example::DrawSun(float sr)
{
	glPushMatrix();
		glTranslatef(-1.0f, 0.0f, -7.0f);
		glRotatef(150,1.0f,0.0f,0.0f);
		glRotatef(15,0.0f,1.0f,0.0f);

		//Sun
		glPushMatrix();
			glRotatef((-1)*day,0.0f,1.0f,1.0f);
			glEnable(GL_TEXTURE_2D);
			GLUquadricObj *quadPlanetsun = gluNewQuadric();  //创建一个二次曲面物体
			gluQuadricTexture(quadPlanetsun,GL_TRUE); //启动该二次曲面的纹理
			glBindTexture(GL_TEXTURE_2D, 1); //绑定纹理
			gluSphere(quadPlanetsun, sr ,20,16);//绘制一个球体
			gluDeleteQuadric(quadPlanetsun);  //删除二次曲面对象

		glPopMatrix();
	
		//Earth
		glPushMatrix();

			glRotatef((-1)*day,0.0f,0.8f,1.0f);	
			glTranslatef(1.5f, 0.0f,0.0f);
			glRotatef((-1)*day,0.0f,0.8f,1.0f);
			GLUquadricObj *quadPlanetearth = gluNewQuadric();  //创建一个二次曲面物体
			gluQuadricTexture(quadPlanetearth, 1); //启动该二次曲面的纹理
			glBindTexture(GL_TEXTURE_2D, 2); //绑定纹理
			gluSphere(quadPlanetearth, 0.25 ,20,16);//绘制一个球体
			gluDeleteQuadric(quadPlanetearth);  //删除二次曲面对象
		//	glDisable(GL_TEXTURE_2D);


			//Moon
			glPushMatrix();
				glRotatef((-1)*day,0.0f,0.8f,1.0f);	
				glTranslatef(0.45f, 0.0f,0.0f);
				glRotatef((-1)*day,0.0f,0.8f,1.0f);

				GLUquadricObj *quadPlanetmoon = gluNewQuadric();  //创建一个二次曲面物体
				gluQuadricTexture(quadPlanetmoon, 1); //启动该二次曲面的纹理
				glBindTexture(GL_TEXTURE_2D, 3); //绑定纹理
				gluSphere(quadPlanetmoon, 0.12 ,20,16);//绘制一个球体
				gluDeleteQuadric(quadPlanetmoon);  //删除二次曲面对象
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();


		glPopMatrix();

		//glPopMatrix();
	glPopMatrix();
}

/** 装载纹理 */
bool Example::LoadTexture()
{
	glEnable(GL_TEXTURE_2D);                          /**< 启用纹理映射 */

	//sun贴图
	if(!Texture.LoadBitmap("image_sun.bmp"))              /**< 载入位图文件 */
	{
		MessageBox(NULL,"装载位图文件01失败！","错误",MB_OK);  /**< 如果载入失败则弹出对话框 */
		return false;
	}
	glGenTextures(1, &Texture.ID);        /**< 生成一个纹理对象名称 */
	glBindTexture(GL_TEXTURE_2D, Texture.ID); /**< 创建纹理对象 */
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, Texture.imageWidth,
		Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		Texture.image);


	//earth贴图

	if(!Texture.LoadBitmap("image_earth.bmp"))              /**< 载入位图文件 */
	{
		MessageBox(NULL,"装载位图文件01失败！","错误",MB_OK);  /**< 如果载入失败则弹出对话框 */
		return false;
	}
	glGenTextures(1, &Texture.ID);        /**< 生成一个纹理对象名称 */
	glBindTexture(GL_TEXTURE_2D, Texture.ID); /**< 创建纹理对象 */
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    /** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, Texture.imageWidth,
	                  Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
	                  Texture.image);


	//moon贴图

	if(!Texture.LoadBitmap("image_moon.bmp"))              /**< 载入位图文件 */
	{
		MessageBox(NULL,"装载位图文件03失败！","错误",MB_OK);  /**< 如果载入失败则弹出对话框 */
		return false;
	}
	glGenTextures(1, &Texture.ID);        /**< 生成一个纹理对象名称 */
	glBindTexture(GL_TEXTURE_2D, Texture.ID); /**< 创建纹理对象 */
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, Texture.imageWidth,
		Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		Texture.image);

	//star
	if(!Texture.LoadBitmap("image_star3.bmp"))              /**< 载入位图文件 */
	{
		MessageBox(NULL,"装载位图文件03失败！","错误",MB_OK);  /**< 如果载入失败则弹出对话框 */
		return false;
	}
	//glTexEnvi(GL_POINT_SPRITE,GL_COORD_REPKACE,GL_TRUE);
	glGenTextures(1, &Texture.ID);        /**< 生成一个纹理对象名称 */
	glBindTexture(GL_TEXTURE_2D, Texture.ID); /**< 创建纹理对象 */
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, Texture.imageWidth,
		Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
		Texture.image);
	
	glDisable(GL_TEXTURE_2D);
	return true;
}

/** 绘制函数// */
void Example::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 /**< 清楚缓存 */		
	glLoadIdentity();	

    /** 绘制内部不透明的木箱 */	
	//glPushMatrix();
    // glTranslatef(0.0f, 0.0f, -5.0f);
	//  glRotatef(rot,1.0f,1.0f,0.0f);
    // DrawBox(0.5);
	//glPopMatrix();

 //   /** 绘制外面透明的木箱 */
	//glPushMatrix();
	//  glTranslatef(0.0f, 0.0f, -5.0f);
	//  glRotatef(rot,1.0f,1.0f,0.0f);
	//  glEnable(GL_BLEND);	               /**< 打开混合 */
	//  glDisable(GL_DEPTH_TEST);            /**< 关闭深度测试 */
	//  glColor4f(1.0f,1.0f,1.0f,0.5f);	   /**< 全亮度， 50% Alpha 混合 */
	//  glBlendFunc(GL_SRC_ALPHA,GL_ONE);	   /**< 基于源象素alpha通道值的半透明混合函数 */
 //     DrawBox(1.0);	
	//  glEnable(GL_DEPTH_TEST);             /**< 启用深度测试 */
	//  glDisable(GL_BLEND);		           /**< 关闭混合 */
	// glPopMatrix();

	DrawSun(0.5);
	DrawStars();







   glFlush();				    /**< 强制执行所有的OpenGL命令 */
}
