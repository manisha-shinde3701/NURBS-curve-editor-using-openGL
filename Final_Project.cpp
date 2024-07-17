
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <crtdbg.h>
#include <GL/freeglut.h>
#include <iostream>


	/*key function



	2		Bez2 B-Spline curve

	3		Bez3 B-Spline curve

	4		Bez4 B-Spline curve

	c/C		clear Control Points

	u/U		Toggle Uniform or Non-Uniform B-Spline mode

	f/F		Toggle clamped mode on first-end

	l/L		Toggle clamped mode on last-end

	b/B		if B-Spline is in closed loop then break it at its original ends

	k/K		reset knots array

	h/H		show BezPts[] of all Bez3 segments in Non-Uniform mode

	v/V		if B-Spline is in Non-Uniform mode then verify it by a double draw of B-Spline with OpenGL command

	s/S		saveData(): CtrlPts/RingPts, OpenNuts/RingNuts & BezPts to <dataLog.txt> file.

	ESC		quit
*/

 
class BitAry
{
	std::vector<unsigned int>  bits;
	int	_max;

public:
	BitAry(int m = 0)
	{
		bits.resize(m / 32 + 1);
		_max = m;
	}

	void SetBit(int k)
	{
		if (k <= _max)
			bits[k / 32] |= (1 << (k % 32));
	}

	void ClearBit(int k)
	{
		if (k <= _max)
			bits[k / 32] &= ~(1 << (k % 32));
	}

	bool ChkBit(int k)
	{
		if (k <= _max) {
			unsigned int  kk = k % 32, bit = (bits[k / 32] & (1 << kk));
			return (1 == bit >> kk);
		}
	}

	void SetAll()
	{
		if (_max > 0) {
			int n = bits.size();
			memset(&bits[0], 0xFF, n * sizeof(int));
		}
	}

	void Resize(int m)
	{
		bits.clear();
		bits.resize(m / 32 + 1);
		_max = m;
	}
};

struct Vec2D
{
	double	x, y;
	Vec2D(double dx = 0.0, double dy = 0.0) { x = dx;		y = dy; }
	Vec2D operator-() { return Vec2D(-x, -y); }
	Vec2D operator+(Vec2D pt) { return Vec2D(x + pt.x, y + pt.y); }
	Vec2D operator-(Vec2D pt) { return Vec2D(x - pt.x, y - pt.y); }
	Vec2D operator*(double d) { return Vec2D(x * d, y * d); }
	Vec2D operator/(double d) { return Vec2D(x / d, y / d); }
	bool  operator==(Vec2D pt) { return (FLT_EPSILON >= abs(x - pt.x) && FLT_EPSILON >= abs(y - pt.y)); }
	bool  operator!=(Vec2D pt) { return (FLT_EPSILON < abs(x - pt.x) || FLT_EPSILON < abs(y - pt.y)); }
};


#define  NoteMsg(msg)   ::MessageBox(::GetActiveWindow(), msg, "Notice:",  MB_ICONINFORMATION)
enum BSpl_Const { QUAD = 2, CUBIC = 3, QUART = 4, };
#define		SEG			64
#define		KEY_ESC		27


 //window size
int		ww = 800, wh = 640;
int		NDEG;
int		pickRadius = 5;
int		lIdx = -1;					// used to remember the moving control index
bool	bUnif = true;				// set to use uniform B-Spline
bool	bRing = false;			// B-Spline is in Ring mode
bool	bClampF = true;				// set clamp at first end
bool	bClampL = true;				// set clamp at Last  end
bool	bVerify = false;			// draw a verification curve by gluNurbsCurve() command
bool	bHull = false;			// show poly-Bezier segments' control hull
char	txtBuf[64];
char	title[] = "NURBS", deg2[] = "Quadratic", deg3[] = "Cubic", deg4[] = "Quartic", typ0[] = "Uniform ", typ1[] = "Non-uniform ";

