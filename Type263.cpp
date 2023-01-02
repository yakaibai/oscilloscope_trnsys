#include <cmath>
#include <fstream>
#include "string.h"
#include <thread>
//TRNSYS
#include "TRNSYS.h"
//FLTK
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/x.H>

//vector
#include<vector>

class meBox:public Fl_Widget{
    std::vector<double> x1,y1;
    int index;
    void draw(){
        fl_push_clip(x(),y(),w(),h());        
        fl_push_matrix();

        if(index>1)
        {
            fl_color(FL_RED);
            fl_line(x1[index-2],y1[index-2],x1[index-1],y1[index-1]);
        }
        else if(index==1)
        {
            fl_color(FL_BLACK);
            fl_rectf(x()+30,y()+30,w()-60,h()-60);

            char cstr[50];

            for(int hloc=100;hloc<600;hloc+=100)
            {
                fl_color(FL_WHITE);
                fl_line(x()+30,hloc,x()+w()-30,hloc);

                fl_color(FL_BLACK);
                sprintf(cstr,"%d",-(hloc-300));
                fl_draw(cstr,x(),hloc);
            }
            
            for(int wloc=100;wloc<1000;wloc+=100)
            {
                fl_color(FL_WHITE);
                fl_line(wloc,y()+30,wloc,y()+h()-30);

                fl_color(FL_BLACK);
                sprintf(cstr,"%d",-(wloc-500));
                fl_draw(cstr,wloc,y()+h()-15);
            }


            
        }

        fl_pop_matrix();
        fl_pop_clip();
    };
public:
    meBox(int x,int y,int w,int h):Fl_Widget(x,y,w,h){index=0;};
    void addXY(int x2,int y2){
        x1.push_back(x2);
        y1.push_back(y2);
        index++;
        draw();
    };
};
extern "C" __declspec(dllexport) void TYPE263(void)
{
	//Variable Declarations
	double Timestep, Time, xin1, xin2, xin1old, xin2old;
	int index, CurrentUnit, CurrentType;
	char type[20];
	char message[400];

    Fl_Window *window;
    meBox *box;
    
    double ihdl;
    HWND hdl;
	//Get the Global Trnsys Simulation Variables
	Time = getSimulationTime();
	Timestep = getSimulationTimeStep();
	CurrentUnit = getCurrentUnit();
	CurrentType = getCurrentType();

	//Set the Version Number for This Type
	if (getIsVersionSigningTime())
	{
		int v = 17;
		setTypeVersion(&v);
		return;
	}

	//Do All of the Last Call Manipulations Here
	if (getIsLastCallofSimulation())
	{
		return;
	}

	//Perform Any "End of Timestep" Manipulations That May Be Required
	if (getIsEndOfTimestep()) 
	{
        	//Get the Parameters of This Type

        //Get the Current Inputs to the Model
        index = 1;
        xin1 = getInputValue(&index);
        index = 2;
        xin2 = getInputValue(&index);

    //---------------------------------------------------------------------------------------------------------------------- -
        index=1;
        ihdl=getStaticArrayValue(&index);
        index=2;
        xin1old=getStaticArrayValue(&index);        
        index=3;
        xin2old=getStaticArrayValue(&index);

        hdl=(HWND)IntToPtr(ihdl);        
        window=fl_find(hdl);        
        
        box=(meBox*)window->child(0);
        box->addXY(-xin1+500,-xin2+300);
        //box->redraw();
        //window->redraw();


    //============================================================
        index = 1;
        setStaticArrayValue(&index, &ihdl);
        index = 2;
        setStaticArrayValue(&index, &xin1);
        index = 3;
        setStaticArrayValue(&index, &xin2);
    //====================================================================
		if (getIsIncludedInSSR())
		{

		}
		return;
	}

	//Do All of the "Very First Call of the Simulation Manipulations" Here
	if (getIsFirstCallofSimulation())
	{
    //========================================================================================

        

    //========================================================================================
		//Tell the TRNSYS Engine How This Type Works
		int npar = 0;
		int nin = 2;
		int nder = 0;
		int nout = 0;
		int mode = 1;
		int staticStore = 3;
		int dynamicStore = 0;

		setNumberofParameters(&npar);
		setNumberofInputs(&nin);
		setNumberofDerivatives(&nder);
		setNumberofOutputs(&nout);
		setIterationMode(&mode);
		setNumberStoredVariables(&staticStore, &dynamicStore);

		//Set the Correct Input and Output Variable Types
		index = 1;
		char str[4];
		strcpy_s(str,"TE1");
		setInputUnits(&index, str, 3);
		index = 2;
		strcpy_s(str, "TE1");
		setInputUnits(&index, str, 3);


		//Set up this Type's entry in the SSR report
		if (getIsIncludedInSSR())
		{
			int nInt = 0;
			int nMinMax = 0;
			int nPars = 0;
			int nText = 0;
			setNumberOfReportVariables(&nInt, &nMinMax, &nPars, &nText);
		}

		return;
	}

	//Do All of the "Start Time" Manipulations Here - There Are No Iterations at the Intial Time
	if (getIsStartTime())
	{

		//Read in the Values of the Parameters from the Input File

		//Check the Parameters for Problems
        window = new Fl_Window (0,0,1000, 600);
        //window->fullscreen();
        window-> label("xyplot"); 
        box = new meBox (0, 0, 1000, 600);
        //box->labelsize (36); 
        //box->labelfont (FL_BOLD+FL_ITALIC); (FL_SHADOW_LABEL); 
        
        window->end();
        window->show();
        //int x=Fl::run();
        HWND hdl = (HWND)fl_xid(window);
        //HWND hdl=FindWindow(NULL,"Hello World!");
        ihdl=PtrToInt(hdl);
        
        std::thread t1(Fl::run);
        t1.detach();

		//Set the Initial Values of the Outputs
		index = 1;
		xin1 = getInputValue(&index);
		index = 2;
		xin2 = getInputValue(&index);
//============================================================
        index = 1;
        setStaticArrayValue(&index, &ihdl);
        index = 2;
        setStaticArrayValue(&index, &xin1);
        index = 3;
        setStaticArrayValue(&index, &xin2);


//====================================================================
		//Initialize SSR report variables
		if (getIsIncludedInSSR())
		{


		}

		return;

	}

//---------------------------------------------------------------------------------------------------------------------- -
	//Get the Parameters of This Type

	//Get the Current Inputs to the Model


//---------------------------------------------------------------------------------------------------------------------- -
	//Perform All of the Calculations Here

	return;
}

