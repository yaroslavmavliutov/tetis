#include "GamePanel.h"
#include "InfoPanel.h"
#include "Frame.h"
#include <wx/stattext.h>
#include <chrono>


GamePanel::GamePanel(wxPanel* parent_t, wxFrame *fr)
        : wxPanel(parent_t, -1, wxPoint(5, 5), wxSize(170, 310), wxBORDER_SUNKEN)
{
//    sock = m_sock;
    special = true;
    timer = new wxTimer(this, 1);
    status_scr = fr->GetStatusBar();
    pieceFallingFinished = false;
    started = false;
    paused = false;
    score = 0;
    lvl = 1;
    curX = 0;
    curY = 0;
    panel = parent_t;
    TIMER_INTERVAL = 500;
    //next.SetShape(PieceShape(rand()%7+1));
//    nb_opponent = m_nb_opponent;
    //current.SetShape(PieceShape(rand()%7+1));

    //RandomPiece(); - цю треба тут, але з нею виходить переповнення
    ClearBoard();

    Connect(wxEVT_PAINT, wxPaintEventHandler(GamePanel::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GamePanel::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(GamePanel::OnTimer));
    std::cout << "GamePanel - constructor vide" << std::endl;
}


GamePanel::GamePanel(wxPanel* parent_t, wxFrame *fr, wxSocketClient *m_sock, int m_nb_opponent)
        : wxPanel(parent_t, -1, wxPoint(5, 5), wxSize(170, 310), wxBORDER_SUNKEN)
{
    special = false;
    sock = m_sock;
    timer = new wxTimer(this, 1);
    status_scr = fr->GetStatusBar();
    pieceFallingFinished = false;
    started = false;
    paused = false;
    score = 0;
    lvl = 1;
    curX = 0;
    curY = 0;
    panel = parent_t;
    TIMER_INTERVAL = 500;

    PieceShape tmp;
    tmp = PieceShape(rand()%7+1);
    if(nb_opponent>0) {
        sendShapeToServer(tmp, 1);
    }
    next.SetShape(tmp);

    nb_opponent = m_nb_opponent;
    //current.SetShape(PieceShape(rand()%7+1));

    //RandomPiece(); - цю треба тут, але з нею виходить переповнення
    ClearBoard();

    Connect(wxEVT_PAINT, wxPaintEventHandler(GamePanel::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GamePanel::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(GamePanel::OnTimer));
    std::cout << "GamePanel - constructor" << std::endl;
}

void GamePanel::setNextPiece(char c){
    PieceShape ps;
    switch (c)
    {
        case 'I':
            ps = I_long;
            break;
        case 'O':
            ps = O_bloc;
            break;
        case 'T':
            ps = T;
            break;
        case 'L':
            ps = L;
            break;
        case 'J':
            ps = J;
            break;
        case 'Z':
            ps = Z;
            break;
        case 'S':
            ps = S;
            break;
        default:
            ps = None;
    }
    next.SetShape(ps);
}

void GamePanel::setCurrentPiece(char c) {
    PieceShape ps;
    switch (c)
    {
        case 'I':
            ps = I_long;
            break;
        case 'O':
            ps = O_bloc;
            break;
        case 'T':
            ps = T;
            break;
        case 'L':
            ps = L;
            break;
        case 'J':
            ps = J;
            break;
        case 'Z':
            ps = Z;
            break;
        case 'S':
            ps = S;
            break;
        default:
            ps = None;
    }
    current.SetShape(ps);
}

void GamePanel::SetMovement(char c) {
    switch (c) {
        case 'd':
            DropDown();
            break;
        case 'm':
            DoMove(current.Rotation(), curX, curY);
            break;
        case 'o':
            DropOneLine();
            break;
        case 'l':
            DoMove(current, curX - 1, curY);
            break;
        case 'r':
            DoMove(current, curX + 1, curY);
            break;
        default:
            //event.Skip();
            std::cout << "Non move value" << std::endl;
    }
}

void GamePanel::Start()
{
    if (paused)
        return;

    started = true;
    pieceFallingFinished = false;
    paused = false;
    lvl = 1;


    status_scr->SetStatusText(wxT("Your lvl: 1"));
    MakeNewPiece();
    timer->Start(this->TIMER_INTERVAL);


}

void GamePanel::Pause()
{
    if (!started)
        return;

    paused = !paused;

    if (paused)
    {
        timer->Stop();
        status_scr->SetStatusText(wxT("Game Paused"));
    }
    else
    {
        timer->Start(this->TIMER_INTERVAL);
        wxString str;
        str.Printf(wxT("Your lvl: %d"), lvl);
        status_scr->SetStatusText(str);
    }
    Refresh();
}

void GamePanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxSize size = GetClientSize();
    int top = size.GetHeight() - BoardHeight * Height();

    for (int i = 0; i < BoardHeight; i++)
        for (int j = 0; j < BoardWidth; j++)
        {
            PieceShape pieceShape = PieceCheck(j, BoardHeight - i - 1);
            if (pieceShape == None)
                continue;
            DrawPieceSquare(dc, j * Width(), top + i * Height(), pieceShape);
        }

    if (current.GetShape() == None)
        return;

    for (int i = 0; i < 4; i++)
    {
        int x = curX + current.x(i);
        int y = curY - current.y(i);
        DrawPieceSquare(dc, x * Width(), top + (BoardHeight - y - 1) * Height(),
                        current.GetShape());
    }
}


void GamePanel::sendMoveToServer(char c) {
    unsigned char len;
    size_t txn;
    char move[6] = "move";
    if(nb_opponent>0){
        move[4] = c;
        txn = strlen(move);
        len = txn;
        sock->Write(&len,1);
        sock->Write(&move, len);
    }
}

void GamePanel::OnKeyDown(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();

    switch (keyCode)
    {
        case 'P':
            Pause();
            return;
    }

    if (!started || current.GetShape() == None || paused)
    {
        event.Skip();
        return;
    }


    if(!special) {
        switch (keyCode) {
            case WXK_SPACE:
                DropDown();
                sendMoveToServer('d');
                break;
            case WXK_UP:
                DoMove(current.Rotation(), curX, curY);
                sendMoveToServer('m');
                break;
            case WXK_DOWN:
                DropOneLine();
                sendMoveToServer('o');
                break;
            case WXK_LEFT:
                //if (!CheckBounds(curX, current))
                DoMove(current, curX - 1, curY);
                sendMoveToServer('l');
                break;
            case WXK_RIGHT:
                //if (!CheckBounds(curX, current))
                DoMove(current, curX + 1, curY);
                sendMoveToServer('r');
                break;
            default:
                event.Skip();
        }
    }
}

void GamePanel::OnTimer(wxCommandEvent& event)
{
    if (pieceFallingFinished)
    {
        pieceFallingFinished = false;
        MakeNewPiece();
    }
    else
        DropOneLine();
}

void GamePanel::ClearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; i++)
        board[i] = None;
}

