

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"
#include <cmath>
#include <iostream>

#define pi	3.14159265
using namespace std;
extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_LINES);
	SetColor(source);

	int size = pDoc->getSize();
	double angle = pi*pDoc->getAngle()/180;

	double  newLX = target.x+(size/2)*cos(angle);
	double newLY = target.y + (size / 2)*sin(angle);
	double newRX = target.x - (size / 2)*cos(angle);
	double  newRY = target.y - (size / 2)*sin(angle);
	
	
	glVertex2d(newLX, newLY);
	glVertex2d(newRX, newRY);

	int width = pDoc->getWidth();
	width /= 4;
	for (int i = 0; i < width; i++){
		glVertex2d(newLX, newLY + i);
		glVertex2d(newRX, newRY + i);

	}

	
	glEnd();
	
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

