#ifndef FIXEDVIEW_H
#define FIXEDVIEW_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <ogr_geometry.h>
#include <memory>
#include <utility>
#include <iostream>
#include <Scene/view.h>
#include <Scene/singlelayer.h>
#include <vector>


/*!
 * \class FixedView
 * \brief Class FixedView implements interface View.
 *
 * It is a static view that cannot be moved, nor zoomed, nor rotated.
 * It is typically used in clock display and minimap.
 *
 * \see View
 */
class FixedView: public View {
protected:
    //std::vector<std::unique_ptr<Layer>> vLayer;
    std::vector<std::unique_ptr<SingleLayer>> vLayer;
    int view_width; ///< View width in pixels
    int view_height; ///< View height in pixels
    int x_position; ///< View x-coordinates of the viewPort
    int y_position; ///< View y-coordinates of the viewPort

    /*!
     * \see View
     */
 //   virtual void setViewCenter(double x, double y);

    /*!
     * \see View
     */
 //   virtual void moveViewCenter(double x_offset, double y_offset);

public:

    void addLayer(std::unique_ptr<SingleLayer> l) { vLayer.push_back(move(l)); }
    /*!
     * \brief Constructor.
     * Initializes base View properties
     * \param c view center
     * \param z zoom level--
     * \param w The width (pixels) of the parent GLDisplay
     * \param h The height (pixels) of the parent GLDisplay
     * \param vw the view width in px
     * \param vh the view height in px
     */
    FixedView(int vw, int vh);

    /*!
     * \brief Destructor
     *
     * Does not do anything currently
     */
    virtual ~FixedView();

    /*!
     * \brief Computes a new zoom value when zooming in.
     *
     * Does not do anything on a FixedView.
     */
//    virtual void zoomIn();

    /*!
     * \brief Computes a new zoom value when zooming out.
     *
     * Does not do anything on a FixedView.
     */
  //  virtual void zoomOut();

    /*!
     * \brief Inits the View GL state.
     *
     * Prepares a viewport.
     */
   virtual void initView();

    /*!
     * \brief Computes the 2D GL Ortho boundaries.
     *
     * It is used to compute the boundaries without changing the image aspect ratio.
     * It uses footprint size and screen size to compute the boundaries.
     */
  //  virtual void updateOrthoBoundaries();

    /*!
     * \brief Gets the minimum values of View boundaries.
     *
     * \return x and y minimum value as a std::pair of double.
     */
  //  virtual std::pair<double, double> getMinBoundaries();

    /*!
     * \brief Modifies the View angle.
     *
     * It modifies the View rotation angle.
     * \param mod_angle The new rotation angle value.
     * Does not do anything in a FixedView.
     */
   // virtual void modAngle(double mod_angle);

    /*!
     * \brief Computes the relative position of FixedView in the parent GLDisplay.
     */
    virtual void updateScreenCoordinates();

    virtual void draw(unsigned long long t);

    virtual void draw();

    virtual void resize();
};

#endif // FIXEDVIEW_H
