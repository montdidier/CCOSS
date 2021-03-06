#ifndef _GUIMANAGER_
#define _GUIMANAGER_

//////////////////////////////////////////////////////////////////////////////////////////
// File:            GUIManager.h
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     GUIManager class
// Project:         GUI Library
// Author(s):       Jason Boettcher
//                  jackal@shplorb.com
//                  www.shplorb.com/~jackal


namespace RTE
{

class Timer;


//////////////////////////////////////////////////////////////////////////////////////////
// Class:           GUIManager
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     The main manager that handles all the panels and inputs
// Parent(s):       None.
// Class history:   12/28/2003 GUIManager Created.

class GUIManager {

//////////////////////////////////////////////////////////////////////////////////////////
// Public member variable, method and friend function declarations

public:


//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     GUIManager
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a GUIManager object in system
//                  memory.
// Arguments:       Input Interface

    GUIManager(GUIInput *input);


//////////////////////////////////////////////////////////////////////////////////////////
// Destructor:      GUIManager
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Destructor method used to clean up a GUIManager object.
// Arguments:       None.

    ~GUIManager();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Clear
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Clears the manager.
// Arguments:       None.

    void Clear(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          AddPanel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Adds a panel to the list.
// Arguments:       Pointer to a panel.

    void AddPanel(GUIPanel *panel);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Update
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Updates the GUI.
// Arguments:       None.

    void Update(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Draw
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draw all the panels
// Arguments:       Screen.

    void Draw(GUIScreen *Screen);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          EnableMouse
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Enables and disables the mouse completely for this.
// Arguments:       Enable?

    void EnableMouse(bool enable = true) { m_MouseEnabled = enable; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          CaptureMouse
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets up capturing a mouse for a panel.
// Arguments:       Panel.

    void CaptureMouse(GUIPanel *Panel);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ReleaseMouse
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Releases a mouse capture.
// Arguments:       None.

    void ReleaseMouse(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetPanelID
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets a unique ID for a panel.
// Arguments:       None.

    int GetPanelID(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetInputController
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the input controller object
// Arguments:       None.

    GUIInput * GetInputController() { return m_Input; }


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          TrackMouseHover
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets up the manager to enable/disable hover tracking of this panel
// Arguments:       Panel, Enabled, Delay (milliseconds)

    void TrackMouseHover(GUIPanel *Pan, bool Enabled, int Delay);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetFocus
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Give focus to a panel.
// Arguments:       Panel.

    void SetFocus(GUIPanel *Pan);


//////////////////////////////////////////////////////////////////////////////////////////
// Private member variable and method declarations

private:


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          FindBottomPanel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Goes through the panel list and selects the bottommost
//                  ('first', render wise) panel on a specific point.
// Arguments:       Mouse Position.

    GUIPanel *FindBottomPanel(int X, int Y);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          FindTopPanel
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Goes through the panel list and selects the topmost ('last', render
//                  wise) panel on a specific point.
// Arguments:       Mouse Position.

    GUIPanel *FindTopPanel(int X, int Y);

/*
//////////////////////////////////////////////////////////////////////////////////////////
// Method:          CreateTimer
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Creates the timer.
// Arguments:       None.

    bool CreateTimer(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GetCurrentTime
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Gets the current time since the timer was created.
// Arguments:       None.

    float GetTime(void);
*/

//////////////////////////////////////////////////////////////////////////////////////////
// Method:          MouseInRect
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Checks if the mouse point is inside a rectangle.
// Arguments:       Rectangle, Mouse position.

    bool MouseInRect(RECT *Rect, int X, int Y);


// Members

    std::vector<GUIPanel *> m_PanelList;
    GUIPanel                *m_CapturedPanel;
    GUIPanel                *m_FocusPanel;
    GUIPanel                *m_MouseOverPanel;

    // Input
    GUIInput                *m_Input;
    bool                    m_MouseEnabled;
    int                     m_OldMouseX, m_OldMouseY;

    // Double click
    int                     m_DoubleClickTime;
    int                     m_DoubleClickSize;
    int                     m_DoubleClickButtons;
    float                   m_LastMouseDown[3];
    RECT                    m_DoubleClickRect;

    // Hover tracking
    bool                    m_HoverTrack;
    GUIPanel                *m_HoverPanel;
    float                   m_HoverTime;

    bool                    m_UseValidation;
    int                     m_UniqueIDCount;

    // Timer
    Timer                   *m_pTimer;
};


}; // namespace RTE

#endif  //  _GUIMANAGER_