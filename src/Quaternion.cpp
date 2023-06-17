#include "Quaternion.hpp"
#include "Math.hpp"

namespace wfe {
	const Quaternion Quaternion::identity{ 0.f, 0.f, 0.f, 1.f };

	Quaternion::Quaternion() : x(0.f), y(0.f), z(0.f), w(1.f) { }
	Quaternion::Quaternion(float32_t x, float32_t y, float32_t z, float32_t w) : x(x), y(y), z(z), w(w) { } 
	Quaternion::Quaternion(const float32_t elements[4]) : x(elements[0]), y(elements[1]), z(elements[2]), w(elements[3]) { }

	Quaternion::Quaternion(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }

	bool8_t Quaternion::operator==(const Quaternion& other) const {
		// Compare the two quaternion's elements
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}
	bool8_t Quaternion::operator!=(const Quaternion& other) const {
		// Compare the two quaternion's elements
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	Quaternion Quaternion::operator*(const Quaternion& other) const {
		// Calculate the resulting quaternion's elements
		return {  x * other.w + y * other.z - z * other.y + w * other.x,
		         -x * other.z + y * other.w + z * other.x + w * other.y,
		          x * other.y - y * other.x + z * other.w + w * other.z,
		         -x * other.x - y * other.y - z * other.z + w * other.w  };
	}
	Quaternion& Quaternion::operator*=(const Quaternion& other) {
		// Save the quaternion's current elements
		float32_t x = this->x, y = this->y, z = this->z, w = this->w;

		// Set its new elements
		this->x =  x * other.w + y * other.z - z * other.y + w * other.x;
		this->y = -x * other.z + y * other.w + z * other.x + w * other.y;
		this->z =  x * other.y - y * other.x + z * other.w + w * other.z;
		this->w = -x * other.x - y * other.y - z * other.z + w * other.w;

		return *this;
	}

	float32_t Quaternion::Magnitude() const {
		// Calculate the quaternion's magnitude
		return Sqrt(x * x + y * y + z * z + w * w);
	}
	float32_t Quaternion::SqrMagnitude() const {
		// Calculate the quaternion's magnitude
		return x * x + y * y + z * z + w * w;
	}
	float32_t Quaternion::InvMagnitude() const {
		// Calculate the quaternion's inverse magnitude using the fast inverse square root function
		return InvSqrt(x * x + y * y + z * z + w * w);
	}

	Quaternion& Quaternion::Normalize() {
		// Calculate the quaternion's inverse magnitude
		float32_t invMag = InvMagnitude();

		// Multiply every element by the inverse magnitude
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;

		return *this;
	}
	Quaternion Quaternion::Normalized() const {
		// Calculate the quaternion's inverse magnitude
		float32_t invMag = InvMagnitude();

		// Calculate the resulting quaternion's elements
		return { x * invMag, y * invMag, z * invMag, w * invMag };
	}
	Quaternion& Quaternion::Invert() {
		// Invert every element of the quaternion
		x = -x;
		y = -y;
		z = -z;
		w = -w;

		return *this;
	}
	Quaternion Quaternion::Inverted() const {
		// Calculate the resulting quaternion's elements
		return { -x, -y, -z, -w };
	}

	float32_t Quaternion::Dot(const Quaternion& other) const {
		// Calculate the two quaternions' dot product
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	Quaternion Quaternion::AroundAxis(Vector3 axis, float32_t angle) {
		// Calculate the sine and cosine of the given angle
		float32_t halfAngle = angle * .5f;
		float32_t halfSin = Sin(halfAngle), halfCos = Cos(halfAngle);

		// Calculate the resulting quaternion's coordinates
		return { axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos };
	}
	Quaternion Quaternion::EulerAngles(Vector3 eulerAngles) {
		// Calculate the halves of all euler angles
		float32_t halfX = eulerAngles.x * .5f;
		float32_t halfY = eulerAngles.y * .5f;
		float32_t halfZ = eulerAngles.z * .5f;

		// Calculate the sines and cosines of the angle halves
		float32_t sinX = Sin(halfX), cosX = Cos(halfX);
		float32_t sinY = Sin(halfY), cosY = Cos(halfY);
		float32_t sinZ = Sin(halfZ), cosZ = Cos(halfZ);

		// Calculate the elements of the resulting quaternion
		return { sinX * cosY * cosZ - cosX * sinY * sinZ,
		         cosX * sinY * cosZ + sinX * cosY * sinZ,
		         cosX * cosY * sinZ + sinX * sinY * cosZ,
		         cosX * cosY * cosZ - sinX * sinY * sinZ  };
	}
}