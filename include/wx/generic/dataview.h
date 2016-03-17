/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/dataview.h
// Purpose:     wxDataViewCtrl generic implementation header
// Author:      Robert Roebling
// Modified By: Bo Yang
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GENERICDATAVIEWCTRLH__
#define __GENERICDATAVIEWCTRLH__

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/control.h"
#include "wx/scrolwin.h"
#include "wx/icon.h"
#include "wx/vector.h"

class WXDLLIMPEXP_FWD_ADV wxDataViewMainWindow;
class WXDLLIMPEXP_FWD_ADV wxDataViewHeaderWindow;

// ---------------------------------------------------------
// wxDataViewColumn
// ---------------------------------------------------------

class WXDLLIMPEXP_ADV wxDataViewColumn : public wxDataViewColumnBase
{
public:
    wxDataViewColumn(const wxString& title,
                     wxDataViewRenderer *renderer,
                     unsigned int model_column,
                     int width = wxDVC_DEFAULT_WIDTH,
                     wxAlignment align = wxALIGN_CENTER,
                     int flags = wxDATAVIEW_COL_RESIZABLE)
        : wxDataViewColumnBase(renderer, model_column),
          m_title(title)
    {
        Init(width, align, flags);
    }

    wxDataViewColumn(const wxBitmap& bitmap,
                     wxDataViewRenderer *renderer,
                     unsigned int model_column,
                     int width = wxDVC_DEFAULT_WIDTH,
                     wxAlignment align = wxALIGN_CENTER,
                     int flags = wxDATAVIEW_COL_RESIZABLE)
        : wxDataViewColumnBase(bitmap, renderer, model_column)
    {
        Init(width, align, flags);
    }

    // implement wxHeaderColumnBase methods
    virtual void SetTitle(const wxString& title) wxOVERRIDE
    {
        m_title = title;
        UpdateWidth();
    }
    virtual wxString GetTitle() const wxOVERRIDE
    {
        return m_title;
    }

    virtual void SetWidth(int width) wxOVERRIDE
    {
        // As a small optimization, use this method to avoid calling
        // UpdateWidth() if the width didn't really change, even if we don't
        // care about its return value.
        (void)WXUpdateWidth(width);
    }
    virtual int GetWidth() const wxOVERRIDE;

    virtual void SetMinWidth(int minWidth) wxOVERRIDE
    {
        m_minWidth = minWidth;
        UpdateWidth();
    }
    virtual int GetMinWidth() const wxOVERRIDE
    {
        return m_minWidth;
    }

    virtual void SetAlignment(wxAlignment align) wxOVERRIDE
    {
        m_align = align;
        UpdateDisplay();
    }
    virtual wxAlignment GetAlignment() const wxOVERRIDE
    {
        return m_align;
    }

    virtual void SetFlags(int flags) wxOVERRIDE
    {
        m_flags = flags;
        UpdateDisplay();
    }
    virtual int GetFlags() const wxOVERRIDE
    {
        return m_flags;
    }

    virtual bool IsSortKey() const wxOVERRIDE
    {
        return m_sort;
    }

    virtual void UnsetAsSortKey() wxOVERRIDE;

    virtual void SetSortOrder(bool ascending) wxOVERRIDE;

    virtual bool IsSortOrderAscending() const wxOVERRIDE
    {
        return m_sortAscending;
    }

    virtual void SetBitmap( const wxBitmap& bitmap ) wxOVERRIDE
    {
        wxDataViewColumnBase::SetBitmap(bitmap);
        UpdateWidth();
    }

    // This method is specific to the generic implementation and is used only
    // by wxWidgets itself.
    bool WXUpdateWidth(int width)
    {
        if ( width == m_width )
            return false;

        m_width = width;
        UpdateWidth();

        return true;
    }

private:
    // common part of all ctors
    void Init(int width, wxAlignment align, int flags);

    // These methods forward to wxDataViewCtrl::OnColumnChange() and
    // OnColumnWidthChange() respectively, i.e. the latter is stronger than the
    // former.
    void UpdateDisplay();
    void UpdateWidth();

    wxString m_title;
    int m_width,
        m_minWidth;
    wxAlignment m_align;
    int m_flags;
    bool m_sort,
         m_sortAscending;

    friend class wxDataViewHeaderWindowBase;
    friend class wxDataViewHeaderWindow;
    friend class wxDataViewHeaderWindowMSW;
};

