//Copyright Bora Okumusoglu, 2006-2021.

// Specifications of a Foundation compiled as an Element (FOR INPUT PURPOSE)
struct element
{
    int type;                                    //Type, 0:rectangular, 1:circular, 2:arch
    double Q;                                    //Uniform Load Magnitude
    coordinate center;                           //Coordinate of Geometrical Center
    double L1;                                   //1st Geometrical Dimension
    double L2;                                   //2nd Geometrical Dimension
    double alfa;                                 //Angle for "Arch Type" Elements
};

//FUNCTIONS RELATED TO FOUNDATION::SIGMA

//Newmark's Integration of Boussinesq
double Boussinesq(double B, double L, double z)
{
    double M,N,V,V1;

    M = B / z;
    N = L / z;
    V = pow(M,2) + pow(N,2) + 1 ;
    V1 = pow((M * N),2);
    
    if (V1>V) 
    {
        return ( 2*M*N*sqrt(V)*(V+1)/(V+V1)/V + pi + atan(2*M*N*sqrt(V)/(V-V1)) )/(4*pi);
    }
    else 
    {
        return ( 2*M*N*sqrt(V)*(V+1)/(V+V1)/V + atan(2*M*N*sqrt(V)/(V-V1)) )/(4*pi); 
    }
}

//Westergaard's Solution for Poisson Ratio=0.0, larger poisson ratios give lower stresses
double Westergaard(double B, double L, double z)
{
    double M,N,a,poisson;

    M = B / z;
    N = L / z;
    
    poisson=0.3;    //adjust poisson ratio here, manually.
    
    a=(1-2*poisson)/(2-2*poisson);
    
    return atan( M * N / ( sqrt(a)*sqrt(pow(M,2)+pow(N,2)+a) ) ) / (2*pi);
}

//For Major and Minor Principle Stress, Functions by Holl,1940
double sigmaZ(double B, double L, double z)
{
    double R1, R2, R3;
    
    R1=sqrt(pow(L,2)+pow(z,2));
    R2=sqrt(pow(B,2)+pow(z,2));
    R3=sqrt(pow(L,2)+pow(B,2)+pow(z,2));

    return ( atan(L*B/(z*R3)) + L*B*z/R3*( 1/pow(R1,2) + 1/pow(R2,2) ) ) / (2*pi);
}

double sigmaY(double B, double L, double z)
{
    double R1, R2, R3;
    
    R1=sqrt(pow(L,2)+pow(z,2));
    R2=sqrt(pow(B,2)+pow(z,2));
    R3=sqrt(pow(L,2)+pow(B,2)+pow(z,2));

    return ( atan(L*B/(z*R3)) - L*B*z/(pow(R2,2)*R3) ) / (2*pi);
}

double sigmaX(double B, double L, double z)
{
    double R1, R2, R3;
    
    R1=sqrt(pow(L,2)+pow(z,2));
    R2=sqrt(pow(B,2)+pow(z,2));
    R3=sqrt(pow(L,2)+pow(B,2)+pow(z,2));

    return ( atan(L*B/(z*R3)) - L*B*z/(pow(R1,2)*R3) ) / (2*pi);
}

double ToYZ(double B, double L, double z)
{
    double R1, R2, R3;
    
    R1=sqrt(pow(L,2)+pow(z,2));
    R2=sqrt(pow(B,2)+pow(z,2));
    R3=sqrt(pow(L,2)+pow(B,2)+pow(z,2));

    return ( L/R1 - pow(z,2)*L/(pow(R2,2)*R3)) / (2*pi);
}

double ToXZ(double B, double L, double z)
{
    double R1, R2, R3;
    
    R1=sqrt(pow(L,2)+pow(z,2));
    R2=sqrt(pow(B,2)+pow(z,2));
    R3=sqrt(pow(L,2)+pow(B,2)+pow(z,2));

    return ( B/R2 - pow(z,2)*B/(pow(R1,2)*R3)) / (2*pi);
}

