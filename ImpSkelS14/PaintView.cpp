//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <cmath>
#include <iostream>
using namespace std;


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;
static Point startingPT;
static Point finishingPT;
static double userAngle = 0;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		
		// This is the event handler
		switch (eventToDo)
		{
		case LEFT_MOUSE_DOWN:
			SaveCurrentContent();
			RestoreContent();
			if (coord.x > m_nStartCol && coord.x < m_nEndCol
				&& coord.y>m_nStartRow && coord.y < m_nEndRow){
				m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
			}
			break;
		case LEFT_MOUSE_DRAG:
			SaveCurrentContent();
			if (coord.x > m_nStartCol && coord.x < m_nEndCol
				&& coord.y>m_nStartRow && coord.y < m_nEndRow){
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
			}
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd(source, target);

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
		{
			SaveCurrentContent();
			RestoreContent();
			startingPT = target;
			glBegin(GL_POINTS);
			glColor3f(1, 0, 0);

			glVertex2d(startingPT.x, startingPT.y);

			glEnd();
			break;
		}

		case RIGHT_MOUSE_DRAG:
			RestoreContent();
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);

			glVertex2d(startingPT.x, startingPT.y);
			glVertex2d(target.x, target.y);
			glEnd();
			break;
		case RIGHT_MOUSE_UP:

			RestoreContent();
			finishingPT = target;
			userAngle = atan2(((double)finishingPT.y - startingPT.y), ((double)finishingPT.x - startingPT.x));
			if (userAngle < 0)userAngle += 2 * 3.14159265;
			userAngle = userAngle *57.2957795;
			m_pDoc->m_pUI->setAngle(userAngle);

			break;

		default:
			printf("Unknown event!!\n");
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch (event)
	{
	case FL_ENTER:
		redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DOWN;
		else
			eventToDo = LEFT_MOUSE_DOWN;
		isAnEvent = 1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo = RIGHT_MOUSE_DRAG;
		else
			eventToDo = LEFT_MOUSE_DRAG;
		isAnEvent = 1;
	
		m_pDoc->red_dot(coord.x, coord.y);
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		m_pDoc->red_dot(coord.x , coord.y);
		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::paintAll(int space) {
	isAnEvent = true;

	// generate a list of distint points to paint
	int x_size = m_nDrawWidth / space;
	int y_size = m_nDrawHeight / space; 
	std::vector<Point> random_arr_point ;
	random_arr_point.resize(x_size*y_size);

	for (int i = 0; i < x_size; i ++){
		for (int j = 0; j < y_size; j++){
			Point temp;
			temp.x = i * space;
			temp.y = j * space;
			random_arr_point[i * y_size + j] = temp;
		}
	}

	//swap the list of points randomly
	srand(time(NULL));
	for (int i = 0; i < x_size*y_size; ++i) {
		int j = rand() %((x_size*y_size)-i); // Pick random number from 0 <= r < n-i.  Pick favorite method
		// j == 0 means don't swap, otherwise swap with the element j away
		if (j != 0) {
			Point tempPoint = random_arr_point[i];
			random_arr_point[i] = random_arr_point[i + j];
			random_arr_point[i + j] = tempPoint;
		}
	}

	for (int k = 0; k < x_size*y_size; k++){
		int i = random_arr_point[k].x;
		int j = random_arr_point[k].y;
		Point source(i + m_nStartCol, m_nEndRow - j);
		Point target(i, m_nWindowHeight - j);
		m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
	}
	refresh();
	isAnEvent = true;
}
