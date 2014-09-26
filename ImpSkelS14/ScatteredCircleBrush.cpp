//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredcirclebrush.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <cmath>
#define pi 3.14159265
extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();




	glPointSize((float)1);

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int rand_num = rand() % 4 + 2;
	/* initialize random seed: */
	srand(time(NULL));

	Point tempS, tempT;
	tempS = source;
	tempT = target;

	for (int i = 0; i < rand_num; i++){
		int rand_x = rand() % (size*2) - size;
		int rand_y = rand() % (size * 2) - size ;

		tempS.x = source.x + rand_x;
		tempS.y = source.y + rand_y;
		tempT.x = target.x + rand_x;
		tempT.y = target.y + rand_y;

		glBegin(GL_LINE_LOOP);
		SetColor(tempS);
		/*for (int j = 0; j < 720; j++){
			double k = j / 2;
			glVertex2d(tempT.x + (size/2)*cos(k / 180 * pi), tempT.y + (size/2)*sin((k / 180 * pi)));
		}
		glBegin(GL_LINE_STRIP);
		SetColor(tempS);

		for (int j = 0; j < 720; j++){
			double k = j / 2;
			glVertex2d(tempT.x + (size / 2)*cos(k / 180 * pi), tempT.y + (size / 2)*sin((k / 180 * pi)));
			glVertex2d(tempT.x, tempT.y);
		}*/
		glBegin(GL_TRIANGLES);
		SetColor(tempS);

		for (double j = 1; j < 360;){
			glVertex2d(tempT.x, tempT.y);
			glVertex2d(tempT.x + size*cos(j / 180 * pi), tempT.y + size*sin((j / 180 * pi)));
			glVertex2d(tempT.x + size*cos(++j / 180 * pi), tempT.y + size*sin(j / 180 * pi));
		}
		glEnd();
	}
	
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

