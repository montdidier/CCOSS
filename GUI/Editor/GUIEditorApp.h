#ifndef _GUIEDITORAPP_
#define _GUIEDITORAPP_


//////////////////////////////////////////////////////////////////////////////////////////
// File:            GUIEditorApp.h
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     GUI Editor App Class
// Project:         GUI Library
// Author(s):       Jason Boettcher
//                  jboett@internode.on.net

#include "GUI.h"
#include "GUIPropertyPage.h"
#include "GUIListBox.h"
using namespace RTE;


//////////////////////////////////////////////////////////////////////////////////////////
// Class:           GUIEditorApp
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     GUI Editor Application class that handles the main editor app
// Parent(s):       None.
// Class history:   02/07/2009 GUIEditorApp Created.


class GUIEditorApp
{
//////////////////////////////////////////////////////////////////////////////////////////
// Structures & Enumerations

public:

	// Selection structure
	typedef struct {
		bool		m_GrabbedControl;
		bool		m_GrabbedHandle;
		bool		m_TriggerGrab;

		GUIControl	*m_Control;
		int			m_HandleIndex;

		int			m_GrabX, m_GrabY;
		int			m_ClickX, m_ClickY;

	} Selection;


//////////////////////////////////////////////////////////////////////////////////////////
// Public member variable, method and friend function declarations

public:

//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     GUIEditorApp
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a GUIEditorApp object in system
//                  memory.
// Arguments:       None.

    GUIEditorApp();


//////////////////////////////////////////////////////////////////////////////////////////
// Destructor:      ~GUIEditorApp
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Destructor method used to clean up a GUIEditorApp object.
// Arguments:       None.

    ~GUIEditorApp();


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Initialize
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Initializes the editor app
// Arguments:       None.
// Returns:			False if initialization failed

    bool Initialize(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Update
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Updates the editor app.
// Arguments:       None.
// Returns:			False if the editor has quit

    bool Update(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          OnQuitButton
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the quit button has been pused
// Arguments:       None.

    void OnQuitButton(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Protected member variable, method and friend function declarations

protected:


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          OnLoadButton
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the load button has been pused
// Arguments:       Whether to add controls as opposed to wiping out the current layout.

    void OnLoadButton(bool addControls = false);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          OnSaveButton
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the save button has been pused
// Arguments:       None.

    void OnSaveButton(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          OnSaveAsButton
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Called when the save as button has been pused
// Arguments:       None.

    void OnSaveAsButton(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ProcessEditor
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Process the editor logic
// Arguments:       None.

    void ProcessEditor(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          UpdateActiveBoxList
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Updates the list of Active top level ControlBoxs found in the editor.
// Arguments:       None.

    void UpdateActiveBoxList(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ControlUnderMouse
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Checks if a control is under the mouse point
// Returns:			GUIControl. NULL if no control under the mouse

    GUIControl *ControlUnderMouse(GUIControl *Parent, int MouseX, int MouseY);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          HandleUnderMouse
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Checks if a control's handle is under the mouse point
// Returns:			Handle index. -1 if no handle under the mouse

    int HandleUnderMouse(GUIControl *Control, int MouseX, int MouseY);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          MouseInsideBox
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Checks if the mouse point is inside a box
// Returns:			True/False

    bool MouseInsideBox(int MouseX, int MouseY, int X, int Y, int Width, int Height);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          DrawSelectedControl
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draws selection info around a control
// Arguments:       GUIControl

    void DrawSelectedControl(GUIControl *Control);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          DrawSelectionHandle
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draws a selection handle
// Arguments:       Position & Size

    void DrawSelectionHandle(int X, int Y, int Width, int Height);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ClearSelection
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Clears selection info
// Arguments:       None

    void ClearSelection(void);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          CalculateHandleResize
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Calculates new position/size of a control given a handle movement
// Arguments:       Mouse, Pos, Size

    void CalculateHandleResize(int MouseX, int MouseY, int *X, int *Y, int *Width, int *Height);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          GenerateControlName
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Generates a new control name based on the type
// Arguments:       Control Type

	std::string GenerateControlName(std::string strControlType);


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          ProcessSnapCoord
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Calculates the nearest snap position (if snap is on)
// Arguments:       Position

	int ProcessSnapCoord(int Position);


	


//////////////////////////////////////////////////////////////////////////////////////////
// Private member variable, method and friend function declarations

private:


	unsigned char	m_BlackColor;
	BITMAP			*m_pBackBuffer32;
	GUIControlManager	*m_pControlManager;

	GUIControlManager	*m_pEditorManager;

	GUIPropertyPage	*m_pPropertyPage;

	GUIListBox	        *m_pActiveBoxList;

	GUIControl		*m_pRootControl;

	Selection		m_SelectionInfo;

	std::string		m_strFilename;


	// Editor setup
	bool			m_bDirty;
	bool			m_bSnapGrid;
	int				m_nGridSize;

};



#endif  //  File