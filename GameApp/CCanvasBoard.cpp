#include "pch.h"
#include "CCanvasBoard.h"
CScan::CScan()
{

}
CScan::~CScan()
{

}
CCanvasBoard::CCanvasBoard(void)
    : m_nBoardSize(10)
{
    m_arrColors[0] = RGB(0, 0, 0);
    m_arrColors[1] = RGB(255, 0, 0);
    m_arrColors[2] = RGB(255, 255, 64);
    m_arrColors[3] = RGB(0, 0, 255);
}
CCanvasBoard::~CCanvasBoard(void)
{
    DeleteBoard();
}
void CCanvasBoard::SetupBoard(void) 
{
    CRect rc(POINT{ 0, 0 }, POINT{ 20, 20 });

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            CScan* s = new CScan();
            s->rc = rc;
            s->rc.OffsetRect(POINT{i * 25, j * 25});
            m_vecScans.push_back(s);
        }
    }
}
void CCanvasBoard::DeleteBoard(void)
{
    for (auto& it : m_vecScans) {
        delete it;
    }
    m_vecScans.clear();
}
void CCanvasBoard::CreateBoard(void)
{

}

std::vector<const CScan*> CCanvasBoard::GetScans()
{
    std::vector<const CScan*> rr;
    for (auto& it : m_vecScans) {
        rr.push_back(it);
    }
    return rr;
}