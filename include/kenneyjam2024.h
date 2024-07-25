/**
 * @file kenneyjam2024.h
 * @date 19-Jul-2024
 */

#ifndef __kenneyjam2024_H__
#define __kenneyjam2024_H__

#include "Scroll.h"

/** Game Class
 */
class kenneyjam2024 : public Scroll<kenneyjam2024>
{
public:


private:

                orxSTATUS       Bootstrap() const;

                void            Update(const orxCLOCK_INFO &_rstInfo);

                orxSTATUS       Init();
                orxSTATUS       Run();
                void            Exit();
                void            BindObjects();


private:
};

#endif // __kenneyjam2024_H__
