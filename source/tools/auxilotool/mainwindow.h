#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class
 *  Application's main window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor
     * \param parent Parent object.
     * \pre -
     * \post Main window is initialized.
     */
    explicit MainWindow(QWidget *parent = 0);

    /*!
     * \brief Destructor.
     */
    virtual ~MainWindow();


protected:

    /*!
     * \brief This method is automatically called when MainWindow is closed.
     * \param event Closing event that triggered closing.
     * \post If there are unsaved changes, user is warned, and may cancel,
     *      save and close or close anyway.
     */
    virtual void closeEvent(QCloseEvent* event);


private slots:

    void onEditEvents();
    void onEditSignals();

    // This slot can be used on unimplemented features.
    void onUnimplemented();


private:

    Ui::MainWindow *ui;

    void initView();
};

#endif // MAINWINDOW_H