// ---------------------------------------------------------
// wxDataViewCtrl
// ---------------------------------------------------------

WX_DECLARE_LIST_WITH_DECL(wxDataViewColumn, wxDataViewColumnList,
                          class WXDLLIMPEXP_ADV);

class WXDLLIMPEXP_ADV wxDataViewCtrl : public wxDataViewCtrlBase,
                                       public wxScrollHelper
{
    friend class wxDataViewMainWindow;
    friend class wxDataViewHeaderWindowBase;
    friend class wxDataViewHeaderWindow;
    friend class wxDataViewHeaderWindowMSW;
    friend class wxDataViewColumn;

public:
    wxDataViewCtrl() : wxScrollHelper(this)
    {
        Init();
    }

    wxDataViewCtrl( wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxDataViewCtrlNameStr )
             : wxScrollHelper(this)
    {
        Create(parent, id, pos, size, style, validator, name);
    }

    virtual ~wxDataViewCtrl();

    void Init();

    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxDataViewCtrlNameStr);

    virtual bool AssociateModel( wxDataViewModel *model ) wxOVERRIDE;

    virtual bool AppendColumn( wxDataViewColumn *col ) wxOVERRIDE;
    virtual bool PrependColumn( wxDataViewColumn *col ) wxOVERRIDE;
    virtual bool InsertColumn( unsigned int pos, wxDataViewColumn *col ) wxOVERRIDE;

    virtual void DoSetExpanderColumn() wxOVERRIDE;
    virtual void DoSetIndent() wxOVERRIDE;

    virtual unsigned int GetColumnCount() const wxOVERRIDE;
    virtual wxDataViewColumn* GetColumn( unsigned int pos ) const wxOVERRIDE;
    virtual bool DeleteColumn( wxDataViewColumn *column ) wxOVERRIDE;
    virtual bool ClearColumns() wxOVERRIDE;
    virtual int GetColumnPosition( const wxDataViewColumn *column ) const wxOVERRIDE;

    virtual wxDataViewColumn *GetSortingColumn() const wxOVERRIDE;
    virtual wxVector<wxDataViewColumn *> GetSortingColumns() const wxOVERRIDE;

    virtual int GetSelectedItemsCount() const wxOVERRIDE;
    virtual int GetSelections( wxDataViewItemArray & sel ) const wxOVERRIDE;
    virtual void SetSelections( const wxDataViewItemArray & sel ) wxOVERRIDE;
    virtual void Select( const wxDataViewItem & item ) wxOVERRIDE;
    virtual void Unselect( const wxDataViewItem & item ) wxOVERRIDE;
    virtual bool IsSelected( const wxDataViewItem & item ) const wxOVERRIDE;

    virtual void SelectAll() wxOVERRIDE;
    virtual void UnselectAll() wxOVERRIDE;

    virtual void EnsureVisible( const wxDataViewItem & item,
                                const wxDataViewColumn *column = NULL ) wxOVERRIDE;
    virtual void HitTest( const wxPoint & point, wxDataViewItem & item,
                          wxDataViewColumn* &column ) const wxOVERRIDE;
    virtual wxRect GetItemRect( const wxDataViewItem & item,
                                const wxDataViewColumn *column = NULL ) const wxOVERRIDE;

    virtual bool SetRowHeight( int rowHeight ) wxOVERRIDE;

    virtual void Expand( const wxDataViewItem & item ) wxOVERRIDE;
    virtual void Collapse( const wxDataViewItem & item ) wxOVERRIDE;
    virtual bool IsExpanded( const wxDataViewItem & item ) const wxOVERRIDE;

    virtual void SetFocus() wxOVERRIDE;

    virtual bool SetFont(const wxFont & font) wxOVERRIDE;

    virtual bool AllowMultiColumnSort(bool allow) wxOVERRIDE;
    virtual bool IsMultiColumnSortAllowed() const wxOVERRIDE { return m_allowMultiColumnSort; }
    virtual void ToggleSortByColumn(int column) wxOVERRIDE;

#if wxUSE_DRAG_AND_DROP
    virtual bool EnableDragSource( const wxDataFormat &format ) wxOVERRIDE;
    virtual bool EnableDropTarget( const wxDataFormat &format ) wxOVERRIDE;
