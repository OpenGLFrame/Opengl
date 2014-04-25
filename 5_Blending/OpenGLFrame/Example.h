#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__


#include "GLFrame.h"            /**< ���������Ŀ���� */
#include "CBMPLoader.h"         /**< ����λͼ������ */    


/** ��GL_Application������һ������ */
class Example : GLApplication								
{
public:
	bool	Init();										/**< ִ�����еĳ�ʼ������������ɹ���������true */
	void	Uninit();										/**< ִ�����е�ж�ع��� */
	void	Update(DWORD milliseconds);							/**< ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ */
	void	Draw();												/**< ִ�����еĻ��Ʋ��� */

	bool    LoadTexture();                                        /**< װ������ */
	void    DrawBox(float r);                                     /**< ����һ������ */

	void DrawSun(float sr);

	void DrawStars();
	void AStar();



private:
	friend class GLApplication;								/**< ����Ϊ����һ����Ԫ�࣬�����������������ʵ����������GL_Application * GL_Application::Create(const char * class_name) */
	Example(const char * class_name);						/**< ���캯�� */

	/** �û��Զ���ĳ������ */
	CBMPLoader  Texture;          /**< λͼ������Ķ��� */
	float rot;					  /**< ������ת���� */
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