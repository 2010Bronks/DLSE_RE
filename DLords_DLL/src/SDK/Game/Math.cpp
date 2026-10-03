#include "precompiled.hpp"

inline void SinCosX(const float rad, float& sin, float& cos)
{
	const __m128 _ps_fopi = _mm_set_ss(1.27323954473516f);

	const __m128 _ps_0p5 = _mm_set_ss(0.5f);
	const __m128 _ps_1 = _mm_set_ss(1.0f);

	const __m128 _ps_dp1 = _mm_set_ss(-0.7851562f);
	const __m128 _ps_dp2 = _mm_set_ss(-2.4187564849853515625e-4f);
	const __m128 _ps_dp3 = _mm_set_ss(-3.77489497744594108e-8f);

	const __m128 _ps_sincof_p0 = _mm_set_ss(2.443315711809948e-5f);
	const __m128 _ps_sincof_p1 = _mm_set_ss(8.3321608736e-3f);
	const __m128 _ps_sincof_p2 = _mm_set_ss(-1.6666654611e-1f);
	const __m128 _ps_coscof_p0 = _mm_set_ss(2.443315711809948e-5f);
	const __m128 _ps_coscof_p1 = _mm_set_ss(-1.388731625493765e-3f);
	const __m128 _ps_coscof_p2 = _mm_set_ss(4.166664568298827e-2f);

	const __m128i _pi32_1 = _mm_set1_epi32(1);
	const __m128i _pi32_i1 = _mm_set1_epi32(~1);
	const __m128i _pi32_2 = _mm_set1_epi32(2);
	const __m128i _pi32_4 = _mm_set1_epi32(4);

	auto raw = _mm_set1_epi32(0x80000000);
	auto inv = _mm_set1_epi32(~0x80000000);

	const __m128 _mask_sign_raw = *(__m128*) & raw;
	const __m128 _mask_sign_inv = *(__m128*) & inv;

	__m128  xmm3 = _mm_setzero_ps();
	__m128i emm0, emm2, emm4;

	__m128 sign_bit_cos, sign_bit_sin;

	__m128 x, y, z;
	__m128 y1, y2;

	__m128 a = _mm_set_ss(rad);

	x = _mm_and_ps(a, _mask_sign_inv);
	y = _mm_mul_ps(x, _ps_fopi);

	emm2 = _mm_cvtps_epi32(y);
	emm2 = _mm_add_epi32(emm2, _pi32_1);
	emm2 = _mm_and_si128(emm2, _pi32_i1);
	y = _mm_cvtepi32_ps(emm2);

	emm4 = emm2;

	emm0 = _mm_and_si128(emm2, _pi32_4);
	emm0 = _mm_slli_epi32(emm0, 29);
	__m128 swap_sign_bit_sin = _mm_castsi128_ps(emm0);

	emm2 = _mm_and_si128(emm2, _pi32_2);
	emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());
	__m128 poly_mask = _mm_castsi128_ps(emm2);

	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp1));
	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp2));
	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp3));

	emm4 = _mm_sub_epi32(emm4, _pi32_2);
	emm4 = _mm_andnot_si128(emm4, _pi32_4);
	emm4 = _mm_slli_epi32(emm4, 29);

	sign_bit_cos = _mm_castsi128_ps(emm4);
	sign_bit_sin = _mm_xor_ps(_mm_and_ps(a, _mask_sign_raw), swap_sign_bit_sin);

	z = _mm_mul_ps(x, x);

	y1 = _mm_mul_ps(_ps_coscof_p0, z);
	y1 = _mm_add_ps(y1, _ps_coscof_p1);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_add_ps(y1, _ps_coscof_p2);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_sub_ps(y1, _mm_mul_ps(z, _ps_0p5));
	y1 = _mm_add_ps(y1, _ps_1);

	y2 = _mm_mul_ps(_ps_sincof_p0, z);
	y2 = _mm_add_ps(y2, _ps_sincof_p1);
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_add_ps(y2, _ps_sincof_p2);
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_mul_ps(y2, x);
	y2 = _mm_add_ps(y2, x);

	xmm3 = poly_mask;

	__m128 ysin2 = _mm_and_ps(xmm3, y2);
	__m128 ysin1 = _mm_andnot_ps(xmm3, y1);

	sin = _mm_cvtss_f32(_mm_xor_ps(_mm_add_ps(ysin1, ysin2), sign_bit_sin));
	cos = _mm_cvtss_f32(_mm_xor_ps(_mm_add_ps(_mm_sub_ps(y1, ysin1), _mm_sub_ps(y2, ysin2)), sign_bit_cos));
}

void GetDirections(vector3* rot, vector3* forward, vector3* right, vector3* up)
{
	if (!rot || !forward)
		return;

	float sr, sp, sy, cr, cp, cy;

	SinCosX(DEG2RAD(rot->x), sp, cp);
	SinCosX(DEG2RAD(rot->y), sy, cy);
	SinCosX(DEG2RAD(rot->z), sr, cr);

	if (right)
	{
		right->x = cp * cy;
		right->y = -sp;
		right->z = cp * sy;
	}

	if (forward)
	{
		forward->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		forward->y = -1 * sr * cp;
		forward->z = (-1 * sr * sp * sy + -1 * cr * cy);

		*forward *= -1.0f;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = cr * cp;
		up->z = (cr * sp * sy + -sr * cy);
	}
}

static float NormalizeYaw(float yaw, bool isSource = false)
{
	const float del = isSource ? 360.0f : 1024.0f;

	float fRev = yaw / del;

	if (fRev < 0.0f)
	{
		fRev = -fRev;
	}

	fRev = (float)round(fRev);

	if (yaw < 0.0f)
	{
		yaw = (yaw + del * fRev);
	}
	else
	{
		yaw = (yaw - del * fRev);
	}

	if (!isSource && yaw < 0.0f)
		yaw += del;

	return yaw;
}

void NormalizeAngles(vector3& rot, bool isSource)
{
	rot.x = NormalizeYaw(rot.x, isSource);
	rot.y = NormalizeYaw(rot.y, isSource);
	rot.z = NormalizeYaw(rot.z, isSource);
}

vector3 ToSourceAngles(const vector3& rot)
{
	vector3 buf = rot;

	NormalizeAngles(buf);

	buf /= 1024.0f;
	buf *= 360.0f;

	NormalizeAngles(buf, true);
	buf.y *= -1.0f;


	return buf;
}

float normalizeAngle1024(float angle)
{
	while (angle >= 1024.0f)
		angle -= 1024.0f;

	while (angle < 0.0f)
		angle += 1024.0f;

	return angle;
}

float resolveAngleFromSinCos(float angle_from_asin, float angle_from_acos)
{
	float diff1 = std::fabs(angle_from_acos - angle_from_asin);
	float diff2 = std::fabs(angle_from_asin + angle_from_acos);
	float diff3 = std::fabs(angle_from_acos - (PI - angle_from_asin));

	float min_diff = diff1;
	if (diff3 <= min_diff)
		min_diff = diff3;

	if (diff2 <= min_diff)
		return -angle_from_acos;

	return angle_from_acos;
}