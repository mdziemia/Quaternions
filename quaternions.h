#ifndef __CLASSES
#define __CLASSES

#include <stdio.h>
#include <math.h>
#include <gl/glu.h>
#include <gl/gl.h>
#include <gl/glut.h>




// -----------------------------------------------------------------------------
class CVector3
{
    public:

        float x, y, z;

        CVector3(void) { }
        CVector3(float _x, float _y, float _z):
            x(_x), y(_y), z(_z)
        { }

        float length(void) const;
        CVector3  operator+(const CVector3 &vVector) const;
        CVector3  operator-(const CVector3 &vVector) const;

};

// -----------------------------------------------------------------------------
class CQuaternion
{
    public:

        float w, x, y, z;

    public:

        // -------------------------------------------------
        CQuaternion(void);
		CQuaternion(float _w, float _x, float _y, float _z):
            w(_w), x(_x), y(_y), z(_z)
        { }
        CQuaternion(const CVector3 &_p);
        CQuaternion(float _degree, const CVector3 &_vec);


		CVector3 toPoint(void);
        void getMatrix(float* qMatrix);
        CQuaternion operator+(const CQuaternion &q) const;
		CQuaternion operator-(const CQuaternion &q) const;
		CQuaternion operator*(const CQuaternion &q) const;
		CQuaternion operator*=(const CQuaternion &q);
		CVector3 rotate(const CVector3 &_p);
		CQuaternion  QuatSlerp(const CQuaternion &_to, float t) const;
		void glDraw(void);
};

#endif
