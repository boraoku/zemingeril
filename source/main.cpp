//Copyright Bora Okumusoglu, 2006-2021.

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <thread>
#include <chrono>
using namespace std;

#include "CImg.h"
using namespace cimg_library;

#include "ZG_Foundation.h"
using namespace ZEMiNGERiL;

//create foundation system named as ZG
Foundation ZG; 

//sondaj hesap cizimi icin eksenlerin uzunlugu
int SL=500;
int zL=500;

coordinate e_basla, e_bitis; //kesim icin yuzeydeki baslangic ve bitim noktalari
double e_z; //kesimde inilecek derinlik

//hesap cizimi icin eksenlerin uzunlugu
int dL, uL; //derinlik ve uzunluk, pixel cinsinden

double IIB_z; //belirlenen derinligi
coordinate IIB_basla, IIB_bitis; //hesap dogrultusunu tanimlayan noktalar
double IIB_Smax=0; //dogrultu boyunca rastlanan maksimum gerilme
double IIB_Kmax=0; //dogrultu boyunca rastlanan maksimum gerilmenin konumu
double toplam_mesafe; //hesap dogrultusunun uzunlugu

//SONDAJ

double sondaj_z; //sondaj derinligi
double sondaj_Smax=0; //sondaj boyunca rastlanan maksimum gerilme
double sondaj_zmax=0; //sondaj boyunca rastlanan maksimum gerilmenin konumu
coordinate sondaj_nokta; //sondajin plandaki koordinati

Vec<double> hesap_sdj;  //hesaplanan sondaj

void sondaj_hesap()
{
    double Qnokta,derinlik=0;
    
    //dosya ciktisi icin
    ofstream outfile ("cikti_ZG.txt",ofstream::binary);
    outfile << std::setiosflags (ios_base::fixed);
    outfile << setw (10) << setprecision (5);
    
    cout << "\nEnter X and Y coordinates of stress boring (meters, decimal): ";
    cin >> sondaj_nokta.x; cin >> sondaj_nokta.y;
    
    cout << "\nEnter the depth of stress boring (meters, positive integer): ";
    cin >> sondaj_z;
    
    cout << "\n\nPlease wait while calculation and graphing is being processed... \n";
    
    double yuzde=0;
    double yuzdee=0;
    
    //hesap kismi
    while(hesap_sdj.size()!=zL+1)
    {
        //ilemin yuzde kacnn tamamlandï¿½n yazdr
        yuzde=derinlik * 100 / sondaj_z;
        if (yuzde < 0 ) { yuzde=0; }
        if (yuzde > yuzdee) {
            char *say=formatDouble(yuzde); //integer degerden char eldesi bu sekilde yazdirilir ekrana
            //sprintf hakkinda: http://www.cplusplus.com/ref/cstdio/sprintf.html
            cout << "\b\b\b\b\b"; //silmece
            cout << say; //yazmaca
            cout << " %";
            yuzdee=yuzde;
        }
        
        if(derinlik==0.0)
        {
            //gerilme dagiliminda yuzeyde (z=0) ya olusan matematiksel devamsizligi
            //gidermek icin z=0 noktasi icin z=0.0001 daki gerilme artimi kullanilacak
            Qnokta=ZG.Stress(sondaj_nokta, 0.0001);
        } else {
            Qnokta=ZG.Stress(sondaj_nokta, derinlik);
        }
        
        hesap_sdj.push_back(Qnokta);
        
        //maksimum sondaj gerilmesini ve konumunu bulmak icin
        if (Qnokta>sondaj_Smax)
        {
            sondaj_Smax=Qnokta;
            sondaj_zmax=derinlik;
        }
        
        //dosyaya yazarken cizim yapilan aralik sayisi iki katina cikariliyor
        
        outfile << derinlik << "  " << Qnokta << "\n";
        derinlik += sondaj_z / zL / 2 ;
        
        if(derinlik<=sondaj_z) //sondaj derinliðinin en sonda yarým deðer geçilmesi engellenir
        {
            Qnokta=ZG.Stress(sondaj_nokta, derinlik);
            outfile << derinlik << "  " << Qnokta << "\n";
        }
        
        derinlik += sondaj_z / zL / 2 ;
    }
}

