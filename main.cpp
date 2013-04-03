/*! 
 *  \file main.cpp
 *  \brief    The sc_main
 *  \details Start point of the simulation.
 *  \author    Antti Siirilä, 501449
 *  \version   1.0
 *  \date      28.1.2013
 */


#include "Simulation.hpp"
#include "Configuration.hpp"


//!Defines the file name for the VCD output.
//#define VCD_FILE_NAME "anjosi_ex3_vcd"


using namespace std;
using namespace sc_core;
using namespace sc_dt;

#define SIMULATION_DURATION 20

#define _GUI

/*!
 * \brief sc_main
 * \details Initiates the Simulation module, which builds up the Router modules and starts the simulation.
 */

const char* g_DebugID = "Level_debug:";
const char* g_ReportID = "Level_info:";
const char* g_SimulationVersion = "Test run";

int sc_main(int argc, char * argv [])
{
    /// Establish a socket connection with GUI

    ServerSocket SimulationServer ( 30000 ); 
    ServerSocket GUISocket; 

#ifdef _GUI
    cout << "Waiting the GUI to connect..." << endl;
    SimulationServer.accept ( GUISocket ); 
    GUISocket.set_non_blocking(true);
    string DataWord;
    bool setupLoop = true;
    cout << "Receiving from the GUI..." << endl;

    while(setupLoop)
        {
            try
                {
                    GUISocket >> DataWord;
                }
            catch(SocketException e)
                {
                    std::cout << "got exeption in main: " << e.description() << "\n"; 
                }
                    
            if(DataWord.compare("SETUP") == 0)
                cout << "Set-up received" << endl;
            else if(DataWord.compare("STAR") == 0)
                {
                    cout << "Start received" << endl;
                    DataWord = "Simu";
                    setupLoop = false;
                }
            else
                cout << "Unknown GUI command: " << DataWord <<endl;
            try
                { 
                    GUISocket << DataWord;
                }
            catch(SocketException e)
                {
                    cout << e.description() << endl;
                }
        }
#endif


//testing the simulation configuration
SimulationConfig l_Config(3);
l_Config.addRouterConfig(0, 2);
l_Config.addRouterConfig(1, 2);
l_Config.addRouterConfig(2, 2);
l_Config.addBGPSessionParameters(0, 60, 3);
l_Config.addBGPSessionParameters(1, 60, 3);
l_Config.addBGPSessionParameters(2, 60, 3);

  /* Clock period intialization.
   * The clock period is 10 ns.
   */
  const sc_time clk_Period(1, SC_SEC);
  /* System clock.
   * The clock signal is specified in clk_Period.
   */
  sc_clock clk("clk", clk_Period);

    sc_report rp;
    sc_report_handler::set_log_file_name("test_simu.log");
    sc_report_handler::set_actions(g_ReportID, SC_INFO, SC_DISPLAY);
    sc_report_handler::set_actions(g_DebugID, SC_INFO, SC_DO_NOTHING);
    SC_REPORT_INFO(g_ReportID, g_SimulationVersion);

  ///initiate the simulation
Simulation test("Test", GUISocket, l_Config);

    ///connect the clock
    test.port_Clk(clk);
  SC_REPORT_INFO(g_ReportID, StringTools("Main").newReportString("Simulation starts"));


  ///run the simulation	
  sc_start(SIMULATION_DURATION, SC_SEC);
  SC_REPORT_INFO(g_ReportID, StringTools("Main").newReportString("Simulation ends"));
  GUISocket << "END";

return 0;
}//end of main
