#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// this is *not* arduino code
// I built this with
// gcc write_example.cpp -o test


//A structure which two elements
struct Datastore {
  // note that the types on the arduino are slightly different, and you should
  // make sure that you match the type of the data that you're trying to
  // store!!
  unsigned int Voltage; // this should just be a raw arduino reading DO NOT DO THE OD MATH ON THE ARDUINO!!!
  double time; // I'm making a very general example where we are mixing types.  I don't know what the native type of time is on the arduino!
};

int main(void)
{
    int j;
    int datalen = 10;
    Datastore mydata[datalen];
    for(j=0;j<datalen;j++){
        mydata[j].Voltage = j*10; // generate fake voltage data that's just 0, 10, 20... 90
        mydata[j].time = 1.0*j + 0.1; // generate fake time data that's 0.1, 1.1, 2.1, etc.
        printf("wrote %d %f\n",mydata[j].Voltage, mydata[j].time);
    }
    // I do the following because I don't know offhand how many bytes of
    // memory/filespace are needed to store each type.  this tells me
    // I need this for numpy.
    printf("sizeof voltage %d time %d overall %d\n",sizeof mydata[0].Voltage,
            sizeof mydata[0].time, sizeof mydata[0]);
    // {{{ in this block, the arduino commands will be similar, but somewhat different!
    FILE *f1 = fopen("file.bin", "wb");
    assert(f1);
    size_t r1 = fwrite(mydata, sizeof mydata[0], datalen, f1);
    printf("wrote %zu elements out of %d requested\n", r1,  datalen);
    fclose(f1);
    // }}}
}



