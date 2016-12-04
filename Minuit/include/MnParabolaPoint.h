#ifndef MN_MnParabolaPoint_H_
#define MN_MnParabolaPoint_H_

class MnParabolaPoint {

public:

  MnParabolaPoint(double x, double y) : theX(x), theY(y) {}

  ~MnParabolaPoint() {}

  double x() const {return theX;}

  double y() const {return theY;}

private:

  double theX;
  double theY;
};
#endif //MN_MnParabolaPoint_H_
