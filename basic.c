#include "basic.h"
#include "utils.h"

/*reads a sector into the buffer*/
void read_sector(int sectornumber, char* buffer)
{
        int sec,head,cyl;


        sec=mod(sectornumber,0x12)+1;
        head=mod(div(sectornumber,0x12),2);
        cyl=div(sectornumber,0x24);

        readsect(buffer,sec,head,cyl);
}

void write_sector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

    /*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(div(sectornumber,0x12),2);
        cyl=div(sectornumber,0x24);

        writesect(buffer,sec,head,cyl);
}