std::vector<Vec2D>	CtrlPts;		// core record of NURBS' control points
std::vector<Vec2D>	RingPts;		// RingPts[], control-points used by Ring-NURBS.
std::vector<double>	coreNuts;		// coreNuts is the base of Knots[]
std::vector<double>	OpenNuts;		// knots array used by Open-NURBS
std::vector<double>	RingNuts;		// knots array used by Ring-NURBS.
std::vector<Vec2D>	BezPts;			// this is the polyBez array used to draw NURBS.
std::vector<Vec2D>	tFL(2);			// records the cF(Cubic) or dF(Quartic) and bL of polyBez.

BitAry	Bits;						// records the validity of each curve-segment.

GLUnurbsObj* pNurb = NULL;

void clearData()
{
	bRing = false;
	CtrlPts.clear();
	coreNuts.clear();

}

void drawCtrlPoly(int n, Vec2D pt[])
{
	glLineWidth(1.0);
	glColor3f(0.85f, 0.85f, 0.98f);	// silver
	glPushAttrib(GL_ENABLE_BIT);
	glLineStipple(1, 0x00FF);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n; i++)
		glVertex2d(pt[i].x, pt[i].y);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glPopAttrib();
}

void drawPts(int n, Vec2D pt[])
{
	glBegin(GL_POINTS);
	for (int i = 0; i < n; i++)
		glVertex2d(pt[i].x, pt[i].y);
	glEnd();
}

void drawHullPts(int n, Vec2D pt[])
{
	glBegin(GL_POINTS);
	for (int i = 0; i < n; i++) {
		if (0 == i % NDEG) {
			glColor3f(0.66f, 0.66f, 0.66f);
			glPointSize(8.0);
		}	// gray
		else
		{
			glColor3f(0.8f, 0.5f, 0.2f);
			glPointSize(5.0);
		}	// gold
		glVertex2d(pt[i].x, pt[i].y);
	}
	glEnd();
}

void drawHullLine(int n, Vec2D pt[])
{
	glPushAttrib(GL_ENABLE_BIT);
	glLineWidth(1.0);
	glLineStipple(1, 0x0707);
	glEnable(GL_LINE_STIPPLE);			// dashed pattern

	for (int j = 0; j < n / NDEG; j++)
	{
		if (!Bits.ChkBit(j))		continue;
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= NDEG; i++)
			glVertex2d(pt[j * NDEG + i].x, pt[j * NDEG + i].y);
		glEnd();
	}
	glDisable(GL_LINE_STIPPLE);
	glPopAttrib();
}

void pickPt(int button, int state, int xPos, int yPos)
{
	double	 dx, dy, newX = xPos, newY = wh - yPos;		int  n = CtrlPts.size();

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			for (int i = 0; i < n; i++)
			{
				dx = CtrlPts[i].x - newX;
				dy = CtrlPts[i].y - newY;
				if (sqrt(dx * dx + dy * dy) <= pickRadius)
				{
					lIdx = i;
					break;
				}
			}
		}
	}

	if (state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (-1 == lIdx)
			{
				if (bRing)
					return;
				CtrlPts.push_back(Vec2D(newX, newY));
			}
			else if (-1 < lIdx)
			{
				CtrlPts[lIdx].x = newX;
				CtrlPts[lIdx].y = newY;
				if (bRing)
				{
					if (0 == lIdx)
						CtrlPts[n - 1] = CtrlPts[0];

					else if (n - 1 == lIdx)
						CtrlPts[0] = CtrlPts[n - 1];
					RingPts.clear();
				}
				lIdx = -1;
			}
		}
	}
	glutPostRedisplay();
}

