// Author : huziang
//     this is a mpi cpp file to calculate pi by
// the method : pi/4 = 1 - 1/3 + 1/5 .....

#include <iostream>
#include <mpi.h>
using namespace std;

template <int len>
class Ldouble {
public:
    Ldouble() {
        pointpos = 1;
        for (int i = 0; i < len + 2; i++) {
            data[i] = '0';
        }
        data[len + 1] = '\0';
    }
    static void newMPIType(MPI_Datatype* ctype);
    void setVal(const long a);
    void plus(const Ldouble &);
    void minus(const Ldouble &);
    void multi(const long m);
    void divi(const long b);
    void print();
private:
    char data[len + 2];
    int pointpos;
};

// MPI data
template <int len>
void Ldouble<len>::newMPIType(MPI_Datatype* ctype) {
    int blockcounts[2];  
    MPI_Datatype oldtypes[2];  
    MPI_Aint offsets[2];  
  
    blockcounts[0] = len + 2;  
    blockcounts[1] = 1;  
  
    offsets[0] = 0;  
    offsets[1] = len + 2;  
  
    oldtypes[0] = MPI_CHAR;  
    oldtypes[1] = MPI_INT;  
  
    MPI_Type_struct(2, blockcounts, offsets, oldtypes, ctype);  
    MPI_Type_commit(ctype);
}

template <int len>
void Ldouble<len>::setVal(const long a) {
    data[pointpos - 1] = '0' + a;
    for (int i = 1; i < len + 2; i++) {
        data[i] = '0';
    }
    data[len + 1] = '\0';
}

template <int len>
void Ldouble<len>::plus(const Ldouble<len> &other) {
    int sum;
    bool carrybit = false;
    // bit add bit
    for (int i = len; i >= 0; i--) {
        sum = (this->data[i] - '0') + (other.data[i] - '0') + carrybit;
        carrybit = sum > 9;
        this->data[i] = sum % 10 + '0';
    }
}

template <int len>
void Ldouble<len>::minus(const Ldouble<len> &other) {
    int sum;
    bool carrybit = false;
    // bit minus bit
    for (int i = len; i >= 0; i--) {
        sum = 10 + (this->data[i] - '0') - (other.data[i] - '0') - carrybit;
        carrybit = sum < 10;
        this->data[i] = sum % 10 + '0';
    }
}

template <int len>
void Ldouble<len>::multi(const long m) {
    int carrybit = 0;
    int sum;
    // bit multiply int
    for (int i = len; i >= 0; i--) {
        sum = (data[i] - '0') * m + carrybit;
        carrybit = sum / 10;
        data[i] = sum % 10 + '0';
    }
}

template <int len>
void Ldouble<len>::divi(const long m) {
    // begin from 1 to len
    int remain = 0;
    for (int i = 0; i <= len; i++) {
        remain += data[i] - '0';
        data[i] = remain / m + '0';
        remain = remain % m * 10;
    }
}

template <int len>
void Ldouble<len>::print() {
    // print long double value
    for (int i = 0; i < pointpos; i++) {
        printf("%c", data[i]);
    }

    printf(".");
    printf("%s\n", data + pointpos);
}

const long precision = 10;
const long iteratime = 10000000;

void myProd(Ldouble<precision> *in, Ldouble<precision> *inout, int *lens, MPI_Datatype* dptr)  
{
    inout->plus(*in);
}  

int main(int argc, char **argv) {
    int myid, numprocs;
    int namelen;
    char processname[MPI_MAX_PROCESSOR_NAME];
    double begin, end;
    Ldouble<precision> *sum;

    // STEP : 1
    // init multicore process
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processname, &namelen);

    fprintf(stderr, "Process %d of %d on %s\n", myid, numprocs, processname);
    begin = MPI_Wtime();

    // STEP : 2
    // process main begin
    if (myid == 0) {
        sum = new Ldouble<precision>();
    }

    fprintf(stderr, "%d process enter calculate part!\n", myid);
    // process calculate begin
    Ldouble<precision> pipart;
    Ldouble<precision> temp;
    long beginindex = myid == 0 ? 1 : iteratime / numprocs * myid * 2 + 1;
    long endindex = myid == numprocs - 1 ? iteratime * 2 + 1 : (iteratime / numprocs ) * (myid + 1) * 2 + 1;
    fprintf(stderr, "%d process calculate [%ld, %ld)!\n", myid, beginindex, endindex);

    for (long i = beginindex; i < endindex; i += 2) {
        temp.setVal(1);
        temp.divi(i);
        //printf("debug %ld\n", i);
        if ((i - 1) % 4 == 0) {
            pipart.plus(temp);
        } else {
            pipart.minus(temp);
        }
    }

    // STEP : 3
    // process reduce
    MPI_Op myop;
    MPI_Datatype ctype;
    MPI_Op_create((MPI_User_function*)myProd,1,&myop);
    Ldouble<precision>::newMPIType(&ctype);    

    MPI_Reduce(&pipart, sum, 1, ctype, myop, 0, MPI_COMM_WORLD);  

    if (myid == 0) {
        sum->multi(4);
        sum->print();
    }

    end = MPI_Wtime();

    fprintf(stderr, "%d process over in %lf times!\n", myid, end - begin);

    MPI_Op_free(&myop); 
    MPI_Finalize();

    return 0;
}