

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"
#include <cmath>

#define pi 3.14159265
extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	

	glBegin(GL_LINE_LOOP);
	SetColor(source);

	for (int j = 0; j < 720; j++){
		double k = j / 2;
		glVertex2d(target.x + size*cos(k / 180 * pi), target.y + size*sin((k / 180 * pi) ));
	}
	glBegin(GL_LINE_STRIP);
	SetColor(source);

	for (int j = 0; j < 720; j++){
		double k = j / 2;
		glVertex2d(target.x + size*cos(k / 180 * pi), target.y + size*sin((k / 180 * pi)));
		glVertex2d(target.x, target.y );
	}


	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
