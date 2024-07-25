#include "Object.h"

// Convenience functions
namespace
{
  orxVECTOR GetMousePos()
  {
    // Get mouse position in world space
    orxVECTOR mousePosition = orxVECTOR_0;
    orxMouse_GetPosition(&mousePosition);
    orxRender_GetWorldPosition(&mousePosition, orxNULL, &mousePosition);
    return mousePosition;
  }
}

void Object::OnCreate()
{
  orxConfig_SetBool("IsObject", orxTRUE);

  orxInput_EnableSet(orxConfig_GetString("Input"), orxTRUE);
}

void Object::OnDelete()
{
}

void Object::Update(const orxCLOCK_INFO &_rstInfo)
{
  PushConfigSection();
  UpdateInput(_rstInfo);
  UpdateConnectedObjects(_rstInfo);
  PopConfigSection();
}

void Object::UpdateInput(const orxCLOCK_INFO &_rstInfo)
{
  auto shipFlameVisible = orxFALSE;

  orxInput_PushSet(orxConfig_GetString("Input"));

  orxFLOAT turn = orxInput_GetValue("Right") - orxInput_GetValue("Left");
  orxObject_SetAngularVelocity(GetOrxObject(), turn * orxConfig_GetFloat("MaxTurnSpeed") * orxMATH_KF_DEG_TO_RAD);

  auto speedInput = orxInput_GetValue("Reverse") - orxInput_GetValue("Go");
  if (speedInput != orxFLOAT_0)
  {
    // Speed up!
    auto rotation = GetRotation();
    // Move in the direction we're facing
    orxVECTOR speed = {orxFLOAT_0, speedInput, orxFLOAT_0};
    orxVector_2DRotate(&speed, &speed, rotation);
    // Adjust speed
    orxVector_Mulf(&speed, &speed, orxConfig_GetFloat("MaxSpeed"));
    // Move!
    SetSpeed(speed);

    shipFlameVisible = speedInput < orxFLOAT_0 ? orxTRUE : orxFALSE;
  }
  else
  {
    // Slow down!
    auto speed = orxVECTOR_0;
    GetSpeed(speed);
    auto speedDelta = orxVECTOR_0;
    orxVector_Mulf(&speedDelta, &speed, -1.0f * _rstInfo.fDT);
    orxVector_Add(&speed, &speed, &speedDelta);
    SetSpeed(speed);
  }

  auto child = FindChild("ShipFlame");
  orxASSERT(child != orxNULL);
  if (orxInput_HasBeenActivated("Go"))
  {
    child->AddFX("FlameOn", orxTRUE);
  }
  if (orxInput_HasBeenDeactivated("Go"))
  {
    child->AddFX("FlameOff", orxTRUE);
  }

  orxInput_PopSet();
}

void Object::UpdateConnectedObjects(const orxCLOCK_INFO &_rstInfo)
{
  // This needs to be continuous while the package is in contact with the player body so the speed will be updated properly
  // Also the speed should adjust to match the player's speed as it gets closer to the player

  for (auto connected : connectedObjects)
  {
    // Get the associated container
    connected->PushConfigSection();
    auto container = FindOwnedChild(orxConfig_GetString("Storage"));
    orxASSERT(container != orxNULL);
    connected->PopConfigSection();

    // Package speed and position
    auto connectedSpeed = orxVECTOR_0;
    auto connectedPosition = orxVECTOR_0;
    connected->GetSpeed(connectedSpeed);
    connected->GetPosition(connectedPosition);

    // Player speed and position
    auto speed = orxVECTOR_0;
    auto position = orxVECTOR_0;
    container->GetSpeed(speed);
    container->GetPosition(position, orxTRUE);

    // Vector between player and package
    auto positionDiff = orxVECTOR_0;
    orxVector_Sub(&positionDiff, &position, &connectedPosition);

    // Scalar distance between player and package
    auto distance = orxVector_GetSize(&positionDiff);

    // Attraction specs
    auto maxAttractDistance = orxConfig_GetFloat("AttractDistance");
    auto maxAttraction = orxConfig_GetFloat("Attraction");

    // Scale attraction according to distance
    auto distanceScale = orxMIN(distance / maxAttractDistance, orxFLOAT_1);
    auto attraction = distanceScale * maxAttraction;

    // Attraction speed effect in the direction of the player
    orxVector_Normalize(&positionDiff, &positionDiff);
    auto colliderSpeedDiff = orxVECTOR_0;
    orxVector_Mulf(&colliderSpeedDiff, &positionDiff, attraction);
    orxVector_Add(&connectedSpeed, &connectedSpeed, &colliderSpeedDiff);
    orxVector_Mulf(&connectedSpeed, &connectedSpeed, distanceScale);

    orxVector_Lerp(&connectedSpeed, &connectedSpeed, &colliderSpeedDiff, _rstInfo.fDT);

    // Move package toward player according to the scaled speed
    connected->SetSpeed(connectedSpeed);
  }
}

void Object::OnCollide(ScrollObject *_poCollider, orxBODY_PART *_pstPart, orxBODY_PART *_pstColliderPart, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal)
{
  // This should only cover collisions with packages
  orxASSERT(_poCollider != orxNULL);

  if (!connectedObjects.contains(_poCollider))
  {
    // Add to the set of connected objects
    connectedObjects.insert(_poCollider);

    auto position = orxVECTOR_0;
    _poCollider->GetPosition(position);
    position.fZ = static_cast<orxFLOAT>(connectedObjects.size()) * -10.0f;
    _poCollider->SetPosition(position);
  }
}

void Object::OnSeparate(ScrollObject *_poCollider, orxBODY_PART *_pstPart, orxBODY_PART *_pstColliderPart)
{
}
