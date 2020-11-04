#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define NUM_THREADS	200
int matx[200][200];
int conv[200][200];
int row_count = 0;
int col_count = 0;

// struct 3 int array
typedef struct fileData{
    int row;
    int col;
    int m1;
    int m2;
    int m3;
    int l1;
    int l2;
    int matrix[200][200];
} fileData;

fileData readfile();
void *mask_matrix(void *f1);
void *mask_row(void *f1);
void *mask_ele(void *f1);

int main (int argc, char *argv[]){
    fileData f1 = readfile();
    
    pthread_t threads[NUM_THREADS];
    long t;
    int rc;
    void *c_matrix;
    int x;


    //Method 1, does the entire mask on 1 thread
    rc = pthread_create(&threads[0], NULL, mask_matrix, &f1);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);      
    }
    rc = pthread_join(threads[0], NULL);

    //Method 2, does cov mask by row, 1 thread per row
    // for(int i = 0; i < f1.row; i ++){
    //     rc = pthread_create(&threads[i], NULL, mask_row, &f1);
    // }

    // for(int i = 0; i < f1.row; i++){
    //     pthread_join(threads[i], NULL);
    //     row_count++;
    // }

    //Method 3, create a thread for each index 
    // for(int i = 0; i < f1.row; i ++){
    //     for(int j = 0; j < f1.col; j++){
    //         printf("thread %d created\n", (i*f1.col) + j);
    //         rc = pthread_create(&threads[(i*f1.col) + j], NULL, mask_ele, &f1);
            
    //     }
        
    // }
    // for(int i = 0; i < f1.row; i ++){
    //     for(int j = 0; j < f1.col; j++){
    //         pthread_join(threads[(i*f1.col) + j], NULL);
    //         col_count++;
    //     }
    //     row_count++;
    // }
    pthread_exit(NULL);

}

void *mask_matrix(void *f1){
    fileData f2 = (*(fileData*) f1);
    int c_matrix[f2.row][f2.col];

    for(int i = 0; i < f2.row; i ++){
        for(int j = 0; j < f2.col; j++){
            if (j == 0){
                conv[i][j] = ((f2.matrix[i][j] * f2.m2) + (f2.matrix[i][j+1] * f2.m3));
            }
            if (j == (f2.col-1)){
                conv[i][j] = ((f2.matrix[i][j-1] * f2.m1) + (f2.matrix[i][j] * f2.m2));
            }
            else {              
                conv[i][j] = ((f2.matrix[i][j-1] * f2.m1) + (f2.matrix[i][j] * f2.m2) + (f2.matrix[i][j+1] * f2.m3));
            }
        }
    }

    for(int i = 0; i < f2.row; i ++){
        for(int j = 0; j < f2.col; j++){
            printf("%d\n", conv[i][j]);
        }
    }

    pthread_exit(NULL);
}

fileData readfile(){

    FILE* inFile = NULL;
    int ele;
    fileData f1;


    inFile = fopen("2x10.txt", "r");
    if (inFile == NULL) {
        printf("Could not open file\n");
    }

    //scan in first row to look assign size of row and column
    fscanf(inFile, "%d %d", &f1.row,&f1.col);

    for(int i = 0; i < f1.row; i ++){
        for(int j = 0; j < f1.col; j++){
            //split data in row base on value of J
            fscanf(inFile, "%d", &ele);
            matx[i][j] = ele;
            f1.matrix[i][j] = ele;
        }
    }
    fscanf(inFile, "%d %d", &f1.l1, &f1.l2);
    fscanf(inFile, "%d %d %d", &f1.m1,&f1.m2,&f1.m3);
    return f1;
}

void *mask_row(void *f1){
    fileData f2 = (*(fileData*) f1);

    
    // printf("%d\n", row_count);
    for(int i = 0; i < f2.col; i ++){
        if (i == 0){
            conv[row_count][i] = ((f2.matrix[row_count][i] * f2.m2) + (f2.matrix[row_count][i+1] * f2.m3));
        }
        if (i == (f2.col-1)){
            conv[row_count][i] = ((f2.matrix[row_count][i-1] * f2.m1) + (f2.matrix[row_count][i] * f2.m2));
        }
        else {              
            conv[row_count][i] = ((f2.matrix[row_count][i-1] * f2.m1) + (f2.matrix[row_count][i] * f2.m2) + (f2.matrix[row_count][i+1] * f2.m3));
        }
    }
    // for(int i = 0; i < f2.col; i ++){
    //         printf("%d\n", conv[f2.row_count][i]);
    // }
    pthread_exit(NULL);
}

