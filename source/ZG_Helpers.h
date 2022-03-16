//Copyright Bora Okumusoglu, 2006-2022.

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

enum Multithread { a, b, c, d };

double startValueFor3DCalc(Multithread thr)
{
    switch(thr)
    {
        case Multithread::a: return 0.00; break;
        case Multithread::b: return 0.25; break;
        case Multithread::c: return 0.50; break;
        case Multithread::d: return 0.75; break;
    }
}

double endValueFor3DCalc(Multithread thr)
{
    switch(thr)
    {
        case Multithread::a: return 0.25; break;
        case Multithread::b: return 0.50; break;
        case Multithread::c: return 0.75; break;
        case Multithread::d: return 1.00; break;
    }
}
