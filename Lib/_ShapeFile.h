/******************************************************************************
 * Shape�ļ�������ݽṹ
 *
 *
 *****************************************************************************/

#pragma once

typedef unsigned char byte;


#pragma warning(push)
#pragma warning(disable:4200)

#pragma pack(push)
#pragma pack(1)

namespace UltraGIS
{
	namespace shp
	{
		template<class T>
		void SwapByteOrder(T * pVal)
		{
 			byte * buf = (byte *)pVal;
			int size = sizeof(T);
			byte tmp;
			for ( int i=0; i<(size/2); ++i )
			{
				tmp = buf[i];
				buf[i] = buf[size-i-1];
				buf[size-i-1] = tmp;
			}
		}

		struct ShapeType
		{
			enum
			{
				/// <summary>
				/// Null shape with no geometric data
				/// </summary>
				Null = 0,
				/// <summary>
				/// A Point consists of a pair of double-precision coordinates.
				/// SharpMap interpretes this as <see cref="SharpMap.Geometries.Point"/>
				/// </summary>
				Point = 1,
				/// <summary>
				/// PolyLine is an ordered set of vertices that consists of one or more parts. A part is a
				/// connected sequence of two or more points. Parts may or may not be connected to one
				///	another. Parts may or may not intersect one another.
				/// SharpMap interpretes this as either <see cref="SharpMap.Geometries.LineString"/> or <see cref="SharpMap.Geometries.MultiLineString"/>
				/// </summary>
				PolyLine = 3,
				/// <summary>
				/// A polygon consists of one or more rings. A ring is a connected sequence of four or more
				/// points that form a closed, non-self-intersecting loop. A polygon may contain multiple
				/// outer rings. The order of vertices or orientation for a ring indicates which side of the ring
				/// is the interior of the polygon. The neighborhood to the right of an observer walking along
				/// the ring in vertex order is the neighborhood inside the polygon. Vertices of rings defining
				/// holes in polygons are in a counterclockwise direction. Vertices for a single, ringed
				/// polygon are, therefore, always in clockwise order. The rings of a polygon are referred to
				/// as its parts.
				/// SharpMap interpretes this as either <see cref="SharpMap.Geometries.Polygon"/> or <see cref="SharpMap.Geometries.MultiPolygon"/>
				/// </summary>
				Polygon = 5,
				/// <summary>
				/// A MultiPoint represents a set of points.
				/// SharpMap interpretes this as <see cref="SharpMap.Geometries.MultiPoint"/>
				/// </summary>
				Multipoint = 8,
				/// <summary>
				/// A PointZ consists of a triplet of double-precision coordinates plus a measure.
				/// SharpMap interpretes this as <see cref="SharpMap.Geometries.Point"/>
				/// </summary>
				PointZ = 11,
				/// <summary>
				/// A PolyLineZ consists of one or more parts. A part is a connected sequence of two or
				/// more points. Parts may or may not be connected to one another. Parts may or may not
				/// intersect one another.
				/// SharpMap interpretes this as <see cref="SharpMap.Geometries.LineString"/> or <see cref="SharpMap.Geometries.MultiLineString"/>
				/// </summary>
				PolyLineZ = 13,
				/// <summary>
				/// A PolygonZ consists of a number of rings. A ring is a closed, non-self-intersecting loop.
				/// A PolygonZ may contain multiple outer rings. The rings of a PolygonZ are referred to as
				/// its parts.
				/// SharpMap interpretes this as either <see cref="SharpMap.Geometries.Polygon"/> or <see cref="SharpMap.Geometries.MultiPolygon"/>
				/// </summary>
				PolygonZ = 15,
				/// <summary>
				/// A MultiPointZ represents a set of <see cref="PointZ"/>s.
				/// SharpMap interpretes this as <see cref="SharpMap.Geometries.MultiPoint"/>
				/// </summary>
				MultiPointZ = 18,
				/// <summary>
				/// A PointM consists of a pair of double-precision coordinates in the order X, Y, plus a measure M.
				/// SharpMap interpretes this as <see cref="SharpMap.Geometries.Point"/>
				/// </summary>
				PointM = 21,
				/// <summary>
				/// A shapefile PolyLineM consists of one or more parts. A part is a connected sequence of
				/// two or more points. Parts may or may not be connected to one another. Parts may or may
				/// not intersect one another.
				/// SharpMap interpretes this as <see cref="SharpMap.Geometries.LineString"/> or <see cref="SharpMap.Geometries.MultiLineString"/>
				/// </summary>
				PolyLineM = 23,
				/// <summary>
				/// A PolygonM consists of a number of rings. A ring is a closed, non-self-intersecting loop.
				/// SharpMap interpretes this as either <see cref="SharpMap.Geometries.Polygon"/> or <see cref="SharpMap.Geometries.MultiPolygon"/>
				/// </summary>
				PolygonM = 25,
				/// <summary>
				/// A MultiPointM represents a set of <see cref="PointM"/>s.
				/// SharpMap interpretes this as <see cref="SharpMap.Geometries.MultiPoint"/>
				/// </summary>
				MultiPointM = 28,
				/// <summary>
				/// A MultiPatch consists of a number of surface patches. Each surface patch describes a
				/// surface. The surface patches of a MultiPatch are referred to as its parts, and the type of
				/// part controls how the order of vertices of an MultiPatch part is interpreted.
				/// SharpMap doesn't support this feature type.
				/// </summary>
				MultiPatch = 31,
			};
		};

