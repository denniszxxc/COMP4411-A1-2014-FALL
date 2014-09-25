//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredpointbrush.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	//glPointSize((float)size);

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	int rand_num_x,rand_num_y;

	/* initialize random seed: */
	srand(time(NULL));

	


	glBegin(GL_POINTS);
	


	
	for (int i = 0; i < size*6; i++){
		/* generate secret number between 1 and 10: */
		rand_num_x = rand() % size + 1 -size;
		/* generate secret number between 1 and 10: */
		rand_num_y = rand() % size + 1 - size;
		
		SetColor(Point(source.x + rand_num_x, source.y + rand_num_y));

		glVertex2d(target.x + rand_num_x, target.y + rand_num_y);
	}
	

	glEnd();
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

