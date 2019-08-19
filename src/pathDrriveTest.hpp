
//#include "Movement.hpp"
#include  <cmath>
#include "VectorAngle.hpp"
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include "carcontrol.h"

#define PI 3.14159265
#define POS_INFINITY 1e30
#define NEG_INFINITY -1e30

using namespace cv;
using namespace std;

int getPt(int n1, int n2, float perc)
{
	int diff = n2 - n1;

	return n1 + (diff * perc);
}
double getPt(double n1, double n2, float perc)
{
	double diff = n2 - n1;

	return n1 + (diff * perc);
}

void bezierPoints(Point start, Point end, Point control, vector<Point2f> &points_bezier, Mat &dst,bool checkMaxAngle = true) {

	Point p1;
	Point p2;
	Point p3;
	p1 = start;
	p2 = control;
	p3 = end;
	int xa = p1.x;
	int ya = p1.y;
	int xb = p2.x;
	int yb = p2.y;
	int xLoc = p1.x;
	int yLoc = p1.y;
	int xb1 = p1.x;
	int yb1 = p1.y;
	int xb2 = p1.x;
	int yb2 = p1.y;
	float max_ang = NEG_INFINITY;
	//vector<Point> points_bezier;
	
	float u = 1.0;
	do {

			max_ang = NEG_INFINITY;
			points_bezier.clear();
			int xa = p1.x;
			int ya = p1.y;
			int xb = p2.x;
			int yb = p2.y;
			int xLoc = p1.x;
			int yLoc = p1.y;

			for (float i = 0; i < 1; i += 0.05)
			{
				// The Green Line
				xa = getPt((double)p1.x,(double)( p1.x + (p2.x - p1.x)*u), i);
				ya = getPt((double)p1.y,(double) (p1.y + (p2.y - p1.y)*u), i);
				xb = getPt((double)(p3.x + (p2.x - p3.x)*u), (double)p3.x, i);
				yb = getPt((double)(p3.y + (p2.y - p3.y)*u), (double)p3.y, i);

				// The Black Dot
				xLoc = getPt(xa, xb, i);
				yLoc = getPt(ya, yb, i);
				
				printf("bezierPointNo%f X:%d Y:%d \n",i,xLoc,yLoc);
				//circle(dst, Point(x, y), 1, Scalar(255, 0, 0));
				//Point a = Point(xa, ya);
				//Point b = Point(xb, yb);
				Point2f cont = Point2f(xLoc, yLoc);
				points_bezier.push_back(cont);
				//cout << "angle " << quadraticBezierAngle(i, p1,p3, p2) *180/PI << endl;

			}
			
			for (int i = 0; i < points_bezier.size(); i++) {
				// int i1 = i, i2 = i + 1, i3 = i + 2;
				// if (i2 >= points_bezier.size()) {
				// 	i2 = i;
				// }
				// if (i3 >= points_bezier.size()) {
				// 	i3 = i;
				// }
				// Point a = points_bezier.at(i1);
				// Point b = points_bezier.at(i3);
				// Point cont = points_bezier.at(i2);
				// float angle = quadraticBezierAngle2(i, a, b, cont);
				// max_ang = max(angle, max_ang);
				// cout << "angle " << angle << endl;
			}

			cout << "max angle" << max_ang << endl;
		

		u = u - 0.2;
		
		cout << "max angle " << max_ang << endl;
	} while ((max_ang > 23.0) && (u >= 0.1) && checkMaxAngle );

}
void bezierControlPointsQuadToCubic(Point p1, Point p2,Point p3, float &cx1,float &cy1 , float &cx2,float &cy2) {
	
	cx1 = p1.x + (2.0 / 3.0) * (p2.x - p1.x);
	cy1 = p1.y + (2.0 / 3.0) * (p2.y - p1.y);
	cx2 = p3.x + (2.0 / 3.0) * (p2.x - p3.x);
	cy2 = p3.y + (2.0 / 3.0) * (p2.y - p3.y);
}
void calculateBezierControlPoints(Point start, Point end, vector<Point2f> &points_bezier,Point &control1, Point &control2, Mat &dst) {
	float xDiff = (float)end.x - (float)start.x;
	float x1 = (float)start.x + xDiff / 3.0;
	float x2 = (float)start.x + 2.0 * xDiff / 3.0;

	//int indexY1 = (1 / 3 * 10);
	//int indexY1 = (2 / 3 * 10);

	//float xDiff = p2.x - p1.x;
	//float yDiff = p2.y - p1.y;
	//float x1 = p1.x + xDiff / 3.0;
	//float x2 = p1.x + 2.0 * xDiff / 3.0;
	//float y1 = p1.y + yDiff / 3.0;
	//float y2 = p1.y + 2.0 * yDiff / 3.0;

	float y1 = (float)(points_bezier.at(6).y);
	float y2 = (float)(points_bezier.at(13).y);

	float f1 = 0.296296296296296296296; // (1-1/3)^3
	float f2 = 0.037037037037037037037; // (1-2/3)^3
	float f3 = 0.296296296296296296296; // (2/3)^3

	float b1 = y1 - (float)start.y * f1 - (float)end.y / 27.0;
	float b2 = y2 - (float)start.y * f2 - f3 * (float)end.y;

	float c1 = (-2.0 * b1 + b2) / -0.666666666666666666;
	float c2 = (b2 - 0.2222222222222 * c1) / 0.44444444444444444;


	Point p2 = Point(x1, c1);
	Point p3 = Point(x2, c2);
	control1 = p2;
	control2 = p3;
	//circle(dst, p2, 1, Scalar(0, 255, 0));
	//circle(dst, p3, 1, Scalar(0, 255, 0));

}
void calculateBezierControlPoints(Point start, Point end, vector<Point2f> &points_bezier,float &control1X,float &control1Y , float &control2X,float &control2Y , Mat &dst) {
	float xDiff = (float)end.x - (float)start.x;
	control1X = (float)start.x + xDiff / 3.0;
	control2X = (float)start.x + 2.0 * xDiff / 3.0;

	//int indexY1 = (1 / 3 * 10);
	//int indexY1 = (2 / 3 * 10);

	//float xDiff = p2.x - p1.x;
	//float yDiff = p2.y - p1.y;
	//float x1 = p1.x + xDiff / 3.0;
	//float x2 = p1.x + 2.0 * xDiff / 3.0;
	//float y1 = p1.y + yDiff / 3.0;
	//float y2 = p1.y + 2.0 * yDiff / 3.0;

	double y1 = static_cast< float >(points_bezier.at(6).y);
	double y2 = static_cast< float >(points_bezier.at(13).y);

	//printf("y1:%.2f y2: %.2f \n", y1, y2 );

	float f1 = 0.296296296296296296296; // (1-1/3)^3
	float f2 = 0.037037037037037037037; // (1-2/3)^3
	float f3 = 0.296296296296296296296; // (2/3)^3

	float b1 = y1 - (float)start.y * f1 - (float)end.y / 27.0;
	float b2 = y2 - (float)start.y * f2 - f3 * (float)end.y;

	//printf("b1:%.2f b2: %.2f \n", b1, b2 );

	control1Y = (-2.0 * b1 + b2) / -0.666666666666666666;
	control2Y = (b2 - 0.2222222222222 * control1Y) / 0.44444444444444444;

	//printf("control1Y:%f control2Y: %f \n", control1Y, control2Y );

	//std::cout << "c1x: " << control1X << " c1y: " << control1Y << " c2x: " << control2X << " c2y: " << control2Y << std::endl; 
	// Point p2 = Point(x1, c1);
	// Point p3 = Point(x2, c2);
	// control1 = p2;
	// control2 = p3;
	//circle(dst, p2, 1, Scalar(0, 255, 0));
	//circle(dst, p3, 1, Scalar(0, 255, 0));

}