		/// <summary>
		/// The main file header is 100 bytes long. 
		/// </summary>
		struct ShapeHeader
		{
			// byte order is big.
			int FileCode;	// 9994
			int UnUsed0;
			int UnUsed1;
			int UnUsed2;
			int UnUsed3;
			int UnUsed4;
			/// <summary>
			/// The value for file length is the total length of the file in 16-bit words
			/// (including the fifty 16-bit words that make up the header).
			/// </summary>
			int FileLength;

			// byte order is little.
			int Version;	// 1000
			int ShapeType;
			double Xmin;
			double Ymin;
			double Xmax;
			double Ymax;
			double Zmin;
			double Zmax;
			double Mmin;
			double Mmax;
		};


		struct ShapeObject
		{
			struct Point
			{
				double X;
				double Y;
			};

			struct Box
			{
				double Xmin;
				double Ymin;
				double Xmax;
				double Ymax;
			};

			// byte order is big.
			int Number;
			int Length;

			// byte order is little.
			int ShapeType;
		};

		struct ShpPoint : public ShapeObject
		{
			double X;
			double Y;
		};

		struct ShpMultiPoint : public ShapeObject
		{
			Box Box;
			int NumPoints;
			Point Points[0];	// NumPoints
		};

		struct ShpPolyLine : public ShapeObject
		{
			Box Box;
			int NumParts;
			int NumPoints;
			int Parts[0];	// NumParts
			//Point Points[];	// NumPoints

		public:
			// ��ȡ�����
			inline const Point& Points(int pos) const
			{
				return *(Point *)((byte *)this + sizeof(ShpPolyLine) + sizeof(int) * NumParts + sizeof(Point) * pos);
			}

			// ��ȡһ�����괮
			inline const Point * getPart(int pos, int * pCount)
			{
				if ( pCount )
					*pCount = ((pos + 1) < NumParts) ? (Parts[pos+1] - Parts[pos]) : (NumPoints - Parts[pos]);
				return (Point *)((byte *)this + sizeof(ShpPolyLine) + sizeof(int) * NumParts + sizeof(Point) * Parts[pos]);
			}
		};

		struct ShpPolygon : public ShapeObject
		{
			Box Box;
			int NumParts;
			int NumPoints;
			int Parts[0];	// NumParts
			//Point Points[];	// NumPoints

		public:
			// ��ȡ�����
			inline const Point& Points(int pos) const
			{
				return *(Point *)(this + sizeof(ShpPolygon) + sizeof(int) * NumParts + sizeof(Point) * pos);
			}

			// ��ȡһ�����괮
			inline const Point * getPart(int pos, int * pCount)
			{
				if ( pCount )
					*pCount = ((pos + 1) < NumParts) ? (Parts[pos+1] - Parts[pos]) : (NumPoints - Parts[pos]);
				return (Point *)((byte *)this + sizeof(ShpPolygon) + sizeof(int) * NumParts + sizeof(Point) * Parts[pos]);
			}
		};
	}
}

#pragma pack(pop)

#pragma warning(pop)