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
	};

	template<typename T>
	inline point<T> operator - (point<T> lv, point<T> rv)
	{
		return point<T>(lv.x-rv.x, lv.y-rv.y);
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
	
	typedef point<long>		npoint;
	typedef point<float>	fpoint;

	typedef tsize<unsigned long>		nsize;	
};

#endif