void distanceAnglePointToVector(Point curentPoint, Point va, Point vb, float &distance,float &angleDirection,float &innerAngle) {
	Vector2D<double> p2(curentPoint.x, curentPoint.y);
	Vector2D<double> p1(va.x, va.y);
	Vector2D<double> p3(vb.x, vb.y);

	float rad = Vector2D<double>::angle(p1 - p3, p1 - p2);
	float direction = Vector2D<double>::cross(  p1 - p2, p1 - p3);
	float deg = rad * 180.0 / PI;

	//std::cout << "rad = " << rad << "\tdeg = " << deg << std::endl;
	float comp_theta = fmod(180.0 - deg, 180.0);
	distance = Vector2D<double>::distance(p2,p1);
	innerAngle = comp_theta;
	comp_theta = copysignf(comp_theta, direction);
	angleDirection = comp_theta;

}
void distanceAnglePointToVector(float curentPointX, float curentPointY, Point va, Point vb, float &distanceP0,float &angleDirection,float &innerAngle) {
	Vector2D<double> p2(curentPointX, curentPointY);
	Vector2D<double> p1(va.x, va.y);
	Vector2D<double> p3(vb.x, vb.y);

	float rad = Vector2D<double>::angle(p1 - p3, p1 - p2);
	float direction = Vector2D<double>::cross(  p1 - p2, p1 - p3);
	float deg = rad * 180.0 / PI;

	//std::cout << "rad = " << rad << "\tdeg = " << deg << std::endl;
	float comp_theta = fmod(180.0 - deg, 180.0);
	distanceP0 = Vector2D<double>::distance(p2,p1);

	innerAngle = comp_theta;
	comp_theta = copysignf(comp_theta, direction);
	angleDirection = comp_theta;

}
void distanceAnglePointToVector(float curentPointX, float curentPointY, Point va, Point vb, float &distanceP0,float &distanceP1,float &angleDirection,float &innerAngle) {
	Vector2D<double> p2(curentPointX, curentPointY);
	Vector2D<double> p1(va.x, va.y);
	Vector2D<double> p3(vb.x, vb.y);

	float rad = Vector2D<double>::angle(p1 - p3, p1 - p2);
	float direction = Vector2D<double>::cross(  p1 - p2, p1 - p3);
	float deg = rad * 180.0 / PI;

	//std::cout << "rad = " << rad << "\tdeg = " << deg << std::endl;
	float comp_theta = fmod(180.0 - deg, 180.0);
	distanceP0 = Vector2D<double>::distance(p2,p1);
	distanceP1 = Vector2D<double>::distance(p3,p2);
	innerAngle = comp_theta;
	comp_theta = copysignf(comp_theta, direction);
	angleDirection = comp_theta;

}


