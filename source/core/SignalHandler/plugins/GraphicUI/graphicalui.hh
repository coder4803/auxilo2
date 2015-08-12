/* graphicalui.hh
 * 
 * This header file defines the GraphicalUi class. This class is a graphical 
 * implementation of SignalHandler::ViewInterface
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#ifndef GRAPHICALUI_HH
#define GRAPHICALUI_HH

#include <QMainWindow>
#include <mutex>
#include "../../UserInterface/viewinterface.hh"

namespace Ui {
class GraphicalUI;
}

/*!
 * \brief The GraphicalUI class
 *  Graphical implementation of SignalHandler::ViewInterface.
 */
class GraphicalUI : public QMainWindow, public SignalHandler::ViewInterface
{
    Q_OBJECT
    
public:
    
    /*!
     * \brief Constructor
     * \param verbose Verbose flag for debug and warning messages.
     * \param parent QMainWindow's parent.
     */
    explicit GraphicalUI(bool verbose, QWidget *parent = 0);
    
    //! Destructor.
    virtual ~GraphicalUI();
    
    // SignalHandler::ViewInterface Implementation:
    virtual void critical(const QString &msg);
    virtual void debug(const QString &msg);
    virtual void warning(const QString& msg);
    virtual void fatal(const QString& msg);
    
private Q_SLOTS:
    
    void onQuitBtnClicked();
    
private:
    Ui::GraphicalUI *ui;
    bool verbose_;
    std::mutex outputMx_;
    std::mutex debugMx_;
    std::mutex fatalMx_;
};

#endif // GRAPHICALUI_HH
