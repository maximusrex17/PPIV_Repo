#pragma once

#define PI 4*atan(1)

struct Vertex {
	float pos[4];
	float UV[2];
};

struct Matrix {
	Vertex Verts[4];
};

float curDeg = 0.0f;

unsigned int Position(float x, float y, unsigned int width) {
	return ((y)* width + (x));
}

float ImplicitLineEquation(float x, float y, float x1, float y1, float x2, float y2) {
	float ile = (((y1 - y2) * (x)) + ((x2 - x1) * (y)) + ((x1 * y2) - (y1 * x2)));
	return ile;
}

float Lerp(float begin, float end, float rat) {
	return (begin + rat * (end - begin));
}
/*
bool BoolBarycentric(float curX, float curY, float Ax, float Ay, float Bx, float By, float Cx, float Cy, float _a, float& _b, float& _g) {
	float alpha, beta, gamma, A, B, Y, a, b, c;

	beta = ImplicitLineEquation(Bx, By, Ax, Ay, Cx, Cy);
	gamma = ImplicitLineEquation(Cx, Cy, Bx, By, Ax, Ay);
	alpha = ImplicitLineEquation(Ax, Ay, Cx, Cy, Bx, By);

	B = ImplicitLineEquation(curX, curY, Ax, Ay, Cx, Cy);
	Y = ImplicitLineEquation(curX, curY, Bx, By, Ax, Ay);
	A = ImplicitLineEquation(curX, curY, Cx, Cy, Bx, By);

	a = A / alpha;
	b = B / beta;
	c = Y / gamma;

	_a = a;
	_b = b;
	_g = c;

	return (b >= 0 && b <= 1
		&& c >= 0 && c <= 1
		&& a >= 0 && a <= 1);
}
*/
float Barycentric(float curX, float curY, Vertex vec1, Vertex vec2, Vertex vec3) {
	
	float alpha, beta, gamma, A, B, Y, a, b, c;
	
	beta = ImplicitLineEquation(vec2.pos[0], vec2.pos[1], vec1.pos[0], vec1.pos[1], vec3.pos[0], vec3.pos[1]);
	gamma = ImplicitLineEquation(vec3.pos[0], vec3.pos[1], vec2.pos[0], vec2.pos[1], vec1.pos[0], vec1.pos[1]);
	alpha = ImplicitLineEquation(vec1.pos[0], vec1.pos[1], vec3.pos[0], vec3.pos[1], vec2.pos[0], vec2.pos[1]);

	B = ImplicitLineEquation(curX, curY, vec1.pos[0], vec1.pos[1], vec3.pos[0], vec3.pos[1]);
	Y = ImplicitLineEquation(curX, curY, vec2.pos[0], vec2.pos[1], vec1.pos[0], vec1.pos[1]);
	A = ImplicitLineEquation(curX, curY, vec3.pos[0], vec3.pos[1], vec2.pos[0], vec2.pos[1]);

	a = A / alpha;
	b = B / beta;
	c = Y / gamma;

	return (vec1.pos[2] * a + vec2.pos[2] * b + vec3.pos[2] * c);
}

float NDCtoCARx(float width, float NDC) {
	return ((NDC + 1) * (0.5f * width));
}

float CARtoNDCx(float width, float CAR) {
	return ((CAR - 1) / (0.5f / width));
}

float NDCtoCARy(float height, float NDC) {
	return abs(((1 - NDC) * (0.5f * height)));
}

float UpdateDeg(float time, float deg) {
	float newDeg = deg + time;
	return newDeg;
}

float DegToRad(float deg) {
	float Rad = (deg * (PI / 180));
	return Rad;
}

float cot(float x) {
	return ((cos(x))/(sin(x)));
}

Matrix GetMat(Matrix mat) {
	return mat;
}

Matrix RotateMatrixX(float rad) {
	Matrix tempMat;
	tempMat.Verts[0].pos[0] = 1;
	tempMat.Verts[0].pos[1] = 0;
	tempMat.Verts[0].pos[2] = 0;
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = 0;
	tempMat.Verts[1].pos[1] = cos(rad);
	tempMat.Verts[1].pos[2] = (-sin(rad));
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = 0;
	tempMat.Verts[2].pos[1] = sin(rad);
	tempMat.Verts[2].pos[2] = cos(rad);
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = 0;
	tempMat.Verts[3].pos[1] = 0;
	tempMat.Verts[3].pos[2] = 0;
	tempMat.Verts[3].pos[3] = 1;

	return tempMat;
}

