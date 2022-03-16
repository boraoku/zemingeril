//Copyright Bora Okumusoglu, 2006-2021.

const double pi=3.141592654;

struct coordinate
{
    double x, y;
};

double Distance(coordinate Aa, coordinate Bb)
{
    return sqrt( pow((Aa.x-Bb.x),2) + pow((Aa.y-Bb.y),2) );
}

char* formatDouble(double value) {

    char *say = (char*)malloc(256);

#ifdef _WIN32
    //windows
    sprintf_s(say, 256, "%03.0f", value);
#else
    //linux or mac
    sprintf(say, "%03.0f", value);
#endif

    return say;
}