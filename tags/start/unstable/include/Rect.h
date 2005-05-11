#ifndef _RECT_H_
	#define _RECT_H_

class Rect{
public:
	Rect(){};
	inline Rect(int l, int t, int r, int b)	{	left = l; top = t; right = r; bottom = b;	};
	virtual ~Rect(){};

	inline virtual void Initialise(int l, int t, int r, int b){
		left = l; top = t; right = r; bottom = b;
	};
	
	inline virtual bool Contains(int x, int y) const{

		if((x >= left) && (x <= right))	if((y >= top) && (y <= bottom))
		{
			return true;
		}
		
		return false;
	};

	inline virtual Rect & operator=(class Rect &r){
		left		= r.left;
  	right		= r.right;
   	top			= r.top;
    bottom	= r.bottom;
    
    return *this;
  }

	inline virtual bool operator>(class Rect &r) const{
		if(right-left > r.right-r.left) return true;
		if(bottom-top > r.bottom-r.top) return true;

		return false;
	}

	inline virtual bool operator<(class Rect &r) const{
		if(right-left < r.right-r.left) return true;
		if(bottom-top < r.bottom-r.top) return true;

		return false;
	}

	inline virtual bool operator>=(class Rect &r) const{
		if(right-left >= r.right-r.left)	if(bottom-top >= r.bottom-r.top) return true;

		return false;
	}

	inline virtual bool operator<=(class Rect &r) const{
		if(right-left <= r.right-r.left)	if(bottom-top <= r.bottom-r.top) return true;

		return false;
	}


	int left,top,right,bottom;
};

#endif // #ifndef _RECT_H_
