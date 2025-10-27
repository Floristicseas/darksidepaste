#pragma once
#include "vec_t.hpp"
#include <DirectXMath.h> 

class QAngle
{
public:
	QAngle()
	{
		Init();
	}

	QAngle(float x, float y, float z)
	{
		Init(x, y, z);
	}

	QAngle(const float* angles)
	{
		Init(angles[0], angles[1], angles[2]);
	}

	const void Init(float x = 0.f, float y = 0.f, float z = 0.f)
	{
		this->x = x; this->y = y; this->z = z;
	}

	float operator[](std::size_t nIndex) const
	{
		return ((float*)this)[nIndex];
	}

	float& operator[](std::size_t nIndex)
	{
		return ((float*)this)[nIndex];
	}

	bool operator==(const QAngle& v) const
	{
		return (this->x == v[0] &&
			this->y == v[1] &&
			this->z == v[2]);
	}

	bool operator!=(const QAngle& v) const
	{
		return !(this->x == v[0] &&
			this->y == v[1] &&
			this->z == v[2]);
	}

	QAngle& operator=(const QAngle& v)
	{
		this->x = v.x; this->y = v.y; this->z = v.z; return *this;
	}

	QAngle& operator+=(const QAngle& v)
	{
		this->x += v.x; this->y += v.y; this->z += v.z; return *this;
	}

	QAngle& operator-=(const QAngle& v)
	{
		this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this;
	}

	QAngle& operator*=(const QAngle& v)
	{
		this->x *= v.x; this->y *= v.y; this->z *= v.z; return *this;
	}

	QAngle& operator/=(const QAngle& v)
	{
		this->x /= v.x; this->y /= v.y; this->z /= v.z; return *this;
	}

	QAngle& operator+=(float v)
	{
		this->x += v; this->y += v; this->z += v; return *this;
	}

	QAngle& operator-=(float v)
	{
		this->x -= v; this->y -= v; this->z -= v; return *this;
	}

	QAngle& operator*=(float v)
	{
		this->x *= v; this->y *= v; this->z *= v; return *this;
	}

	QAngle& operator/=(float v)
	{
		this->x /= v; this->y /= v; this->z /= v; return *this;
	}

	QAngle operator+(float v) const
	{
		return QAngle(this->x + v, this->y + v, this->z + v);
	}

	QAngle operator-(float v) const
	{
		return QAngle(this->x - v, this->y - v, this->z - v);
	}

	QAngle operator*(float v) const
	{
		return QAngle(this->x * v, this->y * v, this->z * v);
	}

	QAngle operator/(float v) const
	{
		return QAngle(this->x / v, this->y / v, this->z / v);
	}

	QAngle operator+(const QAngle& v) const
	{
		return QAngle(this->x + v.x, this->y + v.y, this->z + v.z);
	}

	QAngle operator-(const QAngle& v) const
	{
		return QAngle(this->x - v.x, this->y - v.y, this->z - v.z);
	}

	QAngle operator*(const QAngle& v) const
	{
		return QAngle(this->x * v.x, this->y * v.y, this->z * v.z);
	}

	QAngle operator/(const QAngle& v) const
	{
		return QAngle(this->x / v.x, this->y / v.y, this->z / v.z);
	}

