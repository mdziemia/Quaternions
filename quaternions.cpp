// Quaternion and CVector3 classes
#include "quaternions.h"



// -------------------------------------------------
float CVector3::length(void) const
{
    return sqrt(x*x + y*y + z*z);
}

// -------------------------------------------------
CVector3  CVector3::operator+(const CVector3 &vVector) const
{
	return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
}
// -------------------------------------------------
CVector3  CVector3::operator-(const CVector3 &vVector) const
{
	return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
}
    	
// -------------------------------------------------
// -------------------------------------------------
// -------------------------------------------------
CQuaternion::CQuaternion(void) { }

// -------------------------------------------------
CQuaternion::CQuaternion(const CVector3 &_p)
{
    float cr, cp, cy, sr, sp, sy, cpcy, spsy;

    // calculate trig identities
    cr = cos(_p.x/2);

	cp = cos(_p.y/2);
	cy = cos(_p.z/2);


	sr = sin(_p.x/2);
	sp = sin(_p.y/2);
	sy = sin(_p.z/2);
	
	cpcy = cp * cy;
	spsy = sp * sy;


	this->w = cr * cpcy + sr * spsy;
	this->x = sr * cpcy - cr * spsy;
	this->y = cr * sp * cy + sr * cp * sy;
	this->z = cr * cp * sy - sr * sp * cy;
	
}

// -------------------------------------------------
// Create quaternion from axis angles
CQuaternion::CQuaternion(float _degree, const CVector3 &_vec)
{
    
    float angle = (_degree / 180.0f) * 3.1415;
	float coef =  sin( angle / 2.0f );
		
	this->w = cos( angle / 2.0f );
	this->x = _vec.x * coef;
	this->y = _vec.y * coef;
	this->z = _vec.z * coef;
}

// -------------------------------------------------
// to point
CVector3 CQuaternion::toPoint(void)
{
    return CVector3
    (
        1.0f - 2.0f * ( y * y + z * z ),
        2.0f * ( x * y - z * w ),
        2.0f * ( x * z + y * w )
    );
}

// -------------------------------------------------
void CQuaternion::getMatrix(float* qMatrix)
{
    // First row
	qMatrix[ 0] = 1.0f - 2.0f * ( y * y + z * z ); 
	qMatrix[ 1] = 2.0f * (x * y + z * w);
	qMatrix[ 2] = 2.0f * (x * z - y * w);
	qMatrix[ 3] = 0.0f;  

	
    // Second row
	qMatrix[ 4] = 2.0f * ( x * y - z * w );  
	qMatrix[ 5] = 1.0f - 2.0f * ( x * x + z * z ); 
	qMatrix[ 6] = 2.0f * (z * y + x * w );  
	qMatrix[ 7] = 0.0f;  

	// Third row
	qMatrix[ 8] = 2.0f * ( x * z + y * w );
	qMatrix[ 9] = 2.0f * ( y * z - x * w );
	qMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );  
	qMatrix[11] = 0.0f;  

	// Fourth row
	qMatrix[12] = 0;  
	qMatrix[13] = 0;  
	qMatrix[14] = 0;  
	qMatrix[15] = 1.0f;
	
}

// -------------------------------------------------
// rotate _p by this quaternion and return the new point
CVector3 CQuaternion::rotate(const CVector3 &_p)
{
    float qMatrix[16];
    getMatrix(qMatrix);

    return CVector3
    (            
        _p.x * qMatrix[0] + _p.y * qMatrix[1] + _p.z * qMatrix[2],
        _p.x * qMatrix[4] + _p.y * qMatrix[5] + _p.z * qMatrix[6],
        _p.x * qMatrix[8] + _p.y * qMatrix[9] + _p.z * qMatrix[10]
    );
}

// ---------------------------------------------------
// return point between two quaternions on spherical shortest path
CQuaternion  CQuaternion::QuatSlerp(const CQuaternion &_to, float t) const
{
    float   to1[4];
    double  omega, cosom, sinom, scale0, scale1;

    // calc cosine
    cosom = w*_to.w  +  x*_to.x  +  y*_to.y  +  z*_to.z;


    // adjust signs (if necessary)
    if (cosom < 0.0)
    { 
        cosom = -cosom;
        to1[0] = - _to.w;
        to1[1] = - _to.x;
        to1[2] = - _to.y;
        to1[3] = - _to.z;
    } 
    else
    {
        to1[0] = _to.w;
        to1[1] = _to.x;
        to1[2] = _to.y;
        to1[3] = _to.z;
    }
    // calculate coefficients
    if (true || (1.0 - cosom) > 0.1 )
    {
        // standard case (slerp)
        omega = acos(cosom);
        sinom = sin(omega);
        scale0 = sin((1.0 - t) * omega) / sinom;
        scale1 = sin(t * omega) / sinom;
    }
    else
    {
        scale0 = 1.0 - t;
        scale1 = t;
    }
    return CQuaternion
    (
    	scale0 * w + scale1 * to1[0],
    	scale0 * x + scale1 * to1[1],
    	scale0 * y + scale1 * to1[2],
    	scale0 * z + scale1 * to1[3]
	);
}

// -------------------------------------------------
CQuaternion CQuaternion::operator+(const CQuaternion &q) const
{
	return CQuaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}
CQuaternion CQuaternion::operator-(const CQuaternion &q) const
{
	return CQuaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}
CQuaternion CQuaternion::operator*(const CQuaternion &q) const
{
    return CQuaternion
    (
        w*q.w - x*q.x - y*q.y - z*q.z,
        w*q.x + x*q.w + y*q.z - z*q.y,
        w*q.y - x*q.z + y*q.w + z*q.x,
        w*q.z + x*q.y - y*q.x + z*q.w
    );
}

// -------------------------------------------------
CQuaternion CQuaternion::operator*=(const CQuaternion &q)
{
    *this = *this * q;
    return *this;
}

// -------------------------------------------------
void CQuaternion::glDraw(void)
{
    CVector3 p = toPoint();
                
    glBegin (GL_POINTS);
        glVertex3f (p.x, p.y, p.z);
    glEnd();
}