void *mask_ele(void *f1){
    fileData f2 = (*(fileData*) f1);
    printf("row: %d\n", row_count);
    printf("col: %d\n", col_count);
    if (col_count == 0){
        conv[row_count][col_count] = ((f2.matrix[row_count][col_count] * f2.m2) + (f2.matrix[row_count][col_count+1] * f2.m3));
    }
    if (col_count == f2.col-1){
        conv[row_count][col_count] = ((f2.matrix[row_count][col_count-1] * f2.m1) + (f2.matrix[row_count][col_count] * f2.m2));
    }
    else{
        conv[row_count][col_count] = ((f2.matrix[row_count][col_count-1] * f2.m1) + (f2.matrix[row_count][col_count] * f2.m2) + (f2.matrix[row_count][col_count+1] * f2.m3));
    }
    pthread_exit(NULL);
}










































































































// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <time.h>

//  void pthreadConvolveMatx1Row(void);
//  void pthreadConvolveMatx2Row(void);
//  void *convolveMatx1One(void* dicks);
//  void *convolveMatx2One(void* dicks);
//  void readFile1(FILE *fp);
//  void readFile2(FILE *fp);

//  int Matx1[50][50], Matx2[50][50];
//  int Filt1[5], Filt2[5];
//  int Rslt1[50][50], Rslt2[50][50];

// /*typedef struct matrixData{
// 	int rowsMatx1, rowsMatx2;
// 	int colsMatx1, colsMatx2;
// 	int rowsFilt1, rowsFilt2;
// 	int colsFilt1, colsFilt2;
// 	int sizeMatx1, sizeMatx2;
// 	int sizeFilt1, sizeFilt2;
// 	//int Matx1[50][50], Matx2[50][50];
// 	//int Filt1[5][5], Filt2[5][5];
// 	//int Rslt1[50][50], Rslt2[50][50];
// 	//int Matx1[rowsMatx1][colsMatx1];
// 	char *dicks;
// 	int tid;
// } matrixData;
// struct matrixData data;
// */

//  int rowMat, colMat, rowFilt, colFilt, sizeMat, sizeFilt;
//  int rowMat1, colMat1, rowFilt1, colFilt1, sizeMat1, sizeFilt1;
//  struct thread {
//      int p1, p2, result;
//  };

// // -- Matrix 1 --
//  void *convolveMatx1Row(void* info) {

//      struct thread *t = (struct thread*)info;
//      int temp = 0;
//      int i, j;

//      for (int j = 0; j < colMat; j++) {
//          if ((j-1) >= 0) {
//              temp += Matx1[j-1][(*t).p1] * Filt1[0];
//          }
//          temp += Matx1[j][(*t).p1] * Filt1[1];
//          if ((j+1) < colMat) {
//              temp += Matx1[j+1][(*t).p1] * Filt1[2];
//          }
//          Rslt1[j][(*t).p1] = temp;
//          temp = 0;
//      }
//      pthread_exit(0);
//  }

//  void pthreadConvolveMatx1Row(void) {
//      pthread_t *p1 = malloc(rowMat * sizeof(pthread_t));
//      struct thread *convolve = malloc(rowMat * sizeof(struct thread));

//      for (int i = 0; i < rowMat; i++) {
//          convolve[i].p1 = i;
//          pthread_create(&p1[i], NULL, convolveMatx1Row, (void *)&convolve[i]);
//      }
//      for (int i = 0; i < rowMat; i++) {
//          pthread_join(p1[i], NULL);
//      }
//      return;
//  }

// // -- Matrix 2 --
//  void *convolveMatx2Row(void* info) {

//      struct thread *t = (struct thread*)info;
//      int temp = 0;
//      int i, j;
//      for (int j = 0; j < colMat1; j++) {
//          if ((j-1) >= 0) {
//              temp += Matx2[j-1][(*t).p1] * Filt2[0];
//          }
//          temp += Matx2[j][(*t).p1] * Filt2[1];
//          if ((j+1) < colMat1) {
//              temp += Matx2[j+1][(*t).p1] * Filt2[2];
//          }
//          Rslt2[j][(*t).p1] = temp;
//          temp = 0;
//      }
//      pthread_exit(0);
//  }

//  void pthreadConvolveMatx2Row(void) {
//      pthread_t *p1 = malloc(rowMat1 * sizeof(pthread_t));
//      struct thread *convolve = malloc(rowMat1 * sizeof(struct thread));

//      for (int i = 0; i < rowMat1; i++) {
//          convolve[i].p1 = i;
//          pthread_create(&p1[i], NULL, convolveMatx2Row, (void *)&convolve[i]);
//      }
//      for (int i = 0; i < rowMat1; i++) {
//          pthread_join(p1[i], NULL);
//      }
//      return;
//  }



// /*void pthreadConvolveMatx1Each(void) {
//     pthread_t *p1 = malloc(sizeMat * sizeof(pthread_t));
//     struct thread *convolve = malloc(sizeMat * sizeof(struct thread));

