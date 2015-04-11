/******************************************************************************
 * Shape文件相关数据结构
 * Main file:counties.shp
 * Index file:counties.shx
 * dBASE table:counties.dbf
 *
 *****************************************************************************/

#pragma once


#pragma warning(push)
#pragma warning(disable:4200)

#pragma pack(push)
#pragma pack(1)


namespace UltraGIS { namespace shp {


	typedef int Integer;		// Signed 32-bit integer (4 bytes)
	typedef double Double;		// Signed 64-bit IEEE double-precision floating point number (8 bytes)
	typedef unsigned char byte;


	template<class T> T byte_reverse(T val)
	{
		T revVal;
		byte * src = (byte *)&val;
		byte * dest = (byte *)&revVal;
		for ( int i=0; i<sizeof(T); ++i )
			dest[i] = src[sizeof(T)-i-1];
		return revVal;
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
	private:
		// byte order is big.
		Integer m_fileCode;	// 9994
		Integer m_unused[5];
		Integer m_fileLength;

	public:
		// byte order is little.
		Integer Version;	// 1000
		Integer ShapeType;
		Double Xmin;
		Double Ymin;
		Double Xmax;
		Double Ymax;
		Double Zmin;
		Double Zmax;
		Double Mmin;
		Double Mmax;

	public:

		/// <summary>
		/// 9994
		/// </summary>
		inline Integer FileCode() const { return byte_reverse(m_fileCode); }

		/// <summary>
		/// The value for file length is the total length of the file in 16-bit words
		/// (including the fifty 16-bit words that make up the header).
		/// </summary>
		inline Integer FileLength() const { return byte_reverse(m_fileLength); }
	};

	struct Point
	{
		Double X;
		Double Y;
	};

	struct Box
	{
		Double Xmin;
		Double Ymin;
		Double Xmax;
		Double Ymax;
	};

	struct ShapeObject
	{
	private:
		// byte order is big.
		Integer m_number;
		Integer m_length;

	public:
		// byte order is little.
		Integer ShapeType;
		
		inline Integer Number() const { return byte_reverse(m_number); }
		inline Integer Length() const { return byte_reverse(m_length); }
	};

	struct ShpPoint : public ShapeObject
	{
		Double X;
		Double Y;
	};

	struct ShpMultiPoint : public ShapeObject
	{
		Box Box;
		Integer NumPoints;
		Point Points[0];	// NumPoints
	};

	struct ShpPolyLine : public ShapeObject
	{
		Box Box;
		Integer NumParts;
		Integer NumPoints;
		Integer Parts[0];	// NumParts
		//Point Points[];	// NumPoints

	public:
		// 获取坐标点
		inline const Point& Points(Integer pos) const
		{
			return *(Point *)((byte *)this + sizeof(ShpPolyLine) + sizeof(Integer) * NumParts + sizeof(Point) * pos);
		}

		// 获取一段坐标串
		inline const Point * getPart(Integer pos, Integer * pCount) const
		{
			if ( pCount )
				*pCount = ((pos + 1) < NumParts) ? (Parts[pos+1] - Parts[pos]) : (NumPoints - Parts[pos]);
			return (Point *)((byte *)this + sizeof(ShpPolyLine) + sizeof(Integer) * NumParts + sizeof(Point) * Parts[pos]);
		}
	};

	struct ShpPolygon : public ShapeObject
	{
		Box Box;
		Integer NumParts;
		Integer NumPoints;
		Integer Parts[0];	// NumParts
		//Point Points[];	// NumPoints

	public:
		// 获取坐标点
		inline const Point& Points(Integer pos) const
		{
			return *(Point *)((byte *)this + sizeof(ShpPolygon) + sizeof(Integer) * NumParts + sizeof(Point) * pos);
		}

		// 获取一段坐标串
		inline const Point * getPart(Integer pos, Integer * pCount) const
		{
			if ( pCount )
				*pCount = ((pos + 1) < NumParts) ? (Parts[pos+1] - Parts[pos]) : (NumPoints - Parts[pos]);
			return (Point *)((byte *)this + sizeof(ShpPolygon) + sizeof(Integer) * NumParts + sizeof(Point) * Parts[pos]);
		}
	};

}}

#pragma pack(pop)

#pragma warning(pop)