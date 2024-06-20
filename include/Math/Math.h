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

	template <uint32_t N>
	using HVector = Eigen::Matrix<HReal, N, 1>;
	typedef HVector<2> HVector2;
	typedef HVector<3> HVector3;
	typedef HVector<4> HVector4;
	typedef HVector<Eigen::Dynamic> HVectorX;

	template <uint32_t N>
	using HVectorI = Eigen::Matrix<int, N, 1>;
	typedef HVectorI<2> HVector2I;
	typedef HVectorI<3> HVector3I;
	typedef HVectorI<4> HVector4I;
	typedef HVectorI<Eigen::Dynamic> HVectorXI;

	template <uint32_t N>
	using HVectorUI = Eigen::Matrix<unsigned int, N, 1>;
	typedef HVectorUI<2> HVector2UI;
	typedef HVectorUI<3> HVector3UI;
	typedef HVectorUI<4> HVector4UI;
	typedef HVectorUI<Eigen::Dynamic> HVectorXUI;

	template <uint32_t N>
	using HMatrix = Eigen::Matrix<HReal, N, N>;
	typedef HMatrix<2> HMatrix2;
	typedef HMatrix<3> HMatrix3;
	typedef HMatrix<4> HMatrix4;
	typedef HMatrix<Eigen::Dynamic> HMatrixX;

	typedef Eigen::SparseMatrix<HReal> HSparseMatrix;
	typedef Eigen::SparseMatrix<HReal, Eigen::RowMajor> HSparseMatrixRowMajor;

	template <uint32_t N>
	using HAABBox = Eigen::AlignedBox<HReal, N>;
	typedef HAABBox<2> HAABBox2D;
	typedef HAABBox<3> HAABBox3D;

	typedef Eigen::AngleAxis<HReal> HAngleAxis;

	template <uint32_t N>
	using HTranslation = Eigen::Translation<HReal, N>;
	typedef HTranslation<2> HTranslation2;
	typedef HTranslation<3> HTranslation3;

	typedef Eigen::Quaternion<HReal> HQuaternion;

	template <uint32_t N>
	using HTransform = Eigen::Transform<HReal, N, Eigen::Affine>;
	typedef HTransform<2> HTransform2;
	typedef HTransform<3> HTransform3;

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

#include <Math/Epsilon.h>