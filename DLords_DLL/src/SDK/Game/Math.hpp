#pragma once

const auto M_PI = 3.141592653589793238462643383279502884L;

#ifndef PI
#define PI			(3.14159265358979323846264338327950288f)
#endif

#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI / 180.f) )
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI) )

// JA Changed to const float from #define to resolve clash with #define in scalarv.h
constexpr float TWO_PI = (PI * 2.0f);
constexpr float THREE_PI = (PI * 3.0f);

#define HALF_PI (PI * 0.5f)
#define QUARTER_PI (PI * 0.25f)
#define EIGHTH_PI (PI * 0.125f)

extern float normalizeAngle1024(float angle);
extern float resolveAngleFromSinCos(float angle_from_asin, float angle_from_acos);

struct vector2
{
	float x;
	float y;
};

struct vector3
{
	float x;
	float y;
	float z;

	vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	vector3(float v) : x(v), y(v), z(v) {}
	vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	__forceinline vector3 operator+(const vector3& other)
	{
		vector3 buf{};
		buf.x = this->x + other.x;
		buf.y = this->y + other.y;
		buf.z = this->z + other.z;

		return buf;
	}

	__forceinline vector3 operator-(const vector3& other)
	{
		vector3 buf{};
		buf.x = this->x - other.x;
		buf.y = this->y - other.y;
		buf.z = this->z - other.z;

		return buf;
	}

	__forceinline vector3 operator*(const float v)
	{
		vector3 buf{};
		buf.x = this->x * v;
		buf.y = this->y * v;
		buf.z = this->z * v;

		return buf;
	}

	__forceinline vector3 operator/(const float v)
	{
		vector3 buf{};
		buf.x = this->x / v;
		buf.y = this->y / v;
		buf.z = this->z / v;

		return buf;
	}

	__forceinline vector3& operator*= (const float v)
	{
		this->x *= v;
		this->y *= v;
		this->z *= v;

		return *this;
	}

	__forceinline vector3& operator/=(const float v)
	{
		this->x /= v;
		this->y /= v;
		this->z /= v;

		return *this;
	}

	__forceinline vector3& operator*= (const vector3& other)
	{
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;

		return *this;
	}

	__forceinline vector3& operator/=(const vector3& other)
	{
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;

		return *this;
	}

	__forceinline vector3& operator+= (const vector3& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;

		return *this;
	}

	__forceinline vector3& operator-=(const vector3& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;

		return *this;
	}

	void normalize()
	{
		float len = std::sqrt(x * x + y * y + z * z);

		if (len != 0.0f)
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}
};

struct vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct m4x4_t
{
	vector4 a1;
	vector4 a2;
	vector4 a3;
	vector4 a4;
};

struct m3x3_t
{
	vector3 right;
	vector3 up;
	vector3 forward;

	m3x3_t() :
		right(1.0f, 0.0f, 0.0f),
		up(0.0f, 1.0f, 0.0f),
		forward(0.0f, 0.0f, 1.0f)
	{}

	m3x3_t(const vector3& r, const vector3& u, const vector3& f)
		: right(r), up(u), forward(f)
	{}

	void identity()
	{
		right = vector3(1.0f, 0.0f, 0.0f);
		up = vector3(0.0f, 1.0f, 0.0f);
		forward = vector3(0.0f, 0.0f, 1.0f);
	}

	void mul(const m3x3_t& other)
	{
		m3x3_t result;
		result.right.x = right.x * other.right.x + up.x * other.right.y + forward.x * other.right.z;
		result.right.y = right.y * other.right.x + up.y * other.right.y + forward.y * other.right.z;
		result.right.z = right.z * other.right.x + up.z * other.right.y + forward.z * other.right.z;

		result.up.x = right.x * other.up.x + up.x * other.up.y + forward.x * other.up.z;
		result.up.y = right.y * other.up.x + up.y * other.up.y + forward.y * other.up.z;
		result.up.z = right.z * other.up.x + up.z * other.up.y + forward.z * other.up.z;

		result.forward.x = right.x * other.forward.x + up.x * other.forward.y + forward.x * other.forward.z;
		result.forward.y = right.y * other.forward.x + up.y * other.forward.y + forward.y * other.forward.z;
		result.forward.z = right.z * other.forward.x + up.z * other.forward.y + forward.z * other.forward.z;

		*this = result;
	}

