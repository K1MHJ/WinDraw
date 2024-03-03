
// GameAppDoc.h : interface of the CGameAppDoc class
//


#pragma once

#include "CCanvasBoard.h"

class CGameAppDoc : public CDocument
{
protected: // create from serialization only
	CGameAppDoc() noexcept;
	DECLARE_DYNCREATE(CGameAppDoc)

// Attributes
public:

// Operations
public:
	/*  Functions for accessing the board */
	void SetupBoard(void) { m_board.SetupBoard(); }
	int GetBoardSize(void) { return m_board.GetSize(); }
	void DeleteBoard(void) { m_board.DeleteBoard(); }
	std::vector<const CScan*> GetScans() { return m_board.GetScans(); }

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGameAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	/*  Instance of the board */
	CCanvasBoard m_board;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
