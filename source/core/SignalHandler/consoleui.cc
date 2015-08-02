
/* consoleui.cc
 * 
 * This is the implementation file for the ConsoleUI class defined in
 * consoleui.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 06-April-2015
 * Last modified: 06-April-2015
 */

#include "consoleui.hh"
#include "signalhandlerbuilder.hh"

ConsoleUI::ConsoleUI(std::istream* input, std::ostream* output) :
    input_(input), output_(output), outputMx_(), model_(nullptr)
{
    model_ = SignalHandler::SignalHandlerBuilder().create();
}


ConsoleUI::~ConsoleUI()
{    
    delete model_;
}


void ConsoleUI::showMessage(const std::string& msg)
{
    outputMx_.lock();
    *output_ << msg << std::endl;
    outputMx_.unlock();
}


int ConsoleUI::exec()
{
    outputMx_.lock();
    this->showMessage("Commands:");
    this->showMessage("Q = quit.");
    this->showMessage("S = start");
    this->showMessage("P = pause.");
    outputMx_.unlock();
    
    // Main loop.
    while (true)
    {
        std::string command;
        std::getline(*input_, command);
        
        if (command == "Q"){
            this->showMessage("Quiting...");
            model_->stop();
            return 0;
        }
        else if (command == "S"){
            model_->start();
        }
        else if (command == "P"){
            model_->stop();
            this->showMessage("System paused. Press S to continue. "
                              "Press Q to quit");
        }
        else{
            outputMx_.lock();
            this->showMessage("Unknown command!");
            this->showMessage("Commands:");
            this->showMessage("Q = quit.");
            this->showMessage("S = start/continue");
            this->showMessage("P = pause.");
            outputMx_.unlock();
        }
    }
}