// float angleAndDirectionOfPoints2(float fx,float fy,float tx,float ty, float curentAngle,float &angleToPoint,float &distance) {
// 	//var w = x2 - x1;
// 	//var h = y2 - y1;

// 	//var atan = Math.Atan(h / w) / Math.PI * 180;
// 	//if (w < 0 && h <= 0)
// 	//	atan -= 180;
// 	//if (w < 0 && h > 0)
// 	//	atan += 180;
// 	//return (atan) % 360;
// 	float dy = ty - fy;
// 	float dx = tx - fx;
// 	distance = sqrt(dx*dx + dy*dy);
// 	float x = (atan(dy / dx) / PI * 180);

// 	if (dx < 0 && dy <= 0)
// 		x -= 180;
// 	if (dx < 0 && dy > 0)
// 		x += 180;
// 	x = fmod(x, 360.0);
// 	float sign = 0;
// 	float comp = copysign(sign, x);


// 	//angles above x must be negative from 0-180 counter clockwise
// 	//angle bellow x must be negative from 0-180 clockwise

// 	//quad IV 
// 	if (curentAngle <= 90.0 && curentAngle >= 0) {
// 		//printf("quad IV curent angle: %f x:%f \n",curentAngle,x);
// 		x = fmod(curentAngle - (x), 360.0);
// 		x = -x;
// 		if (abs(x) > 180) {
// 			x = abs(x) - 90;
// 			//x = -x;

// 		}

// 	}
// 	//quad II
// 	else if (curentAngle < -90.0) {
// 		//printf("quad II curent angle: %f x:%f \n",curentAngle,x);
// 		x = fmod(curentAngle - (x), 360.0);
// 		//printf("quad II after mod x:%f \n",curentAngle,x);
// 		x = -x;
// 		if (abs(x) > 180) {
// 			x = x - 360;
// 		}
// 		//printf("quad II final x:%f \n",x);
// 	}
// 	//quad III
// 	else if (curentAngle > 90) {
// 		//printf("quad III curent angle: %f x:%f \n",curentAngle,x);
// 		x = fmod(curentAngle - (x), 360.0);
		
// 		if (abs(x) > 180) {
// 			x = x - 360;
// 		}
// 		x = -x; // todo ivnerse sign
// 		x = fmod(x, 360.0);
// 	}
// 	//quad I
// 	else if (curentAngle < 0 && curentAngle >= -90) {
// 		//printf("quad I curent angle: %f x:%f \n",curentAngle,x);
// 		x = fmod(curentAngle - (x), 360.0);
// 		x = -x;
// 		if (x > 180) {
// 			x = x - 90;
// 			x = -x;
// 		}