Matrix RotateMatrixY(float rad) {
	Matrix tempMat;
	tempMat.Verts[0].pos[0] = cos(rad);
	tempMat.Verts[0].pos[1] = 0;
	tempMat.Verts[0].pos[2] = (sin(rad));
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = 0;
	tempMat.Verts[1].pos[1] = 1;
	tempMat.Verts[1].pos[2] = 0;
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = (-sin(rad));
	tempMat.Verts[2].pos[1] = 0;
	tempMat.Verts[2].pos[2] = cos(rad);
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = 0;
	tempMat.Verts[3].pos[1] = 0;
	tempMat.Verts[3].pos[2] = 0;
	tempMat.Verts[3].pos[3] = 1;

	return tempMat;
}

Matrix RotateMatrixZ(float rad) {
	Matrix tempMat;
	tempMat.Verts[0].pos[0] = cos(rad);
	tempMat.Verts[0].pos[1] = (-sin(rad));
	tempMat.Verts[0].pos[2] = 0;
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = sin(rad);
	tempMat.Verts[1].pos[1] = cos(rad);
	tempMat.Verts[1].pos[2] = 0;
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = 0;
	tempMat.Verts[2].pos[1] = 0;
	tempMat.Verts[2].pos[2] = 1;
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = 0;
	tempMat.Verts[3].pos[1] = 0;
	tempMat.Verts[3].pos[2] = 0;
	tempMat.Verts[3].pos[3] = 0;

	return tempMat;
}

Vertex VertexMultMatrix(Vertex _vert, Matrix _mat) {
	Vertex newVert;

	newVert.pos[0] = ((_vert.pos[0] * _mat.Verts[0].pos[0]) + (_vert.pos[1] * _mat.Verts[1].pos[0]) + (_vert.pos[2] * _mat.Verts[2].pos[0]) + (_vert.pos[3] * _mat.Verts[3].pos[0]));
	newVert.pos[1] = ((_vert.pos[0] * _mat.Verts[0].pos[1]) + (_vert.pos[1] * _mat.Verts[1].pos[1]) + (_vert.pos[2] * _mat.Verts[2].pos[1]) + (_vert.pos[3] * _mat.Verts[3].pos[1]));
	newVert.pos[2] = ((_vert.pos[0] * _mat.Verts[0].pos[2]) + (_vert.pos[1] * _mat.Verts[1].pos[2]) + (_vert.pos[2] * _mat.Verts[2].pos[2]) + (_vert.pos[3] * _mat.Verts[3].pos[2]));
	newVert.pos[3] = ((_vert.pos[0] * _mat.Verts[0].pos[3]) + (_vert.pos[1] * _mat.Verts[1].pos[3]) + (_vert.pos[2] * _mat.Verts[2].pos[3]) + (_vert.pos[3] * _mat.Verts[3].pos[3]));
	//newVert.pos[4] = _vert.pos[4];
	//newVert.pos[5] = _vert.pos[5];
	return newVert;
}