void mouseMove(int xPos, int yPos)
{
	int  n = CtrlPts.size(), newX = xPos, newY = wh - yPos;
	double  dx, dy;

	if (lIdx > -1)
	{
		CtrlPts[lIdx].x = newX;
		CtrlPts[lIdx].y = newY;

		if (n > 3)
		{
			if (!bRing)
			{
				if (0 == lIdx)
				{
					dx = newX - CtrlPts[n - 1].x;	dy = newY - CtrlPts[n - 1].y;
					bRing = (sqrt(dx * dx + dy * dy) <= pickRadius) ? true : false;
					if (bRing)
					{
						CtrlPts[n - 1] = CtrlPts[0];
						RingPts.clear();
					}
				}
				else if (n - 1 == lIdx)
				{
					dx = newX - CtrlPts[0].x;
					dy = newY - CtrlPts[0].y;
					bRing = (sqrt(dx * dx + dy * dy) <= pickRadius) ? true : false;
					if (bRing)
					{
						CtrlPts[0] = CtrlPts[n - 1];
						RingPts.clear();
					}
				}
			}
			else
			{
				if (0 == lIdx)
					CtrlPts[n - 1] = CtrlPts[0];

				if (n - 1 == lIdx)
					CtrlPts[0] = CtrlPts[n - 1];
				RingPts.clear();
			}
		}
		glutPostRedisplay();
	}
}

void setKnots()
{
	int  n = CtrlPts.size(), ORD = NDEG + 1, nNuts = n + ORD, m = coreNuts.size();
	if (m != nNuts)
	{
		RingPts.clear();
		BezPts.clear();
	}
	coreNuts.resize(nNuts);
	if (bUnif)
	{
		for (int i = 0; i < nNuts; i++)
			coreNuts[i] = i;
	}
	else
	{
		srand(GetTickCount());
		if (0 == m)
		{
			coreNuts[0] = 0;
			for (int i = 1; i < nNuts; i++)
				coreNuts[i] = coreNuts[i - 1] + ((double)(rand() % 10));
		}
		else if (m < nNuts)
		{
			for (int i = m; i < nNuts; i++)
				coreNuts[i] = coreNuts[i - 1] + ((double)(rand() % 10));
		}
	}

	if (bRing)
	{
		m = NDEG - 1;
		nNuts += m;
		RingNuts.resize(nNuts);

		int  k = n + 1;
		memcpy(&RingNuts[0], &coreNuts[0], sizeof(double) * k);
		for (int i = k; i < nNuts; i++)
			RingNuts[i] = RingNuts[i - n + 1] - RingNuts[i - n] + RingNuts[i - 1];

		if (0 == RingPts.size())
		{
			k = n + m;
			RingPts.resize(k);
			memcpy(&RingPts[0], &CtrlPts[0], sizeof(Vec2D) * n);
			for (int i = n; i < k; i++)
				RingPts[i] = CtrlPts[i - n + 1];
		}
	}
	else
	{
		OpenNuts.resize(nNuts);
		memcpy(&OpenNuts[0], &coreNuts[0], sizeof(double) * nNuts);

		if (bClampF) {
			for (int i = NDEG; i > 0; i--)
				OpenNuts[i - 1] = OpenNuts[i];
		}

		if (bClampL) {
			for (int i = n; i < (n + NDEG); i++)
				OpenNuts[i + 1] = OpenNuts[i];
		}
	}
}

void verifyGL(int nPt, Vec2D pt[], double nut[])
{
	int  ORD = NDEG + 1, k = nPt + ORD;
	std::vector<float>  fPt(nPt * 3);
	std::vector<float>  fNut(k);

	for (int j = 0; j < nPt; ++j)
	{
		fPt[j * 3] = (float)pt[j].x;
		fPt[j * 3 + 1] = (float)pt[j].y;
		fPt[j * 3 + 2] = 0;
	}

	for (int j = 0; j < k; ++j)
		fNut[j] = (float)nut[j];

	glLineWidth(1);
	glColor3ub(255, 0, 255);	// purple B-Spline

	pNurb = gluNewNurbsRenderer();
	gluNurbsProperty(pNurb, GLU_SAMPLING_TOLERANCE, 15);
	gluBeginCurve(pNurb);
	gluNurbsCurve(pNurb, k, &fNut[0], 3, &fPt[0], ORD, GL_MAP1_VERTEX_3);
	gluEndCurve(pNurb);

	if (pNurb)
		gluDeleteNurbsRenderer(pNurb);
}

