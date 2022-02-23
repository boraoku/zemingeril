#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <thread>

using std::cin;                 
using std::cout;
using std::streamsize;
using std::setprecision;
using std::ios_base;
using std::setw;
using std::ofstream;
using std::thread;

#include "vec.h"

#include "CImg.h"
using namespace cimg_library;

#include "kaynak_ZEMiNGeRiL_v05.h"
using namespace ZEMiNGERiL;

//program by: Bora Okumusoglu, 2006

//Foundation Class
class Foundation
{
    public:
        int n;
        void Define(int _n, Vec<element> _e);
        double Stress(coordinate _nokta, double _z);
        
    private:
        double Stress_Element(element _e, coordinate _nokta, double _z);
        Vec<element> elements;
};

//Define Type and Dimensions of Elements of Foundation
void Foundation::Define(int _n, Vec<element> _e)
{
    this->n=_n;
    this->elements=_e; 
}

//Stress Increment at any point of the Soil caused by an Element of the Foundation
double Foundation::Stress_Element(element _e, coordinate _nokta, double _z)
{
    switch (_e.type) 
    {
    case 0:
        return _e.Q * If_rectangular(_e.center, _nokta, _e.L1, _e.L2, _z);
    break;
            
    case 1:
        return _e.Q * If_circular(_e.L1, _e.center, _nokta, _z);
    break;
            
    case 2:
        return _e.Q * If_arch(_e.alfa, _e.L1, _e.center, _nokta, _z);
    break;
    }    

    return 0;
}

//Stress Increment at any point of the Soil caused by Foundation
double Foundation::Stress(coordinate _nokta, double _z)
{
    double result=0.0;
    
    for(int i=0; i!=this->n; i++)
    {
        result+=this->Stress_Element(this->elements[i], _nokta, _z);              
    }    
    
    return result;
}   

//console related programming
Foundation ZG; //create foundation system named as ZG
 
void foundation_input() //input routine for console
{
    int nn;

    //rectangular
    cout << "\nEnter the total number of uniformly loaded areas on the\n";
    cout << "surface(z=0) acting as spread footings as the foundation: ";
    cin >> nn;
    cout << "\n";
     
    //temporary elements for input           
    Vec<element> _e;
    element _e_;

    for (int i=0; i!=nn; ++i)
    {
        cout << "For the " << (i+1) << "# Area\n";
        cout << "      Enter the  type of the area  (0:Rectangular, 1:Circular, 2:Arch): ";
        cin >> _e_.type;
        switch (_e_.type) 
        {
            case 0:
                cout << "\n      Enter the  first dimension (on X-axis) of the Area\n      (meters, positive integer): ";
                cin >> _e_.L1;
                cout << "      Enter the second dimension (on Y-axis) of the Area\n      (meters, positive integer): ";
                cin >> _e_.L2;
            break;
            
            case 1:
                cout << "\n      Enter the Radius of Circle           (R): ";
                cin >> _e_.L1;
            break;
            
            case 2:
                cout << "\n      Enter the Degree of the Arc (degrees, max. 180): ";
                cin >> _e_.alfa;
                cout << "      Enter the Radius of Arc                     (R): ";
                cin >> _e_.L1;
            break;
        }
        cout << "\n      Enter the Amount of Distributed Load (q): ";
        cin >> _e_.Q;
        cout << "\n      Enter the Coordinate of the center of the Area \n";
        cout << "      (X and Y respectively, seperated by a space): ";
        cin >> _e_.center.x;
        cin >> _e_.center.y;
        cout << "\n";
        
        _e.push_back(_e_);
    }
    
    ZG.Define(nn,_e);
}

//***          
//eski kod devaminda sadece gerilme hesabi rutinleri degistirildi.
//***

//sondaj hesap cizimi icin eksenlerin uzunlugu
int SL=500;
int zL=500;

int Ox, Oy; //API ekraninda cizim merkezi
double Ax, Ay, Az; //kesim kenarinin koordinatlari
int Lx, Ly, Lz; //3boyutlu seklin kenar uzunluklari ve cizim orani

Vec<pixel> hesap_e, hesapXY, hesapXZ, hesapYZ, hesap;  //hesaplanan cizim ekrani

coordinate e_basla, e_bitis; //kesim icin yuzeydeki baslangic ve bitim noktalari
double e_z; //kesimde inilecek derinlik
double QQ; //temel yuku ve cizim icin karsilastirma yuku

//hesap cizimi icin eksenlerin uzunlugu
int dL, uL; //derinlik ve uzunluk, pixel cinsinden