	// pitch
	void rotateX(float angle1024)
	{
		angle1024 = normalizeAngle1024(angle1024);
		float rad = angle1024 * TWO_PI / 1024.0f;

		m3x3_t rot;
		rot.identity();
		float c = std::cos(rad);
		float s = std::sin(rad);
		rot.up = vector3(0, c, s);
		rot.forward = vector3(0, -s, c);

		mul(rot);
	}

	// yaw
	void rotateY(float angle1024)
	{
		angle1024 = normalizeAngle1024(angle1024);
		float rad = angle1024 * TWO_PI / 1024.0f;

		m3x3_t rot;
		rot.identity();
		float c = std::cos(rad);
		float s = std::sin(rad);
		rot.right = vector3(c, 0, -s);
		rot.forward = vector3(s, 0, c);

		mul(rot);
	}

	// roll
	void rotateZ(float angle1024)
	{
		angle1024 = normalizeAngle1024(angle1024);
		float rad = angle1024 * TWO_PI / 1024.0f;

		m3x3_t rot;
		rot.identity();
		float c = std::cos(rad);
		float s = std::sin(rad);
		rot.right = vector3(c, s, 0);
		rot.up = vector3(-s, c, 0);

		mul(rot);
	}

	void fromEuler(const vector3& angles1024)
	{
		identity();
		rotateY(angles1024.y);
		rotateZ(angles1024.z);
		rotateX(angles1024.x);
	}

	vector3 transform(const vector3& v) const
	{
		return vector3(
			right.x * v.x + up.x * v.y + forward.x * v.z,
			right.y * v.x + up.y * v.y + forward.y * v.z,
			right.z * v.x + up.z * v.y + forward.z * v.z
		);
	}

	void transformInPlace(vector3& v) const
	{
		v = transform(v);
	}

	m3x3_t transpose() const
	{
		return m3x3_t(
			vector3(right.x, up.x, forward.x),
			vector3(right.y, up.y, forward.y),
			vector3(right.z, up.z, forward.z)
		);
	}

	vector3 toEuler() const
	{
		m3x3_t tmp = *this;
		tmp.right.normalize();
		tmp.up.normalize();
		tmp.forward.normalize();

		vector3 radAngles = tmp.toEulerInternal();
		vector3 out;
		out.x = radAngles.x * 1024.0f / TWO_PI;
		out.y = radAngles.z * 1024.0f / TWO_PI;
		out.z = radAngles.y * 1024.0f / TWO_PI;

		return out;
	}

private:
	vector3 toEulerInternal() const
	{
		vector3 out;
		float sin_y = std::asin(right.y);
		float cos_sin = std::cos(sin_y);

		// pitch
		float val1 = up.y / cos_sin;
		if (val1 > 1.0f) val1 = 1.0f;
		if (val1 < -1.0f) val1 = -1.0f;
		float acos1 = std::acos(val1);

		float val2 = -forward.y / cos_sin;
		if (val2 > 1.0f) val2 = 1.0f;
		if (val2 < -1.0f) val2 = -1.0f;
		float asin2 = std::asin(val2);

		out.x = resolveAngleFromSinCos(asin2, acos1);

		// yaw
		float val3 = right.x / cos_sin;
		if (val3 > 1.0f) val3 = 1.0f;
		if (val3 < -1.0f) val3 = -1.0f;
		float acos3 = std::acos(val3);

		float val4 = -right.z / cos_sin;
		if (val4 > 1.0f) val4 = 1.0f;
		if (val4 < -1.0f) val4 = -1.0f;
		float asin4 = std::asin(val4);

		out.y = resolveAngleFromSinCos(asin4, acos3);

		// roll
		float cos_x = std::cos(out.x);
		float val5 = up.y / cos_x;
		if (val5 > 1.0f) val5 = 1.0f;
		if (val5 < -1.0f) val5 = -1.0f;
		float acos5 = std::acos(val5);

		out.z = resolveAngleFromSinCos(sin_y, acos5);

		return out;
	}
};

struct m4x3_t
{
	vector3 vec1;
	vector3 vec2;
	vector3 vec3;
	vector3 vec4;
};

struct color3b_t
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct color6b_t
{
	uint16_t r;
	uint16_t g;
	uint16_t b;
};

extern void GetDirections(vector3* rot, vector3* forward, vector3* right = nullptr, vector3* up = nullptr);
extern void NormalizeAngles(vector3& rot, bool isSource = false);
extern vector3 ToSourceAngles(const vector3& rot);