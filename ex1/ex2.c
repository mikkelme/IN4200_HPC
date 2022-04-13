#include <stdlib.h>
#include <stdio.h>



void read_file();
void count_lines();

typedef struct {
    int n;
    char *times;
    double *temps;

} temp_data;


int main (){

  char filename[] = "temp_data.txt";
  int num_lines = 0;

  count_lines(filename, &num_lines);
  temp_data *data = (temp_data *)malloc(sizeof(temp_data));
  read_file(filename, data, &num_lines);

  for (int i = 0; i < data->n; i ++) {
      printf("%.5s %lf\n", &data->times[5 * i], data->temps[i]);
  }

  // find min/max and average
  int minidx = 0, maxidx = 0;
  double min = data->temps[0] , max = data->temps[0];
  double avg = 0;
  // int  = 0;

  for (int i = 0; i < data->n; i ++){
    avg += data->temps[i];

    if (data->temps[i] < min){
      minidx = i;
      min = data->temps[i];
    }

    if (data->temps[i] > max){
      maxidx = i;
      max = data->temps[i];
    }
  }

avg /= data->n;
printf("%s %f\n", "Minimum", min );
printf("%s %f\n", "Maximum", max );
printf("%s %f\n", "Average", avg );


}

void read_file(char *filename, temp_data *data, int *num_lines){
  FILE *datafile;
  datafile = fopen(filename, "r");

  data->n = *num_lines;
  printf("Data struct: %d \n", data->n);

  data->times = (char *)malloc(5 * data->n * sizeof(char));
  data->temps = (double *)malloc(data->n * sizeof(double));

  for (int i = 0; i < data->n; i++){
    fscanf(datafile, "%5s %lf", &data->times[5*i], &data->temps[i]);
  }






}
// fscanf(datafile, "%s %lf", &time[0], &temp[0]);
// fscanf(datafile, "%s %lf", &time[5], &temp[1]);
// fscanf(datafile, "%s %lf", &time[10], &temp[2]);


void count_lines(char *filename, int *num_lines){
  FILE *datafile;
  datafile = fopen(filename, "r");
  int count_lines = 0;
  char ch;


  if (datafile == NULL){
    printf("%s\n", "Error while opening" );
    exit(EXIT_FAILURE);
  }

  for (ch = getc(datafile); ch != EOF; ch = getc(datafile)){
    if (ch == '\n'){
      count_lines++;
    }
  }

  *num_lines = count_lines;
  fclose(datafile);

}

//
//
// char *time = (char *)malloc(num_lines * 6*sizeof(char*));
// double *temp = (double *)malloc(num_lines * sizeof(double));
//

  // int test;
  // char str1[5];
  //
  // fscanf(datafile, "%s %lf", time, temp);
  // printf("%s\n", time );
  // printf("%lf\n", *temp );


  // char str1[1], str2[10], str3[10];
  // int year;
  // fscanf(datafile, "%s %s %s %d", str1, str2, str3, &year);
  //
  //  printf("Read String1 |%s|\n", str1 );
  //  printf("Read String2 |%s|\n", str2 );
  //  printf("Read String3 |%s|\n", str3 );
  //  printf("Read Integer |%d|\n", year );
  //
  // r = fscanf(datafile, "%c %lf \n", time, temp);
  // printf("%s\n", time);


//
// void read_file(char *filename, temp_data *data){
//   FILE *datafile = fopen(filename, "r");
//
//
//   int r
//   r = fscanf(fp, "%f %s\n", &temp, loc);
//
//   // char ch;
//   // for (int i = 0; i < 10; i++){
//   //   ch = fgetc(datafile);
//   //   printf("%c\n", ch );
//   //
//   // }
//
//
//
//   // char c[] = "abcd";
//   // fscanf(datafile, "%s", c );
//   // printf("%s\n", c );
//   // fscanf(datafile, "%s", c );
//   // printf("%s\n", c );
//
