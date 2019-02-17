#ifndef _PIECE_H
#define _PIECE_H

enum PieceShape { None, I_long, O_bloc, T, L, J, Z, S};

const int pieceCoords[8][4][2] =
{
    { {0,  0},  {0, 0},  {0,  0}, {0,  0} }, //none
    { {0,  -1}, {0, 0},  {0,  1}, {0,  2} }, // I
    { { 0, 0},  {-1, 0},  {0,  1}, {-1,  1} }, // square
    { {-1, 0},  {0, 0},  {1,  0}, {0,  1} }, // T
    { {-1, -1}, {0, -1}, {0,  0}, {0,  1} }, // L
    { {0,  -1}, {-1, -1}, {-1,  0}, {-1,  1} }, // J
    { {0,  -1}, {0, 0},  {-1, 0}, {-1, 1} }, // Z
    { {-1,  -1}, {-1, 0},  {0,  0}, {0,  1} } // S

};

class Piece
{
public:
    Piece() { SetShape(None); }

    void SetShape(PieceShape shape);

    PieceShape GetShape() const { return pieceShape; }

    int x(int i) const { return coords[i][0]; }
    int y(int i) const { return coords[i][1]; }
    int MinX() const;
    int MaxX() const;
    int MinY() const;
    int MaxY() const;

    Piece Rotation() const;


private:
    void SetX(int idx, int val) { coords[idx][0] = val; }
    void SetY(int idx, int val) { coords[idx][1] = val; }
    PieceShape pieceShape;
    int coords[4][2];
};

#endif
