#include "Math.hpp"

namespace wfe {
	const Vector2 Vector2::zero {  0.f,  0.f };
	const Vector2 Vector2::one  {  1.f,  1.f };
	const Vector2 Vector2::up   {  0.f,  1.f };
	const Vector2 Vector2::down {  0.f, -1.f };
	const Vector2 Vector2::left { -1.f,  0.f };
	const Vector2 Vector2::right{  1.f,  0.f };

	Vector2::Vector2() : x(0.f), y(0.f) { }
	Vector2::Vector2(float32_t x) : x(x), y(x) { }
	Vector2::Vector2(float32_t x, float32_t y) : x(x), y(y) { }
	Vector2::Vector2(float32_t elements[2]) : x(elements[0]), y(elements[1]) { }

	Vector2::Vector2(const Vector3& other) : x(other.x), y(other.y) { }
	Vector2::Vector2(const Vector4& other) : x(other.x), y(other.y) { }

	bool8_t Vector2::operator==(const Vector2& other) const {
		// Compare each element
		return x == other.x && y == other.y;
	}
	bool8_t Vector2::operator!=(const Vector2& other) const {
		// Compare each element
		return x != other.x || y != other.y;
	}

	Vector2 Vector2::operator+(const Vector2& other) const {
		// Calculate the resulting vector's dimensions
		return { x + other.x, y + other.y };
	}
	Vector2 Vector2::operator-(const Vector2& other) const {
		// Calculate the resulting vector's dimensions
		return { x - other.x, y - other.y };
	}
	Vector2 Vector2::operator-() const {
		// Calculate the resulting vector's dimensions
		return { -x, -y };
	}
	Vector2 Vector2::operator*(const Vector2& other) const {
		// Calculate the resulting vector's dimensions
		return { x * other.x, y * other.y };
	}
	Vector2 Vector2::operator/(const Vector2& other) const {
		// Calculate the resulting vector's dimensions
		return { x / other.x, y / other.y };
	}

	Vector2& Vector2::operator+=(const Vector2& other) {
		// Increase each element
		x += other.x;
		y += other.y;

		return *this;
	}
	Vector2& Vector2::operator-=(const Vector2& other) {
		// Decrease each element
		x -= other.x;
		y -= other.y;

		return *this;
	}
	Vector2& Vector2::operator*=(const Vector2& other) {
		// Multiply each element
		x *= other.x;
		y *= other.y;

		return *this;
	}
	Vector2& Vector2::operator*=(float32_t other) {
		// Multiply each element
		x *= other;
		y *= other;

		return *this;
	}
	Vector2& Vector2::operator/=(const Vector2& other) {
		// Divide each element
		x /= other.x;
		y /= other.y;

		return *this;
	}
	Vector2& Vector2::operator/=(float32_t other) {
		// Calculate the inverse of the given value
		float32_t invOther = 1.f / other;

		// Multiply each element with the inverse of the given value
		x *= invOther;
		y *= invOther;

		return *this;
	}

	float32_t Vector2::Magnitude() const {
		// Calculate the Vector2's magnitude
		return Sqrt(x * x + y * y);
	}
	float32_t Vector2::SqrMagnitude() const {
		// Calculate the Vector2's square magnitude
		return x * x + y * y;
	}
	float32_t Vector2::InvMagnitude() const {
		// Calculate the Vector2's inverse magnitude using the fast inverse square root function
		return InvSqrt(x * x + y * y);
	}

	Vector2& Vector2::Normalize() {
		// Calculate the Vector2's inverse magnitude
		float32_t invMag = InvMagnitude();

		// Multiply each element by the inverse magnitude
		x *= invMag;
		y *= invMag;

		return *this;
	}
	Vector2 Vector2::Normalized() const {
		// Calculate the Vector2's inverse magnitude
		float32_t invMag = InvMagnitude();

		// Calculate the resulting Vector2's elements
		return { x * invMag, y * invMag };
	}
	Vector2& Vector2::Flip() {
		// Set each element to its opposite
		x = -x;
		y = -y;

		return *this;
	}
	Vector2 Vector2::Flipped() const {
		// Calculate the resulting Vector2's elements
		return { -x, -y };
	}

	float32_t Vector2::Dot(const Vector2& other) const {
		// Calculate the dot product
		return x * other.x + y * other.y;
	}
	float32_t Vector2::Angle(const Vector2& other) const {
		// Calculate the dot product
		float32_t dot = Dot(other);

		// Calculate the cosine of the angle using the dot product
		float32_t angleCos = dot * InvMagnitude() * other.InvMagnitude();

		// Calculate the angle between the two Vector2s 
		return Acos(angleCos);
	}

	Vector2 operator*(const Vector2& vec, float32_t val) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val };
	}
	Vector2 operator*(float32_t val, const Vector2& vec) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val };
	}
	Vector2 operator/(const Vector2& vec, float32_t val) {
		// Calculate the inverse of the given value
		float32_t invVal = 1.f / val;

		// Calculate the resulting vector's dimensions
		return { vec.x * invVal, vec.y * invVal };
	}
	Vector2 operator/(float32_t val, const Vector2& vec) {
		// Calculate the resulting vector's dimensions
		return { val / vec.x, val / vec.y };
	}
}