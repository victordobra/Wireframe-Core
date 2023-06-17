#include "Math.hpp"

namespace wfe {
	const Vector3 Vector3::zero   {  0.f,  0.f,  0.f };
	const Vector3 Vector3::one    {  1.f,  1.f,  1.f };
	const Vector3 Vector3::up     {  0.f,  1.f,  0.f };
	const Vector3 Vector3::down   {  0.f, -1.f,  0.f };
	const Vector3 Vector3::left   { -1.f,  0.f,  0.f };
	const Vector3 Vector3::right  {  1.f,  0.f,  0.f };
    const Vector3 Vector3::forward{  0.f,  0.f, -1.f };
    const Vector3 Vector3::back   {  0.f,  0.f,  1.f };

	Vector3::Vector3() : x(0.f), y(0.f), z(0.f) { }
	Vector3::Vector3(float32_t x) : x(x), y(x), z(x) { }
	Vector3::Vector3(float32_t x, float32_t y) : x(x), y(y), z(0.f) { }
	Vector3::Vector3(float32_t x, float32_t y, float32_t z) : x(x), y(y), z(z) { }
	Vector3::Vector3(const float32_t elements[3]) : x(elements[0]), y(elements[1]), z(elements[2]) { }

	Vector3::Vector3(const Vector2& other) : x(other.x), y(other.y), z(0.f) { }
	Vector3::Vector3(const Vector4& other) : x(other.x), y(other.y), z(other.z) { }

	bool8_t Vector3::operator==(const Vector3& other) const {
		// Compare each element
		return x == other.x && y == other.y && z == other.z;
	}
	bool8_t Vector3::operator!=(const Vector3& other) const {
		// Compare each element
		return x != other.x || y != other.y || z != other.z;
	}

	Vector3 Vector3::operator+(const Vector3& other) const {
		// Calculate the resulting vector's dimensions
		return { x + other.x, y + other.y, z + other.z };
	}
	Vector3 Vector3::operator-(const Vector3& other) const {
		// Calculate the resulting vector's dimensions
		return { x - other.x, y - other.y, z - other.z };
	}
	Vector3 Vector3::operator-() const {
		// Calculate the resulting vector's dimensions
		return { -x, -y, -z };
	}
	Vector3 Vector3::operator*(const Vector3& other) const {
		// Calculate the resulting vector's dimensions
		return { x * other.x, y * other.y, z * other.z };
	}
	Vector3 Vector3::operator/(const Vector3& other) const {
		// Calculate the resulting vector's dimensions
		return { x / other.x, y / other.y, z / other.z };
	}

	Vector3& Vector3::operator+=(const Vector3& other) {
		// Increase each element
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}
	Vector3& Vector3::operator-=(const Vector3& other) {
		// Decrease each element
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}
	Vector3& Vector3::operator*=(const Vector3& other) {
		// Multiply each element
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}
	Vector3& Vector3::operator*=(float32_t other) {
		// Multiply each element
		x *= other;
		y *= other;
		z *= other;

		return *this;
	}
	Vector3& Vector3::operator/=(const Vector3& other) {
		// Divide each element
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}
	Vector3& Vector3::operator/=(float32_t other) {
		// Calculate the inverse of the given value
		float32_t invOther = 1.f / other;

		// Multiply each element with the inverse of the given value
		x *= invOther;
		y *= invOther;
		z *= invOther;

		return *this;
	}

	float32_t Vector3::Magnitude() const {
		// Calculate the Vector3's magnitude
		return Sqrt(x * x + y * y + z * z);
	}
	float32_t Vector3::SqrMagnitude() const {
		// Calculate the Vector3's square magnitude
		return x * x + y * y + z * z;
	}
	float32_t Vector3::InvMagnitude() const {
		// Calculate the Vector3's inverse magnitude using the fast inverse square root function
		return InvSqrt(x * x + y * y + z * z);
	}

	Vector3& Vector3::Normalize() {
		// Calculate the Vector3's inverse magnitude
		float32_t invMag = InvMagnitude();

		// Multiply each element by the inverse magnitude
		x *= invMag;
		y *= invMag;
		z *= invMag;

		return *this;
	}
	Vector3 Vector3::Normalized() const {
		// Calculate the Vector3's inverse magnitude
		float32_t invMag = InvMagnitude();

		// Calculate the resulting Vector3's elements
		return { x * invMag, y * invMag, z * invMag };
	}
	Vector3& Vector3::Flip() {
		// Set each element to its opposite
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}
	Vector3 Vector3::Flipped() const {
		// Calculate the resulting Vector3's elements
		return { -x, -y, -z };
	}

	float32_t Vector3::Dot(const Vector3& other) const {
		// Calculate the dot product
		return x * other.x + y * other.y + z * other.z;
	}
	float32_t Vector3::Angle(const Vector3& other) const {
		// Calculate the dot product
		float32_t dot = Dot(other);

		// Calculate the cosine of the angle using the dot product
		float32_t angleCos = dot * InvMagnitude() * other.InvMagnitude();

		// Calculate the angle between the two Vector3s 
		return Acos(angleCos);
	}
    Vector3 Vector3::Cross(const Vector3& other) const {
        // Calculate the cross product
        return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
    }

	Vector3 operator*(const Vector3& vec, float32_t val) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val, vec.z * val };
	}
	Vector3 operator*(float32_t val, const Vector3& vec) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val, vec.z * val };
	}
	Vector3 operator/(const Vector3& vec, float32_t val) {
		// Calculate the inverse of the given value
		float32_t invVal = 1.f / val;

		// Calculate the resulting vector's dimensions
		return { vec.x * invVal, vec.y * invVal, vec.z * invVal };
	}
	Vector3 operator/(float32_t val, const Vector3& vec) {
		// Calculate the resulting vector's dimensions
		return { val / vec.x, val / vec.y, val / vec.z };
	}
}