//     for (int i = 0; i < sizeMat; i++) {
//    	 convolve[i].p
// */
//  void readFile1(FILE *fp) {
//      //get matrix 1 data from file 1
//      fscanf(fp, "%d %d", &rowMat, &colMat);
//      //printf("Rows of matrix 1: %d\n", rowMat);
//      //printf("Columns of matrix 1: %d\n", data.colsMatx1);
//      int i, j;
//      for (i = 0; i < rowMat; i++) {
//          for (j = 0; j < colMat; j++) {
//              if(fscanf(fp, "%d", &(Matx1[j][i])) != 1) {
//                  printf("\nError\n");
//              }
//          }
//      }
//      sizeMat = rowMat * colMat;

//      //get filter 2 data from file 1
//      fscanf(fp, "%d %d", &rowFilt, &colFilt);
//      //printf("Rows of Filter 1: %d\n", data.rowsFilt1);
//      //printf("Columns of Filter 1: %d\n", data.colsFilt1);
//      for (i = 0; i < rowFilt; i++) {
//          for (j = 0; j < colFilt; j++) {
//              if(fscanf(fp, "%d", &(Filt1[j])) != 1) {
//                  printf("\nError\n");
//              }
//          }
//      }
//      sizeFilt = rowFilt * colFilt;

//      /*printf("Size of Filter 1: %d\n", sizeFilt);
//      for (i = 0; i < rowFilt; i++) {
//          for (j = 0; j < colFilt; j++) {
//              printf("%d\n", Filt1[i]);
//          }
//      }
//      printf("Size of matrix 1: %d\n", sizeMat);
//      for (i = 0; i < rowMat; i++) {
//          for (j = 0; j < colMat; j++) {
//              printf("%d\n", Matx1[j][i]);
//          }
//      }*/
//      fclose(fp);
//      return;
//  }

//  void readFile2(FILE *fp) {
// //get matrix 2 data from file 2
//      fscanf(fp, "%d %d", &rowMat1, &colMat1);
//      //printf("Rows of matrix 1: %d\n", rowMat);
//      //printf("Columns of matrix 1: %d\n", data.colsMatx1);
//      int i, j;
//      for (i = 0; i < rowMat1; i++) {
//          for (j = 0; j < colMat1; j++) {
//              if(fscanf(fp, "%d", &(Matx2[j][i])) != 1) {
//                  printf("\nError\n");
//              }
//          }
//      }
//      sizeMat1 = rowMat1 * colMat1;

//      //get filter 2 data from file 1
//      fscanf(fp, "%d %d", &rowFilt1, &colFilt1);
//      //printf("Rows of Filter 1: %d\n", data.rowsFilt1);
//      //printf("Columns of Filter 1: %d\n", data.colsFilt1);
//      for (i = 0; i < rowFilt1; i++) {
//          for (j = 0; j < colFilt1; j++) {
//              if(fscanf(fp, "%d", &(Filt2[j])) != 1) {
//                  printf("\nError\n");
//              }
//          }
//      }
//      sizeFilt1 = rowFilt1 * colFilt1;

//      /*printf("Size of Filter 1: %d\n", sizeFilt1);
//      for (i = 0; i < rowFilt1; i++) {
//          for (j = 0; j < colFilt1; j++) {
//              printf("%d\n", Filt2[i]);
//          }
//      }
//      printf("Size of matrix 1: %d\n", sizeMat1);
//      for (i = 0; i < rowMat1; i++) {
//          for (j = 0; j < colMat1; j++) {
//              printf("%d\n", Matx2[j][i]);
//          }
//      }*/
//      fclose(fp);
//      return;
//  }

//  int main(void) {
//      printf("start\n");
//      FILE *fp1, *fp2;
//      fp1 = fopen("2x10.txt", "r");
//      fp2 = fopen("20X10.txt", "r");
//      if(fp1 == NULL || fp2 == NULL) {
//          printf("Error!\n");
//          exit(1);
//      }

//      //pthread_create(
//      readFile1(fp1);
//      readFile2(fp2);

//      struct timespec start, finish;

//      clock_gettime(CLOCK_MONOTONIC, &start);
//      //rows
//      for (int i = 0; i < 10; i++) {
//          pthreadConvolveMatx1Row();
//      }
//      clock_gettime(CLOCK_MONOTONIC, &finish);
//      printf("\nAvg Time1: %lu nsec\n", (unsigned long)(finish.tv_nsec - start.tv_nsec)/10);

//      clock_gettime(CLOCK_MONOTONIC, &start);
//      //rows
//      for (int i = 0; i < 10; i++) {
//          pthreadConvolveMatx2Row();
//      }
//      clock_gettime(CLOCK_MONOTONIC, &finish);
//      printf("\nAvg Time2: %lu nsec\n", (unsigned long)(finish.tv_nsec - start.tv_nsec)/10);

