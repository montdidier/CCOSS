//////////////////////////////////////////////////////////////////////////////////////////
// File:            MOSprite.cpp
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Source file for the MOSprite class.
// Project:         Retro Terrain Engine
// Author(s):       Daniel Tabar
//                  data@datarealms.com
//                  http://www.datarealms.com


//////////////////////////////////////////////////////////////////////////////////////////
// Inclusions of header files

#include "MOSprite.h"
#include "RTEManagers.h"
#include "DDTTools.h"
#include "AEmitter.h"

using namespace std;

namespace RTE
{

ABSTRACTCLASSINFO(MOSprite, MovableObject)


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Clear
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Clears all the member variables of this MOSprite, effectively
//                  resetting the members of this abstraction level only.

void MOSprite::Clear()
{
    m_SpriteFile.Reset();
    m_aSprite = 0;
    m_FrameCount = 1;
    m_SpriteOffset.Reset();
    m_Frame = 0;
    m_SpriteAnimMode = NOANIM;
    m_SpriteAnimDuration = 500;
    m_SpriteAnimTimer.Reset();
    m_HFlipped = false;
    m_MaxRadius = 1;
    m_MaxDiameter = 2;
    m_Rotation.Reset();
    m_PrevRotation.Reset();
    m_AngularVel = 0;
    m_PrevAngVel = 0;
    m_AngOscillations = 0;
    m_SettleMaterialDisabled = false;
    m_pEntryWound = 0;
    m_pExitWound = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Makes the MOSprite object ready for use.

int MOSprite::Create()
{
    if (MovableObject::Create() < 0)
        return -1;

    // Post-process reading
    delete [] m_aSprite;
    m_aSprite = m_SpriteFile.GetAsAnimation(m_FrameCount);

    if (m_aSprite && m_aSprite[0])
    {
        // Set default sprite offset
        if (m_SpriteOffset.IsZero())
        {
            m_SpriteOffset.m_X = -m_aSprite[0]->w / 2;
            m_SpriteOffset.m_Y = -m_aSprite[0]->h / 2;
        }
        // Calc maximum dimensions from the Pos, based on the sprite
        float maxX = DMax(fabs(m_SpriteOffset.m_X), fabs(m_aSprite[0]->w + m_SpriteOffset.m_X));
        float maxY = DMax(fabs(m_SpriteOffset.m_Y), fabs(m_aSprite[0]->h + m_SpriteOffset.m_Y));
        m_MaxRadius = sqrt((float)(maxX * maxX) + (maxY * maxY));
        m_MaxDiameter = m_MaxRadius * 2;
    }
    else
        return -1;

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Makes the MOSprite object ready for use.

int MOSprite::Create(ContentFile spriteFile,
                     const int frameCount,
                     const float mass,
                     const Vector &position,
                     const Vector &velocity,
                     const unsigned long lifetime)
{
    MovableObject::Create(mass, position, velocity, 0, 0, lifetime);

    m_SpriteFile = spriteFile;
    m_FrameCount = frameCount;
    delete [] m_aSprite;
    m_aSprite = m_SpriteFile.GetAsAnimation(m_FrameCount);
    m_SpriteOffset = Vector(-m_aSprite[0]->w / 2, -m_aSprite[0]->h / 2);

    m_HFlipped = false;

    // Calc maximum dimensions from the Pos, based on the sprite
    float maxX = DMax(fabs(m_SpriteOffset.m_X), fabs(m_aSprite[0]->w + m_SpriteOffset.m_X));
    float maxY = DMax(fabs(m_SpriteOffset.m_Y), fabs(m_aSprite[0]->h + m_SpriteOffset.m_Y));
    m_MaxRadius = sqrt((float)(maxX * maxX) + (maxY * maxY));
    m_MaxDiameter = m_MaxRadius * 2;

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Method:  Create
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Creates a MOSprite to be identical to another, by deep copy.

int MOSprite::Create(const MOSprite &reference)
{
    MovableObject::Create(reference);

    if (!reference.m_aSprite)
        return -1;

    m_SpriteFile = reference.m_SpriteFile;

    m_FrameCount = reference.m_FrameCount;
    m_Frame = reference.m_Frame;
    // Allocate a new array of pointers (owned by this),
    // and copy the pointers' values themselves over by shallow copy (the BITMAPs are not owned by this)
    delete [] m_aSprite;
    m_aSprite = new BITMAP *[m_FrameCount];
    for (int i = 0; i < m_FrameCount; ++i)
    {
        m_aSprite[i] = reference.m_aSprite[i];
    }

    m_SpriteOffset = reference.m_SpriteOffset;
    m_SpriteAnimMode = reference.m_SpriteAnimMode;
    m_SpriteAnimDuration = reference.m_SpriteAnimDuration;
    m_HFlipped = reference.m_HFlipped;
    m_MaxRadius = reference.m_MaxRadius;
    m_MaxDiameter = reference.m_MaxDiameter;

    m_Rotation = reference.m_Rotation;
    m_AngularVel = reference.m_AngularVel;
    m_SettleMaterialDisabled = reference.m_SettleMaterialDisabled;
    m_pEntryWound = reference.m_pEntryWound;
    m_pExitWound = reference.m_pExitWound;
//    if (reference.m_pExitWound)  Not doing anymore since we're not owning
//        m_pExitWound = dynamic_cast<AEmitter *>(reference.m_pExitWound->Clone());

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  ReadProperty
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Reads a property value from a reader stream. If the name isn't
//                  recognized by this class, then ReadProperty of the parent class
//                  is called. If the property isn't recognized by any of the base classes,
//                  false is returned, and the reader's position is untouched.

int MOSprite::ReadProperty(std::string propName, Reader &reader)
{
    if (propName == "SpriteFile")
        reader >> m_SpriteFile;
    else if (propName == "FrameCount")
        reader >> m_FrameCount;
    else if (propName == "SpriteOffset")
        reader >> m_SpriteOffset;
    else if (propName == "SpriteAnimMode")
    {
//        string mode;
//        reader >> mode;
        int mode;
        reader >> mode;
        m_SpriteAnimMode = (SpriteAnimMode)mode;
/*
        if (mode == "NOANIM")
            m_SpriteAnimMode = NOANIM;
        else if (mode == "ALWAYSLOOP")
            m_SpriteAnimMode = ALWAYSLOOP;
        else if (mode == "ALWAYSPINGPONG")
            m_SpriteAnimMode = ALWAYSPINGPONG;
        else if (mode == "LOOPWHENMOVING")
            m_SpriteAnimMode = LOOPWHENMOVING;
        else
            Abort
*/
    }
    else if (propName == "SpriteAnimDuration")
        reader >> m_SpriteAnimDuration;
    else if (propName == "HFlipped")
        reader >> m_HFlipped;
    else if (propName == "Rotation")
        reader >> m_Rotation;
    else if (propName == "AngularVel")
        reader >> m_AngularVel;
    else if (propName == "SettleMaterialDisabled")
        reader >> m_SettleMaterialDisabled;
    else if (propName == "EntryWound")
        m_pEntryWound = dynamic_cast<const AEmitter *>(g_PresetMan.GetEntityPreset(reader));
    else if (propName == "ExitWound")
        m_pExitWound = dynamic_cast<const AEmitter *>(g_PresetMan.GetEntityPreset(reader));
    else
        // See if the base class(es) can find a match instead
        return MovableObject::ReadProperty(propName, reader);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  SetEntryWound
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets entry wound emitter for this MOSprite
void MOSprite::SetEntryWound(std::string presetName, std::string moduleName)
{
	if (presetName == "")
		m_pEntryWound = 0;
	else
		m_pEntryWound = dynamic_cast<const AEmitter *>(g_PresetMan.GetEntityPreset("AEmitter", presetName, moduleName));
}

//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  SetExitWound
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Sets exit wound emitter for this MOSprite
void MOSprite::SetExitWound(std::string presetName, std::string moduleName)
{
	if (presetName == "")
		m_pExitWound = 0;
	else
		m_pExitWound = dynamic_cast<const AEmitter *>(g_PresetMan.GetEntityPreset("AEmitter", presetName, moduleName));
}


//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  GetEntryWoundPresetName
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns entry wound emitter preset name for this MOSprite

std::string MOSprite::GetEntryWoundPresetName() const
{
	return m_pEntryWound ? m_pEntryWound->GetPresetName() : ""; 
};

//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  GetExitWoundPresetName
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Returns exit wound emitter preset name for this MOSprite

std::string MOSprite::GetExitWoundPresetName() const
{ 
	return m_pExitWound ? m_pExitWound->GetPresetName() : ""; 
};

//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  Save
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Saves the complete state of this MOSprite with a Writer for
//                  later recreation with Create(Reader &reader);

int MOSprite::Save(Writer &writer) const
{
    MovableObject::Save(writer);
// TODO: Make proper save system that knows not to save redundant data!
/*
    writer.NewProperty("SpriteFile");
    writer << m_SpriteFile;
    writer.NewProperty("FrameCount");
    writer << m_FrameCount;
    writer.NewProperty("SpriteOffset");
    writer << m_SpriteOffset;
    writer.NewProperty("SpriteAnimMode");
    writer << m_SpriteAnimMode;
    writer.NewProperty("SpriteAnimDuration");
    writer << m_SpriteAnimDuration;
    writer.NewProperty("HFlipped");
    writer << m_HFlipped;
    writer.NewProperty("Rotation");
    writer << m_Rotation.GetRadAngle();
    writer.NewProperty("AngularVel");
    writer << m_AngularVel;
    writer.NewProperty("SettleMaterialDisabled");
    writer << m_SettleMaterialDisabled;
    writer.NewProperty("EntryWound");
    writer << m_pEntryWound;
    writer.NewProperty("ExitWound");
    writer << m_pExitWound;
*/
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          Destroy
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Destroys and resets (through Clear()) the MOSprite object.

void MOSprite::Destroy(bool notInherited)
{
    //  Delete only the array of pointers, not the BITMAP:s themselves... owned by static contentfile maps
    delete[] m_aSprite;
//    delete m_pEntryWound; Not doing this anymore since we're not owning
//    delete m_pExitWound;

    if (!notInherited)
        MovableObject::Destroy();
    Clear();
}


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetFrame
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Hard-sets the frame this sprite is supposed to show.

void MOSprite::SetFrame(unsigned int newFrame)
{
    if (newFrame < 0)
        newFrame = 0;
    if (newFrame >= m_FrameCount)
        newFrame = m_FrameCount - 1;

    m_Frame = newFrame;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Method:          SetNextFrame
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Hard-sets the frame this sprite is supposed to show, to the
//                  consecutive one after the current one. If currently the last fame is
//                  this will set it to the be the first, looping the animation.

bool MOSprite::SetNextFrame()
{
    if (++m_Frame >= m_FrameCount)
    {
        m_Frame = 0;
        return true;
    }
    return false;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  IsOnScenePoint
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Indicates whether this' current graphical representation overlaps
//                  a point in absolute scene coordinates.

bool MOSprite::IsOnScenePoint(Vector &scenePoint) const
{
    if (!m_aSprite[m_Frame])
        return false;
// TODO: TAKE CARE OF WRAPPING
/*
    // Take care of wrapping situations
    bitmapPos = m_Pos + m_BitmapOffset;
    Vector aScenePoint[4];
    aScenePoint[0] = scenePoint;
    int passes = 1;

    // See if need to double draw this across the scene seam if we're being drawn onto a scenewide bitmap
    if (targetPos.IsZero())
    {
        if (g_SceneMan.SceneWrapsX())
        {
            if (bitmapPos.m_X < m_pFGColor->w)
            {
                aScenePoint[passes] = aScenePoint[0];
                aScenePoint[passes].m_X += g_SceneMan.GetSceneWidth();
                passes++;
            }
            else if (aScenePoint[0].m_X > pTargetBitmap->w - m_pFGColor->w)
            {
                aScenePoint[passes] = aScenePoint[0];
                aScenePoint[passes].m_X -= g_SceneMan.GetSceneWidth();
                passes++;
            }
        }
        if (g_SceneMan.SceneWrapsY())
        {
            
        }
    }

    // Check all the passes needed
    for (int i = 0; i < passes; ++i)
    {
        if (WithinBox(aScenePoint[i], m_Pos + m_BitmapOffset, m_pFGColor->w, m_pFGColor->h))
        {
            if (getpixel(m_pFGColor, aScenePoint[i].m_X, aScenePoint[i].m_Y) != g_KeyColor ||
               (m_pBGColor && getpixel(m_pBGColor, aScenePoint[i].m_X, aScenePoint[i].m_Y) != g_KeyColor) ||
               (m_pMaterial && getpixel(m_pMaterial, aScenePoint[i].m_X, aScenePoint[i].m_Y) != g_MaterialAir))
               return true;
        }
    }
*/
    if (WithinBox(scenePoint, m_Pos.m_X - m_MaxRadius, m_Pos.m_Y - m_MaxRadius, m_Pos.m_X + m_MaxRadius, m_Pos.m_Y + m_MaxRadius))
    {
        // Get scene point in object's relative space
        Vector spritePoint = scenePoint - m_Pos;
        spritePoint.FlipX(m_HFlipped);
        // Check over overlap
        int pixel = getpixel(m_aSprite[m_Frame], spritePoint.m_X - m_SpriteOffset.m_X, spritePoint.m_Y - m_SpriteOffset.m_Y);
        // Check that it isn't outside the bitmap, and not of the key color
        if (pixel != -1 && pixel != g_KeyColor)
           return true;
    }

    return false;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  RotateOffset
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Rotates a vector offset from this MORotating's position according to
//                  the rotate angle and flipping.

Vector MOSprite::RotateOffset(const Vector &offset) const
{
    Vector rotOff(offset.GetXFlipped(m_HFlipped));
    rotOff *= const_cast<Matrix &>(m_Rotation);
    return rotOff;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  UnRotateOffset
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Rotates a vector offset from this MORotating's position according to
//                  the NEGATIVE rotate angle and takes flipping into account.

Vector MOSprite::UnRotateOffset(const Vector &offset) const
{
    Vector rotOff(offset.GetXFlipped(m_HFlipped));
    rotOff /= const_cast<Matrix &>(m_Rotation);
    return rotOff;
}

/*
//////////////////////////////////////////////////////////////////////////////////////////
// Pure v. method:  Update
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Updates this MOSprite. Supposed to be done every frame.

void MOSprite::Update()
{
    MovableObject::Update();
}
*/

//////////////////////////////////////////////////////////////////////////////////////////
// Virtual method:  Draw
//////////////////////////////////////////////////////////////////////////////////////////
// Description:     Draws this MOSprite's current graphical representation to a
//                  BITMAP of choice.

void MOSprite::Draw(BITMAP *pTargetBitmap,
                    const Vector &targetPos,
                    DrawMode mode,
                    bool onlyPhysical) const
{
    if (!m_aSprite[m_Frame])
        DDTAbort("Sprite frame pointer is null when drawing MOSprite!");

    // Apply offsets and positions.
    Vector spriteOffset;
    if (m_HFlipped)
        spriteOffset.SetXY(-(m_aSprite[m_Frame]->w + m_SpriteOffset.m_X), m_SpriteOffset.m_Y);
    else
        spriteOffset = m_SpriteOffset;

    Vector spritePos(m_Pos + spriteOffset - targetPos);

    // Take care of wrapping situations
    Vector aDrawPos[4];
    aDrawPos[0] = spritePos;
    int passes = 1;

    // Only bother with wrap drawing if the scene actually wraps around
    if (g_SceneMan.SceneWrapsX())
    {
        // See if need to double draw this across the scene seam if we're being drawn onto a scenewide bitmap
        if (targetPos.IsZero() && m_WrapDoubleDraw)
        {
            if (spritePos.m_X < m_aSprite[m_Frame]->w)
            {
                aDrawPos[passes] = spritePos;
                aDrawPos[passes].m_X += pTargetBitmap->w;
                passes++;
            }
            else if (spritePos.m_X > pTargetBitmap->w - m_aSprite[m_Frame]->w)
            {
                aDrawPos[passes] = spritePos;
                aDrawPos[passes].m_X -= pTargetBitmap->w;
                passes++;
            }
        }
        // Only screenwide target bitmap, so double draw within the screen if the screen is straddling a scene seam
        else if (m_WrapDoubleDraw)
        {
            if (targetPos.m_X < 0)
            {
                aDrawPos[passes] = aDrawPos[0];
                aDrawPos[passes].m_X -= g_SceneMan.GetSceneWidth();
                passes++;
            }
            if (targetPos.m_X + pTargetBitmap->w > g_SceneMan.GetSceneWidth())
            {
                aDrawPos[passes] = aDrawPos[0];
                aDrawPos[passes].m_X += g_SceneMan.GetSceneWidth();
                passes++;
            }
        }
    }

    for (int i = 0; i < passes; ++i)
    {
        if (mode == g_DrawMaterial) {
            DDTAbort("Ordered to draw an MOSprite in its material, which is not possible!");
        }
        else if (mode == g_DrawAir)
            draw_character_ex(pTargetBitmap, m_aSprite[m_Frame], aDrawPos[i].GetFloorIntX(), aDrawPos[i].GetFloorIntY(), g_MaterialAir, -1);
        else if (mode == g_DrawKey)
            draw_character_ex(pTargetBitmap, m_aSprite[m_Frame], aDrawPos[i].GetFloorIntX(), aDrawPos[i].GetFloorIntY(), g_KeyColor, -1);
        else if (mode == g_DrawWhite)
            draw_character_ex(pTargetBitmap, m_aSprite[m_Frame], aDrawPos[i].GetFloorIntX(), aDrawPos[i].GetFloorIntY(), g_WhiteColor, -1);
        else if (mode == g_DrawMOID)
        {
            int spriteX = aDrawPos[i].GetFloorIntX();
            int spriteY = aDrawPos[i].GetFloorIntY();
            draw_character_ex(pTargetBitmap, m_aSprite[m_Frame], spriteX, spriteY, m_MOID, -1);
            g_SceneMan.RegisterMOIDDrawing(spriteX, spriteY, spriteX + m_aSprite[m_Frame]->w, spriteY + m_aSprite[m_Frame]->h);
		}
        else if (mode == g_DrawNoMOID)
            draw_character_ex(pTargetBitmap, m_aSprite[m_Frame], aDrawPos[i].GetFloorIntX(), aDrawPos[i].GetFloorIntY(), g_NoMOID, -1);
        else if (mode == g_DrawTrans)
            draw_trans_sprite(pTargetBitmap, m_aSprite[m_Frame], aDrawPos[i].GetFloorIntX(), aDrawPos[i].GetFloorIntY());
        else if (mode == g_DrawAlpha)
        {
            set_alpha_blender();
            draw_trans_sprite(pTargetBitmap, m_aSprite[m_Frame], aDrawPos[i].GetFloorIntX(), aDrawPos[i].GetFloorIntY());
        }
        else {
            if (!m_HFlipped)
                draw_sprite(pTargetBitmap, m_aSprite[m_Frame], aDrawPos[i].GetFloorIntX(), aDrawPos[i].GetFloorIntY());
            else
                draw_sprite_h_flip(pTargetBitmap, m_aSprite[m_Frame], aDrawPos[i].GetFloorIntX(), aDrawPos[i].GetFloorIntY());
        }
    }
}

} // namespace RTE