Matrix MatrixMultMatrix(Matrix mat1, Matrix mat2) {
	Matrix newMat;

	newMat.Verts[0].pos[0] = (mat1.Verts[0].pos[0] * mat2.Verts[0].pos[0]) + (mat1.Verts[0].pos[1] * mat2.Verts[1].pos[0]) + (mat1.Verts[0].pos[2] * mat2.Verts[2].pos[0]) + (mat1.Verts[0].pos[3] * mat2.Verts[3].pos[0]);
	newMat.Verts[0].pos[1] = (mat1.Verts[0].pos[0] * mat2.Verts[0].pos[1]) + (mat1.Verts[0].pos[1] * mat2.Verts[1].pos[1]) + (mat1.Verts[0].pos[2] * mat2.Verts[2].pos[1]) + (mat1.Verts[0].pos[3] * mat2.Verts[3].pos[1]);
	newMat.Verts[0].pos[2] = (mat1.Verts[0].pos[0] * mat2.Verts[0].pos[2]) + (mat1.Verts[0].pos[1] * mat2.Verts[1].pos[2]) + (mat1.Verts[0].pos[2] * mat2.Verts[2].pos[2]) + (mat1.Verts[0].pos[3] * mat2.Verts[3].pos[2]);
	newMat.Verts[0].pos[3] = (mat1.Verts[0].pos[0] * mat2.Verts[0].pos[3]) + (mat1.Verts[0].pos[1] * mat2.Verts[1].pos[3]) + (mat1.Verts[0].pos[2] * mat2.Verts[2].pos[3]) + (mat1.Verts[0].pos[3] * mat2.Verts[3].pos[3]);
	
	newMat.Verts[1].pos[0] = (mat1.Verts[1].pos[0] * mat2.Verts[0].pos[0]) + (mat1.Verts[1].pos[1] * mat2.Verts[1].pos[0]) + (mat1.Verts[1].pos[2] * mat2.Verts[2].pos[0]) + (mat1.Verts[1].pos[3] * mat2.Verts[3].pos[0]);
	newMat.Verts[1].pos[1] = (mat1.Verts[1].pos[0] * mat2.Verts[0].pos[1]) + (mat1.Verts[1].pos[1] * mat2.Verts[1].pos[1]) + (mat1.Verts[1].pos[2] * mat2.Verts[2].pos[1]) + (mat1.Verts[1].pos[3] * mat2.Verts[3].pos[1]);
	newMat.Verts[1].pos[2] = (mat1.Verts[1].pos[0] * mat2.Verts[0].pos[2]) + (mat1.Verts[1].pos[1] * mat2.Verts[1].pos[2]) + (mat1.Verts[1].pos[2] * mat2.Verts[2].pos[2]) + (mat1.Verts[1].pos[3] * mat2.Verts[3].pos[2]);
	newMat.Verts[1].pos[3] = (mat1.Verts[1].pos[0] * mat2.Verts[0].pos[3]) + (mat1.Verts[1].pos[1] * mat2.Verts[1].pos[3]) + (mat1.Verts[1].pos[2] * mat2.Verts[2].pos[3]) + (mat1.Verts[1].pos[3] * mat2.Verts[3].pos[3]);

	newMat.Verts[2].pos[0] = (mat1.Verts[2].pos[0] * mat2.Verts[0].pos[0]) + (mat1.Verts[2].pos[1] * mat2.Verts[1].pos[0]) + (mat1.Verts[2].pos[2] * mat2.Verts[2].pos[0]) + (mat1.Verts[2].pos[3] * mat2.Verts[3].pos[0]);
	newMat.Verts[2].pos[1] = (mat1.Verts[2].pos[0] * mat2.Verts[0].pos[1]) + (mat1.Verts[2].pos[1] * mat2.Verts[1].pos[1]) + (mat1.Verts[2].pos[2] * mat2.Verts[2].pos[1]) + (mat1.Verts[2].pos[3] * mat2.Verts[3].pos[1]);
	newMat.Verts[2].pos[2] = (mat1.Verts[2].pos[0] * mat2.Verts[0].pos[2]) + (mat1.Verts[2].pos[1] * mat2.Verts[1].pos[2]) + (mat1.Verts[2].pos[2] * mat2.Verts[2].pos[2]) + (mat1.Verts[2].pos[3] * mat2.Verts[3].pos[2]);
	newMat.Verts[2].pos[3] = (mat1.Verts[2].pos[0] * mat2.Verts[0].pos[3]) + (mat1.Verts[2].pos[1] * mat2.Verts[1].pos[3]) + (mat1.Verts[2].pos[2] * mat2.Verts[2].pos[3]) + (mat1.Verts[2].pos[3] * mat2.Verts[3].pos[3]);

	newMat.Verts[3].pos[0] = (mat1.Verts[3].pos[0] * mat2.Verts[0].pos[0]) + (mat1.Verts[3].pos[1] * mat2.Verts[1].pos[0]) + (mat1.Verts[3].pos[2] * mat2.Verts[2].pos[0]) + (mat1.Verts[3].pos[3] * mat2.Verts[3].pos[0]);
	newMat.Verts[3].pos[1] = (mat1.Verts[3].pos[0] * mat2.Verts[0].pos[1]) + (mat1.Verts[3].pos[1] * mat2.Verts[1].pos[1]) + (mat1.Verts[3].pos[2] * mat2.Verts[2].pos[1]) + (mat1.Verts[3].pos[3] * mat2.Verts[3].pos[1]);
	newMat.Verts[3].pos[2] = (mat1.Verts[3].pos[0] * mat2.Verts[0].pos[2]) + (mat1.Verts[3].pos[1] * mat2.Verts[1].pos[2]) + (mat1.Verts[3].pos[2] * mat2.Verts[2].pos[2]) + (mat1.Verts[3].pos[3] * mat2.Verts[3].pos[2]);
	newMat.Verts[3].pos[3] = (mat1.Verts[3].pos[0] * mat2.Verts[0].pos[3]) + (mat1.Verts[3].pos[1] * mat2.Verts[1].pos[3]) + (mat1.Verts[3].pos[2] * mat2.Verts[2].pos[3]) + (mat1.Verts[3].pos[3] * mat2.Verts[3].pos[3]);

	return newMat;
}