double ToXY(double B, double L, double z)
{
    double R1, R2, R3;
    
    R1=sqrt(pow(L,2)+pow(z,2));
    R2=sqrt(pow(B,2)+pow(z,2));
    R3=sqrt(pow(L,2)+pow(B,2)+pow(z,2));

    return ( 1 + z/R3 - z*( 1/R1 - 1/R2 ) ) / (2*pi);
}

//RECTANGULAR (If_rectangular)
double If_rectangular(double kenar1, double kenar2, double z)
{
    //kenar uzunluklari sifir ise hesap yapma
    if ( (kenar1==0) || (kenar2==0) )
    {
       return 0.00;
   }

    //select shear functions of Holl here and disable the rest   
    //return ToXZ(kenar1, kenar2, z);

    //influence factor is equal to 0.25 at the surface(beneath the corner)
   if (z==0)
   {
    return 0.25; 
}    
else
{
        //Select the stress distribution function here, manually.
    return Boussinesq(kenar1, kenar2, z);
        //return Westergaard(kenar1, kenar2, z);        
        //return sigmaZ(kenar1, kenar2, z);
}

}

//RECTANGULAR_OVERLOADED_1
double If_rectangular(coordinate merk,coordinate nokt,double kenar1,double kenar2,double zz)
{
    double lx, ly, a1, a2, b1, b2, qA, qB, qC, qD, sonuc;
    //Gerekli geometrik degerlerin hesabi, aciklamasi icin ==> ZEMiNGeRiL_Noktada.jpg
    lx=fabs(merk.x-nokt.x);
    ly=fabs(merk.y-nokt.y);

    a1=fabs(kenar1/2 - lx);
    b1=fabs(kenar2/2 - ly);
    a2=kenar1/2 + lx;
    b2=kenar2/2 + ly;

    qA=If_rectangular(a1,b2,zz);
    qB=If_rectangular(a2,b1,zz);
    qC=If_rectangular(a1,b1,zz);
    qD=If_rectangular(a2,b2,zz);

    //Belirtilen nokta dikdortgen alan icinde mi disinda mi belirlemek
    //ve ona gore toplam Q'yu hesaplamak
    if ( (kenar1/2 - lx)<0 && (kenar2/2 - ly)<0 ) { sonuc=(qD-(qA+qB)+qC); }      
    if ( (kenar1/2 - lx)>=0 && (kenar2/2 - ly)>=0 ) { sonuc=(qD+qA+qB+qC); }      

    if ( (kenar1/2 - lx)>=0 && (kenar2/2 - ly)<0 ) { sonuc=(qD-(qB+qC)+qA); }       
    if ( (kenar1/2 - lx)<0 && (kenar2/2 - ly)>=0 ) { sonuc=(qD-(qA+qC)+qB); }           

    return sonuc;
}

//RECTANGULAR_OVERLOADED_2
double If_rectangular(element _e,coordinate _nokta, double _z)
{ 
    //yuzeyde etki faktoru kenarlarda 0.5, koselerde 0.25 degerini alir.
    //ama integral bu sonucu uretemediginden burada mudahale ediliyor.
    if (_z==0)
    {
        double kats=1;
        if((_nokta.y==_e.center.y + _e.L2/2) || (_nokta.y==_e.center.y - _e.L2/2) )
        {
            if((_nokta.x>=_e.center.x - _e.L1/2) && (_nokta.x<=_e.center.x + _e.L1/2) )
                { kats *= 0.5; }
            if((_nokta.x==_e.center.x - _e.L1/2) || (_nokta.x==_e.center.x + _e.L1/2) )
                { kats *= 0.5; }
            return kats;         
        }
        if((_nokta.x==_e.center.x + _e.L1/2) || (_nokta.x==_e.center.x - _e.L1/2) )
        {
            if((_nokta.y>_e.center.y - _e.L2/2) && (_nokta.y<_e.center.y + _e.L2/2) )
                { kats = 0.5; }
            return kats;
        }                    
    }         
    return If_rectangular(_e.center, _nokta, _e.L1, _e.L2, _z);
}


