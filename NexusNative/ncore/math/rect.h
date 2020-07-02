#ifndef _NEXUS_CORE_RECT_H_
#define _NEXUS_CORE_RECT_H_

namespace nexus
{
	template<typename T>
	struct point
	{
		T	x, y;

		point()
			: x(0),y(0)
		{}

		point(T _x, T _y)
			: x(_x), y(_y)
		{}

		inline const point& operator = ( const point& rhs )
		{
			x = rhs.x;
			y = rhs.y;
			return *this;
		}
	};

	template<typename T>
	inline bool operator == (const point<T>& pt1, const point<T>& pt2)
	{
		return (pt1.x == pt2.x && pt1.y == pt2.y);
	}

	template<typename T>
	inline bool operator != (const point<T>& pt1, const point<T>& pt2)
	{
		return (pt1.x != pt2.x || pt1.y != pt2.y);
	}

	template<typename T>
	inline point<T> operator - (point<T> lv, point<T> rv)
	{
		return point<T>(lv.x-rv.x, lv.y-rv.y);
	}

	template<typename T>
	inline point<T> operator + (point<T> lv, point<T> rv)
	{
		return point<T>(lv.x+rv.x, lv.y+rv.y);
	}

	template<typename T>
	struct rect
	{
		T	left,
			top,
			right,
			bottom;

		rect()
			: left(0), top(0), right(0), bottom(0)
		{}
			
		rect(T _left, T _top, T _right, T _bottom)
			:left(_left), top(_top), right(_right), bottom(_bottom)
		{}

		T get_width() const		{	return right-left; }
		T get_height() const	{	return bottom-top; }

		point<T> get_center() const
		{
			return point<T>( (left+right)/2, (top+bottom)/2 );
		}

		//bool intersect(const rect<T>& rc) const
		//{
		//	// Note Region is defined as [left, right) and [top, bottom)!
		//	// general tests: test whether rect's edges are inside this rect
		//	bool const leftIsInside   = (rc.left   >= left) && (rc.left   <  right);
		//	bool const rightIsInside  = (rc.right  >  left) && (rc.right  <= right);
		//	bool const topIsInside    = (rc.top    >= top ) && (rc.top    <  bottom);
		//	bool const bottomIsInside = (rc.bottom >  top ) && (rc.bottom <= bottom);

		//	// if and only if (at least) one of the horizontal edges and 
		//	//                (at least) one of the vertical edges inside than the two inersect
		//	if ((leftIsInside || rightIsInside) && (topIsInside || bottomIsInside))
		//		return true;
		//	return false;
		//}

		// 正确的intersect方法
		bool intersect(const rect<T>& rc) const
		{
			// check for total exclusion
			return ((right > rc.left) &&
				(left < rc.right) &&
				(bottom > rc.top) &&
				(top < rc.bottom));
		}

	};


	template<typename T, typename U>
	inline void clip_rect(rect<T>& rc,  U width, U height)
	{
		clip_rect(rc.left, rc.top, rc.right, rc.bottom, width, height);
	}

	template<typename T, typename U>
	inline void clip_rect(T& left, T& top, T& right, T& bottom, U width, U height)
	{
		if( right > width)
			right = width;
		if( bottom > height)
			bottom = height;

		if(left < 0)
			left = 0;
		if(top < 0)
			top = 0;
	}

	struct color_rect
	{
		unsigned int left_top, right_top, left_bottom, right_bottom;

		color_rect()
			: left_top(0), right_top(0), left_bottom(0), right_bottom(0)
		{
		}

		color_rect(const unsigned int color)
			: left_top(color),right_top(color),left_bottom(color),right_bottom(color)
		{
		}

		color_rect(unsigned int _left_top, unsigned int _right_top, unsigned int _left_bottom, unsigned int _right_bottom)
			: left_top(_left_top),right_top(_right_top),left_bottom(_left_bottom),right_bottom(_right_bottom)
		{
		}
	};

	template<typename T>
	struct tsize
	{
		T	x, y;

		tsize()
			: x(0),y(0)
		{}

		tsize(T _x, T _y)
			: x(_x), y(_y)
		{}
	};

	typedef rect<long>	nrect;
	typedef rect<float>	frect;
	typedef rect<unsigned int> uirect;
	
	typedef point<int>		npoint;
	typedef point<long>		lpoint;
	typedef point<float>	fpoint;

	typedef tsize<unsigned long>		nsize;	
	typedef tsize<float>				fsize;	
	typedef tsize<unsigned int>			uisize;
};

#endif