// 	}


// 	angleToPoint = x;
// 	return x;

// }
float angleAndDirectionOfPoints2(float fx,float fy,float tx,float ty, float curentAngle,float &angleToPoint,float &distance) {
	//var w = x2 - x1;
	//var h = y2 - y1;

	//var atan = Math.Atan(h / w) / Math.PI * 180;
	//if (w < 0 && h <= 0)
	//	atan -= 180;
	//if (w < 0 && h > 0)
	//	atan += 180;
	//return (atan) % 360;
	float dy = ty - fy;
	float dx = tx - fx;
	distance = sqrt(dx*dx + dy*dy);
	float x = (atan(dy / dx) / PI * 180);

	if (dx < 0 && dy <= 0)
		x -= 180;
	if (dx < 0 && dy > 0)
		x += 180;
	x = fmod(x, 360.0);
	float sign = 0;
	float comp = copysign(sign, x);


	//angles above x must be negative from 0-180 counter clockwise
	//angle bellow x must be negative from 0-180 clockwise

	//quad IV 
	if (curentAngle <= 90.0 && curentAngle >= 0) {
		//printf("quad IV curent angle: %f x:%f \n",curentAngle,x);
		x = fmod(curentAngle - (x), 360.0);
		x = -x;
		if (abs(x) > 180) {
			x = abs(x) - 90;
			//x = -x;

		}

	}
	//quad II
	else if (curentAngle < -90.0) {
		//printf("quad II curent angle: %f x:%f \n",curentAngle,x);
		x = fmod(curentAngle - (x), 360.0);
		//printf("quad II after mod x:%f \n",curentAngle,x);
		x = -x;
		if (abs(x) > 180) {
			x = x - 360;
		}
		//printf("quad II final x:%f \n",x);
	}
	//quad III
	else if (curentAngle > 90) {
		//printf("quad III curent angle: %f x:%f \n",curentAngle,x);
		x = fmod(curentAngle - (x), 360.0);
		
		if (abs(x) > 180) {
			x = x - 360;
		}
		x = -x; // todo ivnerse sign
		x = fmod(x, 360.0);
	}
	//quad I
	else if (curentAngle < 0 && curentAngle >= -90) {
		//printf("quad I curent angle: %f x:%f \n",curentAngle,x);
		x = fmod(curentAngle - (x), 360.0);
		x = -x;
		if (x > 180) {
			x = x - 90;
			x = -x;
		}

	}


	angleToPoint = x;
	return x;

}
Point moveCurveStartingPoint(float fx,float fy,float directionOfCurve, float curentAngle, int dx = 5,int dy = 5){



	int dir = 1;
	//dx = 2*dx;
	//dy = 2*dy;
	if(directionOfCurve > 0){
		dir = -1; 
		dx = dx*2;
		dy = dy*2;
	}

		//quad IV 
	if (curentAngle <= 135.0 && curentAngle >= 45) {
		fy = fy + (dir * dy);
		fx = fx - (dir * dx);
		printf("quad IV \n");
	}
	//quad II
	else if (curentAngle < -45.0 && curentAngle > -135) {
		fy = fy - (dir * dy);
		fx = fx + (dir * dx);
		printf("quad II \n");
	}
	//quad III
	else if (curentAngle > 135 || curentAngle < -135) {
		fy = fy - (dir * dy);
		fx = fx + (dir * dx);
		printf("quad III \n");
	}
	//quad I
	else if (curentAngle < 45 && curentAngle >= -45) {
		
		fy = fy + (dir * dy);
		fx = fx - ( dx);
		printf("quad I \n");
	}

	return Point(fx,fy);
}

void moveCurveStartingPoint(Point &from,Point &to,float directionOfCurve,float curentAngle){
	

	int dir = 1;
	//dx = 2*dx;
	//dy = 2*dy;
	if(directionOfCurve > 0){
		dir = -1; 
		//dx = dx*2;
		//dy = dy*2;
	}

		//quad IV 
	if (curentAngle <= 135.0 && curentAngle >= 45) {
		from = Point(from.x ,from.y - 10);
		to = Point(to.x + 10,to.y);
		printf("quad IV \n");
	}
	// //quad II
	// else if (curentAngle < -45.0 && curentAngle > -135) {
	// 	fy = fy - (dir * dy);
	// 	fx = fx + (dir * dx);
	// 	printf("quad II \n");
	// }
	// //quad III
	// else if (curentAngle > 135 || curentAngle < -135) {
	// 	fy = fy - (dir * dy);
	// 	fx = fx + (dir * dx);
	// 	printf("quad III \n");
	// }
	//quad I
	else if (curentAngle < 45 && curentAngle >= -45) {
		
		from = Point(from.x - 10,from.y);
		to = Point(to.x,to.y + 10);
		printf("quad I \n");
	}

	//return Point(fx,fy);
	
}