//CIRCULAR (If_circular)
double If_circular(double R, coordinate merkez, coordinate nokta, double zz)
{
    int i;
    double x, y, If;
    double x2, y2;
    coordinate dilimmerkez;

    //Ceyrek dairenin kac dikdortgensel dilime ayrilacagini belirleyen sayi10e4
    const int dilimsayi=100;

    //Eger en bir dilimin genisligi 0.15 den kucuk olursa
    //Bilgisayarin ya da C++ in hesap kabiliyeti, ortaya cikan ufak ondalik degerleri
    //hesaplamakta yetersiz kaliyor, bunun icin kontrol yapiliyor
    //Eger dilim genisligi 0.15den kucukse, dilim genisligi 0.15 olacak sekilde
    //R ve z degerleri ayni orandan buyutuluyor
    //Bu buyutmede M ve N hesap oranlari sabit kaldigindan sonuc degismiyor
    //Misal 10mm capli daire yukun 20cm derinlikte yaratacagi dusey gerilme artis orani ile
    //      100mm capli daire yukun 200cm derinlikte yaratacagi dusey gerilme artis orani aynidir.
    //if (R/dilimsayi<0.1) { zz*=0.1/(R/dilimsayi); R*=0.1/(R/dilimsayi); }
    //MANTIK DEGISIKLIGI DOKUMANTASYONA GIR!!!
    //NEDENI: R ve Z'IN KUCUK OLABILECEKLERI GIBI HESAP HATASI OLUSABILECEK KADAR BUYUK DE OLABILMELERI.
    merkez.x*=0.5/(R/dilimsayi);
    merkez.y*=0.5/(R/dilimsayi);
    nokta.x*=0.5/(R/dilimsayi);
    nokta.y*=0.5/(R/dilimsayi);
    zz*=0.5/(R/dilimsayi);
    R*=0.5/(R/dilimsayi);

    const double Dx=R/dilimsayi;

    x=0; y=R; If=0; i=0; //dilimleme icin baslangic degerleri

    //Eger derinlik = 0 ve daire icindeyse ise Etki Faktoru 1 olarak alinir
    //Eger derinlik = 0 ve daire disindaysa ise Etki Faktoru 0 olarak alinir
    //Eger derinlik = 0 ve tam dairenin ceperinde ise Etki Faktoru 0.5 olarak alinir (teorik cozum, kitaptan)
    //Ayrica herhangi bir dilimleme yapilmamasi icin i=dilimsayi atamasi ile, while dongusunun islemesi engellenir.
    //if ( (zz==0) && (Distance(merkez,nokta)<R) ) { If=1; i=dilimsayi; }
    if ( (zz==0) && (Distance(merkez,nokta)==0) ) { If=0.5; i=dilimsayi; }
    //sag tarafta kalan yarim cember icin ayarlanmistir
    if ( (zz==0) && (Distance(merkez,nokta)==R) && (merkez.x>nokta.x) ) { If=0.5; i=dilimsayi; } //cembersel kenarlarda
    if ( (zz==0) && (Distance(merkez,nokta)==R) && (merkez.x==nokta.x) ) { If=0.25; i=dilimsayi; } //koselerde
    if ( (zz==0) && (merkez.x==nokta.x) && (nokta.y<(merkez.y+R)) && (nokta.y>(merkez.y-R)) ) { If=0.5; i=dilimsayi; }//duz kenarda
    //dikdortgen dilimleme ile hesabin yapildigi dongu

    //dilimparcalari
    double sagust=0, solust=0, sagalt=0, solalt=0;

    while (i<dilimsayi)
    {
        x2=x+Dx;
        y2=sqrt(pow(R,2)-pow(x2,2));

        //Sag ust ceyrek
        dilimmerkez.x=merkez.x+(x+(Dx/2));
        dilimmerkez.y=merkez.y+((y+y2)/4);
        sagust=If_rectangular(dilimmerkez,nokta,Dx,((y+y2)/2),zz);
        
        //Sag alt ceyrek
        dilimmerkez.x=merkez.x+(x+(Dx/2));
        dilimmerkez.y=merkez.y-((y+y2)/4);
        sagalt=If_rectangular(dilimmerkez,nokta,Dx,((y+y2)/2),zz); 
        
        //Sol ust ceyrek
        dilimmerkez.x=merkez.x-(x+(Dx/2));
        dilimmerkez.y=merkez.y+((y+y2)/4);
        solust=If_rectangular(dilimmerkez,nokta,Dx,((y+y2)/2),zz);
        
        //Sol alt ceyrek
        dilimmerkez.x=merkez.x-(x+(Dx/2));
        dilimmerkez.y=merkez.y-((y+y2)/4);
        solalt=If_rectangular(dilimmerkez,nokta,Dx,((y+y2)/2),zz);

        If+=sagust + solust + sagalt + solalt;

        x=x2; y=y2;
        i++;
    }
    return If;
}

