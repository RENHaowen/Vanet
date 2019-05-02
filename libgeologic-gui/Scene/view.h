#ifndef VIEW_H
#define VIEW_H

/*!
 * \interface View
 * \brief Interface View manages a GLDisplay subsection with a variety of Layers.
 *
 * Interface View is embedded to a GLDisplay and defines a view (general view, minimap, etc.)
 * View members are used to define
 * the View properties (footprint, etc.) and are in charge of a GL viewport.
 *
 * \see GLDisplay
 */
class View {
protected:
    bool visible;       ///< Defines if the whole view is visible
                        ///< true if displayed
                        ///< false otherwise
    int screen_width;	///< Screen/frame width, pixels
    int screen_height;	///< Screen/frame height, pixels
    bool resized;       ///< Boolean value indicating when the viewport must be redeclared

public:
    View(): visible(true), screen_width(1), screen_height(1), resized(true) {}
    virtual ~View() = 0;
    int getScreenWidth() const {return screen_width;}
    int getScreenHeight() const {return screen_height;}

    /*!
     * \brief Draws the View when no animation is playing.
     * This method is called by the parent GLDisplay when it requires to refresh itself.
     * Does not do anything if attribute visible is set to false.
     */
    virtual void draw() = 0;

    /*!
     * \brief Draws the View when animation is playing.
     * This method is called by the parent GLDisplay when it plays an animation.
     * Does not do anything if attribute visible is set to false.
     * \param t The current simulation time (seconds since last midnight).
     */
    virtual void draw(unsigned long long t) = 0;

    /*!
     * \brief Sets current View visible.
     * It sets the visible attribute to true, so the View content is displayed.
     */
    virtual void setVisible() {visible = true;}

    /*!
     * \brief Sets current View invisible.
     * It sets the visible attribute to false, so the View is no longer displayed.
     */
    virtual void setHidden() {visible = false;}

    /*!
     * \brief toggles current View visible attribute.
     * It inverses the current View visibility state.
     */
    virtual void toggleVisibility() {visible = !visible;}

    /*!
     * \brief Informs current View of its parent GLDisplay size.
     * Triggered by parent GLDisplay is resizeGL.
     * \param width The width (pixels) of the parent GLDisplay
     * \param height The height (pixels) of the parent GLDisplay
     */
    virtual void setWindowSize(int width, int height);

    /*!
     * \brief Queries visibility status of current View.
     * \return true if visible, false else.
     */
    virtual bool isVisible() {return visible;}

    /*!
     * \brief resize computes new view parameters for OpenGL display.
     * Needed by all types of views
     */
    virtual void resize() = 0;

    /*!
     * \brief Inits the View GL state.
     * This is a virtual method. Its implementation must clear the view and draw it.
     */
    virtual void initView() = 0;

};

#endif // VIEW_H
