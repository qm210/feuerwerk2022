#ifndef LED_H
#define LED_H

#include <cmath>
//int constrain(int x, int lower, int upper) { return x > upper ? upper : x < lower ? lower : x; }
float max(int a, float b) { return a < b ? b : a; }
float min(int a, float b) { return a > b ? b : a; }

class LED {

  private:

    float hue = 0;
    bool isGray = false;

  public:

    float r = 0;
    float g = 0;
    float b = 0;
    float w = 0;
    float a = 0;

  LED() {}

  LED(int r, int g, int b, int w, int a)
  : r(r), g(g), b(b), w(w), a(a)
  {}

  LED(float white, float light)
  {
    setL(light);
    setWhite(white);
    makeGray();
  }

  LED(float hue, float white, float light)
  {
    setHWL(hue, white, light);
    if(white > .99)
    {
      makeGray();
    }
  }

  int getR() { return (int)(r*a); }
  int getG() { return (int)(g*a); }
  int getB() { return (int)(b*a); }
  int getW() { return (int)(w*a); }

  void norm() {
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);
    w = constrain(w, 0, 255);
    a = constrain(a, 0, 1);
  }

  void blacken()
  {
    dim(0);
  }

  void dim(double fadeFactor)
  {
    r = constrain(r*fadeFactor, 0, 255);
    g = constrain(g*fadeFactor, 0, 255);
    b = constrain(b*fadeFactor, 0, 255);
    w = constrain(w*fadeFactor, 0, 255);
  }

  void setHue(double H)
  {
    H = fmod(H, 360.0);
    if (H<0) H+= 360.0;
    double X = fmod(H/60.0, 1.) * 255;

    if      (0 <= H && H < 60)    {r =   255; g =     X; b =     0;}
    else if (60 <= H && H < 120)  {r = 255-X; g =   255; b =     0;}
    else if (120 <= H && H < 180) {r =     0; g =   255; b =     X;}
    else if (180 <= H && H < 240) {r =     0; g = 255-X; b =   255;}
    else if (240 <= H && H < 300) {r =     X; g =     0; b =   255;}
    else if (300 <= H && H < 360) {r =   255; g =     0; b = 255-X;}

    hue = H;
    norm();
  }

  void setWhite(double W)
  {
    w = 255.0 * W;
    norm();
  }

  void setL(double L)
  {
    a = L;
    norm();
  }

  void mix(LED other, double L)
  {
    float other_a = other.a * L;
    float a_ = max(a, other_a);
    if(a_ < 1e-3)
    {
      r = g = b = w = a = 0;
    }
    else if (isGray || other.isGray)
    {
      w = max(w * a, other.w * other_a) / a_;
      a = a_;
      isGray = (isGray && other.isGray);
    }
    else
    {
      if(fabs(hue - other.hue) < 180)
      {
        hue = (hue * a + other.hue * other_a) / (a + other_a);
      }
      else
      {
        hue = ((hue + 360) * a + other.hue * other_a) / (a + other_a);
      }
      w = max(w * a, other.w * other_a) / a_;
      a = a_;
      setHue(hue);
    }
  }

  void setHWL(double H, double W, double L)
  {
    setHue(H);
    setL(L);
    setWhite(W);
    norm();
  }

  void scaleL(double f)
  {
    a *= f;
    norm();
  }

  void makeGray()
  {
    r = 0;
    g = 0;
    b = 0;
    isGray = true;
  }

  float getHue()
  {
    float RGBmin = min(min(r, g), b);
    float RGBmax = max(max(r, g), b);

    if(RGBmin == RGBmax) hue = 0.;
    else
    {
      if(fabs(RGBmax - r) < 1e-3) hue = 60 * (0 + (g - b)/(RGBmax - RGBmin));
      if(fabs(RGBmax - g) < 1e-3) hue = 60 * (2 + (b - r)/(RGBmax - RGBmin));
      if(fabs(RGBmax - b) < 1e-3) hue = 60 * (4 + (r - g)/(RGBmax - RGBmin));
    }
    if(hue > 360) hue -= 360;
    if(hue < 0) hue += 360;

    return hue;
  }

  static float* interpolate(float x, int i_minor, int i_major)
  {
    static float mix[2];
    mix[0] = constrain((i_major-x)/(i_major-i_minor), 0, 1);
    mix[1] = 1-mix[0];
    return mix;
  }

};

#endif
