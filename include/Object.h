#pragma once

#include <set>

#include "kenneyjam2024.h"

/** Object Class
 */
class Object : public ScrollObject
{
public:
protected:
  void OnCreate();
  void OnDelete();
  void Update(const orxCLOCK_INFO &_rstInfo);

  void UpdateInput(const orxCLOCK_INFO &_rstInfo);
  void UpdateConnectedObjects(const orxCLOCK_INFO &_rstInfo);

  void OnCollide(ScrollObject *_poCollider, orxBODY_PART *_pstPart, orxBODY_PART *_pstColliderPart, const orxVECTOR &_rvPosition, const orxVECTOR &_rvNormal);
  void OnSeparate(ScrollObject *_poCollider, orxBODY_PART *_pstPart, orxBODY_PART *_pstColliderPart);

private:
  std::set<ScrollObject *> connectedObjects{};
};
