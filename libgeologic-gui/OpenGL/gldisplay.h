#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QLabel>
#include <vector>
#include <memory>
#include <Scene/view.h>
#include <Scene/movableview.h>
#include <Scene/fixedview.h>
#include <Simulation/simulationclock.h>
#include <sys/time.h>

class GLDisplay : public QOpenGLWidget {
    Q_OBJECT

public:
    GLDisplay(QWidget *parent);
    ~GLDisplay();

//    const std::vector<unique_ptr<View>>& getVViews()const {return vViews;}
//    void addView(std::unique_ptr<View> &view) {vViews.push_back(std::move(view));}
    void draw();
    void draw(unsigned long long t);

    MovableView *getMainView()const { return main_view.get(); }

    /*!
     * \brief start starts a simulation / replay: animates the required
     * data, and counts time from simu start to simu end.
     */
    virtual void start();

    /*!
     * \brief stop stops the simulation and reinitializes current simu time.
     */
    virtual void stop();

    /*!
     * \brief pause pauses the simulation (it keeps current time)
     */
    virtual void pause();

    /*!
     * \brief setSimulationTimeScale defines the simulation speed up
     * \param s the number of seconds elapsed in simulation for each real time second.
     */
    virtual void setSimulationTimeScale(double s);

    /*!
     * \brief setSimulationStartTime defines the time (may be the date) the simulation
     * is starting from.
     * \param d the time/date in seconds
     */
    virtual void setSimulationStartTime(unsigned long long d);

    /*!
     * \brief setSimulationEndTime defines the time (may be the date) the simulation
     * is ending.
     * \param d the time/date in seconds
     */
    virtual void setSimulationEndTime(unsigned long long d);

    /*!
     * \brief setSimulationCurrentTime defines current time in simulation
     * \param d the current time in seconds
     */
    virtual void setSimulationCurrentTime(unsigned long long d);

    bool isRunning() {return is_running;}
    bool isPaused() {return is_paused;}
    void setFpsLabel(QLabel *lbl) {_fps_label = lbl;}
    void setTimeLabel(QLabel *lbl) {_time_label = lbl;}
    void setGeoRefs();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:

    virtual void wheelEvent(QWheelEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    std::unique_ptr<MovableView> main_view;         ///< Main OpenGL view (geographical data display)
    std::unique_ptr<FixedView> clock_view;          ///< View to display clock info.

//    std::unique_ptr<FixedView> minimap_view;        ///< View to display minimap.
// TODO: delete    std::vector<std::unique_ptr<View>> vViews;      ///< Vector of views displayed by the GLDisplay
    SimulationClock *simu_clock;                    ///< Simulation time manager
    bool is_mouse_pressed;                          ///< Boolean value to manage view dragging
    bool is_alt_pressed = false;                    ///< Boolean value to manage view rotation
    bool is_ctrl_pressed = false;                   ///< Boolean value to manage hover behavior
    bool hasMovedBtwClick = false;
    int old_x;                                      ///< previous mouse screen x position, used in dragging
    int old_y;                                      ///< previous mouse screen y position, used in dragging
    bool is_running;                                ///< true when playing a simulation or record
    bool is_paused;                                 ///< true when pausing a simulation or record
    bool requires_redraw;                           ///< true if scene must be refreshed
    unsigned long long simu_start;                  ///< Simulation time start date
    unsigned long long simu_end;                    ///< Simulation time end date

    unsigned int _images;                           ///< Image count for FPS calculation
    struct timeval _last_sec;                       ///< FPS calculation: last second update
    QLabel *_fps_label;                             ///< Used to display FPS in status bar
    QLabel *_time_label;                             ///< Used to display time in status bar
    OGRSpatialReference* displayRef;
    std::string displayRefLabel;
};

#endif // GLDISPLAY_H
