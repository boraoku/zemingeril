//Copyright Bora Okumusoglu, 2006-2021.

void Foundation::III_progressPrint()
{

    cout << "Calculation in progress...\n";
    cout << "|  XY  |  XZ  |  YZ  |\n";
    cout << "| 000% | 000% | 000% |";
    
    while (this->III_calc_progressing) {
        cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"; //silmece
        cout << "| ";
        cout << formatDouble(this->progressXY_a + this->progressXY_b); //yazmaca
        cout << "%";
        cout << " | ";
        cout << formatDouble(this->progressXZ_a + this->progressXZ_b); //yazmaca
        cout << "%";
        cout << " | ";
        cout << formatDouble(this->progressYZ_a + this->progressYZ_b); //yazmaca
        cout << "% |";
    }
    
    cout << "\nCalculation finished";
}

void Foundation::III_hesapXY(double z, double xx, double yy, bool firstHalf)
{
    coordinate nokta;
    double Qnokta=0;
    int ekranx, ekrany;
    color_RGB renk;
    double yuzde, yuzdee;
    yuzde=0;
    yuzdee=0;
    pixel gecici;
    
    const double aci=(3.141592654)/9;
    
    int start;
    int end;

    if (firstHalf) {
        start = 0;
        end = ( this->Ly*this->oran_c + 1 ) * 0.5;
    } else {
        start = ( this->Ly*this->oran_c + 1 ) * 0.5;
        end = ( this->Ly*this->oran_c + 1 );
        yy = yy + this->Ly * 0.5;
    }

    for (int j=start; j!=end; ++j)
    {

        for (int i=0; i!=( this->Lx*this->oran_c + 1 ); ++i)
        {
            nokta.x=xx;
            nokta.y=yy;
            
            //hesap kismi
            Qnokta=this->Stress(nokta, z);
            
            ekranx=this->Ox + i*cos(aci) - j*cos(aci);
            ekrany=this->Oy - j*sin(aci) - i*sin(aci);
            
            
            //ilemin yuzde kacnn tamamlandigin yazdr
            yuzde=( (j-1) * ( this->Lx * this->oran_c ) + i) * 100 / ( this->Lx * this->Ly * this->oran_c * this->oran_c);
            if (yuzde < 0 ) { yuzde=0; }
            if (yuzde > (yuzdee) ) {
                if (firstHalf) {
                    this->progressXY_a=yuzde / this->threadsPerSurface;
                } else {
                    this->progressXY_b=yuzde / this->threadsPerSurface;
                }
                yuzdee=yuzde;
            }
            
            //etki faktorlerini renklerle ifade et
            renk=color_calc(Qnokta,this->QQ);
            
            gecici.x=ekranx;
            gecici.y=ekrany;
            gecici.r=renk;
            if (firstHalf) {
                this->hesapXY_a.push_back(gecici);
            } else {
                this->hesapXY_b.push_back(gecici);
            }
            
            xx = xx + this->oran_ct;
            
        }
        
        xx = this->Ax;
        yy = yy + this->oran_ct;
    }
}

void Foundation::III_hesapXZ(double y, double xx, double zz, bool firstHalf)
{
    coordinate nokta;
    double Qnokta=0;
    int ekranx, ekrany;
    color_RGB renk;
    double yuzde, yuzdee;
    yuzde=0;
    yuzdee=0;
    pixel gecici;
    
    nokta.y=y;
    
    const double aci=(3.141592654)/9;

    int start;
    int end;

    if (firstHalf) {
        start = 0;
        end = ( this->Lz*this->oran_c + 1 ) * 0.5;
    } else {
        start = ( this->Lz*this->oran_c + 1 ) * 0.5;
        end = ( this->Lz*this->oran_c + 1 );
        zz = zz + this->Lz * 0.5;
    }

    for (int j=start; j!=end; ++j)
    {

        for (int i=0; i!=( this->Lx*this->oran_c + 1); ++i)
        {
            nokta.x=xx;
            
            //hesap kismi
            Qnokta=this->Stress(nokta, zz);
            
            ekranx=this->Ox + i*cos(aci);
            ekrany=this->Oy + j - i*sin(aci);
            
            //ilemin yuzde kacnn tamamlandï¿½n yazdr
            yuzde=( (j-1) * ( this->Lx * this->oran_c ) + i) * 100 / ( this->Lz * this->Lx * this->oran_c * this->oran_c );
            if (yuzde < 0 ) { yuzde=0; }
            if (yuzde > yuzdee) {
                if (firstHalf) {
                    this->progressXZ_a=yuzde / this->threadsPerSurface;
                } else {
                    this->progressXZ_b=yuzde / this->threadsPerSurface;
                }
                yuzdee=yuzde;
            }
            
            //etki faktorlerini renklerle ifade et
            renk=color_calc(Qnokta,this->QQ);
            
            gecici.x=ekranx;
            gecici.y=ekrany;
            gecici.r=renk;
            if (firstHalf) {
                this->hesapXZ_a.push_back(gecici);
            } else {
                this->hesapXZ_b.push_back(gecici);
            }
            
            
            xx = xx + this->oran_ct;
            
        }
        
        xx = this->Ax;
        zz = zz + this->oran_ct;
    }
}