void drawBez2Cuv()
{
	Vec2D  A, B, pt;
	double  u;
	int n = BezPts.size();
	glLineWidth(1);
	for (int j = 0; j < n / QUAD; j++)
	{
		if (!Bits.ChkBit(j))continue;

		B = BezPts[j * 2 + 1] - BezPts[j * 2];
		A = BezPts[j * 2 + 2] - BezPts[j * 2 + 1] * 2 + BezPts[j * 2];
		glBegin(GL_LINE_STRIP);
		for (int k = 0; k <= SEG; k++)
		{
			u = ((double)k) / SEG;		pt = (A * u + B * 2) * u + BezPts[j * 2];
			glVertex2d(pt.x, pt.y);
		}
		glEnd();
	}
}

void drawBez3Cuv()
{
	Vec2D  A, B, C, pt;
	double  u;
	int n = BezPts.size();
	glLineWidth(1);

	for (int j = 0; j < n / CUBIC; j++)
	{
		if (!Bits.ChkBit(j))continue;

		A = BezPts[j * 3 + 3] - BezPts[j * 3] - (BezPts[j * 3 + 2] - BezPts[j * 3 + 1]) * 3;
		C = BezPts[j * 3 + 1] - BezPts[j * 3];
		B = BezPts[j * 3 + 2] - BezPts[j * 3 + 1] * 2 + BezPts[j * 3];
		glBegin(GL_LINE_STRIP);
		for (int k = 0; k <= SEG; k++)
		{
			u = ((double)k) / SEG;
			pt = (A * u * u + (B * u + C) * 3) * u + BezPts[j * 3];
			glVertex2d(pt.x, pt.y);
		}
		glEnd();
	}
}

void drawBez4Cuv()
{
	Vec2D  A, B, C, D, pt;
	double  u, uu;
	int n = BezPts.size();
	glLineWidth(1);

	for (int j = 0; j < n / QUART; j++)
	{
		if (!Bits.ChkBit(j))
			continue;

		A = BezPts[j * 4 + 4] + BezPts[j * 4] + BezPts[j * 4 + 2] * 6 - (BezPts[j * 4 + 3] + BezPts[j * 4 + 1]) * 4;

		B = BezPts[j * 4 + 3] - BezPts[j * 4] - (BezPts[j * 4 + 2] - BezPts[j * 4 + 1]) * 3;

		D = BezPts[j * 4 + 1] - BezPts[j * 4];		C = BezPts[j * 4 + 2] - BezPts[j * 4 + 1] * 2 + BezPts[j * 4];

		glBegin(GL_LINE_STRIP);

		for (int k = 0; k <= SEG; k++)
		{
			u = ((double)k) / SEG;
			uu = u * u;
			pt = ((A * uu + C * 6) * u + (B * uu + D) * 4) * u + BezPts[j * 4];

			glVertex2d(pt.x, pt.y);
		}
		glEnd();
	}
}

void setPolyBez2(int n, double nut[], Vec2D pt[])
{
	_ASSERTE(n > QUAD);
	double  d, t;
	int  mx = n - 2, m = mx * 2 + 3, k = m - 2;
	std::vector<Vec2D>  tv(m);
	Bits.Resize(mx);
	Bits.SetAll();
	tv[0] = pt[0];
	tv[m - 1] = pt[n - 1];

	for (int i = 0; i < mx; i++)
	{
		tv[i * 2 + 2] = pt[i + 1];
		d = nut[i + 3] - nut[i + 1];
		if (0 != d)
		{
			t = (nut[i + 2] - nut[i + 1]) / d;
			tv[i * 2 + 1] = (pt[i + 1] - pt[i]) * t + pt[i];
		}

		else
		{
			Bits.ClearBit(i);
			if (0 < i)
				Bits.ClearBit(i - 1);
		}
	}

	d = nut[mx + 3] - nut[mx + 1];
	if (0 != d)
	{
		t = (nut[mx + 2] - nut[mx + 1]) / d;
		tv[k] = (pt[n - 1] - pt[n - 2]) * t + pt[n - 2];
	}
	else
		Bits.ClearBit(mx - 1);
	BezPts.resize(k);
	memcpy(&BezPts[0], &tv[1], sizeof(Vec2D) * k);
}

