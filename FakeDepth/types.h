//
//  types.h
//  FakeDepth
//
//  Created by Simon Harvey on 11-12-20.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef FakeDepth_types_h
#define FakeDepth_types_h

namespace types 
{
	template <typename T>
	struct Point
	{
		T x, y;
	};

	template <typename T>
	struct Rect 
	{
		Point<T> a, b, c, d;
		T width() { return d.x - a.x; }
		T height() { return b.y - a.y; }
		T x() { return a.x; }
		T y() { return a.y; }
	};

	template <typename T>
	Rect<T> make_rect(T x, T y, T w, T h)
	{
		Rect<T> r = {
			{x, y},
			{x, y+h},
			{x+w, y+h},
			{x+w, y}
		};
		return r;
	}
}


#endif