//      printf("Contents of Matrix\n");
//      for (int i = 0; i < rowMat; i++) {
//          for (int j = 0; j < colMat; j++) {
//              printf("%d ", Matx1[j][i]);
//          }
//      }
//      printf("\nContents of Convultion\n");
//      for (int i = 0; i < rowMat; i++) {
//          for (int j = 0; j < colMat; j++) {
//              printf("%d ", Rslt1[j][i]);
//          }
//      }

//      printf("\nContents of Matrix2\n");
//      for (int i = 0; i < rowMat1; i++) {
//          for (int j = 0; j < colMat1; j++) {
//              printf("%d ", Matx2[j][i]);
//          }
//      }
//      printf("\nContents of Convultion 2\n");
//      for (int i = 0; i < rowMat1; i++) {
//          for (int j = 0; j < colMat1; j++) {
//              printf("%d ", Rslt2[j][i]);
//          }
//      }
//      printf("\nend\n");
//      return 0;
//  }

//  PART 2:
// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sched.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <sys/timerfd.h>
// #include <time.h>
// #include <string.h>
// #include <pthread.h>

// #define MY_PRIORITY 51

// //Period values for write and read
// #define Period_nseconds_write 10000000//2000000
// #define Period_nseconds_read 20000000//3000000

// //Initialize times for write and read
// #define InitnSeconds_write 2000000
// #define InitnSeconds_read1 1000000
// #define InitnSeconds_read2 3000000


//  char buff[100];
//  char song[20][50];
//  uint64_t buf = 0;
//  typedef struct Data{
//      char *filename;
//  }data;

//  void *read1(void* filename) {
// // --- Initialize current thread as RT task ---
//      struct sched_param param;
//      // Declare ourself as a real time task by elevating our priority
//      // and setting an appropriate scheduling policy.
//      param.sched_priority = MY_PRIORITY;
//      sched_setscheduler(0, SCHED_FIFO, &param);    // this function has a return value
//      // Always good to check for errors. Look for sched_setscheduler()

// // --- Configure Timer ---
//      int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);    // returns a file descriptor
//      // Always good to check for errors. Look for timerfd_create()

//      struct itimerspec itval;    // structure to hold period and starting time
//      // timer fires every ## nsec
//      itval.it_interval.tv_nsec = Period_nseconds_read;    // check the data type

//      // Timer will start in ## nsec from the moment the timer is started
//      itval.it_value.tv_nsec = InitnSeconds_read1;   	 // check the data type

//      // start the timer (you need the file descriptor and itval structure above)
//      timerfd_settime(timer_fd, 0, &itval, NULL);    // this function has a return value
//      // Always good to check for errors. Look for timerfd_settime()

// // -- Get Line --
//      data* temp;
//      temp = (data*)filename;

//      FILE* fd = fopen(temp->filename, "r");
//      while (fgets(buff, 50, fd) != NULL) {
//          read(timer_fd, &buf, sizeof(uint64_t));
//      }

//      fclose(fd);
//      pthread_exit(0);
//  }

//  void *read2(void* filename) {
// // --- Initialize current thread as RT task ---
//      struct sched_param param;
//      // Declare ourself as a real time task by elevating our priority
//      // and setting an appropriate scheduling policy.
//      param.sched_priority = MY_PRIORITY;
//      sched_setscheduler(0, SCHED_FIFO, &param);    // this function has a return value
//      // Always good to check for errors. Look for sched_setscheduler()

// // --- Configure Timer ---
//      int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);    // returns a file descriptor
//      // Always good to check for errors. Look for timerfd_create()

//      struct itimerspec itval;    // structure to hold period and starting time
//      // timer fires every ## nsec
//      itval.it_interval.tv_nsec = Period_nseconds_read;    // check the data type

//      // Timer will start in ## nsec from the moment the timer is started
//      itval.it_value.tv_nsec = InitnSeconds_read2;   	 // check the data type

//      // start the timer (you need the file descriptor and itval structure above)
//      timerfd_settime(timer_fd, 0, &itval, NULL);    // this function has a return value
//      // Always good to check for errors. Look for timerfd_settime()

// // -- Get Line --
//      data* temp;
//      temp = (data*)filename;
//      FILE* fd = fopen(temp->filename, "r");
//      while (fgets(buff, 50, fd) != NULL) {
//          read(timer_fd, &buf, sizeof(uint64_t));
//      }

//      fclose(fd);
//      pthread_exit(0);
//  }

//  void *writeSong(void* filename) {
// // --- Initialize current thread as RT task ---
//      struct sched_param param;
//      // Declare ourself as a real time task by elevating our priority
//      // and setting an appropriate scheduling policy.
//      param.sched_priority = MY_PRIORITY;
//      sched_setscheduler(0, SCHED_FIFO, &param);    // this function has a return value
//      // Always good to check for errors. Look for sched_setscheduler()

