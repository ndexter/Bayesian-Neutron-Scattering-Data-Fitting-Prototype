/*
 * An MPI testground.
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

#include "mpi.h"

int main(int argc, char *argv[]){

        int count, dest, tag;
        int process_id, process_num, master = 0;

        double start_time, end_time, elapsed_time;
        double data[10];

        MPI::Status status;

        MPI::Init(argc, argv);

        process_id = MPI::COMM_WORLD.Get_rank();
        process_num = MPI::COMM_WORLD.Get_size();

        if(process_id == master){
                start_time = MPI::Wtime();
        }

        double value[process_num-1][10];

        if(process_id == master){
                tag = 55;
                for(int i = 0; i < process_num-1; i++){
                        MPI::COMM_WORLD.Recv(value[i], 10,
                                MPI::DOUBLE, MPI::ANY_SOURCE, tag, status);
                }
                count = status.Get_count(MPI::DOUBLE);
        } else {
                for(int k = 0; k < 10; k++){
                        data[k] = process_id;
                }

                dest = 0;
                tag = 55;
                MPI::COMM_WORLD.Send(data, 10, MPI::DOUBLE, dest, tag);
        }

        end_time = MPI::Wtime();

        elapsed_time = end_time - start_time;
        if(process_id == master)
                cout << elapsed_time << endl;

        MPI::Finalize();

        if(process_id == master){
                for(int i = 0; i < 10; i++){
                        for(int j = 0; j < process_num-1; j++){
                                cout << value[j][i] << " ";
                                if(j == process_num-2)
                                        cout << "\n";
                        }
                }
        }

        return 0;
}
