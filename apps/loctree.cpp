/***************************************************************************
                          loctree.cpp  -  description
                             -------------------
    begin                : Sun Jun 23 2002
    copyright            : (C) 2002 by ARRL
    author               : Jon Bloom
    email                : jbloom@arrl.org
    revision             : $Id$
 ***************************************************************************/

#include "loctree.h"
#include <map>
#include <vector>
#include <algorithm>

#include "tqslcertctrls.h"
#include "dxcc.h"
#include "tqslerrno.h"
#include <errno.h>
#include <wx/imaglist.h>

#include <iostream>

#include "util.h"

using namespace std;

#include "folder.xpm"
#include "home.xpm"
enum {
	FOLDER_ICON = 0,
	HOME_ICON = 1
};

///////////// Location Tree Control ////////////////


BEGIN_EVENT_TABLE(LocTree, wxTreeCtrl)
	EVT_TREE_ITEM_ACTIVATED(-1, LocTree::OnItemActivated)
	EVT_RIGHT_DOWN(LocTree::OnRightDown)
END_EVENT_TABLE()

LocTree::LocTree(wxWindow *parent, const wxWindowID id, const wxPoint& pos,
		const wxSize& size, long style) :
		wxTreeCtrl(parent, id, pos, size, style), _nloc(0) {
	useContextMenu = true;
	wxBitmap homebm(home_xpm);
	wxBitmap folderbm(folder_xpm);
	wxImageList *il = new wxImageList(16, 16, false, 2);
	il->Add(homebm);
	il->Add(folderbm);
	SetImageList(il);
}


LocTree::~LocTree() {
}

typedef pair<wxString,int> locitem;
typedef vector<locitem> loclist;

static bool
cl_cmp(const locitem& i1, const locitem& i2) {
	return i1.first < i2.first;
}
static void
check_tqsl_error(int rval) {
	if (rval == 0)
		return;
	wxLogError(wxT("%hs"), tqsl_getErrorString());
}

int
LocTree::Build(int flags, const TQSL_PROVIDER *provider) {
	typedef map<wxString,loclist> locmap;
	locmap callsigns;

	DeleteAllItems();
	wxTreeItemId rootId = AddRoot(wxT("Station Locations"), FOLDER_ICON);
	tqsl_init();
        tQSL_Location loc;
        check_tqsl_error(tqsl_initStationLocationCapture(&loc));
        check_tqsl_error(tqsl_getNumStationLocations(loc, &_nloc));
        for (int i = 0; i < _nloc && i < 2000; i++) {
                char locname[256];
                check_tqsl_error(tqsl_getStationLocationName(loc, i, locname, sizeof locname));
                char callsign[256];
                check_tqsl_error(tqsl_getStationLocationCallSign(loc, i, callsign, sizeof callsign));
		callsigns[wxString(callsign, wxConvLocal)].push_back(make_pair(wxString(locname, wxConvLocal),i));
        }
	// Sort each callsign list and add items to tree
	locmap::iterator loc_it;
	for (loc_it = callsigns.begin(); loc_it != callsigns.end(); loc_it++) {
		wxTreeItemId id = AppendItem(rootId, loc_it->first, FOLDER_ICON);
		loclist& list = loc_it->second;
		sort(list.begin(), list.end(), cl_cmp);
		for (int i = 0; i < (int)list.size(); i++) {
			LocTreeItemData *loc = new LocTreeItemData(list[i].first);
			AppendItem(id, list[i].first, HOME_ICON, -1, loc);
		}
		Expand(id);
	}
	Expand(rootId);
	return _nloc;
}

void
LocTree::OnItemActivated(wxTreeEvent& event) {
	wxTreeItemId id = event.GetItem();
//	displayCertProperties((LocTreeItemData *)GetItemData(id), this);
}

void
LocTree::OnRightDown(wxMouseEvent& event) {
	if (!useContextMenu)
		return;
	wxTreeItemId id = HitTest(event.GetPosition());
	if (id && GetItemData(id)) {
		SelectItem(id);
		wxString locname = GetItemData(id)->getLocname();
		wxMenu *cm = makeLocationMenu(true);
		PopupMenu(cm, event.GetPosition());
		delete cm;
	}
}
