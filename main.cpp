#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <vector>
#include <algorithm>


int loadInputFile();
int nextLine(char *dst);


//#define DEBUG
#ifdef DEBUG
#define INPUT_FILENAME "input1.txt"
#define SIDESEARCH 20
#else
#define INPUT_FILENAME "input.txt"
#define SIDESEARCH 4000000
#endif

struct sigbeacon
{
    int sx;
    int sy;
    int dist;
    sigbeacon(int sx, int sy, int bx, int by):sx(sx), sy(sy){
        dist = abs(sx - bx) + abs(sy - by); 
    }
};

using namespace std;

char line[100];
int main()
{
    if (loadInputFile() < 0)
    {
        fprintf(stderr, "Can't open: " INPUT_FILENAME "\n");
        return 1;
    }



    int sx, sy, bx, by;
    char *pstr;

    vector<sigbeacon> signalBeaconsVec;

    while (nextLine(line))
    {
        /* read signal - beacon*/
        pstr = line;
        pstr = strchr(pstr, '=');
        sx = (int) strtol(pstr+1, &pstr, 10);
        pstr = strchr(pstr, '=');
        sy = (int) strtol(pstr+1, &pstr, 10);
        pstr = strchr(pstr, '=');
        bx = (int) strtol(pstr+1, &pstr, 10);
        pstr = strchr(pstr, '=');
        by = (int) strtol(pstr+1, &pstr, 10);       

        signalBeaconsVec.push_back(sigbeacon(sx, sy, bx, by));
    }
    
    vector<pair<int, bool>> xcoords;
    int xans = -1, yans = -1;
    for(int y = 0; yans == -1 && y <= SIDESEARCH; y++){
        xcoords.clear();
        int dist, x1, x2;

        //Klee's algorithm
        for(sigbeacon &sb: signalBeaconsVec){
            dist = sb.dist - abs(y-sb.sy);
            if(dist >= 0)
            {
                x1 = max(sb.sx - dist, 0);
                if(x1 > SIDESEARCH) continue;

                x2 = min(sb.sx + dist + 1, SIDESEARCH);
                if(x2 < 0 || x1 == x2) continue;

                xcoords.push_back(make_pair(x1, false));
                xcoords.push_back(make_pair(x2, true));                
            }
        }

        sort(xcoords.begin(), xcoords.end());

        int prevcount, count = 1;
        
        for(auto it = xcoords.begin() + 1; xans == -1 && it != xcoords.end(); it++)
        {
            prevcount = count;            
            (it->second)? count-- : count++;
            if(prevcount == 0 && count == 1 && it->first != (it-1)->first){
                xans = (it-1)->first;
                yans = y;
            }
        }
        
    }
    
    int64_t ans = xans*(int64_t)4000000;
    ans += yans;
    printf("%" PRId64 "\n", ans);

}

#define BUFSIZE 2200

char fileBuf[BUFSIZE];
char *pbuf = fileBuf;
char *pbufend = fileBuf;

int loadInputFile()
{
    FILE *pf = fopen(INPUT_FILENAME, "r");
    if (!pf)
        return -1;
    pbuf = fileBuf;
    pbufend = fileBuf + fread(fileBuf, 1, BUFSIZE, pf);
    fclose(pf);

    return 0;
}

int nextLine(char *dst)
{
    char c;
    char *orig = dst;

    // read line:
    while (pbuf < pbufend && (c = *(pbuf++)) != '\n')
    {
        *(dst++) = c;
    }
    *dst = '\0';
    // return line length
    return dst - orig;
}