// // --- Configure Timer ---
//      int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);    // returns a file descriptor
//      // Always good to check for errors. Look for timerfd_create()

//      struct itimerspec itval;    // structure to hold period and starting time
//      // timer fires every ## nsec
//      itval.it_interval.tv_nsec = Period_nseconds_write;    // check the data type

//      // Timer will start in ## nsec from the moment the timer is started
//      itval.it_value.tv_nsec = InitnSeconds_write;   	 // check the data type

//      // start the timer (you need the file descriptor and itval structure above)
//      timerfd_settime(timer_fd, 0, &itval, NULL);    // this function has a return value
//      // Always good to check for errors. Look for timerfd_settime()

// // -- Write Line to File --
//      data* temp;
//      temp = (data*)filename;
//      int num;
//      FILE *fd = fopen(temp->filename, "w");
//      int i;
//      for (i = 0; i < 20; i++) {
//          strcpy(song[i], buff);
//          fprintf(fd, "%s", song[i]);
//          read(timer_fd, &buf, sizeof(uint64_t));
//      }
//      fclose(fd);
//      pthread_exit(0);
//  }

//  int main(void){
//      char *filename1 = "first.txt";
//      char *filename2 = "second.txt";
//      char *filename3 = "song.txt";

//      pthread_t t1, t2, t3;

//      pthread_create(&t1, NULL, (void *)&read1, (void *)&filename1);
//      pthread_create(&t3, NULL, (void *)&writeSong, (void *)&filename3);
//      pthread_create(&t2, NULL, (void *)&read2, (void *)&filename2);


//      pthread_join(t1, NULL);
//      pthread_join(t3, NULL);
//      pthread_join(t2, NULL);

// // -- Read Line From File and Print to Terminal --
//      FILE *fd = fopen("song.txt", "r");
//      int i;
//      for (i = 0; i < 20; i++) {
//          fgets(buff, 50, fd);
//          printf("%s\n", buff);
//      }

//      return 0;
//  }

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <unistd.h>
// // #include <pthread.h>
// // #include <time.h>

// //  void pthreadConvolveMatx1Row(void);
// //  void pthreadConvolveMatx2Row(void);
// //  void *convolveMatx1One(void* dicks);
// //  void *convolveMatx2One(void* dicks);
// //  void readFile1(FILE *fp);
// //  void readFile2(FILE *fp);

// //  int Matx1[50][50], Matx2[50][50];
// //  int Filt1[5], Filt2[5];
// //  int Rslt1[50][50], Rslt2[50][50];

// // /*typedef struct matrixData{
// // 	int rowsMatx1, rowsMatx2;
// // 	int colsMatx1, colsMatx2;
// // 	int rowsFilt1, rowsFilt2;
// // 	int colsFilt1, colsFilt2;
// // 	int sizeMatx1, sizeMatx2;
// // 	int sizeFilt1, sizeFilt2;
// // 	//int Matx1[50][50], Matx2[50][50];
// // 	//int Filt1[5][5], Filt2[5][5];
// // 	//int Rslt1[50][50], Rslt2[50][50];
// // 	//int Matx1[rowsMatx1][colsMatx1];
// // 	char *dicks;
// // 	int tid;
// // } matrixData;
// // struct matrixData data;
// // */

// //  int rowMat, colMat, rowFilt, colFilt, sizeMat, sizeFilt;
// //  int rowMat1, colMat1, rowFilt1, colFilt1, sizeMat1, sizeFilt1;
// //  struct thread {
// //      int p1, p2, result;
// //  };

// // // -- Matrix 1 --
// //  void *convolveMatx1Row(void* info) {

// //      struct thread *t = (struct thread*)info;
// //      int temp = 0;
// //      int i, j;

// //      for (int j = 0; j < colMat; j++) {
// //          if ((j-1) >= 0) {
// //              temp += Matx1[j-1][(*t).p1] * Filt1[0];
// //          }
// //          temp += Matx1[j][(*t).p1] * Filt1[1];
// //          if ((j+1) < colMat) {
// //              temp += Matx1[j+1][(*t).p1] * Filt1[2];
// //          }
// //          Rslt1[j][(*t).p1] = temp;
// //          temp = 0;
// //      }
// //      pthread_exit(0);
// //  }

// //  void pthreadConvolveMatx1Row(void) {
// //      pthread_t *p1 = malloc(rowMat * sizeof(pthread_t));
// //      struct thread *convolve = malloc(rowMat * sizeof(struct thread));

