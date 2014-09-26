//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"
#include "scatteredlinebrush.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <cmath>
#include <iostream>
using namespace std;

#define pi 3.14159265

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
	
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();


	glPointSize((float)1);
	start = target;
	BrushMove(source, target);


}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	/* initialize random seed: */
	srand(time(NULL));

	int rand_num = rand() % 4 + 3;
	
	Point tempS, tempT;
	bool checker = 0;
	int size = pDoc->getSize();
	size *= 2;
	double angle = 0;
	for (int i = 0; i < rand_num; i++){

		int rand_x = rand() % (size / 2 + 3) - (size / 2 + 3)/2;
		tempS.x = source.x + rand_x;
		tempT.x = target.x + rand_x;

		int rand_y = rand() % (size / 2 + 3) - (size / 2 + 3)/2;
		tempS.y = source.y + rand_y;
		tempT.y = target.y + rand_y;

		glBegin(GL_LINES);
		SetColor(tempS);

		
		
		if (pDoc->getDirectionControlType() == DIR_CONTROL_SLIDER) angle = pi*pDoc->getAngle() / 180;

		else if (pDoc->getDirectionControlType() == DIR_CONTROL_GRADIENT){
			double xL, yR, xR, yL;
			int	max = 0;
			for (int i = 1; i <= 360; i++){
				xR = tempT.x + (size / 2)*cos(i*pi / 180);
				yR = tempT.y + (size / 2)*sin(i*pi / 180);
				xL = tempT.x - (size / 2)*cos(i*pi / 180);
				yL = tempT.y - (size / 2)*sin(i*pi / 180);

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

		if (pDoc->getDirectionControlType() == DIR_CONTROL_BRUSH_DIRECTION && checker!=1){
			Point current = target;
			angle = atan2(((double)current.y - start.y), ((double)current.x - start.x));
			if (angle < 0)angle += 2 * 3.14159265;
			start = current;
			checker = 1;
		}
		double  newLX = tempT.x + (size / 2)*cos(angle);
		double newLY = tempT.y + (size / 2)*sin(angle);
		double newRX = tempT.x - (size / 2)*cos(angle);
		double  newRY = tempT.y - (size / 2)*sin(angle);


		glVertex2d(newLX, newLY);
		glVertex2d(newRX, newRY);

		int width = pDoc->getWidth();
		width /= 4;
		for (int i = 0; i < width; i++){
			if ((angle >= pi / 4 && angle < 3 * pi / 4) || (angle >= 5 * pi / 4 && angle < 7 * pi / 4)){
				glVertex2d(newLX + i, newLY);
				glVertex2d(newRX + i, newRY);
			}
			else{
				glVertex2d(newLX, newLY + i);
				glVertex2d(newRX , newRY + i);
			}
		}
	}
	glEnd();
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

