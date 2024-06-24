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

	template <typename Type, int N>
	using HVector = Eigen::Matrix<Type, N, 1>;

	template <int N>
	using HVectorR = HVector<HReal, N>;
	typedef HVectorR<2> HVector2;
	typedef HVectorR<3> HVector3;
	typedef HVectorR<4> HVector4;
	typedef HVectorR<Eigen::Dynamic> HVectorX;

	template <int N>
	using HVectorI = HVector<int, N>;
	typedef HVectorI<2> HVector2I;
	typedef HVectorI<3> HVector3I;
	typedef HVectorI<4> HVector4I;
	typedef HVectorI<Eigen::Dynamic> HVectorXI;

	template <int N>
	using HVectorUI = HVector<unsigned int, N>;
	typedef HVectorUI<2> HVector2UI;
	typedef HVectorUI<3> HVector3UI;
	typedef HVectorUI<4> HVector4UI;
	typedef HVectorUI<Eigen::Dynamic> HVectorXUI;

	template <int N>
	using HMatrix = Eigen::Matrix<HReal, N, N>;
	typedef HMatrix<2> HMatrix2;
	typedef HMatrix<3> HMatrix3;
	typedef HMatrix<4> HMatrix4;
	typedef HMatrix<Eigen::Dynamic> HMatrixX;

	template <int Option>
	using HSparseMatrix = Eigen::SparseMatrix<HReal, Option>;
	typedef HSparseMatrix<Eigen::ColMajor> HSparseMatrixColMajor;
	typedef HSparseMatrix<Eigen::RowMajor> HSparseMatrixRowMajor;

	template <int N>
	using HAABBox = Eigen::AlignedBox<HReal, N>;
	typedef HAABBox<2> HAABBox2D;
	typedef HAABBox<3> HAABBox3D;
	typedef HAABBox<Eigen::Dynamic> HAABBoxXD;

	using HAngleAxis = Eigen::AngleAxis<HReal>;
	template <int N>
	using HTranslation = Eigen::Translation<HReal, N>;
	typedef HTranslation<2> HTranslation2;
	typedef HTranslation<3> HTranslation3;
	typedef HTranslation<Eigen::Dynamic> HTranslationX;

	using HQuaternion = Eigen::Quaternion<HReal>;
	template <int N>
	using HTransform = Eigen::Transform<HReal, N, Eigen::Affine>;
	typedef HTransform<2> HTransform2;
	typedef HTransform<3> HTransform3;
	typedef HTransform<Eigen::Dynamic> HTransformX;

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
#include <Math/Parallel.h>