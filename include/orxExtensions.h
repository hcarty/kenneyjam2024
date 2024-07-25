/**
 * @file orxExtensions.h
 * !! This file is automatically generated by init, do not modify manually. !!
 * @date 19-Jul-2024
 */

#ifndef _orxEXTENSIONS_H_
#define _orxEXTENSIONS_H_

#define orxIMGUI_IMPL
#include "orxImGui.h"
#undef orxIMGUI_IMPL

#define orxINSPECTOR_IMPL
#include "orxInspector.h"
#undef orxINSPECTOR_IMPL

#define orxBUNDLE_IMPL
#include "orxBundle.h"
#undef orxBUNDLE_IMPL

void InitExtensions()
{
  // Initialize Dear ImGui
  orxImGui_Init();

  // Initialize inspector
  orxInspector_Init();

}

void ExitExtensions()
{
  // Exit from inspector
  orxInspector_Exit();

  // Exit from Dear ImGui
  orxImGui_Exit();

  // Exit from bundle support
  orxBundle_Exit();

}

void BootstrapExtensions()
{
  // Initialize bundle resource type
  orxBundle_Init();

  // Add config storage to find the initial config file
  orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, orxBUNDLE_KZ_RESOURCE_STORAGE, orxFALSE);
  orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, orxBUNDLE_KZ_RESOURCE_STORAGE "kenneyjam2024.obr", orxFALSE);
  orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);
}

#endif // _orxEXTENSIONS_H_
