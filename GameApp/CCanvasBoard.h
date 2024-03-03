#pragma once
#include <vector>

class CScan
{
public:
	CScan();
	~CScan();
	CRect rc;
};
class CCanvasBoard
{
public:
	/*  Default Constructor */
	CCanvasBoard(void);
	/*  Destructor */
	~CCanvasBoard(void);

	/*  Function to randomly setup the board */
	void SetupBoard(void);
	
	/*  Accessor functions to get board size information */
	int GetSize(void) const { return m_nBoardSize; }
	
	/*  Function to delete the board and free memory */
	void DeleteBoard(void);
	std::vector<const CScan*> GetScans();
private:
	/*  Function to create the board and allocate memory */
	void CreateBoard(void);
	
	/* 스캔 정보*/
	std::vector<CScan*> m_vecScans;
	/*  List of colors, 0 is background and 1-3 are piece colors */
	COLORREF m_arrColors[4];
	/*  Board size information */
	int m_nBoardSize;
	
};