//Translate along the X-axis

Matrix TranslateX(float dist) {
	Matrix tempMat;
	tempMat.Verts[0].pos[0] = 1;
	tempMat.Verts[0].pos[1] = 0;
	tempMat.Verts[0].pos[2] = 0;
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = 0;
	tempMat.Verts[1].pos[1] = 1;
	tempMat.Verts[1].pos[2] = 0;
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = 0;
	tempMat.Verts[2].pos[1] = 0;
	tempMat.Verts[2].pos[2] = 1;
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = dist;
	tempMat.Verts[3].pos[1] = 0;
	tempMat.Verts[3].pos[2] = 0;
	tempMat.Verts[3].pos[3] = 1;

	return tempMat;
}

//Translate along the Y-axis

Matrix TranslateY(float dist) {
	Matrix tempMat;
	tempMat.Verts[0].pos[0] = 1;
	tempMat.Verts[0].pos[1] = 0;
	tempMat.Verts[0].pos[2] = 0;
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = 0;
	tempMat.Verts[1].pos[1] = 1;
	tempMat.Verts[1].pos[2] = 0;
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = 0;
	tempMat.Verts[2].pos[1] = 0;
	tempMat.Verts[2].pos[2] = 1;
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = 0;
	tempMat.Verts[3].pos[1] = dist;
	tempMat.Verts[3].pos[2] = 0;
	tempMat.Verts[3].pos[3] = 1;

	return tempMat;
}

//Translate along the Z-axis

Matrix TranslateZ(float dist) {
	Matrix tempMat;
	tempMat.Verts[0].pos[0] = 1;
	tempMat.Verts[0].pos[1] = 0;
	tempMat.Verts[0].pos[2] = 0;
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = 0;
	tempMat.Verts[1].pos[1] = 1;
	tempMat.Verts[1].pos[2] = 0;
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = 0;
	tempMat.Verts[2].pos[1] = 0;
	tempMat.Verts[2].pos[2] = 1;
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = 0;
	tempMat.Verts[3].pos[1] = 0;
	tempMat.Verts[3].pos[2] = dist;
	tempMat.Verts[3].pos[3] = 1;

	return tempMat;
}

//Dynamically move along the X-axis, Y-axis, and the Z-axis

Matrix TranslateCamera(float x, float y, float z) {
	Matrix tempMat;
	tempMat.Verts[0].pos[0] = 1;
	tempMat.Verts[0].pos[1] = 0;
	tempMat.Verts[0].pos[2] = 0;
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = 0;
	tempMat.Verts[1].pos[1] = 1;
	tempMat.Verts[1].pos[2] = 0;
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = 0;
	tempMat.Verts[2].pos[1] = 0;
	tempMat.Verts[2].pos[2] = 1;
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = x;
	tempMat.Verts[3].pos[1] = y;
	tempMat.Verts[3].pos[2] = z;
	tempMat.Verts[3].pos[3] = 1;

	return tempMat;
}