//cizim orani ve carpma islemine gore tersi hesaplarda kullanilmak uzere
double oran_c, oran_ct;

double IIB_z; //belirlenen derinligi
coordinate IIB_basla, IIB_bitis; //hesap dogrultusunu tanimlayan noktalar
double IIB_Smax=0; //dogrultu boyunca rastlanan maksimum gerilme
double IIB_Kmax=0; //dogrultu boyunca rastlanan maksimum gerilmenin konumu
double toplam_mesafe; //hesap dogrultusunun uzunlugu


//3Boyut
void III_hesapXY(double z, double xx, double yy)
{
          coordinate nokta;
          double Qnokta=0;
          int ekranx, ekrany;
          color_RGB renk;
          char say[256];
          double yuzde, yuzdee;
          yuzde=0;
          yuzdee=0;
          pixel gecici;

          //cizim icin
          Ox=400;//ekranin orta koordinatlari Ox Oy
          Oy=300;
          const double aci=(3.141592654)/9;

          for (int j=0; j!=( Ly*oran_c + 1); ++j)
          {

                for (int i=0; i!=( Lx*oran_c + 1 ); ++i)
                {
                     nokta.x=xx;
                     nokta.y=yy;

                     //hesap kismi
                     Qnokta=ZG.Stress(nokta, z);

                     ekranx=Ox + i*cos(aci) - j*cos(aci);
                     ekrany=Oy - j*sin(aci) - i*sin(aci);


                     //ilemin yuzde kacnn tamamlandigin yazdr
                     yuzde=( (j-1) * ( Lx * oran_c ) + i) * 100 / ( Lx * Ly * oran_c * oran_c);
                     if (yuzde < 0 ) { yuzde=0; }
                     if (yuzde > (yuzdee) ) {
                        sprintf(say, "%03.0f", yuzde); //integer degerden char eldesi bu sekilde yazdirilir ekrana
                        //sprintf hakkinda: http://www.cplusplus.com/ref/cstdio/sprintf.html
                        cout << "\b\b\b\b\b"; //silmece
                        cout << say; //yazmaca
                        cout << " %";
                        yuzdee=yuzde;
                     }

                     //etki faktorlerini renklerle ifade et
                     renk=color_calc(Qnokta,QQ);

                     gecici.x=ekranx;
                     gecici.y=ekrany;
                     gecici.r=renk;
                     hesapXY.push_back(gecici);
             
                     xx = xx + oran_ct;
             
                }         

                xx = Ax;
                yy = yy + oran_ct;
          }        
}

void III_hesapXZ(double y, double xx, double zz)
{
          coordinate nokta;
          double Qnokta=0;
          int ekranx, ekrany;
          color_RGB renk;
          char say[256];
          double yuzde, yuzdee;
          yuzde=0;
          yuzdee=0;
          pixel gecici;

          nokta.y=y;

          //cizim icin
          Ox=400;//ekranin orta koordinatlari Ox Oy
          Oy=300;
          const double aci=(3.141592654)/9;

          for (int j=0; j!=( Lz*oran_c + 1); ++j)
          {

                     for (int i=0; i!=( Lx*oran_c + 1); ++i)
                     {
                     nokta.x=xx;

                     //hesap kismi
                     Qnokta=ZG.Stress(nokta, zz);
                     
                     ekranx=Ox + i*cos(aci);
                     ekrany=Oy + j - i*sin(aci);

                      //ilemin yuzde kacnn tamamlandï¿½n yazdr
                     yuzde=( (j-1) * ( Lx * oran_c ) + i) * 100 / ( Lz * Lx * oran_c * oran_c );
                     if (yuzde < 0 ) { yuzde=0; }
                     if (yuzde > yuzdee) 
                     {
                         sprintf(say, "%03.0f", yuzde); //integer degerden char eldesi bu sekilde yazdirilir ekrana
                         //sprintf hakkinda: http://www.cplusplus.com/ref/cstdio/sprintf.html
                         cout << "\b\b\b\b\b"; //silmece
                         cout << say; //yazmaca
                         cout << " %";
                         yuzdee=yuzde;
                     }

                     //etki faktorlerini renklerle ifade et
                     renk=color_calc(Qnokta,QQ);

                     gecici.x=ekranx;
                     gecici.y=ekrany;
                     gecici.r=renk;
                     hesapXZ.push_back(gecici);
             
                     xx = xx + oran_ct;
             
                     }
                     
          xx = Ax;
          zz = zz + oran_ct;
          }
}