// //      for (int i = 0; i < rowMat; i++) {
// //          convolve[i].p1 = i;
// //          pthread_create(&p1[i], NULL, convolveMatx1Row, (void *)&convolve[i]);
// //      }
// //      for (int i = 0; i < rowMat; i++) {
// //          pthread_join(p1[i], NULL);
// //      }
// //      return;
// //  }

// // // -- Matrix 2 --
// //  void *convolveMatx2Row(void* info) {

// //      struct thread *t = (struct thread*)info;
// //      int temp = 0;
// //      int i, j;
// //      for (int j = 0; j < colMat1; j++) {
// //          if ((j-1) >= 0) {
// //              temp += Matx2[j-1][(*t).p1] * Filt2[0];
// //          }
// //          temp += Matx2[j][(*t).p1] * Filt2[1];
// //          if ((j+1) < colMat1) {
// //              temp += Matx2[j+1][(*t).p1] * Filt2[2];
// //          }
// //          Rslt2[j][(*t).p1] = temp;
// //          temp = 0;
// //      }
// //      pthread_exit(0);
// //  }

// //  void pthreadConvolveMatx2Row(void) {
// //      pthread_t *p1 = malloc(rowMat1 * sizeof(pthread_t));
// //      struct thread *convolve = malloc(rowMat1 * sizeof(struct thread));

// //      for (int i = 0; i < rowMat1; i++) {
// //          convolve[i].p1 = i;
// //          pthread_create(&p1[i], NULL, convolveMatx2Row, (void *)&convolve[i]);
// //      }
// //      for (int i = 0; i < rowMat1; i++) {
// //          pthread_join(p1[i], NULL);
// //      }
// //      return;
// //  }



// // /*void pthreadConvolveMatx1Each(void) {
// //     pthread_t *p1 = malloc(sizeMat * sizeof(pthread_t));
// //     struct thread *convolve = malloc(sizeMat * sizeof(struct thread));

// //     for (int i = 0; i < sizeMat; i++) {
// //    	 convolve[i].p
// // */
// //  void readFile1(FILE *fp) {
// //      //get matrix 1 data from file 1
// //      fscanf(fp, "%d %d", &rowMat, &colMat);
// //      //printf("Rows of matrix 1: %d\n", rowMat);
// //      //printf("Columns of matrix 1: %d\n", data.colsMatx1);
// //      int i, j;
// //      for (i = 0; i < rowMat; i++) {
// //          for (j = 0; j < colMat; j++) {
// //              if(fscanf(fp, "%d", &(Matx1[j][i])) != 1) {
// //                  printf("\nError\n");
// //              }
// //          }
// //      }
// //      sizeMat = rowMat * colMat;

// //      //get filter 2 data from file 1
// //      fscanf(fp, "%d %d", &rowFilt, &colFilt);
// //      //printf("Rows of Filter 1: %d\n", data.rowsFilt1);
// //      //printf("Columns of Filter 1: %d\n", data.colsFilt1);
// //      for (i = 0; i < rowFilt; i++) {
// //          for (j = 0; j < colFilt; j++) {
// //              if(fscanf(fp, "%d", &(Filt1[j])) != 1) {
// //                  printf("\nError\n");
// //              }
// //          }
// //      }
// //      sizeFilt = rowFilt * colFilt;

// //      /*printf("Size of Filter 1: %d\n", sizeFilt);
// //      for (i = 0; i < rowFilt; i++) {
// //          for (j = 0; j < colFilt; j++) {
// //              printf("%d\n", Filt1[i]);
// //          }
// //      }
// //      printf("Size of matrix 1: %d\n", sizeMat);
// //      for (i = 0; i < rowMat; i++) {
// //          for (j = 0; j < colMat; j++) {
// //              printf("%d\n", Matx1[j][i]);
// //          }
// //      }*/
// //      fclose(fp);
// //      return;
// //  }

// //  void readFile2(FILE *fp) {
// // //get matrix 2 data from file 2
// //      fscanf(fp, "%d %d", &rowMat1, &colMat1);
// //      //printf("Rows of matrix 1: %d\n", rowMat);
// //      //printf("Columns of matrix 1: %d\n", data.colsMatx1);
// //      int i, j;
// //      for (i = 0; i < rowMat1; i++) {
// //          for (j = 0; j < colMat1; j++) {
// //              if(fscanf(fp, "%d", &(Matx2[j][i])) != 1) {
// //                  printf("\nError\n");
// //              }
// //          }
// //      }
// //      sizeMat1 = rowMat1 * colMat1;

// //      //get filter 2 data from file 1
// //      fscanf(fp, "%d %d", &rowFilt1, &colFilt1);
// //      //printf("Rows of Filter 1: %d\n", data.rowsFilt1);
// //      //printf("Columns of Filter 1: %d\n", data.colsFilt1);
// //      for (i = 0; i < rowFilt1; i++) {
// //          for (j = 0; j < colFilt1; j++) {
// //              if(fscanf(fp, "%d", &(Filt2[j])) != 1) {
// //                  printf("\nError\n");
// //              }
// //          }
// //      }
// //      sizeFilt1 = rowFilt1 * colFilt1;