void setPolyBez3(int n, double nut[], Vec2D pt[])
{
	if (!n > CUBIC)return;

	Vec2D cF, bL;
	int  mx = n - 3, m = mx * 3 + 3, k = m - 2;
	std::vector<Vec2D> tv(m);

	double  t, d = nut[4] - nut[1];
	Bits.Resize(mx);
	Bits.SetAll();

	if (0 != d)
	{
		t = (nut[4] - nut[3]) / d;
		tv[0] = pt[0] * t + pt[1] * (1.0 - t);
	}


	for (int i = 0; i < mx; i++)
	{
		if (0 != (d = nut[i + 5] - nut[i + 2]))
		{

			t = (nut[i + 5] - nut[i + 3]) / d;
			tv[i * 3 + 2] = pt[i + 1] * t + pt[i + 2] * (1.0 - t);
			t = (nut[i + 5] - nut[i + 4]) / d;
			tv[i * 3 + 3] = pt[i + 1] * t + pt[i + 2] * (1.0 - t);

			if (0 == (d = nut[i + 4] - nut[i + 2]))
				Bits.ClearBit(i);
			else
			{
				t = (nut[i + 3] - nut[i + 2]) / d;
				tv[i * 3 + 1] = tv[i * 3 + 2] * t + tv[i * 3] * (1.0 - t);
			}
		}
		else
			Bits.ClearBit(i);

		if ((!Bits.ChkBit(i)) && (0 < i))
			Bits.ClearBit(i - 1);

	}

	if (0 != (d = nut[mx + 5] - nut[mx + 2]))
	{
		t = (nut[mx + 5] - nut[mx + 3]) / d;
		tv[m - 1] = pt[n - 2] * t + pt[n - 1] * (1.0 - t);

		if (0 == (d = nut[mx + 4] - nut[mx + 2]))
			Bits.ClearBit(mx - 1);

		else
		{
			t = (nut[mx + 3] - nut[mx + 2]) / d;
			tv[k] = tv[k + 1] * t + tv[k - 1] * (1.0 - t);
		}
	}
	else
		Bits.ClearBit(mx - 1);

	BezPts.resize(k);
	memcpy(&BezPts[0], &tv[1], sizeof(Vec2D) * k);
	tFL[0] = tv[0];
	tFL[1] = tv[m - 1];
}

void setPolyBez4(int n, double nut[], Vec2D pt[])
{

	_ASSERTE(QUART < n || (3 < n && bRing));

	Vec2D  cF, bL, tp0, tp1;
	int  k, mx = n - 4, m = mx * 4 + 3;
	double  d0, d1, d2, n0, n1, n2, t0, t1, t2, t3;
	std::vector<Vec2D>  tv(m);

	Bits.Resize(mx);
	Bits.SetAll();

	d0 = nut[5] - nut[1];
	d1 = nut[6] - nut[2];
	d2 = nut[5] - nut[2];

	if (0 != d2)
	{
		n0 = nut[5] - nut[4];
		n1 = nut[4] - nut[2];
		t0 = n0 / d0;
		t1 = n1 / d1;
		t2 = n0 / d2;
		tv[0] = (pt[0] * t0 + pt[1] * (1.0 - t0)) * t2 + (pt[2] * t1 + pt[1] * (1.0 - t1)) * (1.0 - t2);
	}

	for (int i = 0; i < mx; i++)
	{
		d0 = nut[i + 6] - nut[i + 2];
		d1 = nut[i + 7] - nut[i + 3];
		d2 = nut[i + 6] - nut[i + 3];

		if (0 == d2)
			Bits.ClearBit(i);
		else {

			n0 = nut[i + 4] - nut[i + 3];
			n1 = nut[i + 6] - nut[i + 4];
			n2 = nut[i + 5] - nut[i + 3];

			t0 = n1 / d0;
			t1 = n0 / d1;
			t2 = n0 / d2;
			t3 = n2 / d1;

			tp0 = pt[i + 1] * t0 + pt[i + 2] * (1.0 - t0);
			tp1 = pt[i + 3] * t3 + pt[i + 2] * (1.0 - t3);

			tv[i * 4 + 2] = tp0 * (1.0 - t2) + (pt[i + 3] * t1 + pt[i + 2] * (1.0 - t1)) * t2;
			n0 = nut[i + 5] - nut[i + 4];
			t1 = n0 / d2;
			t0 = (nut[i + 5] - nut[i + 2]) / d0;
			tv[i * 4 + 3] = tp0 * (1.0 - t1 - t2) + tp1 * t2 + pt[i + 2] * t1;
			t2 = n2 / d2;
			tv[i * 4 + 4] = (pt[i + 1] * (1.0 - t0) + pt[i + 2] * t0) * (1.0 - t2) + tp1 * t2;

			if (0 != n2) {
				t0 = n0 / n2;
				tv[i * 4 + 1] = tv[i * 4] * t0 + tv[i * 4 + 2] * (1.0 - t0);
			}

			else
				Bits.ClearBit(i);
		}
		if ((!Bits.ChkBit(i)) && (0 < i))
			Bits.ClearBit(i - 1);
	}

	d1 = nut[mx + 7] - nut[mx + 3];
	d2 = nut[mx + 6] - nut[mx + 3];
	k = m - 2;

	if (0 != d2)
	{
		t1 = n0 / d1;
		t2 = n0 / d2;
		tv[m - 1] = tp1 * (1 - t2) + (pt[n - 1] * t1 + pt[n - 2] * (1.0 - t1)) * t2;

		if (0 != n1)
		{
			t1 = n0 / n1;
			tv[k] = tv[k - 1] * (1.0 - t1) + tv[k + 1] * t1;
		}
		else
			Bits.ClearBit(mx - 1);
	}
	else
		Bits.ClearBit(mx - 1);
	BezPts.resize(k);
	memcpy(&BezPts[0], &tv[1], sizeof(Vec2D) * k);
	tFL[0] = tv[0];
	tFL[1] = tv[m - 1];
}