void III_hesapYZ(double x, double yy, double zz)
{
          coordinate nokta;
          double Qnokta=0;
          int ekranx, ekrany;
          color_RGB renk;
          char say[256];
          double yuzde, yuzdee;
          yuzde=0;
          yuzdee=0;
          pixel gecici;

          nokta.x=x;

          //cizim icin
          Ox=400;//ekranin orta koordinatlari Ox Oy
          Oy=300;
          const double aci=(3.141592654)/9;

          for (int j=0; j!=( Lz*oran_c + 1); ++j)
          {

                     for (int i=0; i!=( Ly*oran_c + 1); ++i)
                     {
                     nokta.y=yy;

                     //hesap kismi
                     Qnokta=ZG.Stress(nokta, zz);

                     ekranx=Ox - i*cos(aci);
                     ekrany=Oy + j - i*sin(aci);

                     //ilemin yuzde kacnn tamamlandï¿½n yazdr
                     yuzde=( (j-1) * ( Ly * oran_c ) + i) * 100 / ( Lz * Ly * oran_c * oran_c);
                     if (yuzde < 0 ) { yuzde=0; }
                     if (yuzde > yuzdee) {
                        sprintf(say, "%03.0f", yuzde); //integer degerden char eldesi bu sekilde yazdirilir ekrana
                        //sprintf hakkinda: http://www.cplusplus.com/ref/cstdio/sprintf.html
                        cout << "\b\b\b\b\b"; //silmece
                        cout << say; //yazmaca
                        cout << " %";
                        yuzdee=yuzde;
                     }

                     //etki faktorlerini renklerle ifade et
                     renk=color_calc(Qnokta,QQ);

                     gecici.x=ekranx;
                     gecici.y=ekrany;
                     gecici.r=renk;
                     hesapYZ.push_back(gecici);
             
                     yy = yy + oran_ct;
                     
                     }
                                          
          yy = Ay;
          zz = zz + oran_ct;
          }
}
	  
void IIIB_hesap()
{
          cout << "\nEnter X, Y and Z coordinates of the vertical edge of 3D\n(meters, decimal, Z must be >= 0 ): ";
          cin >> Ax; cin >> Ay; cin >> Az;
          //z >= 0 kontrolu
          while(Az<0)
          {
            cout<< "\nPlease enter a positive value for z: ";
            cin >> Az;
          }

          cout << "\nEnter X, Y and Z extension of 3D\n(meters, positive integer): ";
          cin >> Lx; cin >> Ly; cin >> Lz;

          cout << "\nEnter the stress magnitude of comparison\n(kPa, positive decimal): ";
          cin >> QQ;

          cout << "\nEnter the ratio of screen points/meters\n(recommended value=10, positive integer): ";
          cin >> oran_c;

          oran_ct=1/oran_c;

          cout << "\nSurface 1 of 3::calculated 000 %";

          //1.yuzey
          thread th1(III_hesapXY, Az, Ax, Ay);

          //2.yuzey
	      cout << "\nSurface 2 of 3::calculated 000 %";
	      thread th2(III_hesapXZ, Ay, Ax, Az);

          //3.yuzey  
	      cout << "\nSurface 3 of 3::calculated 000 %";
          thread th3(III_hesapYZ, Ax, Ay, Az);
	  
          cout << "\n";

          th1.join();
          th2.join();
          th3.join();

          //hesaplari birlestir
          for (int k=0; k!=hesapXY.size(); ++k)
          {
                hesap.push_back(hesapXY[k]);
          }

          for (int k=0; k!=hesapXZ.size(); ++k)
          {
                hesap.push_back(hesapXZ[k]);
          }

          for (int k=0; k!=hesapYZ.size(); ++k)
          {
                hesap.push_back(hesapYZ[k]);
          }

}

