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

int Piece::MaxX() const {
    int element = coords[0][0];
    for (int i = 0; i < 4; i++) element = max(element, coords[i][0]);
    return element;
}

int Piece::MaxY() const {
    int element = coords[0][1];
    for (int i = 0; i < 4; i++) element = max(element, coords[i][1]);
    return element;
}
int Piece::MinX() const {
    int element = coords[0][0];
    for (int i = 0; i < 4; i++) element = min(element, coords[i][0]);
    return element;
}
int Piece::MinY() const {
    int element = coords[0][1];
    for (int i = 0; i < 4; i++) element = min(element, coords[i][1]);
    return element;
}


Piece Piece::Rotation() const {
    if (pieceShape == O_bloc) return  *this;
    Piece result;
    result.pieceShape = pieceShape;
    for (int i = 0; i<4; i++) {
        result.SetX(i, -y(i));
        result.SetY(i,  x(i));
    }
    return result;
}