//CIRCULAR_OVERLOAD
double If_circular(element _e,coordinate _nokta, double _z)
{
    return If_circular(_e.L1, _e.center, _nokta, _z);
}

//ARCH(0-180) (If_arch)
double If_arch(double alfa, double R, coordinate merkez, coordinate nokta, double zz)
{
    int i;
    double x, y, If;
    double x2, y2, yy, y22;

    coordinate dilimmerkez, acilimerkez;
    double dilimk1, dilimk2, acilik1, acilik2;

    //Ceyrek dairenin kac dikdortgensel dilime ayrilacagini belirleyen sayi10e4
    const int dilimsayi=100;

    //Eger en bir dilimin genisligi 0.15 den kucuk olursa
    //Bilgisayarin ya da C++ in hesap kabiliyeti, ortaya cikan ufak ondalik degerleri
    //hesaplamakta yetersiz kaliyor, bunun icin kontrol yapiliyor
    //Eger dilim genisligi 0.15den kucukse, dilim genisligi 0.15 olacak sekilde
    //R ve z degerleri ayni orandan buyutuluyor
    //Bu buyutmede M ve N hesap oranlari sabit kaldigindan sonuc degismiyor
    //Misal 10mm capli daire yukun 20cm derinlikte yaratacagi dusey gerilme artis orani ile
    //      100mm capli daire yukun 200cm derinlikte yaratacagi dusey gerilme artis orani aynidir.
    //if (R/dilimsayi<0.1) { zz*=0.1/(R/dilimsayi); R*=0.1/(R/dilimsayi); }
    //MANTIK DEGISIKLIGI DOKUMANTASYONA GIR!!!
    //NEDENI: R ve Z'IN KUCUK OLABILECEKLERI GIBI HESAP HATASI OLUSABILECEK KADAR BUYUK DE OLABILMELERI.
    merkez.x*=0.5/(R/dilimsayi);
    merkez.y*=0.5/(R/dilimsayi);
    nokta.x*=0.5/(R/dilimsayi);
    nokta.y*=0.5/(R/dilimsayi);
    zz*=0.5/(R/dilimsayi);
    R*=0.5/(R/dilimsayi);

    const double Dx=R/dilimsayi;

    x=0; y=R; If=0; i=0; //dilimleme icin baslangic degerleri

    //Eger derinlik = 0 ve daire icindeyse ise Etki Faktoru 1 olarak alinir
    //Eger derinlik = 0 ve daire disindaysa ise Etki Faktoru 0 olarak alinir
    //Eger derinlik = 0 ve tam dairenin ceperinde ise Etki Faktoru 0.5 olarak alinir (teorik cozum, kitaptan)
    //Ayrica herhangi bir dilimleme yapilmamasi icin i=dilimsayi atamasi ile, while dongusunun islemesi engellenir.
    //if ( (zz==0) && (Distance(merkez,nokta)<R) ) { If=1; i=dilimsayi; }
    if ( (zz==0) && (Distance(merkez,nokta)==0) ) { If=0.5; i=dilimsayi; }
    //sag tarafta kalan yarim cember icin ayarlanmistir
    if ( (zz==0) && (Distance(merkez,nokta)==R) && (merkez.x>nokta.x) ) { If=0.5; i=dilimsayi; } //cembersel kenarlarda
    if ( (zz==0) && (Distance(merkez,nokta)==R) && (merkez.x==nokta.x) ) { If=0.25; i=dilimsayi; } //koselerde
    if ( (zz==0) && (merkez.x==nokta.x) && (nokta.y<(merkez.y+R)) && (nokta.y>(merkez.y-R)) ) { If=0.5; i=dilimsayi; }//duz kenarda
    //dikdortgen dilimleme ile hesabin yapildigi dongu

    //dilimparcalari
    double sagust=0, solust=0, sagalt=0, solalt=0;

    while (i<dilimsayi)
    {
        coordinate gecicinokta;          
        double galfa;

        //tam ceyrek icin                
        x2=x+Dx;
        y2=sqrt(pow(R,2)-pow(x2,2));

        dilimmerkez.x=x+Dx/2;
        dilimmerkez.y=(y+y2)/4;
        dilimk1=Dx;
        dilimk2=(y+y2)/2;

        //gerekiyorsa alfayi 90dan kucuk hale getir ve ona gore kesik dilim hesabinda kullan
        galfa=alfa;
        if(galfa>90) {galfa=alfa-90;}

        //aciliyla kesilen ceyrek icin                
        yy=x/tan(galfa/180*pi);
        y22=x2/tan(galfa/180*pi);

        acilimerkez.x=x+Dx/2;
        acilimerkez.y=( (y+y2)/2 + (yy+y22)/2 )/2;
        acilik1=Dx;
        acilik2=( (y+y2)/2 - (yy+y22)/2 );     

        gecicinokta.x=nokta.x-merkez.x;
        gecicinokta.y=nokta.y-merkez.y;

        //aciya gore cozumler

        if(alfa<90)
        {
            //sadece aciliyla kesilen ceyrek parcasi icin        
            If+=If_rectangular(acilimerkez,gecicinokta,acilik1,acilik2,zz); 
            //yay acisina ulasilinca cikis
            if( (x2/y2)>=tan(alfa/180*pi) ) {i=dilimsayi;}
        }    
        else if(alfa==90)
        {
            //sadece tam ceyregin dilim parcasi icin                    
            If+=If_rectangular(dilimmerkez,gecicinokta,dilimk1,dilimk2,zz);    
        }    
        else if(alfa>90)
        {
            //tam ceyregin dilim parcasi icin
            If+=If_rectangular(dilimmerkez,gecicinokta,dilimk1,dilimk2,zz);

            //aciliyla kesilen ceyrek parcasi icin
            //yay acisina ulasilmadikca hesaplanacak
            if( (x2/y2) < tan(galfa/180*pi) ) 
            {
                //gecici noktanin koordinatlarini sagalt dilim parcasina gore degistir.
                gecicinokta.x=-(nokta.y-merkez.y);
                gecicinokta.y=nokta.x-merkez.x;
                If+=If_rectangular(acilimerkez,gecicinokta,acilik1,acilik2,zz);        
            }    
        } 

        x=x2; y=y2;
        i++;  
    }
    return If;
}

//ARCH(0-180)_OVERLOADED
double If_arch(element _e,coordinate _nokta, double _z)
{
    return If_arch(_e.alfa, _e.L1, _e.center, _nokta, _z);
}