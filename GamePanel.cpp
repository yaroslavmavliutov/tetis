
#include "GamePanel.h"
#include "Piece.h"
#include "InfoPanel.h"
#include "Frame.h"
#include <chrono>

GamePanel::GamePanel(wxPanel* parent_t, wxFrame *fr, wxSocketClient *m_sock, int m_nb_opponent) : Board(parent_t, fr)
{
    sock = m_sock;
    timer = new wxTimer(this, 1);
    TIMER_INTERVAL = 500;
    nb_opponent = m_nb_opponent;

    Connect(wxEVT_PAINT, wxPaintEventHandler(GamePanel::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GamePanel::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(GamePanel::OnTimer));

//    std::cout << "GamePanel - constructor" << std::endl;
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
    if(nb_opponent==1)
        sendMoveToServer('p');

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

void GamePanel::OnKeyDown(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();

    if(nb_opponent == 0){
        switch (keyCode)
        {
            case 'P':
                Pause();
                return;
        }
    }

    if (!started || current.GetShape() == None || paused)
    {
        event.Skip();
        return;
    }

    switch (keyCode) {
        case WXK_SPACE:
            sendMoveToServer('d');
            DropDown();
            break;
        case WXK_UP:
            sendMoveToServer('m');
            DoMove(current.Rotation(), curX, curY);
            break;
        case WXK_DOWN:
            sendMoveToServer('o');
            DropOneLine();
            break;
        case WXK_LEFT:
            sendMoveToServer('l');
            DoMove(current, curX - 1, curY);
            break;
        case WXK_RIGHT:
            sendMoveToServer('r');
            DoMove(current, curX + 1, curY);
            break;
        default:
            event.Skip();
    }
}


void GamePanel::OnTimer(wxCommandEvent& event)
{
    if (pieceFallingFinished)
    {
        pieceFallingFinished = false;
        MakeNewPiece();
    }
    else {
        DropOneLine();
        if (nb_opponent == 1)
            sendMoveToServer('o');
    }
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
    if(nb_opponent==1)
        sendMoveToServer('x');
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

    Frame *comm = (Frame *) panel->GetParent();
    comm->getm_rp()->strings_score[0]->SetLabel(wxString::Format(wxT("%s score: %d"), comm->getUserName(), score));

    if (nb_opponent > 0) {
        char score_char[15] = "score";
        std::string sc = std::to_string(score);
        char const *pscore = sc.c_str();
        strcat(score_char, comm->getBufferName());
        strcat(score_char, pscore);

        size_t txn = strlen(score_char);

        unsigned char len;
        len = txn;
        sock->Write(&len, 1);//send the length of the message first
        if (sock->Write(score_char, txn).LastCount() != txn) {
//            std::cout << "Write error.\n";
            return;
        } else {
//            std::cout << "CLIENT send score Tx: " << score_char << "\n";
        }
    }

    pieceFallingFinished = true;
    current.SetShape(None);
    timer->Start(this->TIMER_INTERVAL);
    Refresh();
}


void GamePanel::RandomPiece()
{

    current.SetShape(next.GetShape());
    if(nb_opponent==1)
        sendShapeToServer(current.GetShape(), 0);


    PieceShape tmp;
    tmp = PieceShape(rand() % 7 + 1);

    if(nb_opponent==1)
        sendShapeToServer(tmp, 1);
    next.SetShape(tmp);

    Frame *comm = (Frame *) panel->GetParent();
    comm->getm_rp()->piece.SetShape(None);
    comm->getm_rp()->ClearPeace();

    comm->getm_rp()->piece.SetShape(next.GetShape());
    comm->getm_rp()->ChangePeace();

}

void GamePanel::MakeNewPiece()
{

    RandomPiece();
    curX = BoardWidth / 2;
    curY = BoardHeight - 1 + current.MinY();

    if (!DoMove(current, curX, curY))
    {
        current.SetShape(None);
        timer->Stop();
        started = false;
        status_scr->SetStatusText(wxT("You Lose :("));
        Frame *comm = (Frame *) panel->GetParent();
        comm->file->Enable(ID_PLAY, true);
        //write Lose MSG
        if(nb_opponent>0){
            char lose[12] = "lose";
            std::string sc = std::to_string(score);
            char const *pscore = sc.c_str();
            strcat(lose, comm->getBufferName());
            strcat(lose, pscore);

            size_t txn = strlen(lose);
            unsigned char len;
            len = txn;

            sock->Write(&len, 1);
            sock->Write(&lose, len);
            sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
            comm->setServerOn(true);
        }

        comm->Setbusy(true);
        comm->getm_rp()->strings_score[0]->SetLabel(wxString::Format(wxT("%s final score: %d"), comm->getUserName(), score));
        if(nb_opponent==0){
            comm->file->Enable(ID_CREATE_GAME, true);
            comm->file->Enable(ID_JOIN_GAME, true);
        }
    }
}


void GamePanel::sendMoveToServer(char c) {
    unsigned char len;
    size_t txn;
    char move[5] = "move";
    if(nb_opponent==1){
        move[4] = c;
        txn = strlen(move);
        len = txn;
        sock->Write(&len,1);
        sock->Write(&move, len);
    }
}


void GamePanel::sendShapeToServer(PieceShape ps, int curr_or_next) {
    unsigned char len;
    size_t txn;
    char c;


    if(nb_opponent==1){
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




void GamePanel::SetMovement(char c) {
    switch (c) {
        case 'd':
            if (started == true) {
                sendMoveToServer('d');
                DropDown();
            }
            break;

        default:
            //event.Skip();
            std::cout << "Non move value" << std::endl;
    }
}