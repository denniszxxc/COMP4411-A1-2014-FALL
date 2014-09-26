

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
	start = target;

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

	Point t = target;
	glBegin(GL_LINES);
	SetColor(source);
	int width = pDoc->getWidth();
	width = width/4+1;
	for (int i = 0; i < width; i++){


		int size = pDoc->getSize();
		size *= 2;
		double angle = 0;
		if (pDoc->getDirectionControlType() == DIR_CONTROL_SLIDER) angle = pi*pDoc->getAngle() / 180;

		if (pDoc->getDirectionControlType() == DIR_CONTROL_GRADIENT){
			double xL, yR, xR, yL;
			int	max = 0;

			for (int i = 1; i <= 360; i++){
				xR = target.x + (size / 2)*cos(i*pi / 180);
				yR = target.y + (size / 2)*sin(i*pi / 180);
				xL = target.x - (size / 2)*cos(i*pi / 180);
				yL = target.y - (size / 2)*sin(i*pi / 180);

				GLubyte colorR[3];
				memcpy(colorR, pDoc->GetOriginalPixel(Point(xR, yR)), 3);
				GLubyte colorL[3];
				memcpy(colorL, pDoc->GetOriginalPixel(Point(xL, yL)), 3);
				int intensityR = colorR[0] + colorR[1] + colorR[2];
				int intensityL = colorL[0] + colorL[1] + colorL[2];
				int diff = intensityR - intensityL;
				if (diff < 0)diff *= -1;
				if (diff > max){
					max = diff;
					angle = i;
				}

			}

			angle = angle*pi / 180 + pi / 2;
		}

		if (pDoc->getDirectionControlType() == DIR_CONTROL_BRUSH_DIRECTION){
			Point current = target;
			angle = atan2(((double)current.y - start.y), ((double)current.x - start.x));
			cout << angle << endl;
			if (angle < 0)angle += 2 * 3.14159265;
			start = current;
		}
		double newLX, newLY, newRX, newRY;
		if ((angle >= pi / 4 && angle < 3 * pi / 4) || (angle >= 5 * pi / 4 && angle < 7 * pi / 4)){
			  newLX = target.x+i + (size / 2)*cos(angle);
			 newLY = target.y + (size / 2)*sin(angle);
			 newRX = target.x+i - (size / 2)*cos(angle);
			newRY = target.y - (size / 2)*sin(angle);
		}
		else{
			  newLX = target.x  + (size / 2)*cos(angle);
			  newLY = target.y + i + (size / 2)*sin(angle);
			 newRX = target.x  - (size / 2)*cos(angle);
			 newRY = target.y + i - (size / 2)*sin(angle);
		}
		


		glVertex2d(newLX, newLY);
		glVertex2d(newRX, newRY);


	}
	
	glEnd();
	
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

