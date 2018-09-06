
// MFCSkyBoxView.h : CMFCSkyBoxView 类的接口
//
#include "MainFrm.h"

#include "Terrain.h"
#include "Sky.h"
#include "Plane.h"
#include "House.h"
#include "water.h"

#include <GL\glaux.h>
#include <GL\glut.h>

#pragma once
#define PI  3.1415926

class CMFCSkyBoxView : public CView
{
	CTerrain terrain;//地形对象
	CSky sky;//天空对象
	CPlane Plane;
	CHouse house;
	CWater water;
	GLfloat xtexa1, xtexa2, ytexa1, ytexa2;
	//与旋转模型相关变量
	CPoint mousePoint;
	//控制旋转和位置的变量
	GLfloat m_xAngle;//旋转图像角度
	GLfloat m_yAngle;
	float c = PI / 180;
	//显示列表相关变量
	int listTerrain;
	int listLight;
	int listGrid;//网格
	int listSky;
	int listPlane;
	int listHouse;
	int listWater;
	//测试时间
	
	bool moveLeft;//启动左移（按下a）
	bool moveForward;//是否启动前移（按下w）
	bool moveRight;//是否启动右移
	bool altDown;//alt按下，类似吃鸡功能
	float fowardSpeed;
	float l_r_Speed;//左移或右移速度
	float moveSky;//云朵在旋转
	int a;//水波动
protected: // 仅从序列化创建
	CMFCSkyBoxView();
	DECLARE_DYNCREATE(CMFCSkyBoxView)

// 特性
public:
	CMFCSkyBoxDoc* GetDocument() const;

// 操作
public:
	void RenderScene();
	int LightModle();
	void FogModle();
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCSkyBoxView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HGLRC m_hRC;    //Rendering Context着色描述表  
	CClientDC* m_pDC;        //Device Context设备描述表  
	int m_wide, m_height;    //窗口宽高


// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	// 初始化OpenGL
	BOOL InitializeOpenGL();
	// 设置像素格式
	BOOL SetupPixelFormat();



	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // MFCSkyBoxView.cpp 中的调试版本
inline CMFCSkyBoxDoc* CMFCSkyBoxView::GetDocument() const
   { return reinterpret_cast<CMFCSkyBoxDoc*>(m_pDocument); }
#endif