void saveData()
{
	FILE* fp;
	char buf[8192], file[] = "C:/Users/Manis/Desktop/project.txt.txt", nL[] = "---------------- New Log ----------------\n", * p;

	_getcwd(buf, _MAX_PATH);

	_ASSERTE(NULL != buf);

	strcat_s(buf, file);

	int  k, m, n, L, o;
	Vec2D* pt;
	double* dk;

	n = ((bRing) ? RingPts.size() : CtrlPts.size());

	if ((NDEG == QUAD && 3 > n) || (NDEG == CUBIC && 4 > n) || (NDEG == QUART && 5 > n))
	{
		std::cout << "Number of Control points are too few to draw a Curve - Segment." << std::endl;
	}

	if (bRing)
	{
		pt = &RingPts[0];
		dk = &RingNuts[0];
		k = RingNuts.size();
	}
	else
	{
		pt = &CtrlPts[0];
		dk = &OpenNuts[0];
		k = OpenNuts.size();
	}

	if (0 != (fopen_s(&fp, buf, "a+")))
		return;

	fwrite(nL, sizeof(nL), 1, fp);

	if (NDEG == QUAD)
		p = deg2;
	else if (NDEG == CUBIC)
		p = deg3;
	else
		p = deg4;

	m = sprintf_s(buf, 8192, "\t%s%s B-Spline; \tRing-mode: %s\n", ((bUnif) ? typ0 : typ1), p, ((bRing) ? "ON" : "OFF"));
	fwrite(buf, m, 1, fp);

	// list control points
	m = sprintf_s(buf, 8192, "Control Points: %d\n", n);
	L = 5;
	o = n / L;
	for (int i = 0; i < o; i++)
	{
		for (int j = 0; j < L; ++j)
		{
			m += sprintf_s(buf + m, 8192 - m, " (%.02f, %.02f); ", pt[i * L + j].x, pt[i * L + j].y);
		}
		m += sprintf_s(buf + m, 8192 - m, "\n");
	}

	o *= L;
	L = n % L;

	if (0 < L)
	{
		for (int j = 0; j < L; ++j)
		{
			m += sprintf_s(buf + m, 8192 - m, " (%.02f, %.02f); ", pt[o + j].x, pt[o + j].y);
		}
		m += sprintf_s(buf + m, 8192 - m, "\n");
	}
	fwrite(buf, m, 1, fp);

	// list knots array
	m = sprintf_s(buf, 8192, "Knots array: %d\n", k);
	L = 10;
	o = k / L;
	for (int i = 0; i < o; i++)
	{
		m += sprintf_s(buf + m, 8192 - m, " { ");
		for (int j = 0; j < L; ++j)
		{
			m += sprintf_s(buf + m, 8192 - m, "%.04f, ", dk[i * L + j]);
		}
		m += sprintf_s(buf + m, 8192 - m, "}\n");
	}

	o *= L;
	L = k % L;

	if (0 < L)
	{
		m += sprintf_s(buf + m, 8192 - m, " { ");
		for (int j = 0; j < L; ++j)
		{
			m += sprintf_s(buf + m, 8192 - m, "%.04f, ", dk[o + j]);
		}
		m += sprintf_s(buf + m, 8192 - m, "}\n");
	}
	fwrite(buf, m, 1, fp);

	// list BezPts & Bits redords
	m = sprintf_s(buf, 8192, "Control Points of Poly-Bezier Curve:\n");
	k = BezPts.size();
	o = k / NDEG;
	pt = &BezPts[0];

	for (int i = 0; i < o; i++)
	{
		m += sprintf_s(buf + m, 8192 - m, " %s\t", ((Bits.ChkBit(i)) ? "VALID" : "NULL"));

		for (int j = 0; j < NDEG; ++j)
		{
			m += sprintf_s(buf + m, 8192 - m, " (%.15f, %.15f); ", pt[i * NDEG + j].x, pt[i * NDEG + j].y);
		}
		m += sprintf_s(buf + m, 8192 - m, "\n");
	}

	m += sprintf_s(buf + m, 8192 - m, "\t (%.15f, %.15f); \n\n", pt[k - 1].x, pt[k - 1].y);

	fwrite(buf, m, 1, fp);
	fclose(fp);
	std::cout << "Data have been saved.  " << std::endl;	
	//NoteMsg("Data have been saved.  ");

}