bool pointInsideCircle(Point p, Point origin, float radius = 15) {
	float R = radius; //wheelbase is 26.5 cm 
	float dx = abs(p.x - origin.x);
	float dy = abs(p.y - origin.y);

	if (dx > R) {
		return false;
	}
	if (dy > R) {
		return false;
	}
	
	if (dx + dy <= R) {
		return true;
	}
	
	if (dx *dx + dy * dy <= R * R) {
		return true;
	}
	else {
		return false;
	}

}

float quadraticBezierAngle2(float u, Point anchor1, Point anchor2, Point control) {

	Vector2D<double> p2(anchor1.x, anchor1.y);
	Vector2D<double> p1(control.x, control.y);
	Vector2D<double> p3(anchor2.x, anchor2.y);

	double rad = Vector2D<double>::angle(p1 - p3, p1 - p2);
	double direction = Vector2D<double>::cross(p1 - p2, p1 - p3);
	double deg = rad * 180.0 / PI;
	float comp_theta = 180 - deg;
	//std::cout << "rad = " << rad << "\tdeg = " << deg << std::endl;
	//if ((180 - deg) != (180 - deg)) {
	//	return 0;
	//}
	return fmod(comp_theta, 180.0);
}
Point circleCenterFromPoints(Point p1,Point p2, float dir, float radius = 65) {
	
	//float radius = 65; //max turnin angle radius

	double midX = (p1.x + p2.x) / 2;
	double midY = (p1.y + p2.y) / 2;
	double dx = (p1.x - p2.x) / 2;
	double dy = (p1.y - p2.y) / 2;
	//Then, calculate the distance from each point to the midpoint 

	double dist = std::sqrt(dx * dx + dy * dy);
	//We can then calculate the distance from the midpoint to the center of the circle using the Pythagorean theorem :

	double pDist = std::sqrt(radius * radius - dist * dist);
	//We can use all this information to calculate the vector from the midpoint to the center of the circle :
	int mDx, mDy;
	if (dir) {
		mDx = dy * pDist / dist;
		mDy = -dx * pDist / dist;
	}
	else {
		mDx = -dy * pDist / dist;
		mDy = dx * pDist / dist;
	}
	//Now, we can just add that vector to the midpoint, and return:

	return Point(midX + mDx, midY + mDy);
	//return Point(0,0);
}
float distanceWithAngle(Point p1,Point p2,float dir, float steeringAngle) {
	
	float radius = abs(3.4 / sin(steeringAngle)/2);
	Point circleCenter = circleCenterFromPoints(p1, p2, dir, radius);
	float angle = quadraticBezierAngle2(1,p1,p2, circleCenter);
	//L = r * Θ
	return radius * angle;
}
void bezierControlPointsCubic(Point p1, Point p2, Point &control1, Point &control2) {

	float xDiff = p2.x - p1.x;
	float yDiff = p2.y - p1.y;
	float x1 = p1.x + xDiff / 3.0;
	float x2 = p1.x + 2.0 * xDiff / 3.0;
	float y1 = p1.y + yDiff / 3.0;
	float y2 = p1.y + 2.0 * yDiff / 3.0;

	control1 = Point(x1, y1);
	control2 = Point(x2, y2);
}
void bezierControlPointsCubic(Point p1, Point p2,Point p3, Point &control1, Point &control2) {

	float xDiff = p2.x - p1.x;
	float yDiff = p3.y - p1.y;
	float yDiff1 =  p2.y - p3.y;
	float x1 = p3.x - xDiff / 3.0;
	float x2 = p3.x + xDiff / 5.0;
	float y1 = p3.y + yDiff / 3.0;
	float y2 = p3.y + yDiff1 / 3.0;

	control1 = Point(x1, y1);
	control2 = Point(x2, y2);
}
