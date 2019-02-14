#include "main.h"
#include "Frame.h"
#include <unistd.h>

IMPLEMENT_APP(TetrisApp);

bool TetrisApp::OnInit()
{
    int rank, size;
    int argc = 1;
    MPI_Init (&argc, NULL);      // starts MPI
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);       // get current process id
    MPI_Comm_size (MPI_COMM_WORLD, &size);        // get number of processes
    printf( "MAIN from process %d of %d\n", rank, size );

    // try realise -client + tetris
    //if (rank == 1)
    //    sleep(10);
    if (rank == 0) {
        char who;
        std::cout << "Do you want to be a server (s) or a client (c) ? ";
        std::cin >> who;

        if (who == 's') {

            Frame* tetrisGame = new Frame(wxT("WxTetris"), 0);
            tetrisGame->Centre();
            tetrisGame->Show(true);
        }
        if (who == 'c') {
            int i = 0;
            while (i<10){
                cout << "        CCC : " << i << endl;
                i++;
                sleep(1);
            }

        }
    }
    if(rank == 1)
    {

        int i = 0;
        while (i<5){
            cout << " i : " << i << endl;
            i++;
            sleep(5);
        }
    }
    MPI_Finalize();


    return true;
}