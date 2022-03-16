//Copyright Bora Okumusoglu, 2006-2022.

struct color_RGB
{
    unsigned char deger [3];
};

 //Colors for Magnitude Scaling
const int color_skala[] =
{
   255, 0, 0,
   191, 55, 23,
   216, 125, 34,
   218, 201, 27,
   122, 184, 66,
   79, 172, 106,
   50, 157, 147,
   17, 153, 224,
   18, 122, 203,
   15, 94, 179,
   9, 40, 123
};

struct pixel
{
    int x, y;
    color_RGB r;
};

//Coloring a Relavite Magnitude, Qh:Magnitude to be colored, Qt: Magnitude to be referred to
color_RGB color_calc(double Qh, double Qt)
{
    const double tolerans=0.10;
    const double Qoran=Qh/Qt;
    color_RGB gecici;

    if (Qoran==0){gecici.deger[0]=0; gecici.deger[1]=0; gecici.deger[2]=0; return gecici;}

    if (Qh>Qt)                             
        { gecici.deger[0]=color_skala[0]; gecici.deger[1]=color_skala[1]; gecici.deger[2]=color_skala[2]; return gecici; }
    
    if (fabs(Qoran-1)<tolerans)            
        { gecici.deger[0]=color_skala[3]; gecici.deger[1]=color_skala[4]; gecici.deger[2]=color_skala[5]; return gecici; }
    
    if (fabs(Qoran-0.9)<tolerans)          
        { gecici.deger[0]=color_skala[6]; gecici.deger[1]=color_skala[7]; gecici.deger[2]=color_skala[8]; return gecici; }
    
    if (fabs(Qoran-0.8)<tolerans)          
        { gecici.deger[0]=color_skala[9]; gecici.deger[1]=color_skala[10]; gecici.deger[2]=color_skala[11]; return gecici; }
    
    if (fabs(Qoran-0.7)<tolerans)          
        { gecici.deger[0]=color_skala[12]; gecici.deger[1]=color_skala[13]; gecici.deger[2]=color_skala[14]; return gecici; }
    
    if (fabs(Qoran-0.6)<tolerans)          
        { gecici.deger[0]=color_skala[15]; gecici.deger[1]=color_skala[16]; gecici.deger[2]=color_skala[17]; return gecici; }
    
    if (fabs(Qoran-0.5)<tolerans)          
        { gecici.deger[0]=color_skala[18]; gecici.deger[1]=color_skala[19]; gecici.deger[2]=color_skala[20]; return gecici; }
    
    if (fabs(Qoran-0.4)<tolerans)          
        { gecici.deger[0]=color_skala[21]; gecici.deger[1]=color_skala[22]; gecici.deger[2]=color_skala[23]; return gecici; }
    
    if (fabs(Qoran-0.3)<tolerans)          
        { gecici.deger[0]=color_skala[24]; gecici.deger[1]=color_skala[25]; gecici.deger[2]=color_skala[26]; return gecici; }
    
    if (fabs(Qoran-0.2)<tolerans)          
        { gecici.deger[0]=color_skala[27]; gecici.deger[1]=color_skala[28]; gecici.deger[2]=color_skala[29]; return gecici; }
    
    if (fabs(Qoran-0.1)<tolerans)          
        { gecici.deger[0]=color_skala[30]; gecici.deger[1]=color_skala[31]; gecici.deger[2]=color_skala[32]; return gecici; }

    gecici.deger[0] = 0; gecici.deger[1] = 0; gecici.deger[2] = 0; 
    return gecici;
}