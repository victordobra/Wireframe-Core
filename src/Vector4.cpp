#include "Math.hpp"

namespace wfe {
	const Vector4 Vector4::zero{ 0.f, 0.f, 0.f, 0.f };
	const Vector4 Vector4::one { 1.f, 1.f, 1.f, 1.f };

	Vector4::Vector4() : x(0.f), y(0.f), z(0.f), w(0) { }
	Vector4::Vector4(float32_t x) : x(x), y(x), z(x), w(x) { }
	Vector4::Vector4(float32_t x, float32_t y, float32_t z, float32_t w) : x(x), y(y), z(z), w(w) { }
	Vector4::Vector4(float32_t elements[4]) : x(elements[0]), y(elements[1]), z(elements[2]), w(elements[3]) { }

	Vector4::Vector4(const Vector2& other) : x(other.x), y(other.y), z(0.f), w(0.f) { }
	Vector4::Vector4(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(0.f) { }
	Vector4::Vector4(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }

	bool8_t Vector4::operator==(const Vector4& other) const {
		// Compare each element
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}
	bool8_t Vector4::operator!=(const Vector4& other) const {
		// Compare each element
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	Vector4 Vector4::operator+(const Vector4& other) const {
		// Calculate the resulting vector's dimensions
		return { x + other.x, y + other.y, z + other.z, w + other.w };
	}
	Vector4 Vector4::operator-(const Vector4& other) const {
		// Calculate the resulting vector's dimensions
		return { x - other.x, y - other.y, z - other.z, w + other.w };
	}
	Vector4 Vector4::operator-() const {
		// Calculate the resulting vector's dimensions
		return { -x, -y, -z, -w };
	}
	Vector4 Vector4::operator*(const Vector4& other) const {
		// Calculate the resulting vector's dimensions
		return { x * other.x, y * other.y, z * other.z, w * other.w };
	}
	Vector4 Vector4::operator/(const Vector4& other) const {
		// Calculate the resulting vector's dimensions
		return { x / other.x, y / other.y, z / other.z, w / other.w };
	}

	Vector4& Vector4::operator+=(const Vector4& other) {
		// Increase each element
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}
	Vector4& Vector4::operator-=(const Vector4& other) {
		// Decrease each element
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}
	Vector4& Vector4::operator*=(const Vector4& other) {
		// Multiply each element
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}
	Vector4& Vector4::operator*=(float32_t other) {
		// Multiply each element
		x *= other;
		y *= other;
		z *= other;
		w *= other;

		return *this;
	}
	Vector4& Vector4::operator/=(const Vector4& other) {
		// Divide each element
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}
	Vector4& Vector4::operator/=(float32_t other) {
		// Calculate the inverse of the given value
		float32_t invOther = 1.f / other;

		// Multiply each element with the inverse of the given value
		x *= invOther;
		y *= invOther;
		z *= invOther;
		w *= invOther;

		return *this;
	}

	float32_t Vector4::Magnitude() const {
		// Calculate the Vector4's magnitude
		return Sqrt(x * x + y * y + z * z + w * w);
	}
	float32_t Vector4::SqrMagnitude() const {
		// Calculate the Vector4's square magnitude
		return x * x + y * y + z * z + w * w;
	}
	float32_t Vector4::InvMagnitude() const {
		// Calculate the Vector4's inverse magnitude using the fast inverse square root function
		return InvSqrt(x * x + y * y + z * z + w * w);
	}

	Vector4& Vector4::Normalize() {
		// Calculate the Vector4's inverse magnitude
		float32_t invMag = InvMagnitude();

		// Multiply each element by the inverse magnitude
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;

		return *this;
	}
	Vector4 Vector4::Normalized() const {
		// Calculate the Vector4's inverse magnitude
		float32_t invMag = InvMagnitude();

		// Calculate the resulting Vector4's elements
		return { x * invMag, y * invMag, z * invMag, w * invMag };
	}
	Vector4& Vector4::Flip() {
		// Set each element to its opposite
		x = -x;
		y = -y;
		z = -z;
		w = -w;

		return *this;
	}
	Vector4 Vector4::Flipped() const {
		// Calculate the resulting Vector4's elements
		return { -x, -y, -z, -w };
	}

	float32_t Vector4::Dot(const Vector4& other) const {
		// Calculate the dot product
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
	float32_t Vector4::Angle(const Vector4& other) const {
		// Calculate the dot product
		float32_t dot = Dot(other);

		// Calculate the cosine of the angle using the dot product
		float32_t angleCos = dot * InvMagnitude() * other.InvMagnitude();

		// Calculate the angle between the two Vector4s 
		return Acos(angleCos);
	}

	Vector4 operator*(const Vector4& vec, float32_t val) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val, vec.z * val, vec.w * val };
	}
	Vector4 operator*(float32_t val, const Vector4& vec) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val, vec.z * val, vec.w * val };
	}
	Vector4 operator/(const Vector4& vec, float32_t val) {
		// Calculate the inverse of the given value
		float32_t invVal = 1.f / val;

		// Calculate the resulting vector's dimensions
		return { vec.x * invVal, vec.y * invVal, vec.z * invVal, vec.w * invVal };
	}
	Vector4 operator/(float32_t val, const Vector4& vec) {
		// Calculate the resulting vector's dimensions
		return { val / vec.x, val / vec.y, val / vec.z, val / vec.w };
	}
}