#ifndef GRAPHICALUI_HH
#define GRAPHICALUI_HH

#include <QMainWindow>
#include <mutex>
#include "viewinterface.hh"

namespace Ui {
class GraphicalUI;
}

class GraphicalUI : public QMainWindow, public SignalHandler::ViewInterface
{
    Q_OBJECT
    
public:
    explicit GraphicalUI(bool verbose, QWidget *parent = 0);
    
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