void drawNURBS()
{
	int  n = CtrlPts.size(), m = n - 4;
	Vec2D* pt;
	double* pNut;

	if (0 == n)
		return;

	glPointSize(4.0);
	glColor3f(1.0f, 0, 1.0f);	// purple
	drawPts(n, &CtrlPts[0]);
	drawCtrlPoly(n, &CtrlPts[0]);

	if (bRing || (QUAD < n && NDEG == QUAD) || (CUBIC < n && NDEG == CUBIC) || (QUART < n && NDEG == QUART))
		setKnots();
	else
		return;

	// set BezPts[]
	if (bRing)
	{
		pt = &RingPts[0];
		pNut = &RingNuts[0];
		n = RingPts.size();
	}
	else
	{
		pt = &CtrlPts[0];
		pNut = &OpenNuts[0];
	}
	if (NDEG == QUAD)
		setPolyBez2(n, pNut, pt);
	else if (NDEG == CUBIC)
		setPolyBez3(n, pNut, pt);
	else if (NDEG == QUART)
		setPolyBez4(n, pNut, pt);

	// draw B-Spline
	if (bUnif)
		glColor3f(0, 1.0f, 0);	// green uniform B-Spline
	else
		glColor3f(0, 0, 1.0f);	// blue non-uniform B-Spline
	if (NDEG == QUAD)
		drawBez2Cuv();
	else if (NDEG == CUBIC)
		drawBez3Cuv();
	else if (NDEG == QUART)
		drawBez4Cuv();

	// verify curve with OpenGL command 
	if (bVerify)
	{
		verifyGL(n, pt, pNut);			bVerify = false;
	}

	if (bHull)
	{
		drawHullPts(BezPts.size(), &BezPts[0]);
		if ((NDEG == CUBIC || NDEG == QUART) && (!bRing))
		{
			glColor3f(0, 0, 0);
			drawPts(2, &tFL[0]);
		}
		glColor3f(0, 0.8f, 0.8f);
		drawHullLine(BezPts.size(), &BezPts[0]);	// cyan
		bHull = false;
	}
}