void sondaj_cizim()
{
    const unsigned char
    siyah  [3] = {  0,  0,  0},
    beyaz  [3] = {255,255,255},
    kirmizi[3] = {255,  0,  0},
    gri    [3] = {128,128,128};
    
    CImg<unsigned char> back(1,2,1,3,10), img;
    back(0,1,2) = 235;
    back.resize(680,580,1,3,3);
    back.draw_rectangle(0,0,680,580,beyaz);
    
    CImgDisplay disp(back,"ZeminGeril-Temel 0.5",0);
    img=back;
    
    //cizim icin baslangic koordinatlari
    pixel A, B;
    A.x=20; A.y=50;
    B.x=A.x; B.y=SL+A.y;
    
    //legend icin heryere gelecek X otelemesi
    const double Lego=50;
    
    //eksenleri ciz
    img.draw_line((Lego + B.y), B.x, (Lego + B.y), (B.x+zL),gri,-1,1);
    
    //grafik cizim
    //GRAFIK BASASAGI CIZILDIGI ICIN HESAPLANAN VE GIRILEN X VE YLER YER DEGISTI
    
    int g1, g2;
    for(int i=0; i!=hesap_sdj.size(); ++i)
    {
        g1=B.x + i;
        g2=B.y - hesap_sdj[i]/(sondaj_Smax/SL);
        //S_maxdan buyuk yuzey gerilmesi icin ayar
        if(hesap_sdj[i]>sondaj_Smax){ g2 = B.y - SL;}
        
        color_RGB renk=color_calc(hesap_sdj[i],sondaj_Smax);
        
        for(int j=g2; j!=B.y; ++j)
        {
            img.draw_point((Lego + j), g1, renk.deger, 1);
        }
    }
    
    
    //z etiketi, z ekseni icin
    img.draw_text((Lego + B.y), (B.x-18), "Z, meters", siyah, 0, 1, 13);
    
    //gerilme etiketi, gerilme ekseni icin
    img.draw_text((Lego - 20), (B.x-18), "Stress, kPa", siyah, 0, 1, 13);
    
    double zp=0;
    //z eksenini derecelendir
    for(int i=B.x; i<=(B.x+zL); i+=zL/20)
    {
        //cizgiyi at
        if(i!=B.x)
        { //ilk gri cizgiyi atmasin
            img.draw_line((Lego + A.y), i, (Lego + B.y+5), i,gri,-1,1);
        }
        
        //derinlik degerini yaz
        img.draw_text((Lego + B.y+8), i-5, "%4.2f", siyah, 0, 1, 13, zp);
        
        //gerilme degerini yaz
        img.draw_text((Lego+8), i-5, "%4.2f", siyah, 0, 1, 13, hesap_sdj[i-B.x]);
        
        zp+=sondaj_z/20;
    }
    
    //ust taraftan cizgi cek
    img.draw_line((Lego + A.y),A.x,(Lego + A.y),(B.x+zL),gri,-1,1);
    
    //maksimum gerilme miktari ve yerini yaz
    img.draw_text((Lego + 50), (A.x + zL + 20), "%4.2f kPa of Maximum Stress occured at %4.2f meters depth.", kirmizi,0 , 1, 13, sondaj_Smax,sondaj_zmax);
    
    //sondaj koordinatini belirt
    img.draw_text((Lego + 50), (A.x + zL + 30), "Coordinates of Boring are X= %4.2f meter and Y= %4.2f meter.", kirmizi, 0, 1, 13, sondaj_nokta.x,sondaj_nokta.y);
    
    //legend ciz
    //legend(sondaj_Smax); yerine..
    double Qrol=sondaj_Smax;
    double Lger;
    
    //cizim icin baslangic koordinatlari
    pixel AL, BL;
    AL.x=(Lego - 20);  AL.y=B.x;
    BL.x=(Lego - 10);  BL.y=B.x+20;
    
    for(int i=11; i!=0; --i)
    {
        //degeri yaz
        Lger=Qrol * (i-1) / 10;
        img.draw_text(0, (BL.y-5), "%4.2f", siyah, 0, 1, 13, Lger);
        
        //kareyi ciz
        for(int xx=0; xx!=11; ++xx)
        {
            for(int yy=0; yy!=21; ++yy)
            {
                unsigned char picturehasamustache[3] = { 0, 0, 0 };
                picturehasamustache[0]=color_skala[(11-i)*3];
                picturehasamustache[1]=color_skala[(11-i)*3+1];
                picturehasamustache[2]=color_skala[(11-i)*3+2];
                img.draw_point((AL.x + xx), (AL.y + yy), picturehasamustache, 1);
            }
        }
        
        AL.y+=20; BL.y+=20;
    }
    
    cout << "\n\nCalculation and graphing are accomplished.\n\n";
    
    while (!disp.is_closed())
    {
        //y koordinati gore gerilme degerini goster
        if (disp.mouse_y() <= B.x + zL && disp.mouse_y() >= B.x)
        {
            int i = disp.mouse_y() - B.x;
            double derinlik = (i * sondaj_z / zL);
            img.draw_rectangle((Lego + 100),(B.x-18),B.y,(B.x-5),beyaz);
            //cout << derinlik << " " << hesap_sdj[i] << "\n";
            img.draw_text((Lego + 100),(B.x-18), "| Depth: %4.2f meters , Stress increment %4.2f kPa |", kirmizi, 0, 1, 13, derinlik, hesap_sdj[i]);
        }
        
        //goruntuyu ciz
        disp.display(img);
        disp.wait();
    }
}

int main( int argc, char **argv )
{
#ifdef _WIN32
    //windows
    system("cls");
#else
    //linux or mac
    system("clear");
#endif
    
    cout << "ZeminGeril-Temel 0.5\n\n";
    cout << "Graphing a designated stress distribution \n";
    cout << "inside the soil strafa affected by the overburden foundation\n";
    
    //temelin girilmesi
    ZG.Input();
    
    cout << "\n-------------------------------------------------------------\n";
    
    cout << "\nWhat kind of graphing would you like to have?\n\n";
    //    cout << "[0] 2D cross-section view of stress distribution\n\n";
    //    cout << "[1] 2D graph of stress distribution through a line\n inside the soil at a certain depth\n\n";
    cout << "[0] 2D graph of stress distribution through a line of boring\n\n";
    cout << "[1] 3D cross-section view of stress distribution\n\n(0-1):";
    int option;
    cin >> option;
    
    cout << "\n-------------------------------------------------------------\n";
    
    
    if (option==1)
    {
        //IIIB grafik hesap ve cizim
        ZG.IIIB_hesap();
        ZG.IIIB_cizim();
    } else if (option==0) {
        //2boyut hesap ve cizim
        sondaj_hesap();
        sondaj_cizim();
    }
    
#ifdef _WIN32
    //windows
    system("pause");
#else
    //linux or mac
    system("echo 'Press ENTER to continue . . .'");
    cin.ignore();
    cin.ignore();
#endif
    
    return 0;
}