void Foundation::III_hesapYZ(double x, double yy, double zz, bool firstHalf)
{
    coordinate nokta;
    double Qnokta=0;
    int ekranx, ekrany;
    color_RGB renk;
    double yuzde, yuzdee;
    yuzde=0;
    yuzdee=0;
    pixel gecici;
    
    nokta.x=x;
    
    const double aci=(3.141592654)/9;
    
    int start;
    int end;

    if (firstHalf) {
        start = 0;
        end = ( this->Lz * this->oran_c + 1) * 0.5;
    } else {
        start = ( this->Lz * this->oran_c + 1) * 0.5;
        end = ( this->Lz * this->oran_c + 1);
        zz = zz + this->Lz * 0.5;
    }

    for (int j=start; j!=end; ++j)
    {

        for (int i=0; i!=( this->Ly*this->oran_c + 1); ++i)
        {
            nokta.y=yy;
            
            //hesap kismi
            Qnokta=this->Stress(nokta, zz);
            
            ekranx=this->Ox - i*cos(aci);
            ekrany=this->Oy + j - i*sin(aci);
            
            //ilemin yuzde kacnn tamamlandï¿½n yazdr
            yuzde=( (j-1) * ( this->Ly * this->oran_c ) + i) * 100 / ( this->Lz * this->Ly * this->oran_c * this->oran_c);
            if (yuzde < 0 ) { yuzde=0; }
            if (yuzde > yuzdee) {
                if (firstHalf) {
                    this->progressYZ_a=yuzde / this->threadsPerSurface;
                } else {
                    this->progressYZ_b=yuzde / this->threadsPerSurface;
                }
                yuzdee=yuzde;
            }
            
            //etki faktorlerini renklerle ifade et
            renk=color_calc(Qnokta,this->QQ);
            
            gecici.x=ekranx;
            gecici.y=ekrany;
            gecici.r=renk;
            if (firstHalf) {
                this->hesapYZ_a.push_back(gecici);
            } else {
                this->hesapYZ_b.push_back(gecici);
            }
            
            yy = yy + this->oran_ct;
            
        }
        
        yy = this->Ay;
        zz = zz + this->oran_ct;
    }
}

void Foundation::IIIB_hesap()
{
    cout << "\nEnter X, Y and Z coordinates of the vertical edge of 3D\n(meters, decimal, Z must be >= 0 ): ";
    cin >> this->Ax; cin >> this->Ay; cin >> this->Az;
    //z >= 0 kontrolu
    while(this->Az<0)
    {
        cout<< "\nPlease enter a positive value for z: ";
        cin >> this->Az;
    }
    
    cout << "\nEnter X, Y and Z extension of 3D\n(meters, positive integer): ";
    cin >> this->Lx; cin >> this->Ly; cin >> this->Lz;
    
    cout << "\nEnter the stress magnitude of comparison\n(kPa, positive decimal): ";
    cin >> this->QQ;
    
    cout << "\nEnter the ratio of screen points/meters\n(recommended value=10, positive integer): ";
    cin >> this->oran_c;
    
    this->oran_ct=1/this->oran_c;
    
    // start timer
    auto t1 = chrono::high_resolution_clock::now();
    
    //1.yuzey
    thread th1a(&Foundation::III_hesapXY, this, this->Az, this->Ax, this->Ay, true);
    thread th1b(&Foundation::III_hesapXY, this, this->Az, this->Ax, this->Ay, false);
    
    //2.yuzey
    thread th2a(&Foundation::III_hesapXZ, this, this->Ay, this->Ax, this->Az, true);
    thread th2b(&Foundation::III_hesapXZ, this, this->Ay, this->Ax, this->Az, false);
    
    //3.yuzey
    thread th3a(&Foundation::III_hesapYZ, this, this->Ax, this->Ay, this->Az, true);
    thread th3b(&Foundation::III_hesapYZ, this, this->Ax, this->Ay, this->Az, false);
    
    //printing
    thread th4(&Foundation::III_progressPrint, this);
    
    cout << "\n";
    
    const double completedPercentage = 100.0 / this->threadsPerSurface;

    th1a.join();
    progressXY_a = completedPercentage;

    th1b.join();
    progressXY_b = completedPercentage;
    
    th2a.join();
    progressXZ_a = completedPercentage;

    th2b.join();
    progressXZ_b = completedPercentage;
    
    th3a.join();
    progressYZ_a = completedPercentage;
    
    th3b.join();
    progressYZ_b = completedPercentage;

    this->III_calc_progressing = false;
    th4.join();
    
    // end timer
    auto t2 = chrono::high_resolution_clock::now();
    
    // floating-point duration
    chrono::duration<double> fp_seconds = t2 - t1;
    
    // integral duration
    chrono::duration<long> int_seconds = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
    
    // print time in decimals if less than 60 seconds or integer form if longer
    if (int_seconds < chrono::seconds(60))
    {
        cout << " in " << fp_seconds.count() << " seconds." << endl;
    } else {
        cout << " in " << int_seconds.count() << " seconds." << endl;
    }
    
    //hesaplari birlestir
    for (int k=0; k!=this->hesapXY_a.size(); ++k)
    {
        this->hesap.push_back(this->hesapXY_a[k]);
    }

    for (int k=0; k!=this->hesapXY_b.size(); ++k)
    {
        this->hesap.push_back(this->hesapXY_b[k]);
    }
    
    for (int k=0; k!=this->hesapXZ_a.size(); ++k)
    {
        this->hesap.push_back(this->hesapXZ_a[k]);
    }

    for (int k=0; k!=this->hesapXZ_b.size(); ++k)
    {
        this->hesap.push_back(this->hesapXZ_b[k]);
    }
    
    for (int k=0; k!=this->hesapYZ_a.size(); ++k)
    {
        this->hesap.push_back(this->hesapYZ_a[k]);
    }

    for (int k=0; k!=this->hesapYZ_b.size(); ++k)
    {
        this->hesap.push_back(this->hesapYZ_b[k]);
    }
    
}

