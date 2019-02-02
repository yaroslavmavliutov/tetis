#include "main.h"
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


    if (rank == 1)
        sleep(10);
    if (rank == 0) {
        char who;
        std::cout << "Do you want to be a server (s) or a client (c) ? ";
        std::cin >> who;

        if (who == 's') {

            Frame *server = new Frame(wxT("server"), 0);
            server->Show(true);
            /*
            int i = 0;
            while (i<10){
                cout << "    SSS : " << i << endl;
                i++;
                sleep(1);
            }*/
        }
        if (who == 'c') {
            int i = 0;
            while (i<10){
                cout << "        CCC : " << i << endl;
                i++;
                sleep(1);
            }
            /*Frame *client = new Frame(wxT("client"), 1);
            client->Show(true); */
        }
    }
    if(rank == 1)
    {
        /*sf::Thread *thread = 0;
        client_server *cs = new client_server();
        char who;
        std::cout << "Do you want to be a server (s) or a client (c) ? ";
        std::cin >> who;

        if (who == 's') {
            cs->server();
        }
        if (who == 'c') {
            cs->client();
        }*/
        int i = 0;
        while (i<5){
            cout << " i : " << i << endl;
            i++;
            sleep(5);
        }
    }
    MPI_Finalize();

    //Frame *server = new Frame(wxT("server"), 0);
    //server->Show(true);

    return true;
}