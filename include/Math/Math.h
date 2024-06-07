#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <stdint.h>
#include <limits>
#include <unordered_map>
namespace MathLib
{
#ifdef USE_DOUBLE_REAL
	typedef double HReal;
	static HReal H_PI = 3.14159265358979323846;
#else
	typedef float HReal;
	static HReal H_PI = 3.14159265358979323846f;
#endif

	typedef Eigen::Matrix<HReal, 2, 1> HVector2;
	typedef Eigen::Matrix<HReal, 3, 1> HVector3;
	typedef Eigen::Matrix<HReal, 4, 1> HVector4;

	typedef Eigen::Matrix<unsigned int, 2, 1> HVector2UI;
	typedef Eigen::Matrix<unsigned int, 3, 1> HVector3UI;
	typedef Eigen::Matrix<unsigned int, 4, 1> HVector4UI;

	typedef Eigen::Matrix<int, 2, 1> HVector2I;
	typedef Eigen::Matrix<int, 3, 1> HVector3I;
	typedef Eigen::Matrix<int, 4, 1> HVector4I;

	typedef Eigen::Matrix<HReal, Eigen::Dynamic, 1> HVectorX;

	typedef Eigen::Matrix<HReal, 2, 2> HMatrix2;
	typedef Eigen::Matrix<HReal, 3, 3> HMatrix3;
	typedef Eigen::Matrix<HReal, 4, 4> HMatrix4;

	typedef Eigen::Matrix<HReal, Eigen::Dynamic, Eigen::Dynamic> HMatrixX;

	typedef Eigen::SparseMatrix<HReal> HSparseMatrix;
	typedef Eigen::SparseMatrix<HReal, Eigen::RowMajor> HSparseMatrixRowMajor;

	typedef Eigen::AlignedBox<HReal, 2> HAABBox2D;
	typedef Eigen::AlignedBox<HReal, 3> HAABBox3D;

	typedef Eigen::AngleAxis<HReal> HAngleAxis;

	typedef Eigen::Translation<HReal, 2> HTranslation2;
	typedef Eigen::Translation<HReal, 3> HTranslation3;

	typedef Eigen::Quaternion<HReal> HQuaternion;

	typedef Eigen::Transform<HReal, 2, Eigen::Affine> HTransform2;
	typedef Eigen::Transform<HReal, 3, Eigen::Affine> HTransform3;

	template <typename target_ptr, typename source>
	inline target_ptr smart_cast(source *s)
	{
#ifdef NDEBUG
		return static_cast<target_ptr>(s);
#else
		target_ptr t = dynamic_cast<target_ptr>(s);
		assert(t != NULL);
		return t;
#endif
	}
}