void Foundation::IIIB_cizim()
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
    
    CImgDisplay disp(back,"ZeminGeril-Temel 0.5",0);
    img=back;
    
    for (int k=0; k!=this->hesap.size(); ++k)
    {
        img.draw_point(this->hesap[k].x, this->hesap[k].y, this->hesap[k].r.deger, 1);
    }
    
    const double aci=(3.141592654)/9;
    
    int c1, c2, c3, c4;
    
    //sinir cizgileri rengi gri, 3 yuzeyi gostermek icin
    
    //arka sag sinir
    c1=this->Ox + ( this->Lx*this->oran_c )*cos(aci);
    c2=this->Oy - ( this->Lx*this->oran_c )*sin(aci);
    c3=this->Ox + ( this->Lx*this->oran_c )*cos(aci) - ( this->Ly*this->oran_c )*cos(aci);
    c4=this->Oy - ( this->Lx*this->oran_c )*sin(aci) - ( this->Ly*this->oran_c )*sin(aci);
    img.draw_line(c1,c2,c3,c4,gri,-1,1);
    
    //arka sol sinir
    c1=this->Ox - ( this->Ly*this->oran_c )*cos(aci);
    c2=this->Oy - ( this->Ly*this->oran_c )*sin(aci);
    img.draw_line(c1,c2,c3,c4,gri,-1,1);
    
    //kesi$ cizgileri
    //orta cizgi
    c1=this->Ox;
    c2=this->Oy;
    c3=this->Ox;
    c4=this->Oy + this->Lz*this->oran_c;
    img.draw_line(c1,c2,c3,c4,gri,-1,1);
    
    //sag cizgi
    c3=this->Ox - ( this->Ly*this->oran_c )*cos(aci);
    c4=this->Oy - ( this->Ly*this->oran_c )*sin(aci);
    img.draw_line(c1,c2,c3,c4,gri,-1,1);
    
    int a=0;
    
    //her metrede bir cizgi
    for (int i=0; i<=(this->Lz*this->oran_c); i+=this->oran_c)
    {
        if (a==5) { img.draw_line(c1,c2+i,c3,c4+i,siyah,0.50); a=0;} else { img.draw_line(c1,c2+i,c3,c4+i,gri,0.35); }
        ++a;
    }
    a=0;
    
    //sol cizgi
    c3=this->Ox + ( this->Lx*this->oran_c )*cos(aci);
    c4=this->Oy - ( this->Lx*this->oran_c )*sin(aci);
    img.draw_line(c1,c2,c3,c4,gri,-1,1);
    
    //her metrede bir cizgi
    for (int i=0; i<=(this->Lz*this->oran_c); i+=this->oran_c)
    {
        if (a==5) { img.draw_line(c1,c2+i,c3,c4+i,siyah,0.50); a=0;} else { img.draw_line(c1,c2+i,c3,c4+i,gri,0.35); }
        ++a;
    }
    a=0;
    
    //legend ciz
    //legend(sondaj_Smax); yerine..
    double Qrol=this->QQ;
    double Lger;
    
    //legend icin heryere gelecek X otelemesi
    const double Lego=50;
    
    //cizim icin baslangic koordinatlari
    pixel A, B;
    A.x=20; A.y=50;
    B.x=A.x;
    
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