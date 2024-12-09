#pragma once
#include <Math/Math.h>
#include <Math/Array3D.h>
namespace MathLib
{
	class LevelSet3D
	{
	private:
		HReal PointTriangleDistance(const HVector3 &p, const HVector3 &a, const HVector3 &b, const HVector3 &c, HReal &t1, HReal &t2, HReal &t3)
		{
			HVector3 ab, ac, ap, bp;

			ab = b - a;
			ac = c - a;
			ap = p - a;

			HReal d1 = ab.dot(ap);
			HReal d2 = ac.dot(ap);

			if (d1 <= 0.0f && d2 <= 0
							t1 = 1.0f;
				t2 = 0.0f;
				t3 = 0.0f;
				return (p - a).norm();
			}

			bp = p - b;
			HReal d3 = ab.dot(bp);
			HReal d4 = ac.dot(bp);

			if (d3 >= 0.0f && d4 <= d3)
			{
				t1 = 0.0f;
				t2 = 1.0f;
				t3 = 0.0f;
				return (p - b).norm();
			}

			HReal vc = d1 * d4 - d3 * d2;

			if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
			{
				HReal v = d1 / (d1 - d3);
				t1 = 1.0f - v;
				t2 = v;
				t3 = 0.0f;
				return (p - (a + v * ab)).norm();
			}

			HVector3 cp = p - c;
			HReal d5 = ab.dot(cp);
			HReal d6 = ac.dot(cp);

			if (d6 >= 0.0f && d5 <= d6)
			{
				t1 = 0.0f;
				t2 = 0.0f;
				t3 = 1.0f;
				return (p - c).norm();
			}

			HReal vb = d5 * d2 - d1 * d6;

			if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
			{
				HReal w = d2 / (d2 - d6);
				t1 = 0.0f;
				t2 = 1.0f - w;
				t3 = w;
				return (p - (a + w * ac)).norm();
			}

			HReal va = d3 * d6 - d5 * d4;

			if ((va <= 0.0f) && (d4 - d3 >= 0.0f) && (d5 - d6 >= 0.0f))
			{
				HReal w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
				t1 = 0.0f;
				t2 = 1.0f - w;
				t3 = w;
				return (p - (b + w * (c - b))).norm();
			}

			HReal denom = 1.0f / (va + vb + vc);
			HReal v = vb * denom;
			HReal w = vc * denom;
			t1 = 1.0f - v - w;
			t2 = v;
			t3 = w;

			HVector3 closest = a + ab * v + ac * w;
			return (p - closest).norm();
		}

		void CheckNeighbor(const std::vector<HVector3UI> &tri,
						   const std::vector<HVector3> &x,
						   const std::vector<HVector3> &v,
						   Array3D<HReal> &phi,
						   Array3D<HVector3> &phiVel,
						   Array3D<int> &closetTri,
						   const HVector3 &gx,
						   int i0, int j0, int k0, int i1, int j1, int k1)
		{
			if (closetTri(i1, j1, k1) >= 0)
			{
				const HVector3UI &curTri = tri[closetTri(i1, j1, k1)];
				unsigned int p0 = curTri[0];
				unsigned int p1 = curTri[1];
				unsigned int p2 = curTri[2];
				HVector3 p = gx;
				HReal t1, t2, t3;
				HReal d = PointTriangleDistance(p, x[p0], x[p1], x[p2], t1, t2, t3);
				if (d < phi(i0, j0, k0))
				{
					phi(i0, j0, k0) = d;
					phiVel(i0, j0, k0) = t1 * v[p0] + t2 * v[p1] + t3 * v[p2];
					closetTri(i0, j0, k0) = closetTri(i1, j1, k1);
				}
			}
		}

		void Sweep(const std::vector<HVector3UI> &tri,
				   const std::vector<HVector3> &x,
				   const std::vector<HVector3> &v,
				   Array3D<HReal> &phi,
				   Array3D<HVector3> &phiVel,
				   Array3D<int> &closetTri,
				   const HVector3 &origin,
				   HReal dx,
				   int di, int dj, int dk)
		{
			int i0, i1;
			if (di > 0)
			{
				i0 = 1;
				i1 = phi.getSizeX();
			}
			else
			{
				i0 = phi.getSizeX() - 2;
				i1 = -1;
			}
			int j0, j1;
			if (dj > 0)
			{
				j0 = 1;
				j1 = phi.getSizeY();
			}
			else
			{
				j0 = phi.getSizeY() - 2;
				j1 = -1;
			}
			int k0, k1;
			if (dk > 0)
			{
				k0 = 1;
				k1 = phi.getSizeZ();
			}
			else
			{
				k0 = phi.getSizeZ() - 2;
				k1 = -1;
			}
			for (int k = k0; k != k1; k += dk)
				for (int j = j0; j != j1; j += dj)
					for (int i = i0; i != i1; i += di)
					{
						HVector3 gx = origin + HVector3(i * dx, j * dx, k * dx);
						CheckNeighbor(tri, x, v, phi, phiVel, closetTri, gx, i, j, k, i - di, j, k);
						CheckNeighbor(tri, x, v, phi, phiVel, closetTri, gx, i, j, k, i, j - dj, k);
						CheckNeighbor(tri, x, v, phi, phiVel, closetTri, gx, i, j, k, i, j, k - dk);
						CheckNeighbor(tri, x, v, phi, phiVel, closetTri, gx, i, j, k, i - di, j, k - dk);
						CheckNeighbor(tri, x, v, phi, phiVel, closetTri, gx, i, j, k, i, j - dj, k - dk);
						CheckNeighbor(tri, x, v, phi, phiVel, closetTri, gx, i, j, k, i - di, j - dj, k - dk);
					}
		}

		int Orientation(const HVector2 &p1, const HVector2 &p2, HReal &twiceSignedArea)
		{
			twiceSignedArea = p1[0] * p2[1] - p1[1] * p2[0];
			if (twiceSignedArea > 0)
				return 1;
			else if (twiceSignedArea < 0)
				return -1;
			else if (p2[1] < p1[1])
				return 1;
			else if (p2[1] > p1[1])
				return -1;
			else if (p1[0] < p2[0])
				return 1;
			else if (p1[0] > p2[0])
				return -1;
			else
				return 0;
		}

		bool IsPointInTriangle2D(HVector2 &p, HVector2 a, HVector2 b, HVector2 c, HReal &t1, HReal &t2, HReal &t3)
		{
			a -= p;
			b -= p;
			c -= p;
			int sign1 = Orientation(a, b, t1);
			if (sign1 == 0)
				return false;
			int sign2 = Orientation(b, c, t2);
			if (sign2 != sign1)
				return false;
			int sign3 = Orientation(c, a, t3);
			if (sign3 != sign1)
				return false;
			HReal sum = t1 + t2 + t3;
			if (sum == 0)
				return true;
			t1 /= sum;
			t2 /= sum;
			t3 /= sum;
			return true;
		}

	private:
		HMatrix4 mTransformMatrixAtCreation;
		HMatrix4 mTransformMatrixCurrent;
		std::vector<HVector4> mVertices;
		std::vector<HVector4> mTransformedVertices;
		std::vector<HVector4> mGrads;
		std::vector<HVector4> mGradPostions;
	};
} // namespace Utility