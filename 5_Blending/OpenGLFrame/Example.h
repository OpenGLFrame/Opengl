#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__


#include "GLFrame.h"            /**< 包含基本的框架类 */
#include "CBMPLoader.h"         /**< 包含位图载入类 */    


/** 从GL_Application派生出一个子类 */
class Example : GLApplication								
{
public:
	bool	Init();										/**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();										/**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);							/**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();												/**< 执行所有的绘制操作 */

	bool    LoadTexture();                                        /**< 装载纹理 */
	void    DrawBox(float r);                                     /**< 绘制一个盒子 */

	void DrawSun(float sr);

	void DrawStars();
	void AStar();



private:
	friend class GLApplication;								/**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	Example(const char * class_name);						/**< 构造函数 */

	/** 用户自定义的程序变量 */
	CBMPLoader  Texture;          /**< 位图载入类的对象 */
	float rot;					  /**< 用于旋转物体 */
	float day,year;
	float ellx;
	float elly;
	float ellz;
	float deltay;
	float earthy;
	float starscalex;
	float starscaley;
	float starscalez;

};

#endif	// __EXAMPLE_H__