void IIIB_cizim()
{
 const unsigned char 
      siyah  [3] = {  0,  0,  0}, 
      beyaz  [3] = {255,255,255},
      kirmizi[3] = {255,  0,  0},
      gri    [3] = {128,128,128};

      CImg<unsigned char> back(1,2,1,3,10), img;
      back(0,1,2) = 235;
      back.resize(800,600,1,3,3);
      back.draw_rectangle(0,0,800,600,beyaz);
  
      CImgDisplay disp(back,"ZeminGeril-Temel 0.3",0);
      img=back;
               
      for (int k=0; k!=hesap.size(); ++k)
      {
         img.draw_point(hesap[k].x, hesap[k].y, hesap[k].r.deger, 1);
      }

	  const double aci=(3.141592654)/9;
	  
	  int c1, c2, c3, c4;

   	  //sinir cizgileri rengi gri, 3 yuzeyi gostermek icin
   	  
      //arka sag sinir
      c1=Ox + ( Lx*oran_c )*cos(aci);
      c2=Oy - ( Lx*oran_c )*sin(aci);
	  c3=Ox + ( Lx*oran_c )*cos(aci) - ( Ly*oran_c )*cos(aci);
	  c4=Oy - ( Lx*oran_c )*sin(aci) - ( Ly*oran_c )*sin(aci);
      img.draw_line(c1,c2,c3,c4,gri,-1,1);

      //arka sol sinir
      c1=Ox - ( Ly*oran_c )*cos(aci);
      c2=Oy - ( Ly*oran_c )*sin(aci);
      img.draw_line(c1,c2,c3,c4,gri,-1,1);

      //kesi$ cizgileri
      //orta cizgi
      c1=Ox;
      c2=Oy;
      c3=Ox;
      c4=Oy + Lz*oran_c;
      img.draw_line(c1,c2,c3,c4,gri,-1,1);

      //sag cizgi
	  c3=Ox - ( Ly*oran_c )*cos(aci);
	  c4=Oy - ( Ly*oran_c )*sin(aci);
      img.draw_line(c1,c2,c3,c4,gri,-1,1);

      int a=0;

      //her metrede bir cizgi
      for (int i=0; i<=(Lz*oran_c); i+=oran_c)
      {
       if (a==5) { img.draw_line(c1,c2+i,c3,c4+i,siyah,0.50); a=0;} else { img.draw_line(c1,c2+i,c3,c4+i,gri,0.35); }
       ++a;
      }
      a=0;

      //sol cizgi
      c3=Ox + ( Lx*oran_c )*cos(aci);
	  c4=Oy - ( Lx*oran_c )*sin(aci);
	  img.draw_line(c1,c2,c3,c4,gri,-1,1);

      //her metrede bir cizgi
      for (int i=0; i<=(Lz*oran_c); i+=oran_c)
      {
       if (a==5) { img.draw_line(c1,c2+i,c3,c4+i,siyah,0.50); a=0;} else { img.draw_line(c1,c2+i,c3,c4+i,gri,0.35); }
       ++a;
      }
      a=0;

     //legend ciz
     //legend(sondaj_Smax); yerine..
     double Qrol=QQ;    
     double Lger;
     
     //legend icin heryere gelecek X otelemesi
     const double Lego=50;
     
     //cizim icin baslangic koordinatlari
     pixel A, B;
     A.x=20; A.y=50;
     B.x=A.x; B.y=SL+A.y;

     //cizim icin baslangic koordinatlari
     pixel AL, BL;
     AL.x=(Lego - 20);  AL.y=B.x;
     BL.x=(Lego - 10);  BL.y=B.x+20;

     for(int i=11; i!=0; --i)
     {
             //degeri yaz
             Lger=Qrol * (i-1) / 10;
             img.draw_text(0, (BL.y - 5), "%4.2f", siyah, 0, 1, 13, Lger);

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
         //goruntuyu ciz
         disp.display(img);
         disp.wait();
     } 
     
}

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
     
     char say[256];
     double yuzde=0;
     double yuzdee=0;
     
     //hesap kismi
     while(hesap_sdj.size()!=zL+1)
     {
          //ilemin yuzde kacnn tamamlandï¿½n yazdr
          yuzde=derinlik * 100 / sondaj_z;
          if (yuzde < 0 ) { yuzde=0; }
          if (yuzde > yuzdee) {
               	sprintf(say, "%03.0f", yuzde); //integer degerden char eldesi bu sekilde yazdirilir ekrana
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
    system("cls");
    cout << "ZeminGeril-Temel 0.5\n\n";
    cout << "Graphing a designated stress distribution \n";
    cout << "inside the soil strafa affected by the overburden foundation\n";

    //temelin girilmesi
    foundation_input();

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
      IIIB_hesap();
      IIIB_cizim();
     } else if (option==0) {
     //2boyut hesap ve cizim
      sondaj_hesap();
      sondaj_cizim();
     }
    
    #ifdef _WIN32
    //windows
    system("pause")
    #else
    //linux or mac
    system("echo 'Press ENTER to exit or wait for 5 seconds.....'");
    system("read -t5");
    #endif

    return 0;
}