void GamePanel::DropDown()
{
    int y = curY;
    while (y)
        if (!DoMove(current, curX, --y))
            break;
    PieceDropped();
}

void GamePanel::DropOneLine()
{
    if (!DoMove(current, curX, curY - 1))
        PieceDropped();
}

void GamePanel::PieceDropped()
{
    for (int i = 0; i < 4; i++)
    {
        int x = curX + current.x(i);
        int y = curY - current.y(i);
        PieceCheck(x, y) = current.GetShape();
    }
    RemoveFullLines();
    if (!pieceFallingFinished)
        MakeNewPiece();
}

void GamePanel::RemoveFullLines()
{
    int lines = 0;

    for (int i = BoardHeight - 1; i >= 0; i--)
    {
        bool lineFull = true;
        for (int j = 0; j < BoardWidth; j++)
            if (PieceCheck(j, i) == None)
            {
                lineFull = false;
                break;
            }

        if (!lineFull)
            continue;

        lines++;
        for (int j = i; j < BoardHeight - 1; j++)
            for (int k = 0; k < BoardWidth; k++)
                PieceCheck(k, j) = PieceCheck(k, j + 1);
    }

    if (!lines)
        return;
    if (lines == 1) score+= CalculatorScore(40, lvl);
    else if (lines == 2) score+= CalculatorScore(100, lvl);
    else if (lines == 3) score+= CalculatorScore(300, lvl);
    else if (lines == 4) score+= CalculatorScore(1200, lvl);

    lvl = score/500 + 1;
    this->TIMER_INTERVAL = this->TIMER_INTERVAL - lines*25;
    wxString str;
    str.Printf(wxT("Your lvl: %d"), lvl);
    status_scr->SetStatusText(str);

    if(!special) {

        Frame *comm = (Frame *) panel->GetParent();
        comm->m_rp->strings_score[0]->SetLabel(wxString::Format(wxT("%s score: %d"), comm->UserName, score));

        if (nb_opponent > 0) {
            char score_char[15] = "score";
            std::string sc = std::to_string(score);
            char const *pscore = sc.c_str();
            strcat(score_char, comm->BufferName);
            strcat(score_char, pscore);

//            wxString str1 = std::to_string(score);
//            wxCharBuffer buffer = str1.ToUTF8();
            size_t txn = strlen(score_char);
            std::cout << "GAME_PANAL txn = " << txn << std::endl;
            unsigned char len;
            len = txn;
            sock->Write(&len, 1);//send the length of the message first
            if (sock->Write(score_char, txn).LastCount() != txn) {
                std::cout << "Write error.\n";
                return;
            } else {
                std::cout << "CLIENT send score Tx: " << score_char << "\n";
            }
        }
    }
    pieceFallingFinished = true;
    current.SetShape(None);
    timer->Start(this->TIMER_INTERVAL);
    Refresh();


}

