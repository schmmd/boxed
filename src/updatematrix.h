#ifndef _H_UPDATEMATRIX
#define _H_UPDATEMATRIX

class CUpdateMatrix
{
  public:
    ~CUpdateMatrix();
    
    void Create(int width, int height);
    void Clear();
    void Fill();
    
    void Set(int x, int y);
    bool Get(int x, int y);
  
  private:
    int _width, _height;
  
    bool* _matrix;
};

#endif