#endif // wxUSE_DRAG_AND_DROP

    virtual wxBorder GetDefaultBorder() const wxOVERRIDE;

    virtual void EditItem(const wxDataViewItem& item, const wxDataViewColumn *column) wxOVERRIDE;

    // These methods are specific to generic wxDataViewCtrl implementation and
    // should not be used in portable code.
    wxColour GetAlternateRowColour() const { return m_alternateRowColour; }
    void SetAlternateRowColour(const wxColour& colour);

protected:
    void EnsureVisibleRowCol( int row, int column );

    // Notice that row here may be invalid (i.e. >= GetRowCount()), this is not
    // an error and this function simply returns an invalid item in this case.
    wxDataViewItem GetItemByRow( unsigned int row ) const;
    int GetRowByItem( const wxDataViewItem & item ) const;

    // Mark the column as being used or not for sorting.
    void UseColumnForSorting(int idx);
    void DontUseColumnForSorting(int idx);

    // Return true if the given column is sorted
    bool IsColumnSorted(int idx) const;

    // Reset all columns currently used for sorting.
    void ResetAllSortColumns();

    virtual void DoEnableSystemTheme(bool enable, wxWindow* window) wxOVERRIDE;

public:     // utility functions not part of the API

    // returns the "best" width for the idx-th column
    unsigned int GetBestColumnWidth(int idx) const;

    // called by header window after reorder
    void ColumnMoved( wxDataViewColumn* col, unsigned int new_pos );

    // update the display after a change to an individual column
    void OnColumnChange(unsigned int idx);

    // update after the column width changes, also calls OnColumnChange()
    void OnColumnWidthChange(unsigned int idx);

    // update after a change to the number of columns
    void OnColumnsCountChanged();

    wxWindow *GetMainWindow() { return (wxWindow*) m_clientArea; }

    // return the index of the given column in m_cols
    int GetColumnIndex(const wxDataViewColumn *column) const;

    // Return the index of the column having the given model index.
    int GetModelColumnIndex(unsigned int model_column) const;

    // return the column displayed at the given position in the control
    wxDataViewColumn *GetColumnAt(unsigned int pos) const;

    virtual wxDataViewColumn *GetCurrentColumn() const wxOVERRIDE;

    virtual void OnInternalIdle() wxOVERRIDE;

private:
    virtual wxDataViewItem DoGetCurrentItem() const wxOVERRIDE;
    virtual void DoSetCurrentItem(const wxDataViewItem& item) wxOVERRIDE;

    void InvalidateColBestWidths();
    void InvalidateColBestWidth(int idx);
    void UpdateColWidths();

    wxDataViewColumnList      m_cols;
    // cached column best widths information, values are for
    // respective columns from m_cols and the arrays have same size
    struct CachedColWidthInfo
    {
        CachedColWidthInfo() : width(0), dirty(true) {}
        int width;  // cached width or 0 if not computed
        bool dirty; // column was invalidated, header needs updating
    };
    wxVector<CachedColWidthInfo> m_colsBestWidths;
    // This indicates that at least one entry in m_colsBestWidths has 'dirty'
    // flag set. It's cheaper to check one flag in OnInternalIdle() than to
    // iterate over m_colsBestWidths to check if anything needs to be done.
    bool                      m_colsDirty;

    wxDataViewModelNotifier  *m_notifier;
    wxDataViewMainWindow     *m_clientArea;
    wxDataViewHeaderWindow   *m_headerArea;

    // user defined color to draw row lines, may be invalid
    wxColour m_alternateRowColour;

    // columns indices used for sorting, empty if nothing is sorted
    wxVector<int> m_sortingColumnIdxs;

    // if true, allow sorting by more than one column
    bool m_allowMultiColumnSort;

private:
    void OnSize( wxSizeEvent &event );
    virtual wxSize GetSizeAvailableForScrollTarget(const wxSize& size) wxOVERRIDE;

    // we need to return a special WM_GETDLGCODE value to process just the
    // arrows but let the other navigation characters through
#ifdef __WXMSW__
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam) wxOVERRIDE;
#endif // __WXMSW__

    WX_FORWARD_TO_SCROLL_HELPER()

private:
    wxDECLARE_DYNAMIC_CLASS(wxDataViewCtrl);
    wxDECLARE_NO_COPY_CLASS(wxDataViewCtrl);
    wxDECLARE_EVENT_TABLE();
};


#endif // __GENERICDATAVIEWCTRLH__