Matrix IdentityMatrix() {
	Matrix tempMat;
	tempMat.Verts[0].pos[0] = 1;
	tempMat.Verts[0].pos[1] = 0;
	tempMat.Verts[0].pos[2] = 0;
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = 0;
	tempMat.Verts[1].pos[1] = 1;
	tempMat.Verts[1].pos[2] = 0;
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = 0;
	tempMat.Verts[2].pos[1] = 0;
	tempMat.Verts[2].pos[2] = 1;
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = 0;
	tempMat.Verts[3].pos[1] = 0;
	tempMat.Verts[3].pos[2] = 0;
	tempMat.Verts[3].pos[3] = 1;

	return tempMat;
}

Vertex VertexTranslation(Vertex _vert, Matrix _mat) {
	Vertex newVert;

	newVert.pos[0] = (_vert.pos[0] + _mat.Verts[3].pos[0]);
	newVert.pos[1] = (_vert.pos[1] + _mat.Verts[3].pos[1]);
	newVert.pos[2] = (_vert.pos[2] + _mat.Verts[3].pos[2]);
	newVert.pos[3] = (_vert.pos[3] + _mat.Verts[3].pos[3]);

	return newVert;
}

Matrix ViewMatrix(float rad, float dist) {
	Matrix tempMat;

	tempMat.Verts[0].pos[0] = 1;
	tempMat.Verts[0].pos[1] = 0;
	tempMat.Verts[0].pos[2] = 0;
	tempMat.Verts[0].pos[3] = 0;

	tempMat.Verts[1].pos[0] = 0;
	tempMat.Verts[1].pos[1] = cos(rad);
	tempMat.Verts[1].pos[2] = (-sin(rad));
	tempMat.Verts[1].pos[3] = 0;

	tempMat.Verts[2].pos[0] = 0;
	tempMat.Verts[2].pos[1] = sin(rad);
	tempMat.Verts[2].pos[2] = cos(rad);
	tempMat.Verts[2].pos[3] = 0;

	tempMat.Verts[3].pos[0] = 0;
	tempMat.Verts[3].pos[1] = 0;
	tempMat.Verts[3].pos[2] = dist;
	tempMat.Verts[3].pos[3] = 1;

	return tempMat;
}

float DeterminateMatrix(Matrix _mat) {
	float det = ((_mat.Verts[0].pos[0]) * (_mat.Verts[1].pos[1]) * (_mat.Verts[2].pos[2]) * (_mat.Verts[3].pos[3]));
	return det;
}

Matrix InverseMatrix(Matrix mat) {
	Matrix tempMat;

	tempMat.Verts[0].pos[0] = mat.Verts[0].pos[0];
	tempMat.Verts[0].pos[1] = mat.Verts[0].pos[1];
	tempMat.Verts[0].pos[2] = mat.Verts[0].pos[2];

	tempMat.Verts[1].pos[0] = mat.Verts[1].pos[0];
	tempMat.Verts[1].pos[1] = mat.Verts[1].pos[1];
	tempMat.Verts[1].pos[2] = mat.Verts[1].pos[2];

	tempMat.Verts[2].pos[0] = mat.Verts[2].pos[0];
	tempMat.Verts[2].pos[1] = mat.Verts[2].pos[1];
	tempMat.Verts[2].pos[2] = mat.Verts[2].pos[2];

	tempMat.Verts[0].pos[0] *= -1;
	tempMat.Verts[0].pos[1] *= -1;
	tempMat.Verts[0].pos[2] *= -1;
	tempMat.Verts[0].pos[3] *= -1;

	tempMat.Verts[1].pos[0] *= -1;
	tempMat.Verts[1].pos[1] *= -1;
	tempMat.Verts[1].pos[2] *= -1;
	tempMat.Verts[1].pos[3] *= -1;

	tempMat.Verts[2].pos[0] *= -1;
	tempMat.Verts[2].pos[1] *= -1;
	tempMat.Verts[2].pos[2] *= -1;
	tempMat.Verts[2].pos[3] *= -1;

	//tempMat.Verts[3].pos[0] *= -1;
	//tempMat.Verts[3].pos[1] *= -1;
	//tempMat.Verts[3].pos[2] *= -1;
	//tempMat.Verts[3].pos[3] *= -1;

	return tempMat;
}

