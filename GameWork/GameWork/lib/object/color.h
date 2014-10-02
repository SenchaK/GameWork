#pragma once

struct colorS {
	int r;
	int g;
	int b;
	void range(){
		if( this->r >= 255 ) this->r = 255;
		if( this->g >= 255 ) this->g = 255;
		if( this->b >= 255 ) this->b = 255;
		if( this->r <=   0 ) this->r =   0;
		if( this->g <=   0 ) this->g =   0;
		if( this->b <=   0 ) this->b =   0;
	}
	void set( int _r , int _g , int _b ){
		this->r = _r;
		this->g = _g;
		this->b = _b;
		this->range();
	}
	void set( colorS& c ){
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->range();
	}
};