	float Length() const
	{
		return std::sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	float LengthSqr() const
	{
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	float Length2DSqr() const
	{
		return ((this->x * this->x) + (this->y * this->y));
	}

	float Length2D() const
	{
		return std::sqrt(Length2DSqr());
	}

	constexpr QAngle& Clamp()
	{
		this->x = std::clamp(this->x, -89.f, 89.f);
		this->y = std::clamp(this->y, -180.f, 180.f);
		this->z = std::clamp(this->z, -45.f, 45.f);
		return *this;
	}

	constexpr QAngle& ClampXY()
	{
		this->x = std::clamp(this->x, -89.f, 89.f);
		this->y = std::clamp(this->y, -180.f, 180.f);
		return *this;
	}

	bool IsValid()
	{
		return !std::isnan(x) || !std::isnan(y) || !std::isnan(z);
	}

	[[nodiscard]] bool IsEqual(const QAngle& angEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon()) const
	{
		return (std::fabsf(this->x - angEqual.x) < flErrorMargin &&
			std::fabsf(this->y - angEqual.y) < flErrorMargin &&
			std::fabsf(this->z - angEqual.z) < flErrorMargin);
	}

	bool IsZero() const
	{
		return (std::fpclassify(this->x) == FP_ZERO &&
			std::fpclassify(this->y) == FP_ZERO &&
			std::fpclassify(this->z) == FP_ZERO);
	}

	QAngle Normalize()
	{
		this->x = std::isfinite(this->x) ? std::remainder(this->x, 360.f) : 0.f;
		this->y = std::isfinite(this->y) ? std::remainder(this->y, 360.f) : 0.f;
		this->z = 0.f;
		return *this;
	}

	QAngle Mod(float flValue)
	{
		this->x = std::fmodf(this->x, flValue);
		this->y = std::fmodf(this->y, flValue);
		this->z = std::fmodf(this->z, flValue);
		return *this;
	}

	vec3_t ValueInVector()
	{
		return { x, y, z };
	}

	vec3_t ToVec()
	{
		float	sp, sy, cp, cy;
		sy = sin(deg2rad(((float*)this)[1]));
		cy = cos(deg2rad(((float*)this)[1]));

		sp = sin(deg2rad(((float*)this)[0]));
		cp = cos(deg2rad(((float*)this)[0]));

		return { cp * cy, cp * sy, -sp };
	}

	QAngle DeltaAngle(const QAngle& a) const noexcept
	{
		float flDeltaX = x - a.x;
		if (flDeltaX > 180.f)
			flDeltaX -= 360.f;
		else if (flDeltaX < -180.f)
			flDeltaX += 360.f;

		float flDeltaY = y - a.y;
		if (flDeltaY > 180.f)
			flDeltaY -= 360.f;
		else if (flDeltaY < -180.f)
			flDeltaY += 360.f;

		return QAngle(flDeltaX, flDeltaY, 0.f);
	}

	float Delta(const QAngle& angOther) const
	{
		QAngle angDelta;

		angDelta.x = x - angOther.x;
		angDelta.y = y - angOther.y;
		angDelta.z = z - angOther.z;

		return angDelta.Length2D();
	}

	void ToDirections(vec3_t* pvecForward, vec3_t* pvecRight, vec3_t* pvecUp) const
	{

		float flPitchSin, flPitchCos, flYawSin, flYawCos, flRollSin, flRollCos;
		DirectX::XMScalarSinCos(&flPitchSin, &flPitchCos, deg2rad(this->x));
		DirectX::XMScalarSinCos(&flYawSin, &flYawCos, deg2rad(this->y));
		DirectX::XMScalarSinCos(&flRollSin, &flRollCos, deg2rad(this->z));

		if (pvecForward != nullptr)
		{
			pvecForward->x = flPitchCos * flYawCos;
			pvecForward->y = flPitchCos * flYawSin;
			pvecForward->z = -flPitchSin;
		}

		if (pvecRight != nullptr)
		{
			pvecRight->x = (-flRollSin * flPitchSin * flYawCos) + (-flRollCos * -flYawSin);
			pvecRight->y = (-flRollSin * flPitchSin * flYawSin) + (-flRollCos * flYawCos);
			pvecRight->z = (-flRollSin * flPitchCos);
		}

		if (pvecUp != nullptr)
		{
			pvecUp->x = (flRollCos * flPitchSin * flYawCos) + (-flRollSin * -flYawSin);
			pvecUp->y = (flRollCos * flPitchSin * flYawSin) + (-flRollSin * flYawCos);
			pvecUp->z = (flRollCos * flPitchCos);
		}
	}

public:
	float x, y, z;
};