Matrix AdjointMatrix(Matrix mat) {
	Matrix tempMat;

	//Row 1 == Verts[0]
	tempMat.Verts[0].pos[0] = (mat.Verts[0].pos[0] * (mat.Verts[1].pos[1] * (((mat.Verts[2].pos[2]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[2]))) + (-mat.Verts[1].pos[2] * ((-mat.Verts[2].pos[1]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (mat.Verts[3].pos[1]))) + (mat.Verts[1].pos[3] * (((mat.Verts[2].pos[2]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[2]))))));
	tempMat.Verts[0].pos[1] = (-mat.Verts[0].pos[1] * (-mat.Verts[1].pos[0] * (((mat.Verts[2].pos[2]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[2])))) + (-mat.Verts[1].pos[2] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[0])))) + (mat.Verts[1].pos[3] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[3].pos[2]) * (-mat.Verts[3].pos[0])))));
	tempMat.Verts[0].pos[2] = (mat.Verts[0].pos[2] * (-mat.Verts[1].pos[0] * (((-mat.Verts[2].pos[1]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (mat.Verts[3].pos[1])))) + (mat.Verts[1].pos[1] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[0])))) + (mat.Verts[1].pos[3] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[1])) - ((-mat.Verts[2].pos[1]) * (-mat.Verts[3].pos[0])))));
	tempMat.Verts[0].pos[3] = (-mat.Verts[0].pos[3] * (-mat.Verts[1].pos[0] * (((-mat.Verts[2].pos[1]) * (-mat.Verts[3].pos[2])) - ((mat.Verts[2].pos[2]) * (mat.Verts[3].pos[1])))) + (mat.Verts[1].pos[1] * (((mat.Verts[2].pos[0]) * (-mat.Verts[3].pos[2])) - ((mat.Verts[2].pos[2]) * (-mat.Verts[3].pos[0])))) + (mat.Verts[1].pos[2] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[1])) - ((-mat.Verts[2].pos[1]) * (-mat.Verts[3].pos[0])))));

	//Row 2 == Verts[1]
	tempMat.Verts[1].pos[0] = (-mat.Verts[1].pos[0] * (-mat.Verts[0].pos[1] * (((mat.Verts[2].pos[2]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[2]))) + (mat.Verts[0].pos[2] * ((-mat.Verts[2].pos[1]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (mat.Verts[3].pos[1]))) + (-mat.Verts[0].pos[3] * (((-mat.Verts[2].pos[1]) * (-mat.Verts[3].pos[2])) - ((mat.Verts[2].pos[2]) * (mat.Verts[3].pos[1]))))));
	tempMat.Verts[1].pos[1] = (mat.Verts[1].pos[1] * (mat.Verts[0].pos[0] * (((mat.Verts[2].pos[2]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[2])))) + (mat.Verts[0].pos[2] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[0])))) + (-mat.Verts[0].pos[3] * (((mat.Verts[2].pos[0]) * (-mat.Verts[3].pos[2])) - ((mat.Verts[2].pos[2]) * (-mat.Verts[3].pos[0])))));
	tempMat.Verts[1].pos[2] = (-mat.Verts[1].pos[2] * (mat.Verts[0].pos[0] * (((-mat.Verts[2].pos[1]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (mat.Verts[3].pos[1])))) + (-mat.Verts[0].pos[1] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[3])) - ((-mat.Verts[2].pos[3]) * (-mat.Verts[3].pos[0])))) + (mat.Verts[0].pos[3] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[1])) - ((-mat.Verts[2].pos[1]) * (-mat.Verts[3].pos[0])))));
	tempMat.Verts[1].pos[3] = (mat.Verts[1].pos[3] * (mat.Verts[0].pos[0] * (((-mat.Verts[2].pos[1]) * (-mat.Verts[3].pos[2])) - ((mat.Verts[2].pos[2]) * (mat.Verts[3].pos[1])))) + (-mat.Verts[0].pos[1] * (((mat.Verts[2].pos[0]) * (-mat.Verts[3].pos[2])) - ((mat.Verts[2].pos[2]) * (-mat.Verts[3].pos[0])))) + (mat.Verts[0].pos[2] * (((mat.Verts[2].pos[0]) * (mat.Verts[3].pos[1])) - ((-mat.Verts[2].pos[1]) * (-mat.Verts[3].pos[0])))));

	//Row 3 == Verts[2]
	tempMat.Verts[2].pos[0] = (mat.Verts[2].pos[0] * (-mat.Verts[0].pos[1] * (((-mat.Verts[1].pos[2]) * (mat.Verts[3].pos[3])) - ((mat.Verts[1].pos[3]) * (-mat.Verts[3].pos[2]))) + (mat.Verts[0].pos[2] * ((mat.Verts[1].pos[0]) * (mat.Verts[3].pos[3])) - ((mat.Verts[1].pos[3]) * (mat.Verts[3].pos[1]))) + (-mat.Verts[0].pos[3] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[3].pos[2])) - ((-mat.Verts[1].pos[2]) * (mat.Verts[3].pos[1]))))));
	tempMat.Verts[2].pos[1] = (mat.Verts[2].pos[1] * (mat.Verts[0].pos[0] * (((-mat.Verts[1].pos[2]) * (mat.Verts[3].pos[3])) - ((mat.Verts[1].pos[3]) * (-mat.Verts[3].pos[2])))) + (mat.Verts[0].pos[2] * (((-mat.Verts[1].pos[0]) * (mat.Verts[3].pos[3])) - ((mat.Verts[1].pos[3]) * (-mat.Verts[3].pos[0])))) + (-mat.Verts[0].pos[3] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[3].pos[2])) - ((-mat.Verts[1].pos[2]) * (-mat.Verts[3].pos[0])))));
	tempMat.Verts[2].pos[2] = (-mat.Verts[2].pos[2] * (mat.Verts[0].pos[0] * (((mat.Verts[1].pos[1]) * (mat.Verts[3].pos[3])) - ((mat.Verts[1].pos[3]) * (mat.Verts[3].pos[1])))) + (-mat.Verts[0].pos[1] * (((-mat.Verts[1].pos[0]) * (mat.Verts[3].pos[3])) - ((mat.Verts[1].pos[3]) * (-mat.Verts[3].pos[0])))) + (-mat.Verts[0].pos[3] * (((-mat.Verts[1].pos[0]) * (mat.Verts[3].pos[1])) - ((mat.Verts[1].pos[1]) * (-mat.Verts[3].pos[0])))));
	tempMat.Verts[2].pos[3] = (mat.Verts[2].pos[3] * (mat.Verts[0].pos[0] * (((mat.Verts[1].pos[1]) * (-mat.Verts[3].pos[2])) - ((-mat.Verts[1].pos[2]) * (-mat.Verts[3].pos[1])))) + (-mat.Verts[0].pos[1] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[3].pos[2])) - ((-mat.Verts[1].pos[2]) * (-mat.Verts[3].pos[0])))) + (mat.Verts[0].pos[2] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[3].pos[2])) - ((-mat.Verts[1].pos[2]) * (-mat.Verts[3].pos[0])))));

	//Row 4 == Verts[3]
	tempMat.Verts[3].pos[0] = (mat.Verts[3].pos[0] * (-mat.Verts[0].pos[1] * (((-mat.Verts[1].pos[2]) * (-mat.Verts[2].pos[3])) - ((mat.Verts[1].pos[3]) * (mat.Verts[2].pos[2]))) + (mat.Verts[0].pos[2] * ((mat.Verts[1].pos[1]) * (-mat.Verts[2].pos[3])) - ((mat.Verts[1].pos[3]) * (-mat.Verts[2].pos[1]))) + (-mat.Verts[0].pos[3] * (((mat.Verts[1].pos[1]) * (-mat.Verts[2].pos[3])) - ((mat.Verts[1].pos[3]) * (-mat.Verts[2].pos[1]))))));
	tempMat.Verts[3].pos[1] = (mat.Verts[3].pos[1] * (mat.Verts[0].pos[0] * (((-mat.Verts[1].pos[2]) * (-mat.Verts[2].pos[3])) - ((mat.Verts[1].pos[3]) * (mat.Verts[2].pos[2])))) + (mat.Verts[0].pos[2] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[2].pos[3])) - ((mat.Verts[1].pos[3]) * (mat.Verts[2].pos[0])))) + (-mat.Verts[0].pos[3] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[3].pos[2])) - ((-mat.Verts[1].pos[2]) * (mat.Verts[2].pos[0])))));
	tempMat.Verts[3].pos[2] = (-mat.Verts[3].pos[2] * (mat.Verts[0].pos[0] * (((mat.Verts[1].pos[1]) * (-mat.Verts[2].pos[3])) - ((mat.Verts[1].pos[3]) * (-mat.Verts[2].pos[1])))) + (-mat.Verts[0].pos[1] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[2].pos[3])) - ((mat.Verts[1].pos[3]) * (mat.Verts[2].pos[0])))) + (-mat.Verts[0].pos[3] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[2].pos[1])) - ((mat.Verts[1].pos[1]) * (mat.Verts[2].pos[0])))));
	tempMat.Verts[3].pos[3] = (mat.Verts[3].pos[3] * (mat.Verts[0].pos[0] * (((mat.Verts[1].pos[1]) * (mat.Verts[2].pos[2])) - ((-mat.Verts[1].pos[2]) * (mat.Verts[2].pos[0])))) + (-mat.Verts[0].pos[1] * (((-mat.Verts[1].pos[0]) * (mat.Verts[2].pos[2])) - ((-mat.Verts[1].pos[2]) * (mat.Verts[2].pos[0])))) + (mat.Verts[0].pos[2] * (((-mat.Verts[1].pos[0]) * (-mat.Verts[2].pos[1])) - ((-mat.Verts[1].pos[2]) * (mat.Verts[2].pos[0])))));

	return tempMat;
}

