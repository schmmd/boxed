#include <gtk/gtk.h>

class CEntity
{
public:
  long GetX(    ) { return _point.x; }
  long GetY(    ) { return _point.y; }
  void SetX(long x) { _point.x = x; }
  void SetY(long y) { _point.y = y; }

  void Left () { _point.x -= 1; }
  void Right() { _point.x += 1; }
  void Up   () { _point.y -= 1; }
  void Down () { _point.y += 1; }

  GdkPoint const * GetPointReference() { return &_point; }

private:
  GdkPoint _point;
};

class CPlayer : public CEntity
{

};