// //      /*printf("Size of Filter 1: %d\n", sizeFilt1);
// //      for (i = 0; i < rowFilt1; i++) {
// //          for (j = 0; j < colFilt1; j++) {
// //              printf("%d\n", Filt2[i]);
// //          }
// //      }
// //      printf("Size of matrix 1: %d\n", sizeMat1);
// //      for (i = 0; i < rowMat1; i++) {
// //          for (j = 0; j < colMat1; j++) {
// //              printf("%d\n", Matx2[j][i]);
// //          }
// //      }*/
// //      fclose(fp);
// //      return;
// //  }

// //  int main(void) {
// //      printf("start\n");
// //      FILE *fp1, *fp2;
// //      fp1 = fopen("2x10.txt", "r");
// //      fp2 = fopen("20X10.txt", "r");
// //      if(fp1 == NULL || fp2 == NULL) {
// //          printf("Error!\n");
// //          exit(1);
// //      }

// //      //pthread_create(
// //      readFile1(fp1);
// //      readFile2(fp2);

// //      struct timespec start, finish;

// //      clock_gettime(CLOCK_MONOTONIC, &start);
// //      //rows
// //      for (int i = 0; i < 10; i++) {
// //          pthreadConvolveMatx1Row();
// //      }
// //      clock_gettime(CLOCK_MONOTONIC, &finish);
// //      printf("\nAvg Time1: %lu nsec\n", (unsigned long)(finish.tv_nsec - start.tv_nsec)/10);

// //      clock_gettime(CLOCK_MONOTONIC, &start);
// //      //rows
// //      for (int i = 0; i < 10; i++) {
// //          pthreadConvolveMatx2Row();
// //      }
// //      clock_gettime(CLOCK_MONOTONIC, &finish);
// //      printf("\nAvg Time2: %lu nsec\n", (unsigned long)(finish.tv_nsec - start.tv_nsec)/10);

// //      printf("Contents of Matrix\n");
// //      for (int i = 0; i < rowMat; i++) {
// //          for (int j = 0; j < colMat; j++) {
// //              printf("%d ", Matx1[j][i]);
// //          }
// //      }
// //      printf("\nContents of Convultion\n");
// //      for (int i = 0; i < rowMat; i++) {
// //          for (int j = 0; j < colMat; j++) {
// //              printf("%d ", Rslt1[j][i]);
// //          }
// //      }

// //      printf("\nContents of Matrix2\n");
// //      for (int i = 0; i < rowMat1; i++) {
// //          for (int j = 0; j < colMat1; j++) {
// //              printf("%d ", Matx2[j][i]);
// //          }
// //      }
// //      printf("\nContents of Convultion 2\n");
// //      for (int i = 0; i < rowMat1; i++) {
// //          for (int j = 0; j < colMat1; j++) {
// //              printf("%d ", Rslt2[j][i]);
// //          }
// //      }
// //      printf("\nend\n");
// //      return 0;
// //  }

// //  PART 2:
// // #include <stdio.h>
// // #include <unistd.h>
// // #include <fcntl.h>
// // #include <sched.h>
// // #include <stdint.h>
// // #include <stdlib.h>
// // #include <sys/timerfd.h>
// // #include <time.h>
// // #include <string.h>
// // #include <pthread.h>

// // #define MY_PRIORITY 51

// // //Period values for write and read
// // #define Period_nseconds_write 10000000//2000000
// // #define Period_nseconds_read 20000000//3000000

// // //Initialize times for write and read
// // #define InitnSeconds_write 2000000
// // #define InitnSeconds_read1 1000000
// // #define InitnSeconds_read2 3000000


// //  char buff[100];
// //  char song[20][50];
// //  uint64_t buf = 0;
// //  typedef struct Data{
// //      char *filename;
// //  }data;

// //  void *read1(void* filename) {
// // // --- Initialize current thread as RT task ---
// //      struct sched_param param;
// //      // Declare ourself as a real time task by elevating our priority
// //      // and setting an appropriate scheduling policy.
// //      param.sched_priority = MY_PRIORITY;
// //      sched_setscheduler(0, SCHED_FIFO, &param);    // this function has a return value
// //      // Always good to check for errors. Look for sched_setscheduler()

// // // --- Configure Timer ---
// //      int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);    // returns a file descriptor
// //      // Always good to check for errors. Look for timerfd_create()

// //      struct itimerspec itval;    // structure to hold period and starting time
// //      // timer fires every ## nsec
// //      itval.it_interval.tv_nsec = Period_nseconds_read;    // check the data type