Matrix Transpose(Matrix mat) {
	Matrix transpose;

	transpose.Verts[0].pos[0] = mat.Verts[0].pos[0];
	transpose.Verts[0].pos[1] = mat.Verts[1].pos[0];
	transpose.Verts[0].pos[2] = mat.Verts[2].pos[0];
	transpose.Verts[0].pos[3] = mat.Verts[3].pos[0];

	transpose.Verts[1].pos[0] = mat.Verts[0].pos[1];
	transpose.Verts[1].pos[1] = mat.Verts[1].pos[1];
	transpose.Verts[1].pos[2] = mat.Verts[2].pos[1];
	transpose.Verts[1].pos[3] = mat.Verts[3].pos[1];

	transpose.Verts[2].pos[0] = mat.Verts[0].pos[2];
	transpose.Verts[2].pos[1] = mat.Verts[1].pos[2];
	transpose.Verts[2].pos[2] = mat.Verts[2].pos[2];
	transpose.Verts[2].pos[3] = mat.Verts[3].pos[2];

	transpose.Verts[3].pos[0] = mat.Verts[0].pos[3];
	transpose.Verts[3].pos[1] = mat.Verts[1].pos[3];
	transpose.Verts[3].pos[2] = mat.Verts[2].pos[3];
	transpose.Verts[3].pos[3] = mat.Verts[3].pos[3];

	return transpose;
}

Matrix Projection(float zNear, float zFar, float VerticalFOV, float ratio) {
	
	float yScale = cot(DegToRad((0.5f)*(VerticalFOV)));
	float xScale = yScale * ratio;
	
	Matrix proMat;

	proMat.Verts[0].pos[0] = xScale;
	proMat.Verts[0].pos[1] = 0;
	proMat.Verts[0].pos[2] = 0;
	proMat.Verts[0].pos[3] = 0;

	proMat.Verts[1].pos[0] = 0;
	proMat.Verts[1].pos[1] = yScale;
	proMat.Verts[1].pos[2] = 0;
	proMat.Verts[1].pos[3] = 0;

	proMat.Verts[2].pos[0] = 0;
	proMat.Verts[2].pos[1] = 0;
	proMat.Verts[2].pos[2] = (zFar/(zFar-zNear));
	proMat.Verts[2].pos[3] = 1;

	proMat.Verts[3].pos[0] = 0;
	proMat.Verts[3].pos[1] = 0;
	proMat.Verts[3].pos[2] = (-1*((zFar * zNear) / (zFar-zNear)));
	proMat.Verts[3].pos[3] = 0;

	return proMat;
}