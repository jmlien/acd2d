//------------------------------------------------------------------------------
//  Copyright 2007-2008 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2d_STATE_H_
#define _CD2d_STATE_H_

#include "acd2d_data.h"
using namespace acd2d;

class cd_state
{
public:
    
    cd_state();

    list<cd_line> cut_lines;

    bool show_hull;       //show hull
    bool show_normal;     //show poly normal
    bool show_cutl;       //show cut lines
    bool show_origpoly;   //show original polygon
    bool show_bridge;     //show the bridge
};

#endif //_CD2d_STATE_H_