// //      // Timer will start in ## nsec from the moment the timer is started
// //      itval.it_value.tv_nsec = InitnSeconds_read1;   	 // check the data type

// //      // start the timer (you need the file descriptor and itval structure above)
// //      timerfd_settime(timer_fd, 0, &itval, NULL);    // this function has a return value
// //      // Always good to check for errors. Look for timerfd_settime()

// // // -- Get Line --
// //      data* temp;
// //      temp = (data*)filename;

// //      FILE* fd = fopen(temp->filename, "r");
// //      while (fgets(buff, 50, fd) != NULL) {
// //          read(timer_fd, &buf, sizeof(uint64_t));
// //      }

// //      fclose(fd);
// //      pthread_exit(0);
// //  }

// //  void *read2(void* filename) {
// // // --- Initialize current thread as RT task ---
// //      struct sched_param param;
// //      // Declare ourself as a real time task by elevating our priority
// //      // and setting an appropriate scheduling policy.
// //      param.sched_priority = MY_PRIORITY;
// //      sched_setscheduler(0, SCHED_FIFO, &param);    // this function has a return value
// //      // Always good to check for errors. Look for sched_setscheduler()

// // // --- Configure Timer ---
// //      int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);    // returns a file descriptor
// //      // Always good to check for errors. Look for timerfd_create()

// //      struct itimerspec itval;    // structure to hold period and starting time
// //      // timer fires every ## nsec
// //      itval.it_interval.tv_nsec = Period_nseconds_read;    // check the data type

// //      // Timer will start in ## nsec from the moment the timer is started
// //      itval.it_value.tv_nsec = InitnSeconds_read2;   	 // check the data type

// //      // start the timer (you need the file descriptor and itval structure above)
// //      timerfd_settime(timer_fd, 0, &itval, NULL);    // this function has a return value
// //      // Always good to check for errors. Look for timerfd_settime()

// // // -- Get Line --
// //      data* temp;
// //      temp = (data*)filename;
// //      FILE* fd = fopen(temp->filename, "r");
// //      while (fgets(buff, 50, fd) != NULL) {
// //          read(timer_fd, &buf, sizeof(uint64_t));
// //      }

// //      fclose(fd);
// //      pthread_exit(0);
// //  }

// //  void *writeSong(void* filename) {
// // // --- Initialize current thread as RT task ---
// //      struct sched_param param;
// //      // Declare ourself as a real time task by elevating our priority
// //      // and setting an appropriate scheduling policy.
// //      param.sched_priority = MY_PRIORITY;
// //      sched_setscheduler(0, SCHED_FIFO, &param);    // this function has a return value
// //      // Always good to check for errors. Look for sched_setscheduler()

// // // --- Configure Timer ---
// //      int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);    // returns a file descriptor
// //      // Always good to check for errors. Look for timerfd_create()

// //      struct itimerspec itval;    // structure to hold period and starting time
// //      // timer fires every ## nsec
// //      itval.it_interval.tv_nsec = Period_nseconds_write;    // check the data type

// //      // Timer will start in ## nsec from the moment the timer is started
// //      itval.it_value.tv_nsec = InitnSeconds_write;   	 // check the data type

// //      // start the timer (you need the file descriptor and itval structure above)
// //      timerfd_settime(timer_fd, 0, &itval, NULL);    // this function has a return value
// //      // Always good to check for errors. Look for timerfd_settime()

// // // -- Write Line to File --
// //      data* temp;
// //      temp = (data*)filename;
// //      int num;
// //      FILE *fd = fopen(temp->filename, "w");
// //      int i;
// //      for (i = 0; i < 20; i++) {
// //          strcpy(song[i], buff);
// //          fprintf(fd, "%s", song[i]);
// //          read(timer_fd, &buf, sizeof(uint64_t));
// //      }
// //      fclose(fd);
// //      pthread_exit(0);
// //  }

// //  int main(void){
// //      char *filename1 = "first.txt";
// //      char *filename2 = "second.txt";
// //      char *filename3 = "song.txt";

// //      pthread_t t1, t2, t3;

// //      pthread_create(&t1, NULL, (void *)&read1, (void *)&filename1);
// //      pthread_create(&t3, NULL, (void *)&writeSong, (void *)&filename3);
// //      pthread_create(&t2, NULL, (void *)&read2, (void *)&filename2);


// //      pthread_join(t1, NULL);
// //      pthread_join(t3, NULL);
// //      pthread_join(t2, NULL);

// // // -- Read Line From File and Print to Terminal --
// //      FILE *fd = fopen("song.txt", "r");
// //      int i;
// //      for (i = 0; i < 20; i++) {
// //          fgets(buff, 50, fd);
// //          printf("%s\n", buff);
// //      }

// //      return 0;
// //  }

