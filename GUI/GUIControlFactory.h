#ifndef _GUICONTROLFACTORY_
#define _GUICONTROLFACTORY_

//////////////////////////////////////////////////////////////////////////////////////////
// File:            GUIControlFactory.h
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     GUIControlFactory class
// Project:         GUI Library
// Author(s):       Jason Boettcher
//                  jackal@shplorb.com
//                  www.shplorb.com/~jackal


namespace RTE
{

//////////////////////////////////////////////////////////////////////////////////////////
// Class:           GUIControlFactory
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     A class used to create the different controls based on name
// Parent(s):       None.
// Class history:   1/5/2004 GUIControlFactory Created.

class GUIControlFactory {

//////////////////////////////////////////////////////////////////////////////////////////
// Public member variable, method and friend function declarations

public:


//////////////////////////////////////////////////////////////////////////////////////////
// Constructor:     GUIControlFactory
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Constructor method used to instantiate a GUIControlFactory object in
//                  system memory.
// Arguments:       None.

    GUIControlFactory() { };


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          CreateControl
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Method used for creating controls
// Arguments:       Control Type Name.

    static GUIControl *CreateControl(GUIManager *Manager, 
                                     GUIControlManager *ControlManager,
                                     const std::string ControlName);

};


}; // namespace RTE


#endif  //  _GUICONTROLFACTORY_