//Copyright Bora Okumusoglu, 2006-2021.

namespace ZEMiNGERiL
{
    #include <thread>
    #include "vec.h"
    #include "ZG_Helpers.h"
    #include "ZG_Pixels.h"
    #include "ZG_StressCalculation.h"
    
    // Foundation Class
    class Foundation
    {
    public:
        int n;
        void Define(int _n, Vec<element> _e);
        double Stress(coordinate _nokta, double _z);
        void Input();

        Vec<pixel> hesapXY_a, hesapXY_b, hesapXZ_a, hesapXZ_b, hesapYZ_a, hesapYZ_b, hesap;

        //3D Screen drawing center coordinates
        int Ox=400;
        int Oy=200; 

        void IIIB_hesap();
        void IIIB_cizim();

    private:
        double Stress_Element(element _e, coordinate _nokta, double _z);
        Vec<element> elements;

        //3D Screen cutting corder coordinates
        double Ax, Ay, Az;
        //3D Screen drawing extensions
        int Lx, Ly, Lz;

        //ratio for drawing
        double oran_c, oran_ct;

        //base load to compare
        double QQ;

        char *progressXY_a = (char*)malloc(256);
        bool printReadyXY_a = false;
        char *progressXY_b = (char*)malloc(256);
        bool printReadyXY_b = false;
        char *progressXZ_a = (char*)malloc(256);
        bool printReadyXZ_a = false;
        char *progressXZ_b = (char*)malloc(256);
        bool printReadyXZ_b = false;
        char *progressYZ_a = (char*)malloc(256);
        bool printReadyYZ_a = false;
        char *progressYZ_b = (char*)malloc(256);
        bool printReadyYZ_b = false;
        bool III_calc_progressing=true;

        void III_progressPrint();
        void III_hesapXY(double z, double xx, double yy, bool firstHalf);
        void III_hesapXZ(double y, double xx, double zz, bool firstHalf);
        void III_hesapYZ(double x, double yy, double zz, bool firstHalf);
    };

    // Define Type and Dimensions of Elements of Foundation
    void Foundation::Define(int _n, Vec<element> _e)
    {
        this->n=_n;
        this->elements=_e; 
    }

    // Stress Increment at any point of the Soil caused by an Element of the Foundation
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

    // Stress Increment at any point of the Soil caused by Foundation
    double Foundation::Stress(coordinate _nokta, double _z)
    {
        double result=0.0;

        for(int i=0; i!=this->n; i++)
        {
            result+=this->Stress_Element(this->elements[i], _nokta, _z);              
        }    

        return result;
    }   

    // Input routine for terminal
    void Foundation::Input() 
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

        this->Define(nn,_e);
    }

    #include "ZG_StressCalculation3D.h"
}