int GamePanel::CalculatorScore(int points, int n) {
    return points*(n+1);
}


void GamePanel::sendShapeToServer(PieceShape ps, int curr_or_next) {
    unsigned char len;
    size_t txn;
    char c;


    if(nb_opponent>0){
        switch (ps)
        {
            case I_long:
                c = 'I';
                break;
            case O_bloc:
                c = 'O';
                break;
            case T:
                c = 'T';
                break;
            case L:
                c = 'L';
                break;
            case J:
                c = 'J';
                break;
            case Z:
                c = 'Z';
                break;
            case S:
                c = 'S';
                break;
            default:
                c = 'N';
        }

        if (curr_or_next == 1) {    // next piece
            char move[6] = "next";
            move[4] = c;
            txn = strlen(move);
            len = txn;
            sock->Write(&len,1);
            sock->Write(&move, len);
        }else{                      // current piece
            char move[6] = "curr";
            move[4] = c;
            txn = strlen(move);
            len = txn;
            sock->Write(&len,1);
            sock->Write(&move, len);
        }

    }
}


void GamePanel::RandomPiece()
{

        current.SetShape(next.GetShape());
//        sendShapeToServer(current.GetShape(), 0);

    if (!special) {
        PieceShape tmp;
        tmp = PieceShape(rand() % 7 + 1);
        if(nb_opponent>0)
            sendShapeToServer(tmp, 1);
        next.SetShape(tmp);
        Frame *comm = (Frame *) panel->GetParent();

        comm->m_rp->piece.SetShape(None);
        comm->m_rp->ClearPeace();

        comm->m_rp->piece.SetShape(next.GetShape());
        comm->m_rp->ChangePeace();
    }
}

void GamePanel::MakeNewPiece()
{
    RandomPiece();
    curX = BoardWidth / 2;
    curY = BoardHeight - 1 + current.MinY();

    if (!DoMove(current, curX, curY))
    {
        current.SetShape(None);
        if(nb_opponent>0)
            sendShapeToServer(None, 0);
        timer->Stop();
        started = false;
        status_scr->SetStatusText(wxT("You Lose :("));
        if(!special) {
            Frame *comm = (Frame *) panel->GetParent();
            comm->file->Enable(ID_PLAY, true);
            comm->file->Enable(ID_CREATE_GAME, true);
            comm->file->Enable(ID_JOIN_GAME, true);
            //write Lose MSG
            if(nb_opponent>0){
                char lose[12] = "lose";
                std::string sc = std::to_string(score);
                char const *pscore = sc.c_str();
                strcat(lose, comm->BufferName);
                strcat(lose, pscore);

                size_t txn = strlen(lose);
                unsigned char len;
                len = txn;

                sock->Write(&len, 1);
                sock->Write(&lose, len);
                sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
                comm->server_on = true;
            }



            comm->Setbusy(true);
            comm->m_rp->strings_score[0]->SetLabel(wxString::Format(wxT("%s Lose"), comm->UserName));
            //comm->CloseConnection();
            //panel->Destroy();
        }
    }
}

bool GamePanel::DoMove(const Piece& piece, int newX, int newY)
{

    for (int i = 0; i < 4; i++)
    {
        int x = newX + piece.x(i);
        int y = newY - piece.y(i);

        if (!CheckBounds(x, y) || PieceCheck(x, y) != None)
            return false;
    }

    current = piece;
    curX = newX;
    curY = newY;

    Refresh();

    return true;
}

void GamePanel::DrawPieceSquare(wxPaintDC& dc, int x, int y, PieceShape pieceShape)
{
    wxPen lightPen(light[int(pieceShape)]);
    lightPen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(lightPen);

    dc.DrawLine(x, y + Height() - 1, x, y);
    dc.DrawLine(x, y, x + Width() - 1, y);

    wxPen darkPen(dark[int(pieceShape)]);
    darkPen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(darkPen);

    dc.DrawLine(x + 1, y + Height() - 1, x + Width() - 1, y + Height() - 1);
    dc.DrawLine(x + Width() - 1, y + Height() - 1, x + Width() - 1, y + 1);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(colors[int(pieceShape)]));

    dc.DrawRectangle(x + 1, y + 1, Width() - 2, Height() - 2);
}


