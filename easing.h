#pragma once 

class EaseUtils
{
public:
	// Linear interpolation (no easing)
	static float LinearInterpolation(float p);

	// Quadratic easing; p^2
	static float QuadraticEaseIn(float p);
	static float QuadraticEaseOut(float p);
	static float QuadraticEaseInOut(float p);

	// Cubic easing; p^3
	static float CubicEaseIn(float p);
	static float CubicEaseOut(float p);
	static float CubicEaseInOut(float p);

	// Quartic easing; p^4
	static float QuarticEaseIn(float p);
	static float QuarticEaseOut(float p);
	static float QuarticEaseInOut(float p);

	// Quintic easing; p^5
	static float QuinticEaseIn(float p);
	static float QuinticEaseOut(float p);
	static float QuinticEaseInOut(float p);

	// Sine wave easing; sin(p * PI/2)
	static float SineEaseIn(float p);
	static float SineEaseOut(float p);
	static float SineEaseInOut(float p);

	// Circular easing; sqrt(1 - p^2)
	static float CircularEaseIn(float p);
	static float CircularEaseOut(float p);
	static float CircularEaseInOut(float p);

	// Exponential easing, base 2
	static float ExponentialEaseIn(float p);
	static float ExponentialEaseOut(float p);
	static float ExponentialEaseInOut(float p);

	// Exponentially-damped sine wave easing
	static float ElasticEaseIn(float p);
	static float ElasticEaseOut(float p);
	static float ElasticEaseInOut(float p);

	// Overshooting cubic easing; 
	static float BackEaseIn(float p);
	static float BackEaseOut(float p);
	static float BackEaseInOut(float p);

	// Exponentially-decaying bounce easing
	static float BounceEaseIn(float p);
	static float BounceEaseOut(float p);
	static float BounceEaseInOut(float p);
};