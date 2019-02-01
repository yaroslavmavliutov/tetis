#include <algorithm>
#include "Piece.h"

using namespace std;


void Piece::SetShape(PieceShape shape)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 2; j++)
            coords[i][j] = pieceCoords[shape][i][j];
    pieceShape = shape;
}

void Piece::SetRandomShape()
{
    int rand_shape_idx = rand() % 7 + 1;
    SetShape(PieceShape(rand_shape_idx));
}


/*#define DEFINEFINDMINMAX(coord, op, Name)       \
    int Piece::Name() const                     \
    {                                           \
        int v = coords[0][coord];               \
        for (int i = 0; i < 4; i++)             \
            v = op(v, coords[i][coord]);        \
        return v;                               \
    }*/

int Piece::MaxX() const {
    int element = coords[0][XCOORD];
    for (int i = 0; i < 4; i++) element = max(element, coords[i][XCOORD]);
    return element;
}

int Piece::MaxY() const {
    int element = coords[0][YCOORD];
    for (int i = 0; i < 4; i++) element = max(element, coords[i][YCOORD]);
    return element;
}
int Piece::MinX() const {
    int element = coords[0][XCOORD];
    for (int i = 0; i < 4; i++) element = min(element, coords[i][XCOORD]);
    return element;
}
int Piece::MinY() const {
    int element = coords[0][YCOORD];
    for (int i = 0; i < 4; i++) element = min(element, coords[i][YCOORD]);
    return element;
}

/*#define DEFINEROTATE(coeffy, coeffx, Name)      \
    Piece Piece::Name() const                   \
    {                                           \
        if (pieceShape == Square)               \
            return *this;                       \
                                                \
        Piece res;                              \
        res.pieceShape = pieceShape;            \
        for (int i = 0; i < 4; i++)             \
        {                                       \
            res.SetX(i, coeffy * y(i));         \
            res.SetY(i, coeffx * x(i));         \
        }                                       \
        return res;                             \
    }*/

Piece Piece::Rotation() const {
    if (pieceShape == Square) return  *this;
    Piece result;
    result.pieceShape = pieceShape;
    for (int i = 0; i<4; i++) {
        result.SetX(i, -y(i));
        result.SetY(i,  x(i));
    }
    return result;
}

/*Piece Piece::RotateRight() const {
    if (pieceShape == Square) return  *this;
    Piece result;
    result.pieceShape = pieceShape;
    for (int i = 0; i<4; i++) {
        result.SetX(i, -y(i));
        result.SetY(i, x(i));
    }
    return result;
}*/




/* Good luck debugging this, suckers */
/*DEFINEFINDMINMAX(XCOORD, min, MinX);
DEFINEFINDMINMAX(XCOORD, max, MaxX);
DEFINEFINDMINMAX(YCOORD, min, MinY);
DEFINEFINDMINMAX(YCOORD, max, MaxY);*/

//DEFINEROTATE(1, -1, RotateLeft);
//DEFINEROTATE(-1, 1, RotateRight);