void init()
{
	NDEG = CUBIC;
	bUnif = bClampF = bClampL = false;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	clearData();
}

void keyIn(unsigned char ch, int x, int y)

{

	int oldMode = NDEG;				bool bTxt = false;

	switch (ch) {

	case '2':
		NDEG = QUAD;
		if (oldMode == NDEG)
			return;
		else
		{
			coreNuts.clear();
			bTxt = true;
		}
		break;

	case '3':
		NDEG = CUBIC;
		if (oldMode == NDEG)
			return;
		else
		{
			coreNuts.clear();
			bTxt = true;
		}
		break;

	case '4':
		NDEG = QUART;
		if (oldMode == NDEG)
			return;
		else
		{
			coreNuts.clear();
			bTxt = true;
		}
		break;

	case 'u':
	case 'U':
		bUnif = !bUnif;
		RingPts.clear();
		bTxt = true;
		break;

	case 'f':
	case 'F':
		bClampF = !bClampF;
		break;

	case 'l':
	case 'L':
		bClampL = !bClampL;
		break;

	case 'b':
	case 'B':
		if (bRing)
		{
			CtrlPts[0].x -= 15;
			CtrlPts[0].y += 15;
			RingPts.clear();
			bRing = false;
		}
		else
			return;

		break;

	case 'k':
	case 'K':
		if (bUnif)
			return;
		else
			coreNuts.clear();
		break;

	case 'v':
	case 'V':
		if ((QUAD == NDEG && 2 < CtrlPts.size()) || (CUBIC == NDEG && 3 < CtrlPts.size()) || (QUART == NDEG && 4 < CtrlPts.size()))
			bVerify = true;
		else if (bRing && (QUART == NDEG && 3 < CtrlPts.size()))
			bVerify = true;
		else
			return;
		break;

	case 'h':
	case 'H':
		if ((QUAD == NDEG && 2 < CtrlPts.size()) || (CUBIC == NDEG && 3 < CtrlPts.size()))
			bHull = true;
		else if (QUART == NDEG)
		{
			if (bRing && 3 < CtrlPts.size())
				bHull = true;
			else if (4 < CtrlPts.size())
				bHull = true;
		}

		else
			return;
		break;

	case 'c':
	case 'C':
		clearData();
		break;

	case 's':
	case 'S':
		saveData();
		return;

	case KEY_ESC:	exit(EXIT_SUCCESS);
	}

	if (bTxt)
	{
		glutSetWindowTitle("NURBS Editior");
	}
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawNURBS();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	ww = w;
	wh = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
}

void listCommand()
{

	std::cout << "\nkey function:\n\n";
	std::cout << "\tmouse-click\tadd new control point\n";
	std::cout << "\tmouse-drag \tmove control point\n";
	std::cout << "\t2\tdraw Quadratic B-Spline curve\n";
	std::cout << "\t3\tdraw Cubic     B-Spline curve\n";
	std::cout << "\t4\tdraw Quartic   B-Spline curve\n";
	std::cout << "\tu or U\ttoggle Uniform or Non-Uniform B-Spline mode\n";
	std::cout << "\tf or F\ttoggle clamped curve on first-end\n";
	std::cout << "\tl or L\ttoggle clamped curve on last-end\n";
	std::cout << "\tb or B\tbreak ring-curve mode\n";
	std::cout << "\tk or K\treset knot array (Non-Uniform mode only)\n";
	std::cout << "\th or H\tshow poly-Bezier control points\n";
	std::cout << "\tv or V\tverify NURBS curve by OpenGL-command <gluNurbsCurve()>\n";
	std::cout << "\tc or C\tclear all control point(s)\n";
	std::cout << "\ts or S\tsave data to file <dataLog.txt>\n";
	std::cout << "\tESC \tquit\n\n";
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	listCommand();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(ww, wh);
	glutInitWindowPosition(400, 50);
	glutCreateWindow(" NURBS Editior ");
	glutDisplayFunc(display);
	glutMouseFunc(pickPt